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

static int _ZFP_ZFImpl_ZFLua_ZFCallbackForLua(ZF_IN lua_State *L)
{
    int count = (int)lua_gettop(L);
    if(count != 1)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[ZFCallbackForLua] expect one param, got %zi"),
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }

    zfautoObject ret;
    if(!ZFImpl_ZFLua_toCallback(ret, L, 1))
    {
        ZFLuaErrorOccurredTrim(
            zfText("[ZFCallbackForLua] unable to access the callback, got %s"),
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }
    else
    {
        ZFImpl_ZFLua_luaPush(L, ret);
        return 1;
    }
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFCallbackForLua, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("ZFCallbackForLua"), _ZFP_ZFImpl_ZFLua_ZFCallbackForLua);
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

