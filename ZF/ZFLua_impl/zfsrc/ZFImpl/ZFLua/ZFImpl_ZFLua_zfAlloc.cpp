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

static int _ZFP_ZFImpl_ZFLua_zfAlloc(ZF_IN lua_State *L)
{
    zfint count = (zfint)lua_gettop(L);
    if(count != 1)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[zfAlloc] takes only one param, got %zi"),
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }

    {
        zfautoObject clsHolder;
        if(ZFImpl_ZFLua_toObject(clsHolder, L, 1))
        {
            v_ZFClass *clsWrapper = ZFCastZFObject(v_ZFClass *, clsHolder);
            if(clsWrapper != zfnull)
            {
                if(clsWrapper->zfv == zfnull)
                {
                    ZFImpl_ZFLua_luaPush(L, zfautoObjectNull());
                    return 1;
                }
                ZFImpl_ZFLua_luaPush(L, clsWrapper->zfv->newInstance());
                return 1;
            }
        }
    }

    zfstring className;
    if(!ZFImpl_ZFLua_toString(className, L, 1))
    {
        ZFLuaErrorOccurredTrim(zfText("[zfAlloc] unknown param type: %s"),
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    const ZFClass *cls = ZFClass::classForName(className);
    if(cls == zfnull)
    {
        zfstring classNameTmp = ZFImpl_ZFLua_PropTypePrefix;
        classNameTmp += className;
        cls = ZFClass::classForName(classNameTmp);
    }
    if(cls == zfnull)
    {
        ZFImpl_ZFLua_luaPush(L, zfautoObjectNull());
        return 1;
    }
    if(cls->classIsAbstract())
    {
        ZFImpl_ZFLua_luaPush(L, zfautoObjectNull());
        return 1;
    }
    zfautoObject ret = cls->newInstance();
    if(ret == zfautoObjectNull())
    {
        ZFImpl_ZFLua_luaPush(L, zfautoObjectNull());
        return 1;
    }
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
        ZFImpl_ZFLua_implDispatch_AssertParamCount(0);
        ZFImpl_ZFLua_implDispatch_AssertIsStaticMethod();

        dispatchInfo.returnValue = dispatchInfo.classOrNull->newInstance();
        return dispatchInfo.dispatchSuccess();
    })

ZF_NAMESPACE_GLOBAL_END

