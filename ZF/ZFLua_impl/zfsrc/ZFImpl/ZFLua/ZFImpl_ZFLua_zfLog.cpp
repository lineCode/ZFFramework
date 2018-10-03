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
#include "ZFImpl_ZFLua_PathInfo.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFImpl_ZFLua_zfLog_DEBUG_ENABLE ZF_ENV_DEBUG

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
    int count = (int)lua_gettop(L);
    if(count != 0)
    {
        ZFLuaErrorOccurredTrim(
            "[zfLogT] takes no param, got %zi param",
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
    int count = (int)lua_gettop(L);
    if(count != 0)
    {
        ZFLuaErrorOccurredTrim(
            "[zfLogTrimT] takes no param, got %zi param",
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
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfLog", _ZFP_ZFImpl_ZFLua_zfLog);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfLogTrim", _ZFP_ZFImpl_ZFLua_zfLogTrim);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfLogT", _ZFP_ZFImpl_ZFLua_zfLogT);
        ZFImpl_ZFLua_luaCFunctionRegister(L, "zfLogTrimT", _ZFP_ZFImpl_ZFLua_zfLogTrimT);
    }, {
    })

#if _ZFP_ZFImpl_ZFLua_zfLog_DEBUG_ENABLE
    ZFImpl_ZFLua_implPathInfo_DEFINE(zfLog,
            "function (f, ...)"
            "    return _G['zfLog']('[' .. tostring(zfl_l) .. ' (' .. debug.getinfo(2).currentline .. ')] ' .. (f or ''), ...);"
            "end"
        )
    ZFImpl_ZFLua_implPathInfo_DEFINE(zfLogT,
            "function ()"
            "    return _G['zfLogT']():log('[%s (%s)]', zfl_l, zfint(debug.getinfo(2).currentline));"
            "end"
        )
#else
    ZFImpl_ZFLua_implPathInfo_DEFINE(zfLog,
            "function (f, ...)"
            "    return _G['zfLog']('[' .. tostring(zfl_l) .. '] ' .. (f or ''), ...);"
            "end"
        )
    ZFImpl_ZFLua_implPathInfo_DEFINE(zfLogT,
            "function ()"
            "    return _G['zfLogT']():log('[%s]', zfl_l);"
            "end"
        )
#endif

ZFImpl_ZFLua_implDispatch_DEFINE(zfLogT_log, v_ZFCallback::ClassData()->classNameFull(), "log", {
        ZFImpl_ZFLua_implDispatch_AssertClassExist();
        ZFImpl_ZFLua_implDispatch_AssertParamCountRange(0, ZFMETHOD_MAX_PARAM);
        ZFImpl_ZFLua_implDispatch_AssertNotStaticMethod();
        if(dispatchInfo.paramCount == 0)
        {
            return dispatchInfo.dispatchSuccess();
        }
        zfstring fmt;
        if(!ZFImpl_ZFLua_toString(fmt, dispatchInfo.paramList[0], zftrue))
        {
            return dispatchInfo.dispatchError(
                "[zfLogT::log] unable to accss fmt from: %s",
                ZFObjectInfo(dispatchInfo.paramList[0]).cString());
        }
        ZFOutput output = ZFCastZFObject(v_ZFCallback *, dispatchInfo.objectOrNull)->zfv;
        zfstring s;
        zfstringAppend(s, fmt.cString()
                , ZFObjectInfo(dispatchInfo.paramList[1]).cString()
                , ZFObjectInfo(dispatchInfo.paramList[2]).cString()
                , ZFObjectInfo(dispatchInfo.paramList[3]).cString()
                , ZFObjectInfo(dispatchInfo.paramList[4]).cString()
                , ZFObjectInfo(dispatchInfo.paramList[5]).cString()
                , ZFObjectInfo(dispatchInfo.paramList[6]).cString()
                , ZFObjectInfo(dispatchInfo.paramList[7]).cString()
            );
        output.execute(s.cString(), s.length() * sizeof(zfchar));
        dispatchInfo.returnValue = dispatchInfo.objectOrNull;
        return dispatchInfo.dispatchSuccess();
    })

ZF_NAMESPACE_GLOBAL_END

