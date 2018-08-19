/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFOperationAsync.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFOperationAsyncPrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFOperationAsyncPrivate, ZFObject)

public:
    ZFOperationAsync *pimplOwner;
    ZFCoreArrayPOD<zfidentity> runningOperationId;
    ZFCoreArrayPOD<zfidentity> runningThreads;

public:
    ZFLISTENER_INLINE(onNotifyFinishInMainThread)
    {
        this->pimplOwner->_ZFP_ZFOperationAsync_taskNotifyFinishAsync(
            ZFCastZFObjectUnchecked(ZFOperationParam *, listenerData.param0),
            ZFCastZFObjectUnchecked(ZFOperationResult *, listenerData.param1));
    }
    ZFLISTENER_INLINE(onNotifyFinishInMainThreadWithOperationId)
    {
        this->pimplOwner->_ZFP_ZFOperationAsync_taskNotifyFinishAsync(
            ZFCastStatic(zfidentity, ZFCastZFObjectUnchecked(ZFValue *, listenerData.param0)->indexValue()),
            ZFCastZFObjectUnchecked(ZFOperationResult *, listenerData.param1));
    }
    ZFLISTENER_INLINE(onNotifyProgressInMainThread)
    {
        this->pimplOwner->_ZFP_ZFOperationAsync_taskNotifyProgressAsync(
            ZFCastZFObjectUnchecked(ZFOperationParam *, listenerData.param0),
            ZFCastZFObjectUnchecked(ZFOperationProgress *, listenerData.param1));
    }
    ZFLISTENER_INLINE(onNotifyProgressInMainThreadWithOperationId)
    {
        this->pimplOwner->_ZFP_ZFOperationAsync_taskNotifyProgressAsync(
            ZFCastStatic(zfidentity, ZFCastZFObjectUnchecked(ZFValue *, listenerData.param0)->indexValue()),
            ZFCastZFObjectUnchecked(ZFOperationProgress *, listenerData.param1));
    }
    ZFLISTENER_INLINE(onStartInNewThread)
    {
        this->pimplOwner->_ZFP_ZFOperationAsync_taskOnStartAsync(ZFCastZFObjectUnchecked(ZFOperationTaskData *, userData));
    }
};

ZFOBJECT_REGISTER(ZFOperationAsync)

ZFMETHOD_DEFINE_4(ZFOperationAsync, void, taskNotifyFinishAfterDelay,
                  ZFMP_IN(zftimet, delay),
                  ZFMP_IN(ZFOperation *, operation),
                  ZFMP_IN(ZFOperationParam *, operationParam),
                  ZFMP_IN(ZFOperationResult *, operationResult))
{
    zfautoObject operationTaskDataHolder = operation->createTaskData();
    ZFOperationTaskData *operationTaskData = operationTaskDataHolder.to<ZFOperationTaskData *>();
    operationTaskData->operationSet(operation);
    operationTaskData->operationParamSet(operationParam);
    operationTaskData->operationResultSet(operationResult);
    ZFLISTENER_LOCAL(onNotify, {
        ZFOperationTaskData *operationTaskData = ZFCastZFObjectUnchecked(ZFOperationTaskData *, userData);
        operationTaskData->operation()->taskNotifyFinish(operationTaskData->operationParam(), operationTaskData->operationResult());
    })
    ZFThreadExecuteInMainThreadAfterDelay(delay, onNotify, operationTaskData);
}
ZFMETHOD_DEFINE_4(ZFOperationAsync, void, taskNotifyFinishAfterDelay,
                  ZFMP_IN(zftimet, delay),
                  ZFMP_IN(ZFOperation *, operation),
                  ZFMP_IN(zfidentity, operationId),
                  ZFMP_IN(ZFOperationResult *, operationResult))
{
    zfautoObject operationTaskDataHolder = operation->createTaskData();
    ZFOperationTaskData *operationTaskData = operationTaskDataHolder.to<ZFOperationTaskData *>();
    operationTaskData->operationSet(operation);
    operationTaskData->operationIdSet(operationId);
    operationTaskData->operationResultSet(operationResult);
    ZFLISTENER_LOCAL(onNotify, {
        ZFOperationTaskData *operationTaskData = ZFCastZFObjectUnchecked(ZFOperationTaskData *, userData);
        operationTaskData->operation()->taskNotifyFinish(operationTaskData->operationId(), operationTaskData->operationResult());
    })
    ZFThreadExecuteInMainThreadAfterDelay(delay, onNotify, operationTaskData);
}
ZFMETHOD_DEFINE_4(ZFOperationAsync, void, taskNotifyProgressAfterDelay,
                  ZFMP_IN(zftimet, delay),
                  ZFMP_IN(ZFOperation *, operation),
                  ZFMP_IN(ZFOperationParam *, operationParam),
                  ZFMP_IN_OPT(ZFOperationProgress *, operationProgress, zfnull))
{
    zfautoObject operationTaskDataHolder = operation->createTaskData();
    ZFOperationTaskData *operationTaskData = operationTaskDataHolder.to<ZFOperationTaskData *>();
    operationTaskData->operationSet(operation);
    operationTaskData->operationParamSet(operationParam);
    operationTaskData->operationProgressSet(operationProgress);
    ZFLISTENER_LOCAL(onNotify, {
        ZFOperationTaskData *operationTaskData = ZFCastZFObjectUnchecked(ZFOperationTaskData *, userData);
        operationTaskData->operation()->taskNotifyProgress(operationTaskData->operationParam(), operationTaskData->operationProgress());
    })
    ZFThreadExecuteInMainThreadAfterDelay(delay, onNotify, operationTaskData);
}
ZFMETHOD_DEFINE_4(ZFOperationAsync, void, taskNotifyProgressAfterDelay,
                  ZFMP_IN(zftimet, delay),
                  ZFMP_IN(ZFOperation *, operation),
                  ZFMP_IN(zfidentity, operationId),
                  ZFMP_IN_OPT(ZFOperationProgress *, operationProgress, zfnull))
{
    zfautoObject operationTaskDataHolder = operation->createTaskData();
    ZFOperationTaskData *operationTaskData = operationTaskDataHolder.to<ZFOperationTaskData *>();
    operationTaskData->operationSet(operation);
    operationTaskData->operationIdSet(operationId);
    operationTaskData->operationProgressSet(operationProgress);
    ZFLISTENER_LOCAL(onNotify, {
        ZFOperationTaskData *operationTaskData = ZFCastZFObjectUnchecked(ZFOperationTaskData *, userData);
        operationTaskData->operation()->taskNotifyProgress(operationTaskData->operationId(), operationTaskData->operationProgress());
    })
    ZFThreadExecuteInMainThreadAfterDelay(delay, onNotify, operationTaskData);
}

void ZFOperationAsync::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfAlloc(_ZFP_ZFOperationAsyncPrivate);
    d->pimplOwner = this;
}
void ZFOperationAsync::objectOnDealloc(void)
{
    zfRelease(d);
    zfsuper::objectOnDealloc();
}
void ZFOperationAsync::objectOnDeallocPrepare(void)
{
    zfsynchronizeLock(this);
    ZFCoreArrayPOD<zfidentity> threadsToWait;
    threadsToWait.addFrom(d->runningThreads);
    zfsynchronizeUnlock(this);

    this->taskStopAll();

    for(zfindex i = 0; i < threadsToWait.count(); ++i)
    {
        ZFThreadExecuteWait(threadsToWait[i]);
    }

    zfsuper::objectOnDeallocPrepare();
}

void ZFOperationAsync::taskOnStart(ZF_IN ZFOperationTaskData *operationTaskData)
{
    zfsuper::taskOnStart(operationTaskData);

    zfidentity threadId = ZFThreadExecuteInNewThread(
        ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFOperationAsyncPrivate, onStartInNewThread)),
        operationTaskData);
    {
        zfsynchronizeLock(this);
        d->runningOperationId.add(operationTaskData->operationId());
        d->runningThreads.add(threadId);
        zfsynchronizeUnlock(this);
    }
}
void ZFOperationAsync::taskOnStop(ZF_IN ZFOperationTaskData *operationTaskData)
{
    zfsuper::taskOnStop(operationTaskData);
    this->taskOnStopAsync(operationTaskData);
    {
        zfsynchronize(this);
        for(zfindex i = 0; i < d->runningOperationId.count(); ++i)
        {
            if(d->runningOperationId[i] == operationTaskData->operationId())
            {
                d->runningOperationId.remove(i);
                d->runningThreads.remove(i);
                break;
            }
        }
    }
}

void ZFOperationAsync::taskNotifyFinish(ZF_IN ZFOperationParam *operationParam,
                                        ZF_IN ZFOperationResult *operationResult)
{
    ZFThreadExecuteInMainThread(
        ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFOperationAsyncPrivate, onNotifyFinishInMainThread)),
        zfnull,
        ZFListenerData()
            .param0Set(operationParam)
            .param1Set(operationResult)
        );
}
void ZFOperationAsync::taskNotifyFinish(ZF_IN zfidentity operationId,
                                        ZF_IN ZFOperationResult *operationResult)
{
    ZFThreadExecuteInMainThread(
        ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFOperationAsyncPrivate, onNotifyFinishInMainThread)),
        zfnull,
        ZFListenerData()
            .param0Set(ZFValue::indexValueCreate(operationId))
            .param1Set(operationResult)
        );
}
void ZFOperationAsync::taskNotifyProgress(ZF_IN ZFOperationParam *operationParam,
                                          ZF_IN_OPT ZFOperationProgress *operationProgress /* = zfnull */)
{
    ZFThreadExecuteInMainThread(
        ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFOperationAsyncPrivate, onNotifyProgressInMainThread)),
        zfnull,
        ZFListenerData()
            .param0Set(operationParam)
            .param1Set(operationProgress)
        );
}
void ZFOperationAsync::taskNotifyProgress(ZF_IN zfidentity operationId,
                                          ZF_IN_OPT ZFOperationProgress *operationProgress /* = zfnull */)
{
    ZFThreadExecuteInMainThread(
        ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFOperationAsyncPrivate, onNotifyFinishInMainThread)),
        zfnull,
        ZFListenerData()
            .param0Set(ZFValue::indexValueCreate(operationId))
            .param1Set(operationProgress)
        );
}

void ZFOperationAsync::taskOnStartAsync(ZF_IN ZFOperationTaskData *operationTaskData)
{
    // nothing to do
}
void ZFOperationAsync::taskOnStopAsync(ZF_IN ZFOperationTaskData *operationTaskData)
{
    // nothing to do
}

void ZFOperationAsync::_ZFP_ZFOperationAsync_taskNotifyFinishAsync(ZF_IN ZFOperationParam *operationParam,
                                                                   ZF_IN ZFOperationResult *operationResult)
{
    zfsuper::taskNotifyFinish(operationParam, operationResult);
}
void ZFOperationAsync::_ZFP_ZFOperationAsync_taskNotifyFinishAsync(ZF_IN zfidentity operationId,
                                                                   ZF_IN ZFOperationResult *operationResult)
{
    zfsuper::taskNotifyFinish(operationId, operationResult);
}
void ZFOperationAsync::_ZFP_ZFOperationAsync_taskNotifyProgressAsync(ZF_IN ZFOperationParam *operationParam,
                                                                     ZF_IN_OPT ZFOperationProgress *operationProgress /* = zfnull */)
{
    zfsuper::taskNotifyProgress(operationParam, operationProgress);
}
void ZFOperationAsync::_ZFP_ZFOperationAsync_taskNotifyProgressAsync(ZF_IN zfidentity operationId,
                                                                     ZF_IN_OPT ZFOperationProgress *operationProgress /* = zfnull */)
{
    zfsuper::taskNotifyProgress(operationId, operationProgress);
}
void ZFOperationAsync::_ZFP_ZFOperationAsync_taskOnStartAsync(ZF_IN ZFOperationTaskData *operationTaskData)
{
    this->taskOnStartAsync(operationTaskData);
}

ZF_NAMESPACE_GLOBAL_END

