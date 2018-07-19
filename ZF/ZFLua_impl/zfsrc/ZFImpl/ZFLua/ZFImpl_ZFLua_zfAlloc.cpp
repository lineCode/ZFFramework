/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_ZFLua_zfAlloc.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static zfbool _ZFP_ZFImpl_ZFLua_zfAllocGeneric(ZF_OUT zfautoObject &ret,
                                               ZF_IN const ZFClass *cls,
                                               ZF_IN const zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM],
                                               ZF_IN zfindex paramCount);
zfbool ZFImpl_ZFLua_zfAlloc(ZF_OUT zfautoObject &ret,
                            ZF_IN lua_State *L,
                            ZF_IN const ZFClass *cls,
                            ZF_IN int paramCount,
                            ZF_IN int luaParamOffset)
{
    if(paramCount == 0)
    {
        ret = cls->newInstance();
        return zftrue;
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
    for(int i = 0; i < paramCount; ++i)
    {
        if(!ZFImpl_ZFLua_toGeneric(paramList[i], L, luaParamOffset + i))
        {
            return zffalse;
        }
    }
    return _ZFP_ZFImpl_ZFLua_zfAllocGeneric(
            ret,
            cls,
            paramList,
            paramCount
        );
}

// ============================================================
static zfbool _ZFP_ZFImpl_ZFLua_zfAllocGeneric(ZF_OUT zfautoObject &ret,
                                               ZF_IN const ZFClass *cls,
                                               ZF_IN const zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM],
                                               ZF_IN zfindex paramCount)
{
    ZFCoreArrayPOD<const ZFMethod *> objectOnInitMethodList;
    cls->methodForNameGetAllT(objectOnInitMethodList, zfText("objectOnInit"));
    if(objectOnInitMethodList.isEmpty())
    {
        return zffalse;
    }
    ZFToken token = cls->newInstanceGenericBegin();
    if(token == zfnull)
    {
        return zffalse;
    }

    for(zfindex iMethod = 0; iMethod < objectOnInitMethodList.count(); ++iMethod)
    {
        const ZFMethod *method = objectOnInitMethodList[iMethod];
        zfautoObject paramListTmp[ZFMETHOD_MAX_PARAM] = {
            paramList[0],
            paramList[1],
            paramList[2],
            paramList[3],
            paramList[4],
            paramList[5],
            paramList[6],
            paramList[7],
        };
        zfbool parseParamSuccess = zftrue;
        for(zfindex i = 0; i < paramCount && parseParamSuccess; ++i)
        {
            ZFImpl_ZFLua_UnknownParam *t = ZFCastZFObject(ZFImpl_ZFLua_UnknownParam *, paramListTmp[i].toObject());
            if(t != zfnull)
            {
                parseParamSuccess = ZFImpl_ZFLua_fromUnknown(paramListTmp[i], method->methodParamTypeIdAtIndex(i), t);
            }
        }
        if(!parseParamSuccess)
        {
            continue;
        }
        if(cls->newInstanceGenericCheck(token, method, paramListTmp))
        {
            ret = cls->newInstanceGenericEnd(token, zftrue);
            return zftrue;
        }
    }
    cls->newInstanceGenericEnd(token, zffalse);
    return zffalse;
}

static int _ZFP_ZFImpl_ZFLua_zfAlloc(ZF_IN lua_State *L)
{
    static const int luaParamOffset = 2;
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1)
    {
        ZFLuaErrorOccurredTrim(zfText("[zfAlloc] takes at least one param"));
        return ZFImpl_ZFLua_luaError(L);
    }
    int paramCount = (count - (luaParamOffset - 1));

    const ZFClass *cls = zfnull;

    zfautoObject clsHolder;
    if(ZFImpl_ZFLua_toObject(clsHolder, L, 1))
    {
        v_ZFClass *clsWrapper = clsHolder;
        if(clsWrapper != zfnull)
        {
            cls = clsWrapper->zfv;
            if(cls == zfnull)
            {
                ZFImpl_ZFLua_luaPush(L, zfautoObjectNull());
                return 1;
            }
        }
    }
    else
    {
        zfstring classNameFull;
        if(!ZFImpl_ZFLua_toString(classNameFull, L, 1))
        {
            ZFLuaErrorOccurredTrim(zfText("[zfAlloc] unknown param type: %s"),
                ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
            return ZFImpl_ZFLua_luaError(L);
        }
        cls = ZFClass::classForName(classNameFull);
        if(cls == zfnull)
        {
            zfindex dotPos = zfstringFindReversely(classNameFull, zfindexMax(), ZFNamespaceSeparator());
            if(dotPos == zfindexMax())
            {
                zfstring tmp = ZFImpl_ZFLua_PropTypePrefix;
                tmp += classNameFull;
                cls = ZFClass::classForName(tmp);
            }
            else
            {
                classNameFull.insert(dotPos + 1, ZFImpl_ZFLua_PropTypePrefix);
                cls = ZFClass::classForName(classNameFull);
            }
        }
    }
    if(cls == zfnull)
    {
        ZFImpl_ZFLua_luaPush(L, zfautoObjectNull());
        return 1;
    }
    if(paramCount == 0)
    {
        ZFImpl_ZFLua_luaPush(L, cls->newInstance());
        return 1;
    }

    zfautoObject ret;
    ZFImpl_ZFLua_zfAlloc(ret, L, cls, paramCount, luaParamOffset);
    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfAlloc, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfAlloc"), _ZFP_ZFImpl_ZFLua_zfAlloc);
    }, {
    })

// ============================================================
ZFImpl_ZFLua_implDispatch_DEFINE(ZFClass_zfAlloc, ZFImpl_ZFLua_implDispatchAll, zfText("zfAlloc"), {
        ZFImpl_ZFLua_implDispatch_AssertClassExist();
        ZFImpl_ZFLua_implDispatch_AssertParamCountRange(0, ZFMETHOD_MAX_PARAM);
        ZFImpl_ZFLua_implDispatch_AssertIsStaticMethod();

        if(dispatchInfo.paramCount == 0)
        {
            dispatchInfo.returnValue = dispatchInfo.classOrNull->newInstance();
            if(dispatchInfo.returnValue == zfnull)
            {
                return dispatchInfo.dispatchError(zfText("unable to create %s"),
                    dispatchInfo.classOrNull->classNameFull());
            }
            return dispatchInfo.dispatchSuccess();
        }
        else
        {
            if(_ZFP_ZFImpl_ZFLua_zfAllocGeneric(
                dispatchInfo.returnValue,
                dispatchInfo.classOrNull,
                dispatchInfo.paramList,
                dispatchInfo.paramCount))
            {
                return dispatchInfo.dispatchSuccess();
            }
            else
            {
                zfstring paramHint;
                for(zfindex i = 0; i < dispatchInfo.paramCount; ++i)
                {
                    if(i != 0)
                    {
                        paramHint += zfText(", ");
                    }
                    ZFObjectInfoT(paramHint, dispatchInfo.paramList[i].toObject());
                }
                return dispatchInfo.dispatchError(
                    zfText("unable to create class %s, no matching objectOnInit with params: %s"),
                    dispatchInfo.classOrNull->classNameFull(),
                    paramHint.cString());
            }
        }
    })

ZF_NAMESPACE_GLOBAL_END

