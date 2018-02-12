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
 * @file ZFLuaState.h
 * @brief native lua state impl
 */

#ifndef _ZFI_ZFLuaState_h_
#define _ZFI_ZFLuaState_h_

#include "ZFLuaDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief global native lua state currently used by #ZFLuaExecute,
 *   ensured not null
 *
 * ZFFramework can be used in any native lua state (by using #ZFLuaExecute),
 * but only if you have attached them\n
 * by default #ZFLuaState would have a builtin state that already setup properly,
 * while you can attach your own native lua state to ZFFramework,
 * by #ZFLuaStateAttach/#ZFLuaStateDetach\n
 * the builtin lua state can also be changed by #ZFLuaStateChange
 */
ZFMETHOD_FUNC_DECLARE_0(void *, ZFLuaState)

/** @brief see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_1(void, ZFLuaStateChange,
                        ZFMP_IN(void *, L))

/** @brief see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_1(void, ZFLuaStateAttached,
                        ZFMP_OUT(ZFCoreArrayPOD<void *> &, ret))

/** @brief see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_1(void, ZFLuaStateAttach,
                        ZFMP_IN(void *, L))

/** @brief see #ZFLuaState */
ZFMETHOD_FUNC_DECLARE_1(void, ZFLuaStateDetach,
                        ZFMP_IN(void *, L))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLuaState_h_

