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
static int _ZFP_ZFImpl_ZFLua_zfl_luaValue(ZF_IN lua_State *L)
{
    int count = (int)lua_gettop(L);
    if(count != 1)
    {
        ZFLuaErrorOccurredTrim(
            "[zfl_luaValue] takes only one param, got %zi",
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }

    if(!lua_isuserdata(L, 1) && lua_islightuserdata(L, 1))
    {
        lua_pushvalue(L, 1);
        return 1;
    }

    return ZFImpl_ZFLua_toLuaValue(L, ZFImpl_ZFLua_luaGet(L, 1));
}

ZFImpl_ZFLua_implSetupCallback_DEFINE(zfl_luaValue, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfl_luaValue", _ZFP_ZFImpl_ZFLua_zfl_luaValue);
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

