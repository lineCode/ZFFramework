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
static int _ZFP_ZFImpl_ZFLua_ZFTypeIdWrapper(ZF_IN lua_State *L)
{
    zfint count = (zfint)lua_gettop(L);
    zfstring className;
    if(!ZFImpl_ZFLua_toString(className, L, 1))
    {
        ZFLuaErrorOccurredTrim(zfText("[ZFTypeIdWrapper] unable to access class name, expect string type, got %s"),
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }
    const ZFClass *cls = ZFClass::classForName(className);
    if(cls == zfnull)
    {
        ZFLuaErrorOccurredTrim(zfText("[ZFTypeIdWrapper] no such class %s"), className.cString());
        return ZFImpl_ZFLua_luaError(L);
    }
    else if(!cls->classIsTypeOf(ZFTypeIdWrapper::ClassData()))
    {
        ZFLuaErrorOccurredTrim(zfText("[ZFTypeIdWrapper] class %s is not type of %s"),
            className.cString(),
            ZFTypeIdWrapper::ClassData()->className());
        return ZFImpl_ZFLua_luaError(L);
    }

    const zfindex paramCount = count - 1;
    if(paramCount == 0)
    {
        ZFImpl_ZFLua_luaPush(L, cls->newInstance());
        return 1;
    }
    static const zfint luaParamOffset = 1;

    ZFCoreArrayPOD<const ZFMethod *> methodList = cls->methodForNameGetAll(zfText("objectOnInit"));
    if(!methodList.isEmpty())
    {
        zfautoObject paramList_[ZFMETHOD_MAX_PARAM] = {
                  ZFMethodGenericInvokerDefaultParamHolder()
                , ZFMethodGenericInvokerDefaultParamHolder()
                , ZFMethodGenericInvokerDefaultParamHolder()
                , ZFMethodGenericInvokerDefaultParamHolder()
                , ZFMethodGenericInvokerDefaultParamHolder()
                , ZFMethodGenericInvokerDefaultParamHolder()
                , ZFMethodGenericInvokerDefaultParamHolder()
                , ZFMethodGenericInvokerDefaultParamHolder()
            };
        for(zfindex i = 0; i < paramCount; ++i)
        {
            if(!ZFImpl_ZFLua_toObject(paramList_[i], L, luaParamOffset + i + 1))
            {
                zfblockedAlloc(ZFImpl_ZFLua_UnknownParam, t);
                if(!ZFImpl_ZFLua_toString(t->zfv, L, luaParamOffset + i + 1))
                {
                    ZFLuaErrorOccurredTrim(zfText("[v_%s] failed to get param%d, expect zfautoObject, got %s"),
                        cls->className(),
                        i,
                        ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i + 1, zftrue).cString());
                    return ZFImpl_ZFLua_luaError(L);
                }
                paramList_[i] = t;
            }
        }
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

            zfautoObject paramList[ZFMETHOD_MAX_PARAM] = {
                paramList_[0],
                paramList_[1],
                paramList_[2],
                paramList_[3],
                paramList_[4],
                paramList_[5],
                paramList_[6],
                paramList_[7],
            };
            zfbool parseParamSuccess = zftrue;
            for(zfindex i = 0; i < paramCount; ++i)
            {
                ZFImpl_ZFLua_UnknownParam *t = ZFCastZFObject(ZFImpl_ZFLua_UnknownParam *, paramList[i].toObject());
                if(t != zfnull)
                {
                    const ZFTypeIdBase *typeIdData = ZFTypeIdGet(method->methodParamTypeIdAtIndex(i));
                    if(typeIdData == zfnull || !typeIdData->typeIdWrapper(paramList[i]))
                    {
                        parseParamSuccess = zffalse;
                        break;
                    }

                    if(paramList[i] == zfnull)
                    {
                        if(!ZFObjectFromString(paramList[i], t->zfv, t->zfv.length()))
                        {
                            parseParamSuccess = zffalse;
                        }
                    }
                    else if(!paramList[i].to<ZFTypeIdWrapper *>()->wrappedValueFromString(t->zfv, t->zfv.length()))
                    {
                        parseParamSuccess = zffalse;
                    }

                    if(!parseParamSuccess)
                    {
                        break;
                    }
                }
            }
            if(!parseParamSuccess)
            {
                continue;
            }

            zfautoObject ret = cls->newInstanceGenericWithMethod(method
                    , paramList[0]
                    , paramList[1]
                    , paramList[2]
                    , paramList[3]
                    , paramList[4]
                    , paramList[5]
                    , paramList[6]
                    , paramList[7]
                );
            if(ret != zfnull)
            {
                ZFImpl_ZFLua_luaPush(L, ret);
                return 1;
            }
        }
    }

    zfstring paramsInfo;
    for(zfindex i = 0; i < paramCount; ++i)
    {
        if(i > 0)
        {
            paramsInfo += zfText(", ");
        }
        zfstringAppend(paramsInfo, zfText("%s"),
            ZFImpl_ZFLua_luaObjectInfo(L, luaParamOffset + i + 1, zftrue).cString());
    }
    ZFLuaErrorOccurredTrim(zfText("[v_%s] no matching objectOnInit to call, params: %s"),
        cls->className(),
        paramsInfo.cString());
    return ZFImpl_ZFLua_luaError(L);
}
static void _ZFP_ZFImpl_ZFLua_ZFTypeIdWrapperSetup(ZF_IN lua_State *L, ZF_IN const ZFClass *cls)
{
    if(zfsncmp(cls->className(), ZFImpl_ZFLua_PropTypePrefix, ZFImpl_ZFLua_PropTypePrefixLen) != 0)
    {
        return ;
    }

    zfstring code;
    zfstringAppend(code, zfText(
            "function %s(...)\n"
            "    return _ZFP_ZFImpl_ZFLua_ZFTypeIdWrapper('%s', ...)\n"
            "end\n"
        ), cls->className() + ZFImpl_ZFLua_PropTypePrefixLen, cls->className());
    ZFImpl_ZFLua_execute(L, code);
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFImpl_ZFLua_ZFTypeIdWrapper_classOnChange)
{
    const ZFClassDataChangeData *data = listenerData.param0->to<ZFPointerHolder *>()->holdedDataPointer<const ZFClassDataChangeData *>();
    if(data->changedClass != zfnull && data->changeType == ZFClassDataChangeTypeAttach)
    {
        if(data->changedClass->classIsTypeOf(ZFTypeIdWrapper::ClassData()))
        {
            const ZFCoreArrayPOD<lua_State *> &luaStateList = ZFImpl_ZFLua_luaStateAttached();
            for(zfindex i = 0; i < luaStateList.count(); ++i)
            {
                _ZFP_ZFImpl_ZFLua_ZFTypeIdWrapperSetup(luaStateList[i], data->changedClass);
            }
        }
    }
}
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFTypeIdWrapper, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("_ZFP_ZFImpl_ZFLua_ZFTypeIdWrapper"), _ZFP_ZFImpl_ZFLua_ZFTypeIdWrapper);

        ZFCoreArrayPOD<const ZFClass *> allClass = ZFClassGetAll();
        if(!allClass.isEmpty())
        {
            const ZFClass *target = ZFTypeIdWrapper::ClassData();
            for(zfindex i = 0; i < allClass.count(); ++i)
            {
                const ZFClass *cls = allClass[i];
                if(cls != target && cls->classIsTypeOf(target))
                {
                    _ZFP_ZFImpl_ZFLua_ZFTypeIdWrapperSetup(L, cls);
                }
            }
        }

        ZFClassDataChangeObserver.observerAdd(
            ZFGlobalEvent::EventClassDataChange(),
            ZFCallbackForFunc(_ZFP_ZFImpl_ZFLua_ZFTypeIdWrapper_classOnChange));
    }, {
        ZFClassDataChangeObserver.observerRemove(
            ZFGlobalEvent::EventClassDataChange(),
            ZFCallbackForFunc(_ZFP_ZFImpl_ZFLua_ZFTypeIdWrapper_classOnChange));
    })

ZF_NAMESPACE_GLOBAL_END

