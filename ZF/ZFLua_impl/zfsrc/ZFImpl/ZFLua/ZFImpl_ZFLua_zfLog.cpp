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

static int _ZFP_ZFImpl_ZFLua_zfLog(ZF_IN lua_State *L)
{
    zfstring s;
    if(ZFImpl_ZFLua_zfstringAppend(L, s))
    {
        zfLogTrimT() << zfLogCurTimeString() << s;
    }
    return 0;
}

static int _ZFP_ZFImpl_ZFLua_zfLogTrim(ZF_IN lua_State *L)
{
    zfstring s;
    if(ZFImpl_ZFLua_zfstringAppend(L, s))
    {
        zfLogTrimT() << s;
    }
    return 0;
}

static int _ZFP_ZFImpl_ZFLua_zfLogT(ZF_IN lua_State *L)
{
    zfint count = (zfint)lua_gettop(L);
    if(count != 0)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[zfLogT] takes no param, got %zi param"),
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }

    zfblockedAlloc(v_ZFCallback, ret);
    ret->zfv = (zfLogTrimT() << zfLogCurTimeString());
    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

static int _ZFP_ZFImpl_ZFLua_zfLogTrimT(ZF_IN lua_State *L)
{
    zfint count = (zfint)lua_gettop(L);
    if(count != 0)
    {
        ZFLuaErrorOccurredTrim(
            zfText("[zfLogTrimT] takes no param, got %zi param"),
            (zfindex)count);
        return ZFImpl_ZFLua_luaError(L);
    }

    zfblockedAlloc(v_ZFCallback, ret);
    ret->zfv = zfLogTrimT();
    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfLog, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfLog"), _ZFP_ZFImpl_ZFLua_zfLog);
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfLogTrim"), _ZFP_ZFImpl_ZFLua_zfLogTrim);
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfLogT"), _ZFP_ZFImpl_ZFLua_zfLogT);
        ZFImpl_ZFLua_luaCFunctionRegister(L, zfText("zfLogTrimT"), _ZFP_ZFImpl_ZFLua_zfLogTrimT);
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

