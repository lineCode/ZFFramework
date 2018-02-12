/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl.h"
#include "ZFLua/protocol/ZFProtocolZFLua.h"
#include "ZFImpl_ZFLua.h"
#include "ZFImpl_ZFLua_PathInfo.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFLua_impl, ZFLua, ZFProtocolLevel::e_Default)
public:
    virtual void *luaState(void)
    {
        return ZFImpl_ZFLua_luaState;
    }

    virtual void luaStateChange(ZF_IN void *L)
    {
        ZFImpl_ZFLua_luaStateChange((lua_State *)L);
    }

    virtual void luaStateList(ZF_OUT ZFCoreArrayPOD<void *> &ret)
    {
        typedef void *_T;
        ret.addFrom((const _T *)ZFImpl_ZFLua_luaStateAttached().arrayBuf(), ZFImpl_ZFLua_luaStateAttached().count());
    }

    virtual void luaStateAttach(ZF_IN void *L)
    {
        ZFImpl_ZFLua_luaStateAttach((lua_State *)L);
    }

    virtual void luaStateDetach(ZF_IN void *L)
    {
        ZFImpl_ZFLua_luaStateDetach((lua_State *)L);
    }

    virtual zfbool luaExecute(ZF_IN void *L,
                              ZF_IN const ZFPathInfo *pathInfoOrNull,
                              ZF_IN const ZFInputCallback &input,
                              ZF_OUT_OPT zfautoObject *luaResult = zfnull,
                              ZF_IN_OPT const ZFCoreArray<zfautoObject> *luaParams = zfnull,
                              ZF_OUT_OPT zfstring *errorHint = zfnull)
    {
        if(ZFFrameworkStateCheck(ZFLevelZFFrameworkLow) != ZFFrameworkStateAvailable)
        {
            ZFLuaErrorOccurredTrim(
                zfText("[ZFLua] lua module only available after ZFFrameworkStateCheck(ZFLevelZFFrameworkLow)")
                );
            return zffalse;
        }

        zfstring buf;
        ZFImpl_ZFLua_implSetupPathInfo(buf, pathInfoOrNull);
        ZFInputCallbackReadToString(buf, input);
        return ZFImpl_ZFLua_execute((lua_State *)L, buf.cString(), buf.length(), luaResult, luaParams, errorHint);
    }

    virtual void luaGC(ZF_IN void *L)
    {
        lua_gc((lua_State *)L, LUA_GCCOLLECT, 0);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFLua_impl)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFLua_impl)

ZF_NAMESPACE_GLOBAL_END

