/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFLuaState.h"
#include "protocol/ZFProtocolZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_0(void *, ZFLuaState)
{
    return ZFPROTOCOL_ACCESS(ZFLua)->luaState();
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateChange,
                       ZFMP_IN(void *, L))
{
    ZFPROTOCOL_ACCESS(ZFLua)->luaStateChange(L);
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateAttached,
                       ZFMP_OUT(ZFCoreArrayPOD<void *> &, ret))
{
    ZFPROTOCOL_ACCESS(ZFLua)->luaStateList(ret);
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateAttach,
                       ZFMP_IN(void *, L))
{
    ZFPROTOCOL_ACCESS(ZFLua)->luaStateAttach(L);
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaStateDetach,
                       ZFMP_IN(void *, L))
{
    ZFPROTOCOL_ACCESS(ZFLua)->luaStateDetach(L);
}

ZF_NAMESPACE_GLOBAL_END

