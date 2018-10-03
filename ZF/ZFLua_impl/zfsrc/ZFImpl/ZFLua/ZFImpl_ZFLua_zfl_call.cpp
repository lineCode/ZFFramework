/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_ZFLua.h"
#include "ZFImpl_ZFLua_zfAlloc.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/*
 * zfl_call(obj, "classInstanceMethodName"/ZFMethod, param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_call(ZF_IN lua_State *L);
/*
 * zfl_callStatic("Scope1.Scope2.methodName"/ZFMethod/ZFClass, param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_callStatic(ZF_IN lua_State *L);
/*
 * zfl_callStatic2("Scope", "methodName", param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_callStatic2(ZF_IN lua_State *L);

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_call, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_call", _ZFP_ZFImpl_ZFLua_zfl_call);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_callStatic", _ZFP_ZFImpl_ZFLua_zfl_callStatic);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_callStatic2", _ZFP_ZFImpl_ZFLua_zfl_callStatic2);
    }, {
    })

// ============================================================
/*
 * type can be:
 * -  string type
 * -  #v_ZFMethod
 * -  #v_ZFClass
 *
 * only string type would perform custom dispatch
 */
static int _ZFP_ZFImpl_ZFLua_zfl_call_impl(ZF_IN lua_State *L,
                                           ZF_IN const zfchar *zfl_funcName,
                                           ZF_IN ZFObject *obj,
                                           ZF_IN const zfchar *NS,
                                           ZF_IN ZFObject *type,
                                           ZF_IN int paramCount,
                                           ZF_IN int luaParamOffset)
{
    zfautoObject paramList[ZFMETHOD_MAX_PARAM] = {
              ZFMethodGenericInvokerDefaultParamHolder()
            , ZFMethodGenericInvokerDefaultParamHolder()
            , ZFMethodGenericInvokerDefaultParamHolder()
            , ZFMethodGenericInvokerDefaultParamHolder()
            , ZFMethodGenericInvokerDefaultParamHolder()
            , ZFMethodGenericInvokerDefaultParamHolder()
            , ZFMethodGenericInvokerDefaultParamHolder()
            , ZFMethodGenericInvokerDefaultParamHolder()
        };
    for(int i = 0; i < paramCount; ++i)
    {
        if(!ZFImpl_ZFLua_toGeneric(paramList[i], L, luaParamOffset + i))
        {
            ZFLuaErrorOccurredTrim("[%s] failed to get param%d, got %s, while executing: %s",
                zfl_funcName,
                i,
                ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i, zftrue).cString(),
                ZFObjectInfo(type).cString());
            return ZFImpl_ZFLua_luaError(L);
        }
    }

    const zfchar *methodName = ZFDI_toString(type);
    if(methodName != zfnull && zfstringFind(methodName, zfindexMax(), ZFNamespaceSeparator()) == zfindexMax())
    { // custom dispatch
        ZFImpl_ZFLua_ImplDispatchInfo dispatchInfo(
                L, luaParamOffset,
                obj == zfnull, NS, obj ? obj->classData() : zfnull, obj,
                methodName,
                paramList, (zfindex)paramCount
            );
        ZFImpl_ZFLua_implDispatch(dispatchInfo);
        switch(dispatchInfo.dispatchResult)
        {
            case ZFImpl_ZFLua_ImplDispatchResultSuccess:
                if(dispatchInfo.returnValueCustom != -1)
                {
                    return dispatchInfo.returnValueCustom;
                }
                else
                {
                    if(dispatchInfo.returnValue != ZFImpl_ZFLua_implDispatchReturnValueNotSet)
                    {
                        ZFImpl_ZFLua_luaPush(L, dispatchInfo.returnValue);
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
            case ZFImpl_ZFLua_ImplDispatchResultError:
                ZFLuaErrorOccurredTrim("[ImplDispatch] %s", dispatchInfo.errorHint.cString());
                return ZFImpl_ZFLua_luaError(L);
            case ZFImpl_ZFLua_ImplDispatchResultForward:
            default:
                break;
        }
    }

    zfautoObject ret;
    zfstring errorHint;
    if(ZFDI_invoke(ret, &errorHint, obj, NS, type, (zfindex)paramCount
            , paramList[0]
            , paramList[1]
            , paramList[2]
            , paramList[3]
            , paramList[4]
            , paramList[5]
            , paramList[6]
            , paramList[7]
        ))
    {
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
    ZFLuaErrorOccurredTrim("[%s] %s",
        zfl_funcName,
        errorHint.cString());
    return ZFImpl_ZFLua_luaError(L);
}

// ============================================================
/*
 * zfl_call(obj, "classInstanceMethodName"/ZFMethod, param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_call(ZF_IN lua_State *L)
{
    static const int luaParamOffset = 3;
    static const zfchar *zfl_funcName = "zfl_call";
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1 || count > ZFMETHOD_MAX_PARAM + luaParamOffset - 1)
    {
        ZFLuaErrorOccurredTrim(
            "[%s] invalid param, expect zfl_call(obj, \"methodName\", param0, param1, ...), got %zi param",
            zfl_funcName,
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }
    int paramCount = count - (luaParamOffset - 1);

    zfautoObject obj;
    if(!ZFImpl_ZFLua_toObject(obj, L, 1))
    {
        ZFLuaErrorOccurredTrim("[%s] failed to access caller object, expect zfautoObject, got %s, while executing: %s",
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString(),
            ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
        return ZFImpl_ZFLua_luaError(L);
    }
    if(obj == zfnull)
    {
        ZFLuaErrorOccurredTrim("[%s] caller object must not be null, while executing: %s",
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
        return ZFImpl_ZFLua_luaError(L);
    }
    zfautoObject type;
    if(!ZFImpl_ZFLua_toGeneric(type, L, 2))
    {
        ZFLuaErrorOccurredTrim("[%s] unable to access method, got: %s",
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
        L,
        zfl_funcName,
        obj,
        zfnull,
        type,
        paramCount,
        luaParamOffset);
}

/*
 * zfl_callStatic("Scope1.Scope2.methodName"/ZFMethod/ZFClass, param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_callStatic(ZF_IN lua_State *L)
{
    static const int luaParamOffset = 2;
    static const zfchar *zfl_funcName = "zfl_callStatic";
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1 || count > ZFMETHOD_MAX_PARAM + luaParamOffset - 1)
    {
        ZFLuaErrorOccurredTrim(
            "[%s] invalid param, expect zfl_callStatic(\"Scope1.Scope2.methodName\", param0, param1, ...), got %zi param",
            zfl_funcName,
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }
    int paramCount = count - (luaParamOffset - 1);

    zfautoObject type;
    if(!ZFImpl_ZFLua_toGeneric(type, L, 1))
    {
        ZFLuaErrorOccurredTrim("[%s] unable to access method, got: %s",
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 1).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
        L,
        zfl_funcName,
        zfnull,
        zfnull,
        type,
        paramCount,
        luaParamOffset);
}

/*
 * zfl_callStatic2("Scope", "methodName", param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_callStatic2(ZF_IN lua_State *L)
{
    static int luaParamOffset = 3;
    static const zfchar *zfl_funcName = "zfl_callStatic2";
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1 || count > ZFMETHOD_MAX_PARAM + luaParamOffset - 1)
    {
        ZFLuaErrorOccurredTrim(
            "[%s] invalid param, expect zfl_callStatic2(\"Scope\", \"methodName\", param0, param1, ...), got %zi param",
            zfl_funcName,
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }
    int paramCount = count - (luaParamOffset - 1);

    zfstring classOrNamespace;
    if(!ZFImpl_ZFLua_toString(classOrNamespace, L, 1, zfHint("allowEmpty")zftrue))
    {
        ZFLuaErrorOccurredTrim("[%s] failed to access method scope, expect string type, got %s",
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }
    zfautoObject type;
    if(!ZFImpl_ZFLua_toGeneric(type, L, 2))
    {
        ZFLuaErrorOccurredTrim("[%s] failed to access method name, expect string type, got %s",
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
        L,
        zfl_funcName,
        zfnull,
        classOrNamespace,
        type,
        paramCount,
        luaParamOffset);
}

ZF_NAMESPACE_GLOBAL_END

