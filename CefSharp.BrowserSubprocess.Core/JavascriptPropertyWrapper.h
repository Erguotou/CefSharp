// Copyright © 2010-2016 The CefSharp Project. All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.

#pragma once

#include "Stdafx.h"
#include "include/cef_v8.h"
#include "JavascriptCallbackRegistry.h"

namespace CefSharp
{
	private ref class JavascriptPropertyWrapper
	{
	public:
		static void Bind(JavascriptProperty^ javascriptProperty, const CefRefPtr<CefV8Value>& v8Value);
	};
}