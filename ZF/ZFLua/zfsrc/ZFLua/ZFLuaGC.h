/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
/**
 * @file ZFLuaGC.h
 * @brief perform garbage collect in lua
 */

#ifndef _ZFI_ZFLuaGC_h_
#define _ZFI_ZFLuaGC_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief performa garbage collector in lua immediately,
 *   use #ZFLuaGC for performance
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFLuaGCImmediately,
                        ZFMP_IN_OPT(void *, L, zfnull))

/**
 * @brief schedule #ZFLuaGC to run at proper time
 *
 * @note #ZFLuaGC would be called each time #ZFGlobalEvent::EventClassDataChange notified
 *   when class detach,
 *   to all lua state attached #ZFLuaStateListT
 * @note #ZFLuaGC would be called after each #ZFLuaExecute
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFLuaGC,
                        ZFMP_IN_OPT(void *, L, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLuaGC_h_

