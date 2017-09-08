// Copyright © 2010-2016 The CefSharp Project. All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.

#include "Stdafx.h"
#include "TypeUtils.h"
#include "JavascriptMethodHandler.h"
#include "JavascriptObjectWrapper.h"

namespace CefSharp
{
    bool JavascriptMethodHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
    {
        auto parameter = gcnew array<Object^>(arguments.size());

        for (std::vector<CefRefPtr<CefV8Value>>::size_type i = 0; i != arguments.size(); i++)
        {
            if (arguments[i]->IsFunction())
            {
                parameter[i] = _callbackRegistry->Register(CefV8Context::GetCurrentContext(), arguments[i]);
            }
            else
            {
                parameter[i] = TypeUtils::ConvertFromCef(arguments[i]);
            }
        }

        try
        {
            auto response = _method->Invoke(parameter);

            retval = ConvertToCefObject(response->Result);
            if (!response->Success)
            {
                exception = StringUtils::ToNative(response->Message);
            }
        }
        catch (Exception^ ex)
        {
            exception = StringUtils::ToNative(ex->ToString());
        }

        //NOTE: Return true otherwise exception is ignored
        return true;
    }

    CefRefPtr<CefV8Value> JavascriptMethodHandler::ConvertToCefObject(Object^ obj)
    {
        if (obj == nullptr)
        {
            return CefV8Value::CreateNull();
        }

        auto type = obj->GetType();

        if (type == JavascriptObject::typeid)
            return JavascriptObjectWrapper::Wrap(_browserProcess, safe_cast<JavascriptObject^>(obj), _callbackRegistry);
        else if (type->IsArray && type->GetElementType() == JavascriptObject::typeid)
            return JavascriptObjectWrapper::Wrap(_browserProcess, safe_cast<Array^>(obj), _callbackRegistry);
        return TypeUtils::ConvertToCef(obj, nullptr);
    }
}