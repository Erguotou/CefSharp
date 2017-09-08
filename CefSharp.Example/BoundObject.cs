﻿// Copyright © 2010-2016 The CefSharp Authors. All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace CefSharp.Example
{
    public class BoundObject
    {
        public int MyProperty { get; set; }

        public string MyReadOnlyProperty { get; internal set; }
        public Type MyUnconvertibleProperty { get; set; }
        public SubBoundObject SubObject { get; set; }
        public ExceptionTestBoundObject ExceptionTestObject { get; set; }

        public SubBoundObject[] MyObjects
        {
            get
            {
                return new[]
                {
                    new SubBoundObject { SimpleProperty = "Hello" },
                    new SubBoundObject { SimpleProperty = "CefSharp" }
                };
            }
        }

        public uint[] MyUintArray { get; set; }

        public int[] MyIntArray { get; set; }


        public Array MyArray
        {
            get { return new short[] { 1, 2, 3 }; }
        }

        public byte[] MyBytes
        {
            get { return new byte[] { 3, 4, 5 }; }
        }

        public List<uint> MyUintList { get; set; }

        public BoundObject()
        {
            MyProperty = 42;
            MyReadOnlyProperty = "I'm immutable!";
            IgnoredProperty = "I am an Ignored Property";
            MyUnconvertibleProperty = GetType();
            SubObject = new SubBoundObject() { Parent = this };
            MyUintArray = new uint[] { 7, 8 };
            MyIntArray = new[] { 1, 2, 3, 4, 5, 6, 7, 8 };
            MyUintList = new List<uint>(MyUintArray);
            ExceptionTestObject = new ExceptionTestBoundObject();
        }

        public void TestCallback(IJavascriptCallback javascriptCallback)
        {
            const int taskDelay = 1500;

            Task.Run(async () =>
            {
                await Task.Delay(taskDelay);

                using (javascriptCallback)
                {
                    //NOTE: Classes are not supported, simple structs are
                    var response = new CallbackResponseStruct("This callback from C# was delayed " + taskDelay + "ms");
                    await javascriptCallback.ExecuteAsync(response);
                }
            });
        }

        public int EchoMyProperty()
        {
            return MyProperty;
        }

        public string Repeat(string str, int n)
        {
            string result = String.Empty;
            for (int i = 0; i < n; i++)
            {
                result += str;
            }
            return result;
        }

        public string EchoParamOrDefault(string param = "This is the default value")
        {
            return param;
        }

        public void EchoVoid()
        {
        }

        public Boolean EchoBoolean(Boolean arg0)
        {
            return arg0;
        }

        public Boolean? EchoNullableBoolean(Boolean? arg0)
        {
            return arg0;
        }

        public SByte EchoSByte(SByte arg0)
        {
            return arg0;
        }

        public SByte? EchoNullableSByte(SByte? arg0)
        {
            return arg0;
        }

        public Int16 EchoInt16(Int16 arg0)
        {
            return arg0;
        }

        public Int16? EchoNullableInt16(Int16? arg0)
        {
            return arg0;
        }

        public Int32 EchoInt32(Int32 arg0)
        {
            return arg0;
        }

        public Int32? EchoNullableInt32(Int32? arg0)
        {
            return arg0;
        }

        public Int64 EchoInt64(Int64 arg0)
        {
            return arg0;
        }

        public Int64? EchoNullableInt64(Int64? arg0)
        {
            return arg0;
        }

        public Byte EchoByte(Byte arg0)
        {
            return arg0;
        }

        public Byte? EchoNullableByte(Byte? arg0)
        {
            return arg0;
        }

        public UInt16 EchoUInt16(UInt16 arg0)
        {
            return arg0;
        }

        public UInt16? EchoNullableUInt16(UInt16? arg0)
        {
            return arg0;
        }

        public UInt32 EchoUInt32(UInt32 arg0)
        {
            return arg0;
        }

        public UInt32? EchoNullableUInt32(UInt32? arg0)
        {
            return arg0;
        }

        public UInt64 EchoUInt64(UInt64 arg0)
        {
            return arg0;
        }

        public UInt64? EchoNullableUInt64(UInt64? arg0)
        {
            return arg0;
        }

        public Single EchoSingle(Single arg0)
        {
            return arg0;
        }

        public Single? EchoNullableSingle(Single? arg0)
        {
            return arg0;
        }

        public Double EchoDouble(Double arg0)
        {
            return arg0;
        }

        public Double? EchoNullableDouble(Double? arg0)
        {
            return arg0;
        }

        public Char EchoChar(Char arg0)
        {
            return arg0;
        }

        public Char? EchoNullableChar(Char? arg0)
        {
            return arg0;
        }

        public DateTime EchoDateTime(DateTime arg0)
        {
            return arg0;
        }

        public DateTime? EchoNullableDateTime(DateTime? arg0)
        {
            return arg0;
        }

        public Decimal EchoDecimal(Decimal arg0)
        {
            return arg0;
        }

        public Decimal? EchoNullableDecimal(Decimal? arg0)
        {
            return arg0;
        }

        public String EchoString(String arg0)
        {
            return arg0;
        }

        // TODO: This will currently not work, as it causes a collision w/ the EchoString() method. We need to find a way around that I guess.
        //public String echoString(String arg)
        //{
        //    return "Lowercase echo: " + arg;
        //}

        public String lowercaseMethod()
        {
            return "lowercase";
        }

        public string ReturnJsonEmployeeList()
        {
            return "{\"employees\":[{\"firstName\":\"John\", \"lastName\":\"Doe\"},{\"firstName\":\"Anna\", \"lastName\":\"Smith\"},{\"firstName\":\"Peter\", \"lastName\":\"Jones\"}]}";
        }

        [JavascriptIgnore]
        public string IgnoredProperty { get; set; }

        [JavascriptIgnore]
        public string IgnoredMethod()
        {
            return "I am an Ignored Method";
        }

        public string ComplexParamObject(object param)
        {
            if (param == null)
            {
                return "param is null";
            }
            return "The param type is:" + param.GetType();
        }

        public SubBoundObject GetSubObject()
        {
            return SubObject;
        }
        public SubBoundObject[] GetMySubObjects()
        {
            return MyObjects;
        }

        public List<SubBoundObject> GetMySubObjectList()
        {
            return new List<SubBoundObject>(MyObjects);
        }

        public class NestedSubClass : BoundObject
        {
            public string[] MyStrings { get; set; }

            public NestedSubClass()
            {
                MyStrings = new string[] { "subclass string 1", "subclass string 2" };
            }
        }

        public void ToggleSubObjectParent()
        {
            SubObject.Parent = SubObject.Parent != null ? null : new NestedSubClass();
        }
        /// <summary>
        /// Demonstrates the use of params as an argument in a bound object
        /// </summary>
        /// <param name="name">Dummy Argument</param>
        /// <param name="args">Params Argument</param>
        public string MethodWithParams(string name, params object[] args)
        {
            return "Name:" + name + ";Args:" + string.Join(", ", args.ToArray());
        }

        public string MethodWithoutParams(string name, string arg2)
        {
            return string.Format("{0}, {1}", name, arg2);
        }

        public string MethodWithoutAnything()
        {
            return "Method without anything called and returned successfully.";
        }

        public string MethodWithThreeParamsOneOptionalOneArray(string name, string optionalParam = null, params object[] args)
        {
            return "MethodWithThreeParamsOneOptionalOneArray:" + (name ?? "No Name Specified") + " - " + (optionalParam ?? "No Optional Param Specified") + ";Args:" + string.Join(", ", args.ToArray());
        }
    }
}
