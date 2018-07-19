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
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfl_call"), _ZFP_ZFImpl_ZFLua_zfl_call);
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfl_callStatic"), _ZFP_ZFImpl_ZFLua_zfl_callStatic);
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfl_callStatic2"), _ZFP_ZFImpl_ZFLua_zfl_callStatic2);
    }, {
    })

// ============================================================
/*
 * * methodList
 *     * empty to detect by methodName
 *     * not empty to explicit use the method
 * * obj : ensured not null for zfl_call, ensured null for zfl_callStatic/zfl_callStatic2
 * * classOrNamespace/methodName : used only if methodList is empty, to detect the method
 */
static int _ZFP_ZFImpl_ZFLua_zfl_call_impl(ZF_IN lua_State *L,
                                           ZF_IN const zfchar *zfl_funcName,
                                           ZF_IN_OUT ZFCoreArrayPOD<const ZFMethod *> &methodList,
                                           ZF_IN ZFObject *obj,
                                           ZF_IN int paramCount,
                                           ZF_IN int luaParamOffset,
                                           ZF_IN const zfchar *classOrNamespace,
                                           ZF_IN const zfchar *methodName)
{
    zfstring errorHint;
    zfautoObject ret;
    classOrNamespace = ZFNamespaceSkipGlobal(classOrNamespace);

    if(obj == zfnull && methodList.isEmpty())
    { // zfAlloc
        const ZFClass *cls = ZFClass::classForName(methodName, classOrNamespace);
        if(cls == zfnull)
        {
            zfstring typeWrapperClass = ZFImpl_ZFLua_PropTypePrefix;
            typeWrapperClass += methodName;
            cls = ZFClass::classForName(typeWrapperClass, classOrNamespace);
        }
        if(cls != zfnull)
        {
            ZFImpl_ZFLua_zfAlloc(ret, L, cls, paramCount, luaParamOffset);
            ZFImpl_ZFLua_luaPush(L, ret);
            return 1;
        }
    }

    zfautoObject paramListOrig[ZFMETHOD_MAX_PARAM] = {
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
        if(!ZFImpl_ZFLua_toGeneric(paramListOrig[i], L, luaParamOffset + i))
        {
            ZFLuaErrorOccurredTrim(zfText("[%s] failed to get param%d, got %s, while executing: %s"),
                zfl_funcName,
                i,
                ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i, zftrue).cString(),
                ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
            return ZFImpl_ZFLua_luaError(L);
        }
    }

    if(methodList.isEmpty())
    {
        { // custom dispatch
            ZFImpl_ZFLua_ImplDispatchInfo dispatchInfo(
                    L, luaParamOffset,
                    obj == zfnull, classOrNamespace, obj ? obj->classData() : zfnull, obj,
                    methodName,
                    paramListOrig, (zfindex)paramCount
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
                    ZFLuaErrorOccurredTrim(zfText("[ImplDispatch] %s"), dispatchInfo.errorHint.cString());
                    return ZFImpl_ZFLua_luaError(L);
                case ZFImpl_ZFLua_ImplDispatchResultForward:
                default:
                    break;
            }
        }

        if(obj != zfnull)
        {
            obj->classData()->methodForNameGetAllT(methodList, methodName);
        }
        else
        {
            const ZFClass *cls = ZFClass::classForName(classOrNamespace);
            if(cls == zfnull)
            {
                zfindex dotPos = zfstringFindReversely(classOrNamespace, zfindexMax(), ZFNamespaceSeparator());
                if(dotPos == zfindexMax())
                {
                    zfstring classNameTmp = ZFImpl_ZFLua_PropTypePrefix;
                    classNameTmp += classOrNamespace;
                    cls = ZFClass::classForName(classNameTmp);
                }
                else
                {
                    zfstring classNameTmp;
                    classNameTmp.append(classOrNamespace, dotPos + ZFNamespaceSeparatorLen());
                    classNameTmp += ZFImpl_ZFLua_PropTypePrefix;
                    classNameTmp += classOrNamespace + dotPos + ZFNamespaceSeparatorLen();
                    cls = ZFClass::classForName(classNameTmp);
                }
            }
            if(cls == zfnull)
            {
                ZFMethodFuncGetAllT(methodList, classOrNamespace, methodName);
            }
            else
            {
                cls->methodForNameGetAllT(methodList, methodName);
            }
        }
        if(methodList.isEmpty())
        {
            ZFLuaErrorOccurredTrim(
                zfText("[%s] no such method \"%s\" in \"%s\""),
                zfl_funcName,
                methodName,
                classOrNamespace);
            return ZFImpl_ZFLua_luaError(L);
        }
    } // if(methodList.isEmpty())

    for(zfindex iMethod = 0; iMethod < methodList.count(); ++iMethod)
    {
        errorHint.removeAll();

        const ZFMethod *method = methodList[iMethod];
        if(method->methodPrivilegeType() != ZFMethodPrivilegeTypePublic
            || (method->methodType() != ZFMethodTypeStatic && obj == zfnull)
            || paramCount > method->methodParamCount()
            || (method->methodParamDefaultBeginIndex() != zfindexMax()
                && paramCount < method->methodParamDefaultBeginIndex()))
        {
            continue;
        }

        zfautoObject paramList[ZFMETHOD_MAX_PARAM] = {
            paramListOrig[0],
            paramListOrig[1],
            paramListOrig[2],
            paramListOrig[3],
            paramListOrig[4],
            paramListOrig[5],
            paramListOrig[6],
            paramListOrig[7],
        };
        zfbool parseParamSuccess = zftrue;
        for(int i = 0; i < paramCount && parseParamSuccess; ++i)
        {
            ZFImpl_ZFLua_UnknownParam *t = ZFCastZFObject(ZFImpl_ZFLua_UnknownParam *, paramList[i].toObject());
            if(t != zfnull)
            {
                parseParamSuccess = ZFImpl_ZFLua_fromUnknown(paramList[i], method->methodParamTypeIdAtIndex(i), t, &errorHint);
            }
        }
        if(!parseParamSuccess)
        {
            continue;
        }

        ret = zfnull;
        if(method->methodGenericInvoker()(method, obj, &errorHint, ret, paramList))
        {
            ZFImpl_ZFLua_luaPush(L, ret);
            return 1;
        }
    }

    zfstring err;
    zfstringAppend(err,
        zfText("[%s] no matching method to call, obj: \"%s\", params: "),
        zfl_funcName,
        ZFObjectInfo(obj).cString()
        );
    err += zfText("[");
    for(zfindex i = 0; i < paramCount; ++i)
    {
        if(i != 0)
        {
            err += zfText(", ");
        }
        err += ZFObjectInfo(paramListOrig[i].toObject());
    }
    err += zfText("]");
    err += zfText(", last error reason: ");
    if(errorHint.isEmpty())
    {
        err += zfText("method is not public or proto type / param count mismatch");
    }
    else
    {
        err += errorHint;
    }

    err += zfText(", candidate methods:");
    for(zfindex i = 0; i < methodList.count(); ++i)
    {
        err += zfText("\n    ");
        methodList[i]->objectInfoT(err);
    }

    ZFLuaErrorOccurredTrim(zfText("%s"), err.cString());
    return ZFImpl_ZFLua_luaError(L);
}

// ============================================================
/*
 * zfl_call(obj, "classInstanceMethodName"/ZFMethod, param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_call(ZF_IN lua_State *L)
{
    static const int luaParamOffset = 3;
    static const zfchar *zfl_funcName = zfText("zfl_call");
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1 || count > ZFMETHOD_MAX_PARAM + luaParamOffset - 1)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[%s] invalid param, expect zfl_call(obj, \"methodName\", param0, param1, ...), got %zi param"),
            zfl_funcName,
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }
    int paramCount = count - (luaParamOffset - 1);

    zfautoObject obj;
    if(!ZFImpl_ZFLua_toObject(obj, L, 1))
    {
        ZFLuaErrorOccurredTrim(zfText("[%s] failed to access caller object, expect zfautoObject, got %s, while executing: %s"),
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString(),
            ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
        return ZFImpl_ZFLua_luaError(L);
    }
    if(obj == zfnull)
    {
        ZFLuaErrorOccurredTrim(zfText("[%s] caller object must not be null, while executing: %s"),
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    ZFCoreArrayPOD<const ZFMethod *> methodList;
    {
        zfautoObject methodHolder;
        if(ZFImpl_ZFLua_toObject(methodHolder, L, 2))
        {
            v_ZFMethod *methodWrapper = methodHolder;
            if(methodWrapper != zfnull)
            {
                if(methodWrapper->zfv == zfnull)
                {
                    ZFLuaErrorOccurredTrim(zfText("[%s] null method, called on object: %s"),
                        zfl_funcName,
                        ZFObjectInfo(obj).cString());
                    return ZFImpl_ZFLua_luaError(L);
                }
                methodList.add(methodWrapper->zfv);
                return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
                    L, zfl_funcName, methodList, obj, paramCount, luaParamOffset, zfnull, zfnull);
            }
        }
    }

    zfstring methodName;
    if(!ZFImpl_ZFLua_toString(methodName, L, 2))
    {
        ZFLuaErrorOccurredTrim(zfText("[%s] failed to access function name, expect string type, got %s, called on object: %s"),
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue).cString(),
            ZFObjectInfo(obj).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
        L, zfl_funcName, methodList, obj, paramCount, luaParamOffset, obj->classData()->classNameFull(), methodName);
}

/*
 * zfl_callStatic("Scope1.Scope2.methodName"/ZFMethod/ZFClass, param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_callStatic(ZF_IN lua_State *L)
{
    static const int luaParamOffset = 2;
    static const zfchar *zfl_funcName = zfText("zfl_callStatic");
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1 || count > ZFMETHOD_MAX_PARAM + luaParamOffset - 1)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[%s] invalid param, expect zfl_callStatic(\"Scope1.Scope2.methodName\", param0, param1, ...), got %zi param"),
            zfl_funcName,
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }
    int paramCount = count - (luaParamOffset - 1);

    zfautoObject methodHolder;
    if(ZFImpl_ZFLua_toObject(methodHolder, L, 1))
    {
        v_ZFMethod *methodWrapper = methodHolder;
        if(methodWrapper != zfnull)
        {
            if(methodWrapper->zfv == zfnull)
            {
                ZFLuaErrorOccurredTrim(zfText("[%s] null method"), zfl_funcName);
                return ZFImpl_ZFLua_luaError(L);
            }
            ZFCoreArrayPOD<const ZFMethod *> methodList;
            methodList.add(methodWrapper->zfv);
            return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
                L, zfl_funcName, methodList, zfnull, paramCount, luaParamOffset, zfnull, zfnull);
        }
        v_ZFClass *classWrapper = methodHolder;
        if(classWrapper != zfnull)
        {
            zfautoObject ret;
            ZFImpl_ZFLua_zfAlloc(ret, L, classWrapper->zfv, paramCount, luaParamOffset);
            ZFImpl_ZFLua_luaPush(L, ret);
            return 1;
        }
    }

    zfstring methodSig;
    if(!ZFImpl_ZFLua_toString(methodSig, L, 1))
    {
        ZFLuaErrorOccurredTrim(zfText("[%s] failed to access function name, expect string type, got %s"),
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    ZFCoreArrayPOD<const ZFMethod *> methodList;
    zfindex dotPos = zfstringFindReversely(methodSig, methodSig.length(), ZFNamespaceSeparator());
    if(dotPos == zfindexMax())
    {
        return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
            L, zfl_funcName, methodList, zfnull, paramCount, luaParamOffset, zfnull, methodSig);
    }
    else
    {
        methodSig[dotPos] = '\0';
        return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
            L, zfl_funcName, methodList, zfnull, paramCount, luaParamOffset, methodSig, methodSig.cString() + dotPos + ZFNamespaceSeparatorLen());
    }
}

/*
 * zfl_callStatic2("Scope", "methodName", param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_callStatic2(ZF_IN lua_State *L)
{
    static int luaParamOffset = 3;
    static const zfchar *zfl_funcName = zfText("zfl_callStatic2");
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1 || count > ZFMETHOD_MAX_PARAM + luaParamOffset - 1)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[%s] invalid param, expect zfl_callStatic2(\"Scope\", \"methodName\", param0, param1, ...), got %zi param"),
            zfl_funcName,
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }
    int paramCount = count - (luaParamOffset - 1);

    zfstring classOrNamespace;
    if(!ZFImpl_ZFLua_toString(classOrNamespace, L, 1, zfHint("allowEmpty")zftrue))
    {
        ZFLuaErrorOccurredTrim(zfText("[%s] failed to access method scope, expect string type, got %s"),
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }
    zfstring methodName;
    if(!ZFImpl_ZFLua_toString(methodName, L, 2))
    {
        ZFLuaErrorOccurredTrim(zfText("[%s] failed to access method name, expect string type, got %s"),
            zfl_funcName,
            ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    ZFCoreArrayPOD<const ZFMethod *> methodList;
    return _ZFP_ZFImpl_ZFLua_zfl_call_impl(
        L, zfl_funcName, methodList, zfnull, paramCount, luaParamOffset, classOrNamespace, methodName);
}

ZF_NAMESPACE_GLOBAL_END

