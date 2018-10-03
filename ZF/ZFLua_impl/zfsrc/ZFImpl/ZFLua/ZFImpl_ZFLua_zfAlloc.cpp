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
    return ZFDI_alloc(ret, zfnull, cls, (zfindex)paramCount
            , paramList[0]
            , paramList[1]
            , paramList[2]
            , paramList[3]
            , paramList[4]
            , paramList[5]
            , paramList[6]
            , paramList[7]
        );
}

// ============================================================
static int _ZFP_ZFImpl_ZFLua_zfAlloc(ZF_IN lua_State *L)
{
    static const int luaParamOffset = 2;
    int count = (int)lua_gettop(L);
    if(count < luaParamOffset - 1)
    {
        ZFLuaErrorOccurredTrim("[zfAlloc] takes at least one param");
        return ZFImpl_ZFLua_luaError(L);
    }
    int paramCount = (count - (luaParamOffset - 1));

    zfautoObject clsHolder;
    if(!ZFImpl_ZFLua_toGeneric(clsHolder, L, 1))
    {
        ZFLuaErrorOccurredTrim("[zfAlloc] unable to access class");
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
    for(int i = 0; i < paramCount; ++i)
    {
        if(!ZFImpl_ZFLua_toGeneric(paramList[i], L, luaParamOffset + i))
        {
            return zffalse;
        }
    }

    zfautoObject ret;
    ZFDI_alloc(ret, zfnull, zfnull, clsHolder, (zfindex)paramCount
            , paramList[0]
            , paramList[1]
            , paramList[2]
            , paramList[3]
            , paramList[4]
            , paramList[5]
            , paramList[6]
            , paramList[7]
        );
    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfAlloc, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfAlloc", _ZFP_ZFImpl_ZFLua_zfAlloc);
    }, {
    })

// ============================================================
ZFImpl_ZFLua_implDispatch_DEFINE(ZFClass_zfAlloc, ZFImpl_ZFLua_implDispatchAll, "zfAlloc", {
        ZFImpl_ZFLua_implDispatch_AssertClassExist();
        ZFImpl_ZFLua_implDispatch_AssertParamCountRange(0, ZFMETHOD_MAX_PARAM);
        ZFImpl_ZFLua_implDispatch_AssertIsStaticMethod();

        if(dispatchInfo.paramCount == 0)
        {
            dispatchInfo.returnValue = dispatchInfo.classOrNull->newInstance();
            if(dispatchInfo.returnValue == zfnull)
            {
                return dispatchInfo.dispatchError("unable to create %s",
                    dispatchInfo.classOrNull->classNameFull());
            }
            return dispatchInfo.dispatchSuccess();
        }
        else
        {
            zfstring errorHint;
            if(!ZFDI_alloc(dispatchInfo.returnValue, &errorHint, dispatchInfo.classOrNull, dispatchInfo.paramCount
                    , dispatchInfo.paramList[0]
                    , dispatchInfo.paramList[1]
                    , dispatchInfo.paramList[2]
                    , dispatchInfo.paramList[3]
                    , dispatchInfo.paramList[4]
                    , dispatchInfo.paramList[5]
                    , dispatchInfo.paramList[6]
                    , dispatchInfo.paramList[7]
                ))
            {
                return dispatchInfo.dispatchError(
                    "unable to create class %s, reason: %s",
                    dispatchInfo.classOrNull->classNameFull(),
                    errorHint.cString());
            }
            else
            {
                return dispatchInfo.dispatchSuccess();
            }
        }
    })

ZF_NAMESPACE_GLOBAL_END

