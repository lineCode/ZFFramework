/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFOperation.h"

#include "ZFCore/ZFSTLWrapper/zfstl_hashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFOperationPrivate
zfclassFwd _ZFP_I_ZFOperationPrivateTaskData;
zfclass _ZFP_I_ZFOperationPrivateTaskObserverData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFOperationPrivateTaskObserverData, ZFObject)

public:
    _ZFP_I_ZFOperationPrivateTaskData *ownerTaskData; // assign only
    ZFOperationTaskData *operationTaskData;
    zfbool taskIsDuplicate; // used when remove taskObserverData before taskOnStop, to mark whether it's duplicated task
    zftimet cacheExpireTime;

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->ownerTaskData = zfnull;
        this->operationTaskData = zfnull;
        this->taskIsDuplicate = zffalse;
        this->cacheExpireTime = ZFOperationCacheExpireTimeUnspecified;
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfRelease(this->operationTaskData);
        zfsuper::objectOnDealloc();
    }
};
zfclass _ZFP_I_ZFOperationPrivateTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFOperationPrivateTaskData, ZFObject)
public:
    ZFOperationParam *operationParam; // auto release
    ZFCoreArrayPOD<zfidentity> operationIds;
    ZFArrayEditable *taskObserverDatas; // array of _ZFP_I_ZFOperationPrivateTaskObserverData, auto create and release

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->operationParam = zfnull;
        this->taskObserverDatas = zfAlloc(ZFArrayEditable);
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfRelease(operationParam);
        zfRelease(this->taskObserverDatas);
        zfsuper::objectOnDealloc();
    }
};
zfclassNotPOD _ZFP_ZFOperationPrivate
{
public:
    ZFOperation *pimplOwner;
    const ZFClass *classForOperationParam;
    const ZFClass *classForOperationResult;
    const ZFClass *classForOperationObserver;
    const ZFClass *classForOperationCache;
    const ZFClass *classForOperationProgress;
    const ZFClass *classForOperationTaskData;
    ZFArrayEditable *tasks; // array of _ZFP_I_ZFOperationPrivateTaskData, newer task appends to tail
    ZFArrayEditable *tasksQueued; // array of _ZFP_I_ZFOperationPrivateTaskData, newer task appends to tail
    ZFArrayEditable *caches; // array of ZFOperationCache, newer cache appends to tail
    ZFIdentityGenerator idGenerator;
    zfbool cacheNeedSave;
    zfbool cacheNeedRestore;
    zfbool startFirstFlag;
    ZFOperationParam *dummyParam;

public:
    ZFOperationParam *dummyParamAccess(void)
    {
        if(this->dummyParam)
        {
            this->dummyParam = zfRetain(this->pimplOwner->createParam().to<ZFOperationParam *>());
        }
        return this->dummyParam;
    }

public:
    _ZFP_I_ZFOperationPrivateTaskData *findTaskDataForOperationParam(ZF_IN ZFOperationParam *operationParam,
                                                                     ZF_IN_OPT zfbool includeQueued = zftrue,
                                                                     ZF_OUT_OPT zfbool *isInQueue = zfnull)
    {
        for(zfindex iTaskData = 0; iTaskData < this->tasks->count(); ++iTaskData)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = this->tasks->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
            if(taskData->operationParam->paramIsEqual(operationParam))
            {
                if(isInQueue != zfnull)
                {
                    *isInQueue = zffalse;
                }
                return taskData;
            }
        }
        if(includeQueued)
        {
            for(zfindex iTaskData = 0; iTaskData < this->tasksQueued->count(); ++iTaskData)
            {
                _ZFP_I_ZFOperationPrivateTaskData *taskData = this->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
                if(taskData->operationParam->paramIsEqual(operationParam))
                {
                    if(isInQueue != zfnull)
                    {
                        *isInQueue = zftrue;
                    }
                    return taskData;
                }
            }
        }
        return zfnull;
    }
    _ZFP_I_ZFOperationPrivateTaskData *findTaskDataForOperationId(ZF_IN zfidentity operationId,
                                                                  ZF_IN_OPT zfbool includeQueued = zftrue,
                                                                  ZF_OUT_OPT zfbool *isInQueue = zfnull)
    {
        if(isInQueue != zfnull)
        {
            *isInQueue = zffalse;
        }
        for(zfindex iTaskData = 0; iTaskData < this->tasks->count(); ++iTaskData)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = this->tasks->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
            for(zfindex iOperationId = 0; iOperationId < taskData->operationIds.count(); ++iOperationId)
            {
                if(taskData->operationIds[iOperationId] == operationId)
                {
                    return taskData;
                }
            }
        }
        if(includeQueued)
        {
            for(zfindex iTaskData = 0; iTaskData < this->tasksQueued->count(); ++iTaskData)
            {
                _ZFP_I_ZFOperationPrivateTaskData *taskData = this->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
                for(zfindex iTaskObserverData = 0; iTaskObserverData < taskData->taskObserverDatas->count(); ++iTaskObserverData)
                {
                    _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iTaskObserverData);
                    if(taskObserverData->operationTaskData->operationId() == operationId)
                    {
                        if(isInQueue != zfnull)
                        {
                            *isInQueue = zftrue;
                        }
                        return taskData;
                    }
                }
            }
        }
        return zfnull;
    }
    _ZFP_I_ZFOperationPrivateTaskObserverData *findTaskObserverDataForOperationId(ZF_IN zfidentity operationId,
                                                                                  ZF_IN_OPT zfbool includeQueued = zftrue,
                                                                                  ZF_OUT_OPT zfbool *isInQueue = zfnull)
    {
        if(isInQueue != zfnull)
        {
            *isInQueue = zffalse;
        }
        for(zfindex iTaskData = 0; iTaskData < this->tasks->count(); ++iTaskData)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = this->tasks->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
            for(zfindex iTaskObserverData = 0; iTaskObserverData < taskData->taskObserverDatas->count(); ++iTaskObserverData)
            {
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iTaskObserverData);
                if(taskObserverData->operationTaskData->operationId() == operationId)
                {
                    return taskObserverData;
                }
            }
        }
        if(includeQueued)
        {
            for(zfindex iTaskData = 0; iTaskData < this->tasksQueued->count(); ++iTaskData)
            {
                _ZFP_I_ZFOperationPrivateTaskData *taskData = this->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
                for(zfindex iTaskObserverData = 0; iTaskObserverData < taskData->taskObserverDatas->count(); ++iTaskObserverData)
                {
                    _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iTaskObserverData);
                    if(taskObserverData->operationTaskData->operationId() == operationId)
                    {
                        if(isInQueue != zfnull)
                        {
                            *isInQueue = zftrue;
                        }
                        return taskObserverData;
                    }
                }
            }
        }
        return zfnull;
    }

public:
    void checkAddCache(ZF_IN ZFOperationParam *operationParam,
                       ZF_IN ZFOperationResult *operationResult,
                       ZF_IN zftimet cacheExpireTime)
    {
        if(cacheExpireTime != ZFOperationCacheExpireTimeDisable && operationResult->resultType() == ZFResultType::e_Success)
        {
            if(cacheExpireTime > 0)
            {
                this->pimplOwner->cacheAdd(this->pimplOwner->createCache(operationParam, operationResult, cacheExpireTime).to<ZFOperationCache *>());
                this->pimplOwner->cacheSaveRequest();
            }
            else if(this->pimplOwner->cacheExpireTimeDefault() > 0)
            {
                this->pimplOwner->cacheAdd(this->pimplOwner->createCache(operationParam, operationResult).to<ZFOperationCache *>());
                this->pimplOwner->cacheSaveRequest();
            }
        }
        this->pimplOwner->cacheSave();
    }

public:
    void doStart(ZF_IN _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData,
                 ZF_IN zfbool taskIsDuplicate)
    {
        taskObserverData->taskIsDuplicate = taskIsDuplicate;
        zfidentity operationId = taskObserverData->operationTaskData->operationId();
        taskObserverData->ownerTaskData->operationIds.add(operationId);
        if(!this->startFirstFlag)
        {
            this->startFirstFlag = zftrue;
            this->pimplOwner->taskOnStartFirst();
        }
        if(!taskIsDuplicate)
        {
            this->pimplOwner->taskOnStart(taskObserverData->operationTaskData);
            this->pimplOwner->operationTaskOnStart(taskObserverData->operationTaskData);
        }
        else
        {
            this->pimplOwner->taskOnStartDuplicated(taskObserverData->operationTaskData);
        }
    }

public:
    void prepareStopAll(ZF_IN ZFArrayEditable *taskArray,
                        ZF_IN_OUT ZFArrayEditable *toNotifyTaskObserverDatas)
    {
        for(zfindex iTask = 0;
            iTask < taskArray->count();
            ++iTask)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = taskArray->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTask);
            if(taskArray == this->tasks)
            {
                taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(0)->taskIsDuplicate = zffalse;
                for(zfindex iObserver = taskData->taskObserverDatas->count() - 1; iObserver != 0; --iObserver)
                {
                    taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iObserver)->taskIsDuplicate = zftrue;
                }
            }
            else
            {
                for(zfindex iObserver = taskData->taskObserverDatas->count() - 1; iObserver != zfindexMax(); --iObserver)
                {
                    taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iObserver)->taskIsDuplicate = zffalse;
                }
            }

            for(zfindex iOperationId = 0; iOperationId < taskData->operationIds.count(); ++iOperationId)
            {
                this->idGenerator.idRelease(taskData->operationIds[iOperationId]);
            }
            toNotifyTaskObserverDatas->addFrom(taskData->taskObserverDatas);
        }
        taskArray->removeAll();
    }
    void prepareStopForOperationId(ZF_IN ZFArrayEditable *taskArray,
                                   ZF_IN_OUT ZFArrayEditable *toNotifyTaskObserverDatas,
                                   ZF_IN zfidentity operationId,
                                   ZF_IN zfbool isCancel)
    {
        for(zfindex iTask = 0;
            iTask < taskArray->count();
            ++iTask)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = taskArray->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTask);
            if(isCancel)
            {
                for(zfindex iTaskObserver = 0; iTaskObserver < taskData->taskObserverDatas->count(); ++iTaskObserver)
                {
                    _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iTaskObserver);
                    if(taskObserverData->operationTaskData->operationId() == operationId)
                    {
                        toNotifyTaskObserverDatas->add(taskObserverData);
                        taskData->taskObserverDatas->remove(iTaskObserver);
                        if(taskData->taskObserverDatas->isEmpty())
                        {
                            for(zfindex iOperationIdTmp = 0; iOperationIdTmp < taskData->operationIds.count(); ++iOperationIdTmp)
                            {
                                this->idGenerator.idRelease(taskData->operationIds[iOperationIdTmp]);
                            }

                            taskArray->remove(iTask);
                            taskObserverData->taskIsDuplicate = zffalse;
                        }
                        else
                        {
                            taskObserverData->taskIsDuplicate = (taskArray == this->tasks);
                        }
                        return ;
                    }
                }
            }
            else
            {
                for(zfindex iOperationId = 0;
                    iOperationId < taskData->operationIds.count();
                    ++iOperationId)
                {
                    if(taskData->operationIds[iOperationId] == operationId)
                    {
                        if(taskArray == this->tasks)
                        {
                            taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(0)->taskIsDuplicate = zffalse;
                            for(zfindex iTaskObserver = taskData->taskObserverDatas->count() - 1; iTaskObserver != 0; --iTaskObserver)
                            {
                                taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iTaskObserver)->taskIsDuplicate = zftrue;
                            }
                        }
                        else
                        {
                            for(zfindex iTaskObserver = taskData->taskObserverDatas->count() - 1; iTaskObserver != zfindexMax(); --iTaskObserver)
                            {
                                taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iTaskObserver)->taskIsDuplicate = zffalse;
                            }
                        }

                        for(zfindex iOperationIdTmp = 0; iOperationIdTmp < taskData->operationIds.count(); ++iOperationIdTmp)
                        {
                            this->idGenerator.idRelease(taskData->operationIds[iOperationIdTmp]);
                        }

                        toNotifyTaskObserverDatas->addFrom(taskData->taskObserverDatas);
                        taskArray->remove(iTask);
                        return ;
                    }
                }
            }
        }
    }
    void prepareStopForOperationParam(ZF_IN ZFArrayEditable *taskArray,
                                      ZF_IN_OUT ZFArrayEditable *toNotifyTaskObserverDatas,
                                      ZF_IN ZFOperationParam *operationParam,
                                      ZF_IN_OPT zfbool stopOnFirstMatch = zftrue)
    {
        if(!this->pimplOwner->paramIsValid(operationParam))
        {
            return ;
        }
        for(zfindex iTask = 0;
            iTask < taskArray->count();
            ++iTask)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = taskArray->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTask);
            if(taskData->operationParam->paramIsEqual(operationParam))
            {
                if(taskArray == this->tasks)
                {
                    taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(0)->taskIsDuplicate = zffalse;
                    for(zfindex iObserver = taskData->taskObserverDatas->count() - 1; iObserver != 0; --iObserver)
                    {
                        taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iObserver)->taskIsDuplicate = zftrue;
                    }
                }
                else
                {
                    for(zfindex iObserver = taskData->taskObserverDatas->count() - 1; iObserver != 0; --iObserver)
                    {
                        taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iObserver)->taskIsDuplicate = zffalse;
                    }
                }

                for(zfindex iOperationId = 0; iOperationId < taskData->operationIds.count(); ++iOperationId)
                {
                    this->idGenerator.idRelease(taskData->operationIds[iOperationId]);
                }

                toNotifyTaskObserverDatas->addFrom(taskData->taskObserverDatas);
                taskArray->remove(iTask);
                --iTask;

                if(stopOnFirstMatch)
                {
                    return ;
                }
            }
        }
    }
    void prepareStopForCategory(ZF_IN ZFArrayEditable *taskArray,
                                ZF_IN_OUT ZFArrayEditable *toNotifyTaskObserverDatas,
                                ZF_IN ZFObject *category)
    {
        if(category == zfnull)
        {
            return ;
        }

        for(zfindex iTask = 0;
            iTask < taskArray->count();
            ++iTask)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = taskArray->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTask);
            for(zfindex iObserver = 0;
                iObserver < taskData->taskObserverDatas->count();
                ++iObserver)
            {
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iObserver);
                if(taskObserverData->operationTaskData->taskCategory()->isContain(category))
                {
                    toNotifyTaskObserverDatas->add(taskObserverData);
                    taskData->taskObserverDatas->remove(iObserver);
                    --iObserver;

                    if(taskData->taskObserverDatas->count() == 0)
                    {
                        for(zfindex iOperationId = 0; iOperationId < taskData->operationIds.count(); ++iOperationId)
                        {
                            this->idGenerator.idRelease(taskData->operationIds[iOperationId]);
                        }

                        taskArray->remove(iTask);
                        taskObserverData->taskIsDuplicate = zffalse;
                        --iTask;
                    }
                    else
                    {
                        taskObserverData->taskIsDuplicate = (taskArray == this->tasks);
                    }
                }
            }
        }
    }
    void doStop(ZF_IN ZFArrayEditable *toNotifyTaskObserverDatas, // array of _ZFP_I_ZFOperationPrivateTaskObserverData *
                ZF_IN ZFOperationResult *operationResult)
    {
        for(zfindex i = 0; i < toNotifyTaskObserverDatas->count(); ++i)
        {
            _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = toNotifyTaskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i);
            taskObserverData->operationTaskData->operationResultSet(operationResult);
            if(taskObserverData->taskIsDuplicate)
            {
                this->pimplOwner->taskOnStopDuplicated(taskObserverData->operationTaskData);
            }
            else
            {
                this->pimplOwner->taskOnStop(taskObserverData->operationTaskData);
                this->pimplOwner->operationTaskOnStop(taskObserverData->operationTaskData);
            }
            if(i == toNotifyTaskObserverDatas->count() - 1 && this->tasks->isEmpty() && this->tasksQueued->isEmpty()
                && this->startFirstFlag)
            {
                this->startFirstFlag = zffalse;
                this->pimplOwner->taskOnStopLast();
            }
        }

        // check start queued task
        while(this->tasks->count() < this->pimplOwner->taskToStartMax()
            && !this->tasksQueued->isEmpty())
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = this->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(0);
            this->tasks->add(taskData);
            this->tasksQueued->remove(0);
            for(zfindex i = 0; i < taskData->taskObserverDatas->count(); ++i)
            {
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i);
                this->doStart(taskObserverData, (i > 0));
            }
        }
    }

public:
    void notifyEventOperationTaskOnProgress(ZF_IN ZFOperationTaskData *operationTaskData,
                                            ZF_IN ZFOperationProgress *operationProgress)
    {
        operationTaskData->operationProgressSet(operationProgress);
        this->pimplOwner->taskOnProgress(operationTaskData);
        this->pimplOwner->operationTaskOnProgress(operationTaskData);
    }

public:
    _ZFP_ZFOperationPrivate(void)
    : pimplOwner(zfnull)
    , classForOperationParam(zfnull)
    , classForOperationResult(zfnull)
    , classForOperationObserver(zfnull)
    , classForOperationCache(zfnull)
    , classForOperationProgress(zfnull)
    , classForOperationTaskData(zfnull)
    , tasks(zfnull)
    , tasksQueued(zfnull)
    , caches(zfnull)
    , idGenerator()
    , cacheNeedSave(zftrue)
    , cacheNeedRestore(zftrue)
    , startFirstFlag(zffalse)
    , dummyParam(zfnull)
    {
        this->tasks = zfAlloc(ZFArrayEditable);
        this->tasksQueued = zfAlloc(ZFArrayEditable);
        this->caches = zfAlloc(ZFArrayEditable);
    }
    ~_ZFP_ZFOperationPrivate(void)
    {
        zfRelease(this->tasks);
        zfRelease(this->tasksQueued);
        zfRelease(this->caches);

        zfRelease(this->dummyParam);
    }
};

// ============================================================
// cacheTrim
typedef zfstlhashmap<ZFOperation *, zfbool,
        zfpointer_zfstlHasher<ZFOperation *>, zfpointer_zfstlHashComparer<ZFOperation *>
    > _ZFP_ZFOperationMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFOperationDataHolder, ZFLevelZFFrameworkEssential)
{
    this->cacheTrimListener = ZFCallbackForFunc(zfself::cacheTrim);
}
public:
    ZFListener cacheTrimListener;
    _ZFP_ZFOperationMapType attachedObject;
public:
    static ZFLISTENER_PROTOTYPE_EXPAND(cacheTrim)
    {
        const _ZFP_ZFOperationMapType &attachedObject = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFOperationDataHolder)->attachedObject;
        for(_ZFP_ZFOperationMapType::const_iterator it = attachedObject.begin(); it != attachedObject.end(); ++it)
        {
            it->first->cacheTrim();
        }
    }
    void cacheTrimListenerSetup(ZF_IN ZFOperation *operation, ZF_IN zfbool autoTrim)
    {
        zfCoreMutexLocker();

        if(autoTrim)
        {
            this->attachedObject[operation] = zftrue;
        }
        else
        {
            this->attachedObject.erase(operation);
        }

        if(autoTrim && this->attachedObject.size() == 1)
        {
            ZFObjectGlobalEventObserver().observerAdd(
                ZFGlobalEvent::EventAppOnMemoryLow(),
                this->cacheTrimListener);
        }
        else if(!autoTrim && this->attachedObject.size() == 0)
        {
            ZFObjectGlobalEventObserver().observerRemove(
                ZFGlobalEvent::EventAppOnMemoryLow(),
                this->cacheTrimListener);
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFOperationDataHolder)

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFOperation, zfbool, cacheTrimWhenReceiveMemoryWarning)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFOperationDataHolder)->cacheTrimListenerSetup(
        this, this->cacheTrimWhenReceiveMemoryWarning());
}

void ZFOperation::cacheTrim(void)
{
    this->cacheTrimBySize(this->cacheTrimThreshold());
}

void ZFOperation::cacheTrimBySize(ZF_IN zfindex size)
{
    zfsynchronize(this);
    if(size >= d->caches->count())
    {
        return ;
    }
    zfblockedAlloc(ZFArrayEditable, tmp);
    for(zfindex i = size; i < d->caches->count(); ++i)
    {
        tmp->add(d->caches->get(i));
    }
    d->caches->remove(size, zfindexMax());
    for(zfindex i = 0; i < tmp->count(); ++i)
    {
        this->cacheOnRemove(tmp->get<ZFOperationCache *>(i));
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFOperation)

ZFOBSERVER_EVENT_REGISTER(ZFOperation, OperationCacheOnMatch)
ZFOBSERVER_EVENT_REGISTER(ZFOperation, OperationCacheOnInvalid)
ZFOBSERVER_EVENT_REGISTER(ZFOperation, OperationCacheOnExpire)
ZFOBSERVER_EVENT_REGISTER(ZFOperation, OperationTaskOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFOperation, OperationTaskOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFOperation, OperationTaskOnProgress)

void ZFOperation::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFOperationPrivate);
    d->pimplOwner = this;
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFOperationDataHolder)->cacheTrimListenerSetup(this, this->cacheTrimWhenReceiveMemoryWarning());
}
void ZFOperation::objectOnDealloc(void)
{
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFOperationDataHolder)->cacheTrimListenerSetup(this, zffalse);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}
void ZFOperation::objectOnDeallocPrepare(void)
{
    this->taskStopAll();
    d->caches->removeAll();
    zfsuper::objectOnDeallocPrepare();
}

// ============================================================
// type create
ZFMETHOD_DEFINE_0(ZFOperation, zfautoObject, createParam)
{
    return this->classForOperationParam()->newInstance();
}
ZFMETHOD_DEFINE_0(ZFOperation, zfautoObject, createResult)
{
    return this->classForOperationResult()->newInstance();
}
ZFMETHOD_DEFINE_1(ZFOperation, zfautoObject, createResult,
                  ZFMP_IN(ZFResultTypeEnum, resultType))
{
    zfautoObject operationResultTmp = this->createResult();
    ZFOperationResult *operationResult = operationResultTmp.to<ZFOperationResult *>();
    operationResult->resultTypeSet(resultType);
    return operationResultTmp;
}
ZFMETHOD_DEFINE_0(ZFOperation, zfautoObject, createObserver)
{
    return this->classForOperationObserver()->newInstance();
}
ZFMETHOD_DEFINE_0(ZFOperation, zfautoObject, createCache)
{
    return this->classForOperationCache()->newInstance();
}
ZFMETHOD_DEFINE_4(ZFOperation, zfautoObject, createCache,
                  ZFMP_IN(ZFOperationParam *, operationParam),
                  ZFMP_IN(ZFOperationResult *, operationResult),
                  ZFMP_IN_OPT(zftimet, cacheExpireTime, zftimetZero()),
                  ZFMP_IN_OPT(zftimet, cacheTime, zftimetZero()))
{
    zfautoObject operationCacheTmp = this->createCache();
    ZFOperationCache *operationCache = operationCacheTmp.to<ZFOperationCache *>();
    operationCache->operationParamSet(operationParam);
    operationCache->operationResultSet(operationResult);
    operationCache->cacheExpireTimeSet((cacheExpireTime <= 0) ? this->cacheExpireTimeDefault() : cacheExpireTime);
    operationCache->cacheTimeSet((cacheTime == 0) ? ZFTime::currentTimeValue().sec : cacheTime);
    return operationCacheTmp;
}
ZFMETHOD_DEFINE_0(ZFOperation, zfautoObject, createProgress)
{
    return this->classForOperationProgress()->newInstance();
}
ZFMETHOD_DEFINE_0(ZFOperation, zfautoObject, createTaskData)
{
    return this->classForOperationTaskData()->newInstance();
}
ZFMETHOD_DEFINE_6(ZFOperation, zfautoObject, createTaskData,
                  ZFMP_IN(ZFOperationParam *, operationParam),
                  ZFMP_IN_OPT(ZFOperationResult *, operationResult, zfnull),
                  ZFMP_IN_OPT(ZFOperationObserver *, operationObserver, zfnull),
                  ZFMP_IN_OPT(ZFOperationCache *, operationCache, zfnull),
                  ZFMP_IN_OPT(zfidentity, operationId, zfidentityInvalid()),
                  ZFMP_IN_OPT(ZFOperationProgress *, operationProgress, zfnull))
{
    zfautoObject operationTaskDataTmp = this->createTaskData();
    ZFOperationTaskData *operationTaskData = operationTaskDataTmp.to<ZFOperationTaskData *>();
    operationTaskData->operationParamSet(operationParam);
    operationTaskData->operationResultSet(operationResult);
    operationTaskData->operationObserverSet(operationObserver);
    operationTaskData->operationCacheSet(operationCache);
    operationTaskData->operationIdSet(operationId);
    operationTaskData->operationProgressSet(operationProgress);
    return operationTaskDataTmp;
}

static const ZFClass *_ZFP_ZFOperation_findTypeClass(ZF_IN const ZFClass *operationClass,
                                                     ZF_IN const zfchar *desiredTypeName,
                                                     ZF_IN const ZFClass *endTypeClass)
{
    const ZFClass *operationClassToCheck = operationClass;
    while(operationClassToCheck != zfnull && operationClassToCheck != ZFOperation::ClassData())
    {
        const ZFClass *tmp = ZFClass::classForName(zfsConnectLineFree(operationClassToCheck->classNameFull(), desiredTypeName));
        if(tmp != zfnull)
        {
            return tmp;
        }
        operationClassToCheck = operationClassToCheck->parentClass();
    }
    return endTypeClass;
}
const ZFClass *ZFOperation::classForOperationParam(void)
{
    if(d->classForOperationParam == zfnull)
    {
        d->classForOperationParam = _ZFP_ZFOperation_findTypeClass(this->classData(), "Param", ZFOperationParam::ClassData());
    }
    return d->classForOperationParam;
}
const ZFClass *ZFOperation::classForOperationResult(void)
{
    if(d->classForOperationResult == zfnull)
    {
        d->classForOperationResult = _ZFP_ZFOperation_findTypeClass(this->classData(), "Result", ZFOperationResult::ClassData());
    }
    return d->classForOperationResult;
}
const ZFClass *ZFOperation::classForOperationObserver(void)
{
    if(d->classForOperationObserver == zfnull)
    {
        d->classForOperationObserver = _ZFP_ZFOperation_findTypeClass(this->classData(), "Observer", ZFOperationObserver::ClassData());
    }
    return d->classForOperationObserver;
}
const ZFClass *ZFOperation::classForOperationCache(void)
{
    if(d->classForOperationCache == zfnull)
    {
        d->classForOperationCache = _ZFP_ZFOperation_findTypeClass(this->classData(), "Cache", ZFOperationCache::ClassData());
    }
    return d->classForOperationCache;
}
const ZFClass *ZFOperation::classForOperationProgress(void)
{
    if(d->classForOperationProgress == zfnull)
    {
        d->classForOperationProgress = _ZFP_ZFOperation_findTypeClass(this->classData(), "Progress", ZFOperationProgress::ClassData());
    }
    return d->classForOperationProgress;
}
const ZFClass *ZFOperation::classForOperationTaskData(void)
{
    if(d->classForOperationTaskData == zfnull)
    {
        d->classForOperationTaskData = _ZFP_ZFOperation_findTypeClass(this->classData(), "TaskData", ZFOperationTaskData::ClassData());
    }
    return d->classForOperationTaskData;
}

// ============================================================
// operation control
ZFMETHOD_DEFINE_2(ZFOperation, zfidentity, taskStart,
                  ZFMP_IN_OPT(ZFOperationParam *, operationParam, zfnull),
                  ZFMP_IN_OPT(ZFOperationObserver *, operationObserver, zfnull))
{
    return this->taskStart(this->createTaskData(operationParam, zfnull, operationObserver).to<ZFOperationTaskData *>());
}
ZFMETHOD_DEFINE_1(ZFOperation, zfidentity, taskStart,
                  ZFMP_IN(ZFOperationTaskData *, operationTaskData))
{
    if(operationTaskData == zfnull)
    {
        zfCoreLog("invalid param");
        return zfidentityInvalid();
    }
    zfCoreAssert(operationTaskData->operationOwner() == zfnull);

    zfsynchronize(this);

    operationTaskData->operationOwnerSet(this);

    ZFOperationParam *operationParam = operationTaskData->operationParam();
    if(operationParam == zfnull)
    {
        if(this->taskAllowDummyParam())
        {
            operationParam = d->dummyParamAccess();
        }
        else
        {
            zfCoreLog("dummy param is not allowed");
            return zfidentityInvalid();
        }
    }
    if(!this->paramIsValid(operationParam))
    {
        return zfidentityInvalid();
    }
    if(!this->taskOnCheckNeedStart(operationTaskData))
    {
        operationTaskData->operationResultSet(this->createResult(ZFResultType::e_Cancel).to<ZFOperationResult *>());
        this->operationTaskOnStart(operationTaskData);
        this->operationTaskOnStop(operationTaskData);
        return zfidentityInvalid();
    }

    this->cacheRestore();

    // search for cache
    zfindex cacheIndex = d->caches->count() - 1;
    while(cacheIndex != zfindexMax())
    {
        if(d->caches->get<ZFOperationCache *>(cacheIndex)->operationParam()->paramIsEqual(operationParam))
        {
            break;
        }
        --cacheIndex;
    }
    if(cacheIndex != zfindexMax())
    {
        this->cacheSaveRequest();

        ZFOperationCache *operationCache = d->caches->get<ZFOperationCache *>(cacheIndex);
        zfRetain(operationCache);
        zfblockedRelease(operationCache);
        d->caches->remove(cacheIndex);

        zfbool cacheValid = this->cacheIsValid(operationCache);
        zfbool cacheIsExpired = operationCache->cacheIsExpired();
        if(!cacheValid)
        {
            this->cacheOnInvalid(operationCache);
            this->operationCacheOnInvalid(operationCache);
        }
        if(cacheIsExpired)
        {
            this->cacheOnExpire(operationCache);
            this->operationCacheOnExpire(operationCache);
        }
        if(cacheValid)
        {
            {
                operationTaskData->operationCacheSet(operationCache);
                this->cacheOnMatch(operationCache);
                this->operationCacheOnMatch(operationTaskData);
                zfRelease(operationTaskData);
            }

            ZFOperationCacheMatchActionEnum cacheMatchAction = operationTaskData->cacheMatchAction();
            if(cacheMatchAction == ZFOperationCacheMatchAction::e_Unspecified)
            {
                cacheMatchAction = this->cacheMatchAction();
            }
            if(cacheMatchAction == ZFOperationCacheMatchAction::e_Unspecified)
            {
                cacheMatchAction = ZFOperationCacheMatchAction::e_NotifyFinish;
            }
            switch(cacheMatchAction)
            {
                case ZFOperationCacheMatchAction::e_NotifyFinish:
                {
                    if(!cacheIsExpired)
                    {
                        d->caches->add(operationCache);
                        operationTaskData->operationResultSet(operationCache->operationResult());
                        operationTaskData->operationCacheSet(operationCache);
                        this->operationTaskOnStart(operationTaskData);
                        this->operationTaskOnStop(operationTaskData);
                        return zfidentityInvalid();
                    }
                    else
                    {
                        this->cacheOnRemove(operationCache);
                        break;
                    }
                }
                case ZFOperationCacheMatchAction::e_NotifyFinishThenRemove:
                {
                    if(!cacheIsExpired)
                    {
                        operationTaskData->operationResultSet(operationCache->operationResult());
                        operationTaskData->operationCacheSet(operationCache);
                        this->operationTaskOnStart(operationTaskData);
                        this->operationTaskOnStop(operationTaskData);
                        this->cacheRemove(operationParam);
                        return zfidentityInvalid();
                    }
                    else
                    {
                        this->cacheOnRemove(operationCache);
                        break;
                    }
                }
                case ZFOperationCacheMatchAction::e_StartNew:
                {
                    if(!cacheIsExpired)
                    {
                        d->caches->add(operationCache);
                    }
                    else
                    {
                        this->cacheOnRemove(operationCache);
                    }
                    break;
                }
                case ZFOperationCacheMatchAction::e_StartNewThenRemove:
                {
                    this->cacheOnRemove(operationCache);
                    break;
                }
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zfidentityInvalid();
            }
        }
        else
        {
            this->cacheOnRemove(operationCache);
        }
    }

    // find duplicate operation
    ZFOperationTaskDuplicateActionEnum taskDuplicateAction = operationTaskData->taskDuplicateAction();
    if(taskDuplicateAction == ZFOperationTaskDuplicateAction::e_Unspecified)
    {
        taskDuplicateAction = this->taskDuplicateAction();
    }
    if(taskDuplicateAction == ZFOperationTaskDuplicateAction::e_Unspecified)
    {
        taskDuplicateAction = ZFOperationTaskDuplicateAction::e_Merge;
    }
    if(taskDuplicateAction != ZFOperationTaskDuplicateAction::e_NewTask)
    {
        zfbool isInQueue = zffalse;
        _ZFP_I_ZFOperationPrivateTaskData *dupTaskData = d->findTaskDataForOperationParam(operationParam, zftrue, &isInQueue);
        if(dupTaskData != zfnull)
        {
            if(taskDuplicateAction == ZFOperationTaskDuplicateAction::e_Merge)
            {
                operationTaskData->operationIdSet(d->idGenerator.idAcquire());
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = zfAlloc(_ZFP_I_ZFOperationPrivateTaskObserverData);
                taskObserverData->ownerTaskData = dupTaskData;
                taskObserverData->operationTaskData = operationTaskData;
                taskObserverData->cacheExpireTime = operationTaskData->cacheExpireTime();
                dupTaskData->taskObserverDatas->add(taskObserverData);
                if(!isInQueue)
                {
                    d->doStart(taskObserverData, zftrue);
                }
                return operationTaskData->operationId();
            }
            else
            {
                operationTaskData->operationResultSet(this->createResult(ZFResultType::e_Cancel).to<ZFOperationResult *>());
                return zfidentityInvalid();
            }
        }
    }

    zfblockedAlloc(_ZFP_I_ZFOperationPrivateTaskData, taskData);
    zfRetainChange(taskData->operationParam, operationParam);

    operationTaskData->operationIdSet(d->idGenerator.idAcquire());
    zfblockedAlloc(_ZFP_I_ZFOperationPrivateTaskObserverData, taskObserverData);
    taskData->taskObserverDatas->add(taskObserverData);
    taskObserverData->ownerTaskData = taskData;
    taskObserverData->operationTaskData = operationTaskData;
    taskObserverData->cacheExpireTime = operationTaskData->cacheExpireTime();

    if(d->tasks->count() > 10000 || d->tasksQueued->count() > 10000)
    {
        zfCoreLog("[%s] too many operations attached, do you forget taskNotifyFinish?", this->objectInfoOfInstance().cString());
    }

    if(d->tasks->count() < this->taskToStartMax())
    {
        d->tasks->add(taskData);
        d->doStart(taskObserverData, zffalse);
    }
    else
    {
        d->tasksQueued->add(taskData);
    }

    return operationTaskData->operationId();
}
ZFMETHOD_DEFINE_1(ZFOperation, void, taskStop,
                  ZFMP_IN(zfidentity, operationId))
{
    if(operationId != zfidentityInvalid())
    {
        zfsynchronizeLock(this);
        zfblockedAlloc(ZFArrayEditable, toNotifyTaskObserverDatas);
        d->prepareStopForOperationId(d->tasks, toNotifyTaskObserverDatas, operationId, zftrue);
        if(toNotifyTaskObserverDatas->isEmpty())
        {
            d->prepareStopForOperationId(d->tasksQueued, toNotifyTaskObserverDatas, operationId, zftrue);
        }
        d->doStop(toNotifyTaskObserverDatas, this->createResult(ZFResultType::e_Cancel).to<ZFOperationResult *>());
        zfsynchronizeUnlock(this);
    }
}
ZFMETHOD_DEFINE_1(ZFOperation, zfautoObject, taskStopAndGetResult,
                  ZFMP_IN(zfidentity, operationId))
{
    if(operationId != zfidentityInvalid())
    {
        zfsynchronize(this);
        zfautoObject operationResultTmp = this->createResult(ZFResultType::e_Cancel);
        ZFOperationResult *operationResult = operationResultTmp.to<ZFOperationResult *>();
        zfblockedAlloc(ZFArrayEditable, toNotifyTaskObserverDatas);
        d->prepareStopForOperationId(d->tasks, toNotifyTaskObserverDatas, operationId, zftrue);
        if(toNotifyTaskObserverDatas->isEmpty())
        {
            d->prepareStopForOperationId(d->tasksQueued, toNotifyTaskObserverDatas, operationId, zftrue);
        }
        d->doStop(toNotifyTaskObserverDatas, operationResult);

        if(toNotifyTaskObserverDatas->count() == 1)
        {
            operationResult = toNotifyTaskObserverDatas->getFirst<_ZFP_I_ZFOperationPrivateTaskObserverData *>()->operationTaskData->operationResult();
            return operationResult;
        }
    }
    return zfnull;
}
ZFMETHOD_DEFINE_1(ZFOperation, void, taskStop,
                  ZFMP_IN_OPT(ZFOperationParam *, operationParam, zfnull))
{
    zfsynchronize(this);
    if(operationParam == zfnull)
    {
        operationParam = d->dummyParamAccess();
    }
    if(!this->paramIsValid(operationParam))
    {
        return ;
    }
    zfblockedAlloc(ZFArrayEditable, toNotifyTaskObserverDatas);
    d->prepareStopForOperationParam(d->tasks, toNotifyTaskObserverDatas, operationParam, zffalse);
    d->prepareStopForOperationParam(d->tasksQueued, toNotifyTaskObserverDatas, operationParam, zffalse);
    d->doStop(toNotifyTaskObserverDatas, this->createResult(ZFResultType::e_Cancel).to<ZFOperationResult *>());
}
ZFMETHOD_DEFINE_1(ZFOperation, void, taskStopForCategory,
                  ZFMP_IN(ZFObject *, category))
{
    if(category != zfnull)
    {
        zfsynchronizeLock(this);
        zfblockedAlloc(ZFArrayEditable, toNotifyTaskObserverDatas);
        d->prepareStopForCategory(d->tasks, toNotifyTaskObserverDatas, category);
        d->prepareStopForCategory(d->tasksQueued, toNotifyTaskObserverDatas, category);
        d->doStop(toNotifyTaskObserverDatas, this->createResult(ZFResultType::e_Cancel).to<ZFOperationResult *>());
        zfsynchronizeUnlock(this);
    }
}
ZFMETHOD_DEFINE_0(ZFOperation, void, taskStopAll)
{
    zfsynchronizeLock(this);
    zfblockedAlloc(ZFArrayEditable, toNotifyTaskObserverDatas);
    d->prepareStopAll(d->tasks, toNotifyTaskObserverDatas);
    d->prepareStopAll(d->tasksQueued, toNotifyTaskObserverDatas);
    d->doStop(toNotifyTaskObserverDatas, this->createResult(ZFResultType::e_Cancel).to<ZFOperationResult *>());
    zfsynchronizeUnlock(this);
}

ZFMETHOD_DEFINE_1(ZFOperation, void, taskStopObserver,
                  ZFMP_IN(zfidentity, operationId))
{
    if(operationId != zfidentityInvalid())
    {
        zfsynchronize(this);
        _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = d->findTaskObserverDataForOperationId(operationId);
        if(taskObserverData != zfnull)
        {
            taskObserverData->operationTaskData->operationObserverSet(zfnull);
        }
    }
}
ZFMETHOD_DEFINE_1(ZFOperation, void, taskStopObserver,
                  ZFMP_IN_OPT(ZFOperationParam *, operationParam, zfnull))
{
    zfsynchronizeLock(this);
    if(operationParam == zfnull)
    {
        operationParam = d->dummyParamAccess();
    }
    if(!this->paramIsValid(operationParam))
    {
        return ;
    }
    for(zfindex i = 0; i < d->tasks->count(); ++i)
    {
        _ZFP_I_ZFOperationPrivateTaskData *taskData = d->tasks->get<_ZFP_I_ZFOperationPrivateTaskData *>(i);
        if(taskData->operationParam->paramIsEqual(operationParam))
        {
            for(zfindex iObserver = 0; iObserver < taskData->taskObserverDatas->count(); ++iObserver)
            {
                taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i)->operationTaskData->operationObserverSet(zfnull);
            }
        }
    }
    for(zfindex i = 0; i < d->tasksQueued->count(); ++i)
    {
        _ZFP_I_ZFOperationPrivateTaskData *taskData = d->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(i);
        if(taskData->operationParam->paramIsEqual(operationParam))
        {
            for(zfindex iObserver = 0; iObserver < taskData->taskObserverDatas->count(); ++iObserver)
            {
                taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i)->operationTaskData->operationObserverSet(zfnull);
            }
        }
    }
}
ZFMETHOD_DEFINE_1(ZFOperation, void, taskStopObserverForCategory,
                  ZFMP_IN(ZFObject *, category))
{
    if(category != zfnull)
    {
        zfsynchronize(this);

        for(zfindex i = 0; i < d->tasks->count(); ++i)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = d->tasks->get<_ZFP_I_ZFOperationPrivateTaskData *>(i);
            for(zfindex iObserver = 0; iObserver < taskData->taskObserverDatas->count(); ++iObserver)
            {
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i);
                if(taskObserverData->operationTaskData->taskCategory()->isContain(category))
                {
                    taskObserverData->operationTaskData->operationObserverSet(zfnull);
                }
            }
        }
        for(zfindex i = 0; i < d->tasksQueued->count(); ++i)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = d->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(i);
            for(zfindex iObserver = 0; iObserver < taskData->taskObserverDatas->count(); ++iObserver)
            {
                _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i);
                if(taskObserverData->operationTaskData->taskCategory()->isContain(category))
                {
                    taskObserverData->operationTaskData->operationObserverSet(zfnull);
                }
            }
        }
    }
}

ZFMETHOD_DEFINE_0(ZFOperation, zfindex, taskCount)
{
    zfsynchronize(this);
    return d->tasks->count();
}
ZFMETHOD_DEFINE_0(ZFOperation, zfindex, taskQueuedCount)
{
    zfsynchronize(this);
    return d->tasksQueued->count();
}
ZFMETHOD_DEFINE_0(ZFOperation, zfbool, taskRunning)
{
    zfsynchronize(this);
    return (!d->tasks->isEmpty() || !d->tasksQueued->isEmpty());
}

ZFMETHOD_DEFINE_1(ZFOperation, zfbool, taskIsAlive,
                  ZFMP_IN(ZFOperationParam *, operationParam))
{
    zfsynchronize(this);
    return (d->findTaskDataForOperationParam(operationParam) != zfnull);
}
ZFMETHOD_DEFINE_1(ZFOperation, zfbool, taskIsAlive,
                  ZFMP_IN(zfidentity, operationId))
{
    zfsynchronize(this);
    return (d->findTaskDataForOperationId(operationId) != zfnull);
}

ZFMETHOD_DEFINE_1(ZFOperation, zfbool, paramIsValid,
                  ZFMP_IN(ZFOperationParam *, operationParam))
{
    return zftrue;
}
ZFMETHOD_DEFINE_2(ZFOperation, void, taskNotifyFinish,
                  ZFMP_IN(ZFOperationParam *, operationParam),
                  ZFMP_IN(ZFOperationResult *, operationResult))
{
    zfsynchronize(this);

    if(operationParam == zfnull)
    {
        operationParam = d->dummyParamAccess();
    }
    zfCoreAssertWithMessage(this->paramIsValid(operationParam) && operationResult != zfnull, "invalid param or result");

    zfblockedAlloc(ZFArrayEditable, toNotifyTaskObserverDatas);
    d->prepareStopForOperationParam(d->tasks, toNotifyTaskObserverDatas, operationParam);

    if(toNotifyTaskObserverDatas->isEmpty())
    { // notify a canceled task, not an error, simply return
        return ;
    }
    d->doStop(toNotifyTaskObserverDatas, operationResult);

    d->checkAddCache(
        operationParam,
        operationResult,
        toNotifyTaskObserverDatas->getLast<_ZFP_I_ZFOperationPrivateTaskObserverData *>()->cacheExpireTime);
}
ZFMETHOD_DEFINE_2(ZFOperation, void, taskNotifyFinish,
                  ZFMP_IN(zfidentity, operationId),
                  ZFMP_IN(ZFOperationResult *, operationResult))
{
    zfCoreAssertWithMessage(operationResult != zfnull, "result is null");

    if(operationId == zfidentityInvalid())
    {
        return ;
    }

    zfsynchronize(this);

    zfblockedAlloc(ZFArrayEditable, toNotifyTaskObserverDatas);
    d->prepareStopForOperationId(d->tasks, toNotifyTaskObserverDatas, operationId, operationResult->resultType() == ZFResultType::e_Cancel);

    if(toNotifyTaskObserverDatas->isEmpty())
    { // notify a canceled task, not an error, simply return
        return ;
    }
    d->doStop(toNotifyTaskObserverDatas, operationResult);

    d->checkAddCache(
        toNotifyTaskObserverDatas->getFirst<_ZFP_I_ZFOperationPrivateTaskObserverData *>()->ownerTaskData->operationParam,
        operationResult,
        toNotifyTaskObserverDatas->getLast<_ZFP_I_ZFOperationPrivateTaskObserverData *>()->cacheExpireTime);
}
ZFMETHOD_DEFINE_2(ZFOperation, void, taskNotifyProgress,
                  ZFMP_IN(ZFOperationParam *, operationParam),
                  ZFMP_IN_OPT(ZFOperationProgress *, operationProgress, zfnull))
{
    zfsynchronize(this);
    if(operationParam == zfnull)
    {
        operationParam = d->dummyParamAccess();
    }
    zfCoreAssertWithMessage(this->paramIsValid(operationParam), "invalid param");
    _ZFP_I_ZFOperationPrivateTaskData *taskData = d->findTaskDataForOperationParam(operationParam, zffalse);
    if(taskData != zfnull)
    {
        for(zfindex i = 0; i < taskData->taskObserverDatas->count(); ++i)
        {
            d->notifyEventOperationTaskOnProgress(
                taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i)->operationTaskData,
                operationProgress);
        }
    }
}
ZFMETHOD_DEFINE_2(ZFOperation, void, taskNotifyProgress,
                  ZFMP_IN(zfidentity, operationId),
                  ZFMP_IN_OPT(ZFOperationProgress *, operationProgress, zfnull))
{
    if(operationId != zfidentityInvalid())
    {
        zfsynchronize(this);
        _ZFP_I_ZFOperationPrivateTaskData *taskData = d->findTaskDataForOperationId(operationId, zffalse);
        if(taskData != zfnull)
        {
            for(zfindex i = 0; i < taskData->taskObserverDatas->count(); ++i)
            {
                d->notifyEventOperationTaskOnProgress(
                    taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i)->operationTaskData,
                    operationProgress);
            }
        }
    }
}

void ZFOperation::taskOnStart(ZF_IN ZFOperationTaskData *operationTaskData)
{
    // nothing to do
}

// ============================================================
// observer
void ZFOperation::operationCacheOnMatch(ZF_IN ZFOperationTaskData *operationTaskData)
{
    if(operationTaskData->operationObserver() != zfnull)
    {
        operationTaskData->operationObserver()->observerNotify(
            ZFOperation::EventOperationCacheOnMatch(),
            operationTaskData);
    }
    this->observerNotify(ZFOperation::EventOperationCacheOnMatch(), operationTaskData);
}
void ZFOperation::operationCacheOnInvalid(ZF_IN ZFOperationCache *operationCache)
{
    this->observerNotify(ZFOperation::EventOperationCacheOnInvalid(), operationCache);
}
void ZFOperation::operationCacheOnExpire(ZF_IN ZFOperationCache *operationCache)
{
    this->observerNotify(ZFOperation::EventOperationCacheOnExpire(), operationCache);
}
void ZFOperation::operationTaskOnStart(ZF_IN ZFOperationTaskData *operationTaskData)
{
    if(operationTaskData->operationObserver() != zfnull)
    {
        operationTaskData->operationObserver()->observerNotify(
            ZFOperation::EventOperationTaskOnStart(),
            operationTaskData);
    }
    this->observerNotify(ZFOperation::EventOperationTaskOnStart(), operationTaskData);
}
void ZFOperation::operationTaskOnStop(ZF_IN ZFOperationTaskData *operationTaskData)
{
    if(operationTaskData->operationObserver() != zfnull)
    {
        operationTaskData->operationObserver()->observerNotify(
            ZFOperation::EventOperationTaskOnStop(),
            operationTaskData);
    }
    this->observerNotify(ZFOperation::EventOperationTaskOnStop(), operationTaskData);
}
void ZFOperation::operationTaskOnProgress(ZF_IN ZFOperationTaskData *operationTaskData)
{
    if(operationTaskData->operationObserver() != zfnull)
    {
        operationTaskData->operationObserver()->observerNotify(
            ZFOperation::EventOperationTaskOnProgress(),
            operationTaskData);
    }
    this->observerNotify(ZFOperation::EventOperationTaskOnProgress(), operationTaskData);
}

// ============================================================
// advance operation control
ZFMETHOD_DEFINE_1(ZFOperation, ZFOperationTaskData *, operationGetTaskData,
                  ZFMP_IN(zfidentity, operationId))
{
    zfsynchronize(this);
    _ZFP_I_ZFOperationPrivateTaskObserverData *taskObserverData = d->findTaskObserverDataForOperationId(operationId);
    if(taskObserverData != zfnull)
    {
        return taskObserverData->operationTaskData;
    }
    return zfnull;
}
ZFMETHOD_DEFINE_2(ZFOperation, void, operationGetTaskList,
                  ZFMP_IN_OUT(ZFArrayEditable *, taskDatas),
                  ZFMP_IN(zfidentity, operationId))
{
    if(taskDatas != zfnull)
    {
        zfsynchronize(this);
        _ZFP_I_ZFOperationPrivateTaskData *taskData = d->findTaskDataForOperationId(operationId);
        if(taskData != zfnull)
        {
            ZFArray *taskObserverDatas = taskData->taskObserverDatas;
            for(zfindex i = 0; i < taskObserverDatas->count(); ++i)
            {
                taskDatas->add(taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(i)->operationTaskData);
            }
        }
    }
}
ZFMETHOD_DEFINE_1(ZFOperation, void, operationGetTaskList,
                  ZFMP_IN_OUT(ZFArrayEditable *, taskDatas))
{
    if(taskDatas != zfnull)
    {
        zfsynchronize(this);
        for(zfindex iTaskData = d->tasks->count(); iTaskData < d->tasks->count(); ++iTaskData)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = d->tasks->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
            for(zfindex iTaskObserverData = 0; iTaskObserverData < taskData->taskObserverDatas->count(); ++iTaskObserverData)
            {
                taskDatas->add(taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iTaskObserverData)->operationTaskData);
            }
        }
    }
}
ZFMETHOD_DEFINE_1(ZFOperation, void, operationGetTaskListQueued,
                  ZFMP_IN_OUT(ZFArrayEditable *, taskDatas))
{
    if(taskDatas != zfnull)
    {
        zfsynchronize(this);
        for(zfindex iTaskData = d->tasksQueued->count(); iTaskData < d->tasksQueued->count(); ++iTaskData)
        {
            _ZFP_I_ZFOperationPrivateTaskData *taskData = d->tasksQueued->get<_ZFP_I_ZFOperationPrivateTaskData *>(iTaskData);
            for(zfindex iTaskObserverData = 0; iTaskObserverData < taskData->taskObserverDatas->count(); ++iTaskObserverData)
            {
                taskDatas->add(taskData->taskObserverDatas->get<_ZFP_I_ZFOperationPrivateTaskObserverData *>(iTaskObserverData)->operationTaskData);
            }
        }
    }
}

// ============================================================
// cache control
ZFMETHOD_DEFINE_1(ZFOperation, void, cacheAdd,
                  ZFMP_IN(ZFOperationCache *, operationCache))
{
    zfCoreAssertWithMessage(this->cacheIsValid(operationCache), "adding a invalid cache");
    if(operationCache->cacheIsExpired())
    {
        return ;
    }
    zfbool found = zffalse;
    for(zfindex i = 0; i < d->caches->count(); ++i)
    {
        if(d->caches->get<ZFOperationCache *>(i)->operationParam()->paramIsEqual(operationCache->operationParam()))
        {
            this->cacheOnRemove(d->caches->get<ZFOperationCache *>(i));
            d->caches->set(i, operationCache);
            found = zftrue;
            break;
        }
    }
    if(!found)
    {
        d->caches->add(operationCache);
        this->cacheOnAdd(operationCache);
    }
    else
    {
        this->cacheOnAdd(operationCache);
    }
    while(d->caches->count() > this->cacheMaxSize())
    {
        ZFOperationCache *cacheTmp = zfRetain(d->caches->get<ZFOperationCache *>(0));
        d->caches->remove(0);
        this->cacheOnRemove(cacheTmp);
        zfRelease(cacheTmp);
    }
}
ZFMETHOD_DEFINE_1(ZFOperation, void, cacheRemove,
                  ZFMP_IN(ZFOperationParam *, operationParam))
{
    zfsynchronize(this);
    if(operationParam == zfnull)
    {
        operationParam = d->dummyParamAccess();
    }
    if(!this->paramIsValid(operationParam))
    {
        return ;
    }

    for(zfindex i = 0; i < d->caches->count(); ++i)
    {
        if(d->caches->get<ZFOperationCache *>(i)->operationParam()->paramIsEqual(operationParam))
        {
            ZFOperationCache *cache = zfRetain(d->caches->get<ZFOperationCache *>(i));
            d->caches->remove(i);
            this->cacheOnRemove(cache);
            zfRelease(cache);
            return ;
        }
    }
}
ZFMETHOD_DEFINE_0(ZFOperation, void, cacheRemoveAll)
{
    ZFArrayEditable *tmpArray = zfAlloc(ZFArrayEditable, d->caches);
    d->caches->removeAll();
    for(zfindex i = 0; i < tmpArray->count(); ++i)
    {
        this->cacheOnRemove(tmpArray->get<ZFOperationCache *>(i));
    }
    zfRelease(tmpArray);
}
ZFMETHOD_DEFINE_1(ZFOperation, zfbool, cacheIsValid,
                  ZFMP_IN(ZFOperationCache *, operationCache))
{
    return (operationCache != zfnull
        && this->paramIsValid(operationCache->operationParam())
        && operationCache->operationResult()->resultType() == ZFResultType::e_Success);
}
ZFMETHOD_DEFINE_0(ZFOperation, ZFCoreArray<zfautoObject>, cacheGetAll)
{
    ZFCoreArray<zfautoObject> ret;
    this->cacheGetAllT(ret);
    return ret;
}
ZFMETHOD_DEFINE_1(ZFOperation, void, cacheGetAllT,
                  ZFMP_OUT(ZFCoreArray<zfautoObject> &, ret))
{
    for(zfindex i = 0; i < d->caches->count(); ++i)
    {
        ret.add(d->caches->get(i));
    }
}

ZFMETHOD_DEFINE_0(ZFOperation, void, cacheSaveRequest)
{
    d->cacheNeedSave = zftrue;
}
ZFMETHOD_DEFINE_0(ZFOperation, void, cacheSave)
{
    if(d->cacheNeedSave)
    {
        d->cacheNeedSave = zffalse;

        this->cacheOnSave(d->caches);
    }
}
ZFMETHOD_DEFINE_0(ZFOperation, void, cacheRestoreRequest)
{
    d->cacheNeedRestore = zftrue;
}
ZFMETHOD_DEFINE_0(ZFOperation, void, cacheRestore)
{
    if(d->cacheNeedRestore)
    {
        d->cacheNeedRestore = zffalse;

        this->cacheRemoveAll();
        zfblockedAlloc(ZFArrayEditable, tmpArray);
        this->cacheOnRestore(tmpArray);
        for(zfindex i = 0; i < tmpArray->count(); ++i)
        {
            this->cacheAdd(tmpArray->get<ZFOperationCache *>(i));
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

