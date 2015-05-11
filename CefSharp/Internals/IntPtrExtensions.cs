﻿// Copyright © 2010-2015 The CefSharp Authors. All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.

using System;

namespace CefSharp.Internals
{
    internal static class IntPtrExtensions
    {
        /// <summary>
        /// Do an unchecked conversion from IntPtr to int
        /// so overflow exceptions don't get thrown.
        /// </summary>
        /// <param name="intPtr"></param>
        /// <returns></returns>
        public static int CastToInt32(this IntPtr intPtr)
        {
            return unchecked((int)intPtr.ToInt64());
        }
    }
}
