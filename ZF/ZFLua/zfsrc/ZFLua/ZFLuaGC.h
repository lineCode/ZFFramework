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
 * @brief performa garbage collector in lua
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFLuaGC,
                        ZFMP_IN_OPT(void *, L, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLuaGC_h_

