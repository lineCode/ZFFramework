/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFCoreMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void *_ZFP_ZFCoreMutexImplObject = zfnull;
ZFCoreMutexImplCallbackLock _ZFP_ZFCoreMutexImplLock = zfnull;
ZFCoreMutexImplCallbackUnlock _ZFP_ZFCoreMutexImplUnlock = zfnull;

ZF_NAMESPACE_GLOBAL_END

