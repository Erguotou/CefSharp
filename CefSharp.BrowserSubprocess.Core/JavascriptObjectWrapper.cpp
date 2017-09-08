// Copyright © 2010-2016 The CefSharp Project. All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.
#pragma once

#include "Stdafx.h"

#include "JavascriptObjectWrapper.h"

namespace CefSharp
{
    void JavascriptObjectWrapper::Bind(JavascriptObject^ object, CefRefPtr<CefV8Value>& v8Value, JavascriptCallbackRegistry^ callbackRegistry)
    {
        _objectId = object->Id;
        _callbackRegistry = callbackRegistry;

        auto objectName = StringUtils::ToNative(object->JavascriptName);
        CefRefPtr<CefV8Value> javascriptObject;

        // if the javascript object is null, set V8Value to null, else it will be an empty object, as if it was created with {} in javascript
        if (object->IsNull)
            javascriptObject = CefV8Value::CreateNull();
        else
        {
            //Create property handler for get and set of Properties of this object
            _jsPropertyHandler = new JavascriptPropertyHandler(
                gcnew Func<String^, BrowserProcessResponse^>(this, &JavascriptObjectWrapper::GetProperty),
                gcnew Func<String^, Object^, BrowserProcessResponse^>(this, &JavascriptObjectWrapper::SetProperty)
            );

            //V8Value that represents this javascript object - only one per complex type
            javascriptObject = CefV8Value::CreateObject(_jsPropertyHandler.get());

            for each (JavascriptMethod^ method in Enumerable::OfType<JavascriptMethod^>(object->Methods))
            {
                auto wrappedMethod = gcnew JavascriptMethodWrapper(object->Id, _browserProcess, _callbackRegistry);
                wrappedMethod->Bind(method, javascriptObject);
                _wrappedMethods->Add(wrappedMethod);
            }

            for each (JavascriptProperty^ prop in Enumerable::OfType<JavascriptProperty^>(object->Properties))
            {
                JavascriptPropertyWrapper::Bind(prop, javascriptObject);
            }
        }

        if (v8Value != nullptr)
            // called by JavascriptRootObjectWrapper.Bind() and v8Value is the window object. Do not overwrite it.
            v8Value->SetValue(objectName, javascriptObject, V8_PROPERTY_ATTRIBUTE_NONE);
        else
            v8Value = javascriptObject;
		_allWrappedObjects[_objectId] = gcnew JavascriptCallbackWrapper(v8Value);
    }

    CefRefPtr<CefV8Value> JavascriptObjectWrapper::Wrap(IBrowserProcess^ _browserProcess, JavascriptObject^ jsObj, JavascriptCallbackRegistry^ _callbackRegistry)
    {
		if (_allWrappedObjects->ContainsKey(jsObj->Id)) {
			auto v = _allWrappedObjects[jsObj->Id];
			return v->GetValue();
		}
        auto objWrapper = gcnew JavascriptObjectWrapper(_browserProcess);
        CefRefPtr<CefV8Value> v8Value;
        objWrapper->Bind(jsObj, v8Value, _callbackRegistry);
        return v8Value;
    }

    CefRefPtr<CefV8Value> JavascriptObjectWrapper::Wrap(IBrowserProcess^ _browserProcess, Array^ array, JavascriptCallbackRegistry^ _callbackRegistry)
    {
        CefRefPtr<CefV8Value> cefArray = CefV8Value::CreateArray(array->Length);
        for (int i = 0; i < array->Length; i++)
        {
            JavascriptObject^ jsObj = safe_cast<JavascriptObject^>(array->GetValue(i));
            cefArray->SetValue(i, jsObj == nullptr ? CefV8Value::CreateNull() : Wrap(_browserProcess, jsObj, _callbackRegistry));
        }
        return cefArray;
    }

    BrowserProcessResponse^ JavascriptObjectWrapper::GetProperty(String^ memberName)
    {
        try
        {
            auto response = _browserProcess->GetProperty(_objectId, memberName);
            if (response->Success && response->Result != nullptr)
            {
                auto type = response->Result->GetType();
                // if javascript object or array, borrow JavascriptCallbackWrapper class as a vehicle to carry our payload
                if (type == JavascriptObject::typeid)
                {
                    response->Result = gcnew JavascriptCallbackWrapper(Wrap(_browserProcess, safe_cast<JavascriptObject^>(response->Result), _callbackRegistry));
                }
                else if (type->IsArray && type->GetElementType() == JavascriptObject::typeid)
                {
                    response->Result = gcnew JavascriptCallbackWrapper(Wrap(_browserProcess, safe_cast<Array^>(response->Result), _callbackRegistry));
                }
            }
            return response;
        }
        catch (const std::exception& e)
        {
            auto response = gcnew BrowserProcessResponse;
            response->Success = false;
            response->Message = String::Format("Get property {0} failed. {1}", memberName, gcnew String(e.what()));
            return response;
        }
    };

    BrowserProcessResponse^ JavascriptObjectWrapper::SetProperty(String^ memberName, Object^ value)
    {
        try
        {
            return _browserProcess->SetProperty(_objectId, memberName, value);
        }
        catch (const std::exception& e)
        {
            auto response = gcnew BrowserProcessResponse;
            response->Success = false;
            response->Message = String::Format("Set property {0}  to {1} failed. {2}", memberName, value, gcnew String(e.what()));
            return response;
        }
    };
}