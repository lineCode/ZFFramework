/**
 * @file ZFOperationAsync.h
 * @brief abstract async operation
 */

#ifndef _ZFI_ZFOperationAsync_h_
#define _ZFI_ZFOperationAsync_h_

#include "ZFOperation.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief see #ZFOperationParam */
#define ZFOperationAsyncParam ZFOperationParam
/** @brief see #ZFOperationResult */
#define ZFOperationAsyncResult ZFOperationResult
/** @brief see #ZFOperationObserver */
#define ZFOperationAsyncObserver ZFOperationObserver
/** @brief see #ZFOperationCache */
#define ZFOperationAsyncCache ZFOperationCache
/** @brief see #ZFOperationProgress */
#define ZFOperationAsyncProgress ZFOperationProgress
/** @brief see #ZFOperationTaskData */
#define ZFOperationAsyncTaskData ZFOperationTaskData

zfclassFwd _ZFP_ZFOperationAsyncPrivate;
/**
 * @brief abstract operation with async logic, see #ZFOperation
 *
 * caller of this operation type must note:
 * you must call #ZFOperation::taskStart & #ZFOperation::taskStop in main thread
 */
zfabstract ZF_ENV_EXPORT ZFOperationAsync : zfextends ZFOperation
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFOperationAsync, ZFOperation)

public:
    /**
     * @brief notify after delay, usually for debug use only
     */
    ZFMETHOD_DECLARE_DETAIL_4(public, ZFMethodTypeStatic,
                              void, taskNotifyFinishAfterDelay,
                              ZFMP_IN(zftimet, delay),
                              ZFMP_IN(ZFOperation *, operation),
                              ZFMP_IN(ZFOperationParam *, operationParam),
                              ZFMP_IN(ZFOperationResult *, operationResult))
    /**
     * @brief notify after delay, usually for debug use only
     */
    ZFMETHOD_DECLARE_DETAIL_4(public, ZFMethodTypeStatic,
                              void, taskNotifyFinishAfterDelay,
                              ZFMP_IN(zftimet, delay),
                              ZFMP_IN(ZFOperation *, operation),
                              ZFMP_IN(zfidentity, operationId),
                              ZFMP_IN(ZFOperationResult *, operationResult))
    /**
     * @brief notify after delay, usually for debug use only
     */
    ZFMETHOD_DECLARE_DETAIL_4(public, ZFMethodTypeStatic,
                              void, taskNotifyProgressAfterDelay,
                              ZFMP_IN(zftimet, delay),
                              ZFMP_IN(ZFOperation *, operation),
                              ZFMP_IN(ZFOperationParam *, operationParam),
                              ZFMP_IN_OPT(ZFOperationProgress *, operationProgress, zfnull))
    /**
     * @brief notify after delay, usually for debug use only
     */
    ZFMETHOD_DECLARE_DETAIL_4(public, ZFMethodTypeStatic,
                              void, taskNotifyProgressAfterDelay,
                              ZFMP_IN(zftimet, delay),
                              ZFMP_IN(ZFOperation *, operation),
                              ZFMP_IN(zfidentity, operationId),
                              ZFMP_IN_OPT(ZFOperationProgress *, operationProgress, zfnull))

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

public:
    /** @brief see #ZFOperation::taskNotifyFinish */
    zfoverride
    virtual void taskNotifyFinish(ZF_IN ZFOperationParam *operationParam,
                                  ZF_IN ZFOperationResult *operationResult);
    /** @brief see #ZFOperation::taskNotifyFinish */
    zfoverride
    virtual void taskNotifyFinish(ZF_IN zfidentity operationId,
                                  ZF_IN ZFOperationResult *operationResult);
    /** @brief see #ZFOperation::taskNotifyProgress */
    zfoverride
    virtual void taskNotifyProgress(ZF_IN ZFOperationParam *operationParam,
                                    ZF_IN_OPT ZFOperationProgress *operationProgress = zfnull);
    /** @brief see #ZFOperation::taskNotifyProgress */
    zfoverride
    virtual void taskNotifyProgress(ZF_IN zfidentity operationId,
                                    ZF_IN_OPT ZFOperationProgress *operationProgress = zfnull);

protected:
    /**
     * @brief for a async operation, you must override #taskOnStartAsync instead
     */
    zfoverride
    virtual void taskOnStart(ZF_IN ZFOperationTaskData *operationTaskData);
    /**
     * @brief for a async operation, you must override #taskOnStopAsync instead
     */
    zfoverride
    virtual void taskOnStop(ZF_IN ZFOperationTaskData *operationTaskData);

protected:
    /**
     * @brief subclass should override this method to do actual operaton,
     *   this method is ensured called in background thread
     * @note duplicated task won't get this being called
     */
    virtual void taskOnStartAsync(ZF_IN ZFOperationTaskData *operationTaskData) zfpurevirtual;
    /**
     * @brief subclass should override this to listen stop event, instead of #ZFOperation::taskOnStop,
     *   this method is ensured called in main thread
     */
    virtual void taskOnStopAsync(ZF_IN ZFOperationTaskData *operationTaskData);

public:
    virtual void _ZFP_ZFOperationAsync_taskNotifyFinishAsync(ZF_IN ZFOperationParam *operationParam,
                                                             ZF_IN ZFOperationResult *operationResult);
    virtual void _ZFP_ZFOperationAsync_taskNotifyFinishAsync(ZF_IN zfidentity operationId,
                                                             ZF_IN ZFOperationResult *operationResult);
    virtual void _ZFP_ZFOperationAsync_taskNotifyProgressAsync(ZF_IN ZFOperationParam *operationParam,
                                                               ZF_IN_OPT ZFOperationProgress *operationProgress = zfnull);
    virtual void _ZFP_ZFOperationAsync_taskNotifyProgressAsync(ZF_IN zfidentity operationId,
                                                               ZF_IN_OPT ZFOperationProgress *operationProgress = zfnull);
    virtual void _ZFP_ZFOperationAsync_taskOnStartAsync(ZF_IN ZFOperationTaskData *operationTaskData);

private:
    _ZFP_ZFOperationAsyncPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOperationAsync_h_

