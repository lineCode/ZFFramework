/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFObjectMutex.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFObjectMutexImplCheckCallback _ZFP_ZFObjectMutexImplCheckCallbackRef = zfnull;
ZFObjectMutexImplInitCallback _ZFP_ZFObjectMutexImplInitCallbackRef = zfnull;
ZFObjectMutexImplCleanupCallback _ZFP_ZFObjectMutexImplCleanupCallbackRef = zfnull;
void ZFObjectMutexImplSet(ZF_IN_OPT ZFObjectMutexImplCheckCallback checkCallback /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplInitCallback initCallback /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplCleanupCallback cleanupCallback /* = zfnull */)
{
    _ZFP_ZFObjectMutexImplCheckCallbackRef = checkCallback;
    _ZFP_ZFObjectMutexImplInitCallbackRef = initCallback;
    _ZFP_ZFObjectMutexImplCleanupCallbackRef = cleanupCallback;
}

ZF_NAMESPACE_GLOBAL_END

