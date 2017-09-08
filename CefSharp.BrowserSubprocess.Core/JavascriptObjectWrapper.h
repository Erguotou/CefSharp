// Copyright © 2010-2016 The CefSharp Project. All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.

#pragma once

#include "Stdafx.h"
#include "include/cef_v8.h"

#include "JavascriptMethodWrapper.h"
#include "JavascriptPropertyWrapper.h"
#include "JavascriptPropertyHandler.h"

using namespace System::Runtime::Serialization;
using namespace System::Linq;
using namespace System::Collections::Generic;

namespace CefSharp
{
    private ref class JavascriptObjectWrapper
    {
    private:
        List<JavascriptMethodWrapper^>^ _wrappedMethods;
        IBrowserProcess^ _browserProcess;
        MCefRefPtr<JavascriptPropertyHandler> _jsPropertyHandler;
        int64 _objectId;
        JavascriptCallbackRegistry^ _callbackRegistry;

		static Dictionary<int64, JavascriptCallbackWrapper^>^ _allWrappedObjects = gcnew Dictionary<int64, JavascriptCallbackWrapper^>(); // will not remove objects because v8 won't tell us when it's done using it

    public:
        JavascriptObjectWrapper(IBrowserProcess^ browserProcess) : _browserProcess(browserProcess), _wrappedMethods(gcnew List<JavascriptMethodWrapper^>())
        {
        }

        !JavascriptObjectWrapper()
        {
            _jsPropertyHandler = nullptr;
        }

        ~JavascriptObjectWrapper()
        {
            this->!JavascriptObjectWrapper();

            for each (JavascriptMethodWrapper^ var in _wrappedMethods)
            {
                delete var;
            }
            _wrappedMethods->Clear();
            delete _wrappedMethods;		
        }

        void Bind(JavascriptObject^ object, CefRefPtr<CefV8Value>& v8Value, JavascriptCallbackRegistry^ callbackRegistry);
        BrowserProcessResponse^ GetProperty(String^ memberName);
        BrowserProcessResponse^ SetProperty(String^ memberName, Object^ value);

        static CefRefPtr<CefV8Value> Wrap(IBrowserProcess^ , JavascriptObject^ , JavascriptCallbackRegistry^ );
        static CefRefPtr<CefV8Value> Wrap(IBrowserProcess^ , Array^ , JavascriptCallbackRegistry^ );
		static void ClearAll() 
		{
			_allWrappedObjects->Clear();
		}
    };
}