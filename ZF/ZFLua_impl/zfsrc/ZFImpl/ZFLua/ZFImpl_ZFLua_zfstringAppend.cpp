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

static int _ZFP_ZFImpl_ZFLua_zfstringAppend(ZF_IN lua_State *L)
{
    zfautoObject obj;
    if(!ZFImpl_ZFLua_toObject(obj, L, 1))
    {
        ZFLuaErrorOccurredTrim("[zfstringAppend] unknown string type, got %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    zfbool stringValid = zffalse;
    zfbool convertSuccess = zffalse;

    if(!stringValid)
    {
        v_zfstring *s = obj;
        if(s != zfnull)
        {
            stringValid = zftrue;
            convertSuccess = ZFImpl_ZFLua_zfstringAppend(L, s->zfv, 2);
        }
    }

    if(!stringValid)
    {
        ZFStringEditable *s = obj;
        if(s != zfnull)
        {
            stringValid = zftrue;
            zfstring tmp = s->stringValue();
            convertSuccess = ZFImpl_ZFLua_zfstringAppend(L, tmp, 2);
            if(convertSuccess)
            {
                s->stringValueSet(tmp);
            }
        }
    }

    if(!stringValid)
    {
        ZFLuaErrorOccurredTrim("[zfstringAppend] unknown string type or not editable, got %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    if(!convertSuccess)
    {
        ZFLuaErrorOccurredTrim("[zfstringAppend] format string failed, format: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 2, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }

    return 0;
}

static int _ZFP_ZFImpl_ZFLua_zfstringWithFormat(ZF_IN lua_State *L)
{
    zfblockedAlloc(v_zfstring, ret);
    if(!ZFImpl_ZFLua_zfstringAppend(L, ret->zfv))
    {
        ZFLuaErrorOccurredTrim("[zfstringWithFormat] format string failed, format: %s",
            ZFImpl_ZFLua_luaObjectInfo(L, 1, zftrue).cString());
        return ZFImpl_ZFLua_luaError(L);
    }
    ZFImpl_ZFLua_luaPush(L, ret);
    return 1;
}

// ============================================================
ZFImpl_ZFLua_implSetupCallback_DEFINE(zfstringAppend, {
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfstringAppend", _ZFP_ZFImpl_ZFLua_zfstringAppend);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfstringWithFormat", _ZFP_ZFImpl_ZFLua_zfstringWithFormat);
    }, {
    })

ZF_NAMESPACE_GLOBAL_END

