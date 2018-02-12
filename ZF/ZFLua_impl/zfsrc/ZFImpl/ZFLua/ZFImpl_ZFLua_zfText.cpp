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

static int _ZFP_ZFImpl_ZFLua_zfText(ZF_IN lua_State *L)
{
    zfint count = (zfint)lua_gettop(L);
    if(count != 1)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[zfText] takes only one param, got %zi"),
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }

    zfblockedAlloc(v_zfstring, ret);
    if(!ZFImpl_ZFLua_toString(ret->zfv, L, -1, zftrue))
    {
        ZFLuaErrorOccurredTrim(
            zfText("[zfText] unable to access param as string: %s"),
            ZFImpl_ZFLua_luaObjectInfo(L, -1).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    zfautoObject t = ret;
    ZFImpl_ZFLua_luaPush(L, t);
    return 1;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfText, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfText"), _ZFP_ZFImpl_ZFLua_zfText);
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

