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

static int _ZFP_ZFImpl_ZFLua_ZFCoreArrayCreate(ZF_IN lua_State *L)
{
    zfblockedAlloc(v_ZFCoreArray, ret);
    int count = (int)lua_gettop(L);

    for(int i = 0; i < count; ++i)
    {
        zfautoObject p;
        if(ZFImpl_ZFLua_toObject(p, L, i + 1)
            || ZFImpl_ZFLua_toNumberT(p, L, i + 1, zftrue)
            || ZFImpl_ZFLua_toCallback(p, L, i + 1))
        {
            ret->zfv.add(p);
            continue;
        }

        zfblockedAlloc(v_zfstring, pTmp);
        if(ZFImpl_ZFLua_toString(pTmp->zfv, L, i + 1, zftrue))
        {
            ret->zfv.add(pTmp);
            continue;
        }

        ZFLuaErrorOccurredTrim("[ZFCoreArrayCreate] unknown param type: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, i + 1, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(ZFCoreArrayCreate, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, "ZFCoreArrayCreate", _ZFP_ZFImpl_ZFLua_ZFCoreArrayCreate);
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

