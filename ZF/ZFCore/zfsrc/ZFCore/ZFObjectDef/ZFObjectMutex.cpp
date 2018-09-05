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

ZFObjectMutexImplCallbackInit _ZFP_ZFObjectMutexImplInit = zfnull;
ZFObjectMutexImplCallbackDealloc _ZFP_ZFObjectMutexImplDealloc = zfnull;
ZFObjectMutexImplCallbackLock _ZFP_ZFObjectMutexImplLock = zfnull;
ZFObjectMutexImplCallbackUnlock _ZFP_ZFObjectMutexImplUnlock = zfnull;
ZFObjectMutexImplCallbackTryLock _ZFP_ZFObjectMutexImplTryLock = zfnull;

// ============================================================
void ZFObjectMutexImplSet(ZF_IN_OPT ZFObjectMutexImplCallbackInit implInit /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplCallbackDealloc implDealloc /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplCallbackLock implLock /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplCallbackUnlock implUnlock /* = zfnull */,
                          ZF_IN_OPT ZFObjectMutexImplCallbackTryLock implTryLock /* = zfnull */)
{
    if(implInit == zfnull && _ZFP_ZFObjectMutexImplInit != zfnull)
    {
        _ZFP_ZFObjectMutexImplDealloc(ZFCoreMutexImplGetObject());
        ZFCoreMutexImplSet(zfnull, zfnull, zfnull);
     }

    _ZFP_ZFObjectMutexImplInit = implInit;
    _ZFP_ZFObjectMutexImplDealloc = implDealloc;
    _ZFP_ZFObjectMutexImplLock = implLock;
    _ZFP_ZFObjectMutexImplUnlock = implUnlock;
    _ZFP_ZFObjectMutexImplTryLock = implTryLock;

    if(_ZFP_ZFObjectMutexImplInit != zfnull)
    {
        void *implObject = _ZFP_ZFObjectMutexImplInit();
        ZFCoreMutexImplSet(implObject, implLock, implUnlock);
    }
}

ZF_NAMESPACE_GLOBAL_END

