/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
/**
 * @file ZFLuaExecute.h
 * @brief execute lua script
 */

#ifndef _ZFI_ZFLuaExecute_h_
#define _ZFI_ZFLuaExecute_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief main entry for lua module, execute lua code
 *
 * thanks to powerful reflection mechanism in ZFFramework,
 * most of components are registered to lua by reflection automatically\n
 * \n
 * all types of ZFFramework are wrapped as #zfautoObject in lua world,
 * including: #ZFObject types (wrapped directly)
 * and non-ZFObject normal C++ types (wrapped by #ZFPropertyTypeWrapper)\n
 * then, all reflectable #ZFMethod supply #ZFMethodGenericInvoker
 * to invoke the method without knowing all actual types\n
 * \n
 * here is a list of functions available in lua to communicate with ZFFramework:
 * -  `zfAlloc("ClassName")`
 *   or `ClassName()`
 *   or `ClassName.zfAlloc()`\n
 *   alloc a ZFObject type (no extra init param supported)\n
 *   "ClassName" can be #v_ZFClass, or converted by #ZFImpl_ZFLua_toString
 * -  invoker
 *   -  `ret = zfl_call(obj, "functionName", param0, param1, ...)`
 *     or `obj:functionName(param0, param1, ...)`\n
 *     call object's instance method, params are optional\n
 *     for "functionName", see #ZFPropertyTypeId_ZFMethod\n
 *     "functionName" can be #v_ZFMethod, or converted by #ZFImpl_ZFLua_toString,
 *     while other types must exactly match the original types
 *   -  `ret = zfl_callStatic("::methodName", param0, param1, ...)`
 *     or `ret = zfl_callStatic("ClassOrNamespace::methodName", param0, param1, ...)`
 *     or `ret = zfl_callStatic2("ClassOrNamespace", "methodName", param0, param1, ...)`
 *     or `ret = ClassOrNamespace.methodName(param0, param1, ...)`\n
 *     call global function or static class member method, params are optional\n
 *     for "functionName", see #ZFPropertyTypeId_ZFMethod\n
 *     "functionName" can be #v_ZFMethod, or converted by #ZFImpl_ZFLua_toString,
 *     while other types must exactly match the original types\n
 *     these namespace are considered as the same:
 *     -  #ZFMethodFuncNamespaceGlobal
 *     -  #ZFLuaFuncNamespaceGlobal
 *     -  empty string
 *   -  while calling functions with lua raw string, such as `func('abc')`,
 *     we will try to convert to proper param type by string converter,
 *     if overload functions exists,
 *     conversion may take more than once,
 *     until it matches first function
 * -  ZFEnum
 *   -  `YourEnum.e_YourEnumValue()`\n
 *     access the enum value
 *   -  `YourEnumFlags('YourEnumValue1|YourEnumValue2')`
 *     or `YourEnumFlags(YourEnum.e_YourEnumValue1(), YourEnum.e_YourEnumValue2, ...)`\n
 *     create enum flags
 * -  value holder
 *   -  `value = YourTypeName("yourTypeData")`
 *     or `value = zfAlloc("YourTypeName")`
 *     or `value = YourTypeName()`
 *     or `value = YourTypeName.zfAlloc()`\n
 *     create a non-ZFObject type registered by #ZFPROPERTY_TYPE_DECLARE,
 *     return the associated `YourTypeName` that holds the value\n
 *     "YourTypeName" represents the type name in #ZFPROPERTY_TYPE_DECLARE\n
 *     "yourTypeData" store string datas that would be decoded by YourTypeNameFromString\n
 *     "yourTypeData" are converted by #ZFImpl_ZFLua_toString\n
 *     if your value holder supplys reflectable #ZFObject::objectOnInit
 *     (#ZFOBJECT_ON_INIT_DECLARE_2 series),
 *     the value holder can also be constructed by function like call:
 *     `YourTypeName(param0, param1)`
 *   -  `value:yourFunc()`
 *     or `YourTypeName.YourFunc()`\n
 *     for non-ZFObject types that wrapped by #ZFPROPERTY_TYPE_DECLARE,
 *     you may use #ZFMETHOD_USER_REGISTER_0 series to register methods
 *     to its wrapper type `YourTypeName`,
 *     then the methods can be invoked directly to your value type
 * -  value converter
 *   -  `zfl_luaValue(v)`\n
 *     convert a value to lua's raw value, by #ZFImpl_ZFLua_toString or #ZFImpl_ZFLua_toNumber,
 *     the result lua value can be:
 *     -  lua string
 *     -  lua integer
 *     -  lua number
 *     -  lua boolean
 * -  callback
 *   -  `ZFCallbackForLua(luaFunc)`\n
 *     create a #ZFListener from lua function\n
 *     the lua function's proto type must be:
 *     @code
 *       function myLuaCallback(listenerData, userData)
 *       endfunction
 *     @endcode
 *   -  `output:output(string [, byteSize])`\n
 *     write to output callback
 *   -  `input:input(string, byteSize)`\n
 *     read from input callback
 * -  array
 *   -  `ZFCoreArrayCreate([a, b, c, ...])`\n
 *     create a array, params support these types:
 *     -  zfautoObject
 *     -  any types that can be converted by #ZFImpl_ZFLua_toNumber
 *       (stored as #ZFValue)
 *     -  any types that can be converted by #ZFImpl_ZFLua_toString
 *       (stored as #v_zfstring)
 * -  param and return value
 *   -  simply use lua standard logic to process params and return values,
 *     here's some example:
 *     @code
 *       -- get all params
 *       local args = {...};
 *       local argsCount = #args;
 *       -- return value
 *       return args[1];
 *     @endcode
 *     note that, all params passed from #ZFLuaExecute are all #zfautoObject type
 * -  util
 *   -  `zfstringAppend(s, fmt, ...)`
 *     or `zfstringWithFormat(fmt, ...)`\n
 *     fmt would be converted by #ZFImpl_ZFLua_toString,
 *     while only "%s" supported\n
 *     following va_args support:
 *     -  #ZFObject, would be converted by #ZFObject::objectInfo
 *     -  lua string type, converted by #ZFImpl_ZFLua_toString
 *     -  any lua type, converted by #ZFImpl_ZFLua_luaObjectInfo
 *
 *     note: the va_args support params up to #ZFMETHOD_MAX_PARAM
 *   -  `zfText('lua string')`\n
 *     same as `zfstring('lua string')`
 * -  path info
 *   -  `zfl_pathInfo()`\n
 *     return path info of current context, null if not available
 *   -  `ZFLuaImport(localFilePath [, luaParams, L])`\n
 *     util method for #ZFLuaExecute + #ZFInputCallbackForLocalFile with local file
 * -  debug helper
 *   -  `zfLog(fmt, ...)`
 *     or zfLogTrim(fmt, ...)\n
 *     use zfstringAppend then output to zfLog
 *   -  `zfLogT()`
 *     or zfLogTrimT()\n
 *     return a output callback to output, see above
 *   -  `zfl_tableInfo(v)`
 *     or `zfl_tableInfoPrint`\n
 *     return string that represents the table
 *
 *
 * note:
 * -  all types would be registered automatically,
 * -  for dynamically loaded library, all types would also be registered normally,
 *   however, won't be automatically unregistered when unloaded
 */
ZFMETHOD_FUNC_DECLARE_3(zfautoObject, ZFLuaExecute,
                        ZFMP_IN(const ZFInputCallback &, input),
                        ZFMP_IN_OPT(const ZFCoreArray<zfautoObject> *, luaParams, zfnull),
                        ZFMP_IN_OPT(void *, L, zfnull))
/** @brief see #ZFLuaExecute */
ZFMETHOD_FUNC_DECLARE_4(zfautoObject, ZFLuaExecute,
                        ZFMP_IN(const zfchar *, buf),
                        ZFMP_IN_OPT(zfindex, bufLen, zfindexMax()),
                        ZFMP_IN_OPT(const ZFCoreArray<zfautoObject> *, luaParams, zfnull),
                        ZFMP_IN_OPT(void *, L, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLuaExecute_h_

