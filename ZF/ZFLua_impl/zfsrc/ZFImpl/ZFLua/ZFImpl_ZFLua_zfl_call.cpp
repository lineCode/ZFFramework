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

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static void _ZFP_ZFImpl_ZFLua_zfl_callStatic_methodScopeFix(ZF_IN_OUT zfstring &methodScope)
{
    if(methodScope.compare(ZFLuaFuncNamespaceGlobal) == 0
        || methodScope.compare(ZFMethodFuncNamespaceGlobal) == 0)
    {
        methodScope.removeAll();
    }
}
static int _ZFP_ZFImpl_ZFLua_zfl_call_invoker(ZF_IN lua_State *L,
                                              ZF_IN_OUT zfautoObject *paramList,
                                              ZF_IN zfindex paramCount,
                                              ZF_IN const ZFCoreArrayPOD<const ZFMethod *> &methodList,
                                              ZF_IN ZFObject *obj)
{
    zfstring errorHint;
    zfautoObject ret;
    for(zfindex iMethod = 0; iMethod < methodList.count(); ++iMethod)
    {
        const ZFMethod *method = methodList[iMethod];
        if(method->methodPrivilegeType() != ZFMethodPrivilegeTypePublic
            || paramCount > method->methodParamCount()
            || (method->methodParamDefaultBeginIndex() != zfindexMax()
                && paramCount < method->methodParamDefaultBeginIndex()))
        {
            continue;
        }

        errorHint.removeAll();

        /* ZFMETHOD_MAX_PARAM */
        zfbool parseParamSuccess = zftrue;
        for(zfindex i = 0; i < paramCount; ++i)
        {
            ZFImpl_ZFLua_UnknownParam *t = ZFCastZFObject(ZFImpl_ZFLua_UnknownParam *, paramList[i].toObject());
            if(t != zfnull)
            {
                zfautoObject tHolder = paramList[i];
                const ZFPropertyTypeIdDataBase *typeIdData = ZFPropertyTypeIdDataGet(method->methodParamTypeIdAtIndex(i));
                if(typeIdData == zfnull || !typeIdData->propertyWrapper(paramList[i]))
                {
                    zfstringAppend(errorHint, zfText("param %zi (%s) can not be converted from string automatically"),
                            i,
                            method->methodParamTypeNameAtIndex(i)
                        );
                    parseParamSuccess = zffalse;
                    paramList[i] = tHolder;
                    break;
                }

                if(paramList[i] == zfnull)
                {
                    if(!ZFObjectFromString(paramList[i], t->zfv, t->zfv.length()))
                    {
                        parseParamSuccess = zffalse;
                    }
                }
                else if(!paramList[i].to<ZFPropertyTypeWrapper *>()->wrappedValueFromString(t->zfv, t->zfv.length()))
                {
                    parseParamSuccess = zffalse;
                }

                if(!parseParamSuccess)
                {
                    zfstringAppend(errorHint, zfText("param %zi (%s) can not be converted from string \"%s\""),
                            i,
                            method->methodParamTypeNameAtIndex(i),
                            t->zfv.cString()
                        );
                    paramList[i] = tHolder;
                    break;
                }
            }
        }
        if(!parseParamSuccess)
        {
            continue;
        }

        ret = zfnull;
        if(method->methodGenericInvoker()(method, obj, &errorHint, ret
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
    }

    if(methodList.count() == 1)
    {
        zfstring err;
        zfstringAppend(err,
            zfText("[zfl_call] failed to perform generic invoke for method: %s, obj: \"%s\", params: "),
            methodList[0]->objectInfo().cString(),
            ZFObjectInfo(obj).cString()
            );
        err += zfText("[");
        for(zfindex i = 0; i < paramCount; ++i)
        {
            if(i != 0)
            {
                err += zfText(", ");
            }
            err += ZFObjectInfo(paramList[i].toObject());
        }
        err += zfText("]");
        err += zfText(", reason: ");
        err += errorHint;
        ZFLuaErrorOccurredTrim(zfText("%s"), err.cString());
    }
    else
    {
        zfstring err;
        zfstringAppend(err,
            zfText("[zfl_call] no matching method to call, obj: \"%s\", params: "),
            ZFObjectInfo(obj).cString()
            );
        err += zfText("[");
        for(zfindex i = 0; i < paramCount; ++i)
        {
            if(i != 0)
            {
                err += zfText(", ");
            }
            err += ZFObjectInfo(paramList[i].toObject());
        }
        err += zfText("]");
        err += zfText(", last error reason: ");
        err += errorHint;

        err += zfText(", candidate methods:");
        for(zfindex i = 0; i < methodList.count(); ++i)
        {
            err += zfText("\n    ");
            methodList[i]->objectInfoT(err);
        }

        ZFLuaErrorOccurredTrim(zfText("%s"), err.cString());
    }
    return ZFImpl_ZFLua_luaError(L);
}

#define _ZFP_ZFImpl_ZFLua_zfl_call_implDispatch( \
        isStatic, classOrNamespace, classOrNull, objectOrNull, \
        methodName \
    ) \
    ZFImpl_ZFLua_ImplDispatchInfo dispatchInfo( \
            L, \
            isStatic, classOrNamespace, classOrNull, objectOrNull, \
            methodName, \
            paramList, paramCount \
        ); \
    ZFImpl_ZFLua_implDispatch(dispatchInfo); \
    switch(dispatchInfo.dispatchResult) \
    { \
        case ZFImpl_ZFLua_ImplDispatchResultSuccess: \
            if(dispatchInfo.returnValueCustom != -1) \
            { \
                return dispatchInfo.returnValueCustom; \
            } \
            else \
            { \
                if(dispatchInfo.returnValue != ZFImpl_ZFLua_implDispatchReturnValueNotSet) \
                { \
                    ZFImpl_ZFLua_luaPush(L, dispatchInfo.returnValue); \
                    return 1; \
                } \
                else \
                { \
                    return 0; \
                } \
            } \
        case ZFImpl_ZFLua_ImplDispatchResultError: \
            ZFLuaErrorOccurredTrim(zfText("[ImplDispatch] %s"), dispatchInfo.errorHint.cString()); \
            return ZFImpl_ZFLua_luaError(L); \
        case ZFImpl_ZFLua_ImplDispatchResultForward: \
        default: \
            break; \
    }

/*
 * zfl_call(obj, "classInstanceMethodName", param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_call(ZF_IN lua_State *L)
{
    static const zfint luaParamOffset = 2;
    zfint count = (zfint)lua_gettop(L);
    if(count < luaParamOffset || count > ZFMETHOD_MAX_PARAM + luaParamOffset)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[zfl_call] invalid param, expect zfl_call(obj, \"methodName\", param0, param1, ...), got %zi param"),
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }
    zfint paramCount = count - luaParamOffset;

    zfautoObject obj;
    if(!ZFImpl_ZFLua_toObject(obj, L, 1))
    {
        ZFLuaErrorOccurredTrim(zfText("[zfl_call] failed to access caller object, expect zfautoObject, got %s, while executing: %s"),
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString(),
            ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
        return ZFImpl_ZFLua_luaError(L);
    }
    v_zfautoObject *objTmp = obj;
    if(objTmp != zfnull)
    {
        obj = objTmp->zfv;
    }

    if(obj == zfnull)
    {
        ZFLuaErrorOccurredTrim(zfText("[zfl_call] caller object must not be null, while executing: %s"),
            ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

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
    for(zfint i = 0; i < paramCount; ++i)
    {
        if(!ZFImpl_ZFLua_toObject(paramList[i], L, luaParamOffset + i + 1))
        {
            zfblockedAlloc(ZFImpl_ZFLua_UnknownParam, t);
            if(!ZFImpl_ZFLua_toString(t->zfv, L, luaParamOffset + i + 1))
            {
                ZFLuaErrorOccurredTrim(zfText("[zfl_call] failed to get param%d, expect zfautoObject, got %s, while executing: %s"),
                    i,
                    ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i + 1, zftrue).cString(),
                    ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
                return ZFImpl_ZFLua_luaError(L);
            }
            paramList[i] = t;
        }
    }

    ZFCoreArrayPOD<const ZFMethod *> methodList;
    do
    {
        zfautoObject methodHolder;
        if(ZFImpl_ZFLua_toObject(methodHolder, L, 2))
        {
            v_ZFMethod *methodWrapper = methodHolder;
            if(methodWrapper != zfnull)
            {
                if(methodWrapper->zfv == zfnull)
                {
                    ZFLuaErrorOccurredTrim(zfText("[zfl_call] null method, called on object: %s"),
                        ZFObjectInfo(obj).cString());
                    return ZFImpl_ZFLua_luaError(L);
                }
                methodList.add(methodWrapper->zfv);
                break;
            }
        }

        zfstring methodSig;
        if(!ZFImpl_ZFLua_toString(methodSig, L, 2))
        {
            ZFLuaErrorOccurredTrim(zfText("[zfl_call] failed to access function name, expect string type, got %s, called on object: %s"),
                ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue).cString(),
                ZFObjectInfo(obj).cString());
            return ZFImpl_ZFLua_luaError(L);
        }
        if(methodSig.isEmpty())
        {
            ZFLuaErrorOccurredTrim(zfText("[zfl_call] method sig must not be null, called on object: %s"),
                ZFObjectInfo(obj).cString());
            return ZFImpl_ZFLua_luaError(L);
        }

        _ZFP_ZFImpl_ZFLua_zfl_call_implDispatch(
            zffalse, obj->classData()->className(), obj->classData(), obj,
            methodSig)

        obj.toObject()->classData()->methodForNameGetAllT(methodList, methodSig);

        if(methodList.isEmpty())
        {
            ZFLuaErrorOccurredTrim(
                zfText("[zfl_call] no such method \"%s\" in class \"%s\""),
                methodSig.cString(),
                obj.toObject()->classData()->className());
            return ZFImpl_ZFLua_luaError(L);
        }
    } while(zffalse);

    return _ZFP_ZFImpl_ZFLua_zfl_call_invoker(L, paramList, paramCount, methodList, obj);
}

/*
 * zfl_callStatic("[Scope]::classInstanceMethodName", param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_callStatic(ZF_IN lua_State *L)
{
    static const zfint luaParamOffset = 1;
    zfint count = (zfint)lua_gettop(L);
    if(count < luaParamOffset || count > ZFMETHOD_MAX_PARAM + luaParamOffset)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[zfl_callStatic] invalid param, expect zfl_callStatic(\"Scope::methodName\", param0, param1, ...), got %zi param"),
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }
    zfint paramCount = count - luaParamOffset;

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
    for(zfint i = 0; i < paramCount; ++i)
    {
        if(!ZFImpl_ZFLua_toObject(paramList[i], L, luaParamOffset + i + 1))
        {
            zfblockedAlloc(ZFImpl_ZFLua_UnknownParam, t);
            if(!ZFImpl_ZFLua_toString(t->zfv, L, luaParamOffset + i + 1))
            {
                ZFLuaErrorOccurredTrim(zfText("[zfl_callStatic] failed to get param%d, expect zfautoObject, got %s, while executing: %s"),
                    i,
                    ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i + 1, zftrue).cString(),
                    ZFImpl_ZFLua_luaObjectInfo(L, 1).cString());
                return ZFImpl_ZFLua_luaError(L);
            }
            paramList[i] = t;
        }
    }

    ZFCoreArrayPOD<const ZFMethod *> methodList;
    do
    {
        zfautoObject methodHolder;
        if(ZFImpl_ZFLua_toObject(methodHolder, L, 1))
        {
            v_ZFMethod *methodWrapper = methodHolder;
            if(methodWrapper != zfnull)
            {
                if(methodWrapper->zfv == zfnull)
                {
                    ZFLuaErrorOccurredTrim(zfText("[zfl_callStatic] null method"));
                    return ZFImpl_ZFLua_luaError(L);
                }
                methodList.add(methodWrapper->zfv);
                break;
            }
        }

        zfstring methodSig;
        if(!ZFImpl_ZFLua_toString(methodSig, L, 1))
        {
            ZFLuaErrorOccurredTrim(zfText("[zfl_callStatic] failed to access function name, expect string type, got %s"),
                ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
            return ZFImpl_ZFLua_luaError(L);
        }

        ZFCoreArrayPOD<zfindexRange> methodSigPos;
        if(!ZFMethodSigSplit(methodSigPos, methodSig.cString(), methodSig.length()))
        {
            ZFLuaErrorOccurredTrim(
                zfText("[zfl_callStatic] failed to parse method sig \"%s\""),
                methodSig.cString());
            return ZFImpl_ZFLua_luaError(L);
        }

        zfstring methodScope(methodSig.cString() + methodSigPos[0].start, methodSigPos[0].count);
        _ZFP_ZFImpl_ZFLua_zfl_callStatic_methodScopeFix(methodScope);
        zfstring methodName(methodSig.cString() + methodSigPos[1].start, methodSigPos[1].count);
        const ZFClass *cls = ZFClass::classForName(methodScope);
        if(cls == zfnull)
        {
            zfstring classNameTmp = ZFImpl_ZFLua_PropTypePrefix;
            classNameTmp += methodScope;
            cls = ZFClass::classForName(classNameTmp);
        }

        _ZFP_ZFImpl_ZFLua_zfl_call_implDispatch(
            zftrue, methodScope, cls, zfnull,
            methodName)

        if(cls != zfnull)
        {
            cls->methodForNameGetAllT(methodList, methodName);
        }
        else
        {
            ZFMethodFuncGetAllT(methodList, methodScope, methodName);
        }

        if(methodList.isEmpty())
        {
            ZFLuaErrorOccurredTrim(
                zfText("[zfl_callStatic] no such method \"%s\""),
                methodSig.cString());
            return ZFImpl_ZFLua_luaError(L);
        }
    } while(zffalse);

    return _ZFP_ZFImpl_ZFLua_zfl_call_invoker(L, paramList, paramCount, methodList, zfnull);
}

/*
 * zfl_callStatic2("Scope", "methodName", param0, param1, ...)
 */
static int _ZFP_ZFImpl_ZFLua_zfl_callStatic2(ZF_IN lua_State *L)
{
    static zfint luaParamOffset = 2;
    zfint count = (zfint)lua_gettop(L);
    if(count < luaParamOffset || count > ZFMETHOD_MAX_PARAM + luaParamOffset)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[zfl_callStatic2] invalid param, expect zfl_callStatic2(\"Scope\", \"methodName\", param0, param1, ...), got %zi param"),
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }
    zfint paramCount = count - luaParamOffset;

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
    for(zfint i = 0; i < paramCount; ++i)
    {
        if(!ZFImpl_ZFLua_toObject(paramList[i], L, luaParamOffset + i + 1))
        {
            zfblockedAlloc(ZFImpl_ZFLua_UnknownParam, t);
            if(!ZFImpl_ZFLua_toString(t->zfv, L, luaParamOffset + i + 1))
            {
                ZFLuaErrorOccurredTrim(zfText("[zfl_callStatic2] failed to get param%d, expect zfautoObject, got %s, while executing: %s::%s"),
                    i,
                    ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i + 1, zftrue).cString(),
                    ZFImpl_ZFLua_luaObjectInfo(L, 1).cString(),
                    ZFImpl_ZFLua_luaObjectInfo(L, 2).cString());
                return ZFImpl_ZFLua_luaError(L);
            }
            paramList[i] = t;
        }
    }

    ZFCoreArrayPOD<const ZFMethod *> methodList;
    do
    {
        zfstring methodScope;
        if(!ZFImpl_ZFLua_toString(methodScope, L, 1))
        {
            ZFLuaErrorOccurredTrim(zfText("[zfl_callStatic2] failed to access method scope, expect string type, got %s"),
                ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
            return ZFImpl_ZFLua_luaError(L);
        }
        _ZFP_ZFImpl_ZFLua_zfl_callStatic_methodScopeFix(methodScope);

        zfstring methodName;
        if(!ZFImpl_ZFLua_toString(methodName, L, 2))
        {
            ZFLuaErrorOccurredTrim(zfText("[zfl_callStatic2] failed to access method name, expect string type, got %s"),
                ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue).cString());
            return ZFImpl_ZFLua_luaError(L);
        }

        const ZFClass *cls = ZFClass::classForName(methodScope);
        if(cls == zfnull)
        {
            zfstring classNameTmp = ZFImpl_ZFLua_PropTypePrefix;
            classNameTmp += methodScope;
            cls = ZFClass::classForName(classNameTmp);
        }

        _ZFP_ZFImpl_ZFLua_zfl_call_implDispatch(
            zftrue, methodScope, cls, zfnull,
            methodName)

        if(cls != zfnull)
        {
            cls->methodForNameGetAllT(methodList, methodName);
        }
        else
        {
            ZFMethodFuncGetAllT(methodList, methodScope, methodName);
        }

        if(methodList.isEmpty())
        {
            ZFLuaErrorOccurredTrim(
                zfText("[zfl_callStatic2] no such method \"%s::%s\""),
                methodScope.cString(),
                methodName.cString());
            return ZFImpl_ZFLua_luaError(L);
        }
    } while(zffalse);

    return _ZFP_ZFImpl_ZFLua_zfl_call_invoker(L, paramList, paramCount, methodList, zfnull);
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_call, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfl_call"), _ZFP_ZFImpl_ZFLua_zfl_call);
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfl_callStatic"), _ZFP_ZFImpl_ZFLua_zfl_callStatic);
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfl_callStatic2"), _ZFP_ZFImpl_ZFLua_zfl_callStatic2);
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

