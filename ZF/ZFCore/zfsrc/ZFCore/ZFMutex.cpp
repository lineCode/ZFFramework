/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFMutex
ZFOBJECT_REGISTER(ZFMutex)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFMutex, void, mutexLock)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFMutex, zfbool, mutexTryLock)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFMutex, void, mutexUnlock)

ZF_NAMESPACE_GLOBAL_END

