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
 * @file ZFOperation.h
 * @brief abstract operation utility with cache and queue logic
 */

#ifndef _ZFI_ZFOperation_h_
#define _ZFI_ZFOperation_h_

#include "ZFOperation_Types.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFOperationPrivate;
/**
 * @brief abstract operation utility with cache and queue logic
 *
 * typically, a operation's progress should be like:\n
 * taskStart -> taskOnStart -> subclass's actual operation
 * -> taskNotifyFinish -> taskOnStop -> EventOperationTaskOnStop\n
 * \n
 * for subclass, you must:
 * -  declare your own operation types
 *   (param, result, observer, cache, data),
 *   and follow those types' guides in their comments (such as #ZFOperationParam)
 * -  override taskOnStart to do your actual operation
 *   and finally call #ZFOperation::taskNotifyFinish,
 *   both on success and on fail
 *
 * optional:
 * -  override taskOnStartFirst/taskOnStopLast if necessary,
 *   to init and cleanup before first and after last operation
 * -  override cacheOnSave/cacheOnRestore and cacheOnAdd/cacheOnRemove,
 *   for cache management
 *
 * for operation's caller, what you should do is simple:
 * -  always create operation types by createXXX (such as #createParam)
 * -  create a operation param and fill the necessary fields
 * -  create a operation observer if necessary to listen operation task's finish event
 * -  call operation's #taskStart to start the operation,
 *   or #taskStop to stop if necessary
 *
 * @note although it's not required that start or stop operation in main thread,
 *   but typically it's a good behavior to do so
 * @note ZFOperation is designed for abstract and heavy task's control,
 *   so it's recommended to make your operation singleton,
 *   you may use #ZFOBJECT_SINGLETON_DECLARE to declare singleton easily
 */
zfabstract ZF_ENV_EXPORT ZFOperation : zfextends ZFObject
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFOperation, ZFObject)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is #ZFOperationTaskData holding the necessary operation/param/result/observer/cache/id\n
     * called when start with cache found even if cache expired (#ZFOperationCache::cacheIsExpired),
     * but won't be called if cache is not valid (#ZFOperation::cacheIsValid)
     */
    ZFOBSERVER_EVENT(OperationCacheOnMatch)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is #ZFOperationCache\n
     * called when cache is not valid (#ZFOperation::cacheIsValid)
     */
    ZFOBSERVER_EVENT(OperationCacheOnInvalid)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is #ZFOperationCache\n
     * called when cache is expired (#ZFOperationCache::cacheIsExpired)
     */
    ZFOBSERVER_EVENT(OperationCacheOnExpire)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is #ZFOperationTaskData holding the necessary operation/param/result/observer/id
     */
    ZFOBSERVER_EVENT(OperationTaskOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is #ZFOperationTaskData holding the necessary operation/param/result/observer/id\n
     * if result is matched from cache,
     * ZFOperationTaskData's cache would hold the cache object
     */
    ZFOBSERVER_EVENT(OperationTaskOnStop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is #ZFOperationTaskData holding the necessary operation/param/result/observer/progress/id
     * (progress may be null)
     */
    ZFOBSERVER_EVENT(OperationTaskOnProgress)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

    // ============================================================
    // settings
public:
    /**
     * @brief whether dummy param is allowed to start operation, false by default
     *
     * if true, a dummy param would be created if param is null while starting operation,
     * param is created by #createParam and would be cached during this operation's life time
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, taskAllowDummyParam, zffalse)
    /**
     * @brief see #ZFOperationTaskDuplicateAction, default value for #ZFOperationTaskData::taskDuplicateActionSet
     *   when not specified
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFOperationTaskDuplicateActionEnum, taskDuplicateAction,
                                ZFOperationTaskDuplicateAction::e_Merge)
    /**
     * @brief see #ZFOperationCacheMatchAction, default value for #ZFOperationTaskData::cacheMatchActionSet
     *   when not specified
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFOperationCacheMatchActionEnum, cacheMatchAction,
                                ZFOperationCacheMatchAction::e_NotifyFinish)
    /**
     * @brief max cache size, or #zfindexMax for no limit, 10 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, cacheMaxSize,
                                10)
    /**
     * @brief default cache expire time, default is ZFOperationCacheExpireTimeDisable
     *
     * if > 0, successed operation result would auto add as cache accorrding CacheExpireTimeDefault,
     * otherwise, no cache would be added automatically,
     * while you may add cache manually by cacheAdd for child classes\n
     * for caller of the operation,
     * you may specify the CacheExpireTime when #taskStart,
     * which would override cache setting if not #ZFOperationCacheExpireTimeUnspecified
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, cacheExpireTimeDefault,
                                ZFOperationCacheExpireTimeDisable)

    /**
     * @brief whether invoke #cacheTrim when receive #ZFGlobalEvent::EventAppOnMemoryLow, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, cacheTrimWhenReceiveMemoryWarning, zftrue)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, cacheTrimWhenReceiveMemoryWarning)

    /**
     * @brief leave how many cache alive while #cacheTrim, 3 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, cacheTrimThreshold, 3)

    /**
     * @brief max operation allowed to start, default is 5, use zfindexMax() for unlimited
     *
     * while starting a opeartion exceeds max count,
     * the newly start operation would be queued
     * and would be started when a operation finished\n
     * changing this value would take effect when next operation's #taskStart is called\n
     * this value is useful when your operation need to start new thread,
     * use to prevent too many thread being started
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, taskToStartMax,
                                5)

    // ============================================================
    // type create
public:
    /**
     * @brief create param
     *
     * subclass should override #classForOperationParam to change type
     */
    ZFMETHOD_DECLARE_0(zfautoObject, createParam)

    /**
     * @brief create result
     *
     * subclass should override #classForOperationResult to change type
     */
    ZFMETHOD_DECLARE_0(zfautoObject, createResult)
    /**
     * @brief create result
     *
     * subclass should override #classForOperationResult to change type
     */
    ZFMETHOD_DECLARE_1(zfautoObject, createResult,
                       ZFMP_IN(ZFResultTypeEnum, resultType))

    /**
     * @brief create observer
     *
     * subclass should override #classForOperationObserver to change type
     */
    ZFMETHOD_DECLARE_0(zfautoObject, createObserver)

    /**
     * @brief create cache
     *
     * subclass should override #classForOperationCache to change type
     */
    ZFMETHOD_DECLARE_0(zfautoObject, createCache)
    /**
     * @brief create cache
     *
     * subclass should override #classForOperationCache to change type\n
     * if cacheExpireTime is 0, use operation's setting (at this time, #cacheExpireTimeDefault must be valid),
     * if cacheTime is 0, use current time
     */
    ZFMETHOD_DECLARE_4(zfautoObject, createCache,
                       ZFMP_IN(ZFOperationParam *, operationParam),
                       ZFMP_IN(ZFOperationResult *, operationResult),
                       ZFMP_IN_OPT(zftimet, cacheExpireTime, zftimetZero()),
                       ZFMP_IN_OPT(zftimet, cacheTime, zftimetZero()))

    /**
     * @brief create progress
     *
     * subclass should override #classForOperationProgress to change type
     */
    ZFMETHOD_DECLARE_0(zfautoObject, createProgress)

    /**
     * @brief create observer
     *
     * subclass should override #classForOperationTaskData to change type
     */
    ZFMETHOD_DECLARE_0(zfautoObject, createTaskData)
    /**
     * @brief create observer
     *
     * subclass should override #classForOperationTaskData to change type
     */
    ZFMETHOD_DECLARE_6(zfautoObject, createTaskData,
                       ZFMP_IN(ZFOperationParam *, operationParam),
                       ZFMP_IN_OPT(ZFOperationResult *, operationResult, zfnull),
                       ZFMP_IN_OPT(ZFOperationObserver *, operationObserver, zfnull),
                       ZFMP_IN_OPT(ZFOperationCache *, operationCache, zfnull),
                       ZFMP_IN_OPT(zfidentity, operationId, zfidentityInvalid()),
                       ZFMP_IN_OPT(ZFOperationProgress *, operationProgress, zfnull))

protected:
    /** @brief see #createParam */
    virtual const ZFClass *classForOperationParam(void);
    /** @brief see #createResult */
    virtual const ZFClass *classForOperationResult(void);
    /** @brief see #createObserver */
    virtual const ZFClass *classForOperationObserver(void);
    /** @brief see #createCache */
    virtual const ZFClass *classForOperationCache(void);
    /** @brief see #createProgress */
    virtual const ZFClass *classForOperationProgress(void);
    /** @brief see #createTaskData */
    virtual const ZFClass *classForOperationTaskData(void);

    // ============================================================
    // operation constroll
public:
    /**
     * @brief start the operation, return a id or #zfidentityInvalid if task not started
     *   (typically because of cache matched or ignore duplicated task)
     */
    ZFMETHOD_DECLARE_2(zfidentity, taskStart,
                       ZFMP_IN_OPT(ZFOperationParam *, operationParam, zfnull),
                       ZFMP_IN_OPT(ZFOperationObserver *, operationObserver, zfnull))
    /**
     * @brief start the operation, see #taskStart
     */
    ZFMETHOD_DECLARE_1(zfidentity, taskStart,
                       ZFMP_IN(ZFOperationTaskData *, operationTaskData))
    /**
     * @brief stop operation by id, do nothing if no such operation
     */
    ZFMETHOD_DECLARE_1(void, taskStop,
                       ZFMP_IN(zfidentity, operationId))
    /**
     * @brief stop and get the result (which should be a result with canceled state)
     */
    ZFMETHOD_DECLARE_1(zfautoObject, taskStopAndGetResult,
                       ZFMP_IN(zfidentity, operationId))
    /**
     * @brief stop operation by param, do nothing if no such operation
     *
     * if operationParam is null, we will create a dummy param for you,
     * see #taskAllowDummyParam
     * @note use #taskStop by operationId is recommended,
     *   since it have no need to compare params
     */
    ZFMETHOD_DECLARE_1(void, taskStop,
                       ZFMP_IN_OPT(ZFOperationParam *, operationParam, zfnull))
    /**
     * @brief stop operation by category (set from #ZFOperationTaskData::taskCategory), do nothing if no such operation
     */
    ZFMETHOD_DECLARE_1(void, taskStopForCategory,
                       ZFMP_IN(ZFObject *, category))
    /**
     * @brief stop all operation
     */
    ZFMETHOD_DECLARE_0(void, taskStopAll)

    /**
     * @brief remove observer only, task would keep running normally
     *
     * this is useful when you want to remove task observer,
     * but keep the task running in background
     */
    ZFMETHOD_DECLARE_1(void, taskStopObserver,
                       ZFMP_IN(zfidentity, operationId))
    /**
     * @brief see #taskStopObserver, #taskStop
     */
    ZFMETHOD_DECLARE_1(void, taskStopObserver,
                       ZFMP_IN_OPT(ZFOperationParam *, operationParam, zfnull))
    /**
     * @brief see #taskStopObserver, #taskStopForCategory
     */
    ZFMETHOD_DECLARE_1(void, taskStopObserverForCategory,
                       ZFMP_IN(ZFObject *, category))

    /**
     * @brief get task count
     */
    ZFMETHOD_DECLARE_0(zfindex, taskCount)
    /**
     * @brief get queued task count
     */
    ZFMETHOD_DECLARE_0(zfindex, taskQueuedCount)
    /**
     * @brief true if any task is running or queued
     */
    ZFMETHOD_DECLARE_0(zfbool, taskRunning)

    /**
     * @brief true if task is started and hasn't stopped
     *
     * note, since operation may put task in queue if too many task,
     * a started task may or may not have #taskOnStart been called
     */
    ZFMETHOD_DECLARE_1(zfbool, taskIsAlive,
                       ZFMP_IN(ZFOperationParam *, operationParam))
    /**
     * @brief true if task is started and hasn't stopped
     *
     * note, since operation may put task in queue if too many task,
     * a started task may or may not have #taskOnStart been called
     */
    ZFMETHOD_DECLARE_1(zfbool, taskIsAlive,
                       ZFMP_IN(zfidentity, operationId))

public:
    /**
     * @brief used to check whether param is valid
     */
    ZFMETHOD_DECLARE_1(zfbool, paramIsValid,
                       ZFMP_IN(ZFOperationParam *, operationParam))

public:
    /**
     * @brief subclass must notify finish after operation finish, including success, fail and cancel
     *
     * do nothing if the desired task already notified and finished\n
     * for merged tasks, this method would notify and finish all the tasks
     */
    ZFMETHOD_DECLARE_2(void, taskNotifyFinish,
                       ZFMP_IN(ZFOperationParam *, operationParam),
                       ZFMP_IN(ZFOperationResult *, operationResult))
    /**
     * @brief subclass must notify finish after operation finish, including success, fail and cancel
     *
     * do nothing if the desired task already notified and finished
     */
    ZFMETHOD_DECLARE_2(void, taskNotifyFinish,
                       ZFMP_IN(zfidentity, operationId),
                       ZFMP_IN(ZFOperationResult *, operationResult))
    /**
     * @brief subclass may use this to notify progress update,
     *   which would notify observer's #EventOperationTaskOnProgress
     *
     * for merged tasks, this method would notify all the tasks
     */
    ZFMETHOD_DECLARE_2(void, taskNotifyProgress,
                       ZFMP_IN(ZFOperationParam *, operationParam),
                       ZFMP_IN_OPT(ZFOperationProgress *, operationProgress, zfnull))
    /**
     * @brief subclass may use this to notify progress update,
     *   which would notify observer's #EventOperationTaskOnProgress
     */
    ZFMETHOD_DECLARE_2(void, taskNotifyProgress,
                       ZFMP_IN(zfidentity, operationId),
                       ZFMP_IN_OPT(ZFOperationProgress *, operationProgress, zfnull))

    // ============================================================
    // protected operation control
protected:
    /**
     * @brief return true by default
     *
     * if false, task would have #operationTaskOnStart and #operationTaskOnStop called with cancel state
     */
    virtual inline zfbool taskOnCheckNeedStart(ZF_IN ZFOperationTaskData *operationTaskData)
    {
        return zftrue;
    }
    /**
     * @brief subclass should override this method to implement actual operation
     */
    virtual void taskOnStart(ZF_IN ZFOperationTaskData *operationTaskData) zfpurevirtual;
    /**
     * @brief called when operation success/fail/cancel
     */
    virtual inline void taskOnStop(ZF_IN ZFOperationTaskData *operationTaskData)
    {
    }
    /**
     * @brief called before first operation's taskOnStart
     *
     * would be called each time to start first operation when running list is empty
     */
    virtual inline void taskOnStartFirst(void)
    {
    }
    /**
     * @brief called after all operation has finished (success/fail/cancel)
     */
    virtual inline void taskOnStopLast(void)
    {
    }

    /**
     * @brief see #ZFOperationTaskDuplicateAction
     */
    virtual inline void taskOnStartDuplicated(ZF_IN ZFOperationTaskData *operationTaskData)
    {
    }
    /**
     * @brief see #ZFOperationTaskDuplicateAction
     */
    virtual inline void taskOnStopDuplicated(ZF_IN ZFOperationTaskData *operationTaskData)
    {
    }
    /**
     * @brief called when #taskNotifyProgress
     */
    virtual inline void taskOnProgress(ZF_IN ZFOperationTaskData *operationTaskData)
    {
    }

    // ============================================================
    // observer
protected:
    /** @brief see #EventOperationCacheOnMatch */
    virtual void operationCacheOnMatch(ZF_IN ZFOperationTaskData *operationTaskData);
    /** @brief see #EventOperationCacheOnInvalid */
    virtual void operationCacheOnInvalid(ZF_IN ZFOperationCache *operationCache);
    /** @brief see #EventOperationCacheOnExpire */
    virtual void operationCacheOnExpire(ZF_IN ZFOperationCache *operationCache);
    /** @brief see #EventOperationTaskOnStart */
    virtual void operationTaskOnStart(ZF_IN ZFOperationTaskData *operationTaskData);
    /** @brief see #EventOperationTaskOnStop */
    virtual void operationTaskOnStop(ZF_IN ZFOperationTaskData *operationTaskData);
    /** @brief see #EventOperationTaskOnProgress */
    virtual void operationTaskOnProgress(ZF_IN ZFOperationTaskData *operationTaskData);

    // ============================================================
    // advance operation control
public:
    /**
     * @brief get operaiton data for operation id, or null if no such task or task already finished
     */
    ZFMETHOD_DECLARE_1(ZFOperationTaskData *, operationGetTaskData,
                       ZFMP_IN(zfidentity, operationId));
    /**
     * @brief usually used to get all merged task datas
     */
    ZFMETHOD_DECLARE_2(void, operationGetTaskList,
                       ZFMP_IN_OUT(ZFArrayEditable *, taskDatas),
                       ZFMP_IN(zfidentity, operationId))
    /**
     * @brief usually used for debug only
     */
    ZFMETHOD_DECLARE_1(void, operationGetTaskList,
                       ZFMP_IN_OUT(ZFArrayEditable *, taskDatas))
    /**
     * @brief usually used for debug only
     */
    ZFMETHOD_DECLARE_1(void, operationGetTaskListQueued,
                       ZFMP_IN_OUT(ZFArrayEditable *, taskDatas))

    // ============================================================
    // cache control
public:
    /**
     * @brief manually add a cache, assert fail if cache not valid,
     *   replace if exist
     */
    ZFMETHOD_DECLARE_1(void, cacheAdd,
                       ZFMP_IN(ZFOperationCache *, operationCache))
    /**
     * @brief remove cache, do nothing if none or param invalid
     */
    ZFMETHOD_DECLARE_1(void, cacheRemove,
                       ZFMP_IN(ZFOperationParam *, operationParam))
    /**
     * @brief remove all cache, call cacheOnRemove for each cache
     */
    ZFMETHOD_DECLARE_0(void, cacheRemoveAll)
    /**
     * @brief trim the cache to reduce memory
     *
     * by default, this method would call #cacheTrimBySize
     */
    ZFMETHOD_DECLARE_0(void, cacheTrim)
    /**
     * @brief util method to trim the cache
     *   so that the cached data won't exceeds specified size
     */
    ZFMETHOD_DECLARE_1(void, cacheTrimBySize,
                       ZFMP_IN(zfindex, size))

    /**
     * @brief called to check whether the cache is valid
     *
     * this method should only check whether the cache is valid,
     * while whether cache is expired is checked by #ZFOperationCache::cacheIsExpired
     */
    ZFMETHOD_DECLARE_1(zfbool, cacheIsValid,
                       ZFMP_IN(ZFOperationCache *, operationCache))
    /**
     * @brief access all cache, for debug use only
     */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfautoObject>, cacheGetAll)
    /** @brief see #cacheGetAll */
    ZFMETHOD_DECLARE_1(void, cacheGetAllT,
                       ZFMP_OUT(ZFCoreArray<zfautoObject> &, ret))

protected:
    /**
     * @brief called when cache matched
     */
    virtual inline void cacheOnMatch(ZF_IN ZFOperationCache *operationCache)
    {
    }
    /**
     * @brief called when cache invalid
     */
    virtual inline void cacheOnInvalid(ZF_IN ZFOperationCache *operationCache)
    {
    }
    /**
     * @brief called when cache expired
     */
    virtual inline void cacheOnExpire(ZF_IN ZFOperationCache *operationCache)
    {
    }
    /**
     * @brief called when add cache
     */
    virtual inline void cacheOnAdd(ZF_IN ZFOperationCache *operationCache)
    {
    }
    /**
     * @brief called when remove cache
     */
    virtual inline void cacheOnRemove(ZF_IN ZFOperationCache *operationCache)
    {
    }

public:
    /**
     * @brief set the cache need save, usually no need to call manually
     *
     * cache would be set need save each time a operation finished with Success state
     */
    ZFMETHOD_DECLARE_0(void, cacheSaveRequest)
    /**
     * @brief manually check to save cache
     */
    ZFMETHOD_DECLARE_0(void, cacheSave)
    /**
     * @brief set the cache need restore, usually no need to call manually, see #cacheRestore
     */
    ZFMETHOD_DECLARE_0(void, cacheRestoreRequest)
    /**
     * @brief manually check to restore cache, would be called before each operation start, see #cacheRestoreRequest
     */
    ZFMETHOD_DECLARE_0(void, cacheRestore)

protected:
    /**
     * @brief called to restore cache, array is ZFOperationCache's array, do nothing by default
     */
    virtual inline void cacheOnRestore(ZF_IN_OUT ZFArrayEditable *array)
    {
    }
    /**
     * @brief called to save cache, array is ZFOperationCache's array, do nothing by default
     */
    virtual inline void cacheOnSave(ZF_IN ZFArray *array)
    {
    }

private:
    _ZFP_ZFOperationPrivate *d;
    friend zfclassFwd _ZFP_ZFOperationPrivate;
};

// ============================================================
/**
 * @brief used for storing object types and states for ZFOperation
 *
 * subclass of this class must follow these rules:
 * -  class name must be "YourOperation+TaskData"
 *   while "YourOperation" is its owner operation
 * -  have class registered by #ZFOBJECT_REGISTER
 *
 * this object is used to store states for operation task only,
 * typically would be created before task start,
 * and released after task stop
 */
zfclass ZF_ENV_EXPORT ZFOperationTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFOperationTaskData, ZFObject)

    ZFCLASS_PRIVATE_ALLOC("can only be created by ZFOperation::createTaskData")
protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
    }

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief owner operation, no auto retain
     */
    ZFPROPERTY_ASSIGN(ZFOperation *, operationOwner)
    /**
     * @brief used for storing an operation, auto retain,
     *   use #operationOwnerSet for assign only
     */
    ZFPROPERTY_RETAIN(ZFOperation *, operation)
    /**
     * @brief used for storing an operation id
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfidentity, operationId,
                                zfidentityInvalid())
    /**
     * @brief task category, can be used to stop task by #ZFOperation::taskStopForCategory
     */
    ZFPROPERTY_RETAIN_READONLY(ZFSetEditable *, taskCategory,
                               zflineAlloc(ZFSetEditable))
    /**
     * @brief used for storing an operation param
     */
    ZFPROPERTY_RETAIN(ZFOperationParam *, operationParam)
    /**
     * @brief used for storing an operation result
     */
    ZFPROPERTY_RETAIN(ZFOperationResult *, operationResult)
    /**
     * @brief used for storing an operation observer
     */
    ZFPROPERTY_RETAIN(ZFOperationObserver *, operationObserver)
    /**
     * @brief used for storing an operation cache
     */
    ZFPROPERTY_RETAIN(ZFOperationCache *, operationCache)
    /**
     * @brief used for storing an operation progress
     */
    ZFPROPERTY_RETAIN(ZFOperationProgress *, operationProgress)
    /**
     * @brief override default cache expire time,
     *   #ZFOperationCacheExpireTimeUnspecified by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, cacheExpireTime, ZFOperationCacheExpireTimeUnspecified)
    /**
     * @brief do what if cache matched,
     *   #ZFOperationCacheMatchAction::e_Unspecified by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFOperationCacheMatchActionEnum, cacheMatchAction, ZFOperationCacheMatchAction::e_Unspecified)
    /**
     * @brief do what if duplicated task started,
     *   #ZFOperationTaskDuplicateAction::e_Unspecified by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFOperationTaskDuplicateActionEnum, taskDuplicateAction, ZFOperationTaskDuplicateAction::e_Unspecified)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOperation_h_

