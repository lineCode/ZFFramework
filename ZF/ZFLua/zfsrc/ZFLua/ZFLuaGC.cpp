/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFLuaGC.h"
#include "ZFLuaState.h"
#include "protocol/ZFProtocolZFLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(void, ZFLuaGC,
                       ZFMP_IN_OPT(void *, L, zfnull))
{
    ZFPROTOCOL_ACCESS(ZFLua)->luaGC(L ? L : ZFLuaState());
}

ZF_NAMESPACE_GLOBAL_END

