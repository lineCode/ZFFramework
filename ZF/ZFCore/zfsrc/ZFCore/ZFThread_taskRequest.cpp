/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFThread_taskRequest.h"
#include "protocol/ZFProtocolZFThreadTaskRequest.h"
#include "ZFArray.h"
#include "ZFMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// task processing
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestCallback_action);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask_action);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask_action);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge_action);

static ZFListener *_ZFP_ZFThread_wrappedTaskCallback = zfnull;
static ZFListener *_ZFP_ZFThread_mergeCallbackIgnoreOldTask = zfnull;
static ZFListener *_ZFP_ZFThread_mergeCallbackIgnoreNewTask = zfnull;
static ZFListener *_ZFP_ZFThread_mergeCallbackDoNotMerge = zfnull;
static zfbool _ZFP_ZFThread_taskRunning = zffalse;
static ZFArrayEditable *_ZFP_ZFThread_taskDatas = zfnull;

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadTaskRequestDataHolder, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFThread_wrappedTaskCallback = zfnew(ZFListener);
    *_ZFP_ZFThread_wrappedTaskCallback = ZFCallbackForFunc(_ZFP_ZFThreadTaskRequestCallback_action);
    _ZFP_ZFThread_mergeCallbackIgnoreOldTask = zfnew(ZFListener);
    *_ZFP_ZFThread_mergeCallbackIgnoreOldTask = ZFCallbackForFunc(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask_action);
    _ZFP_ZFThread_mergeCallbackIgnoreNewTask = zfnew(ZFListener);
    *_ZFP_ZFThread_mergeCallbackIgnoreNewTask = ZFCallbackForFunc(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask_action);
    _ZFP_ZFThread_mergeCallbackDoNotMerge = zfnew(ZFListener);
    *_ZFP_ZFThread_mergeCallbackDoNotMerge = ZFCallbackForFunc(_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge_action);
    _ZFP_ZFThread_taskDatas = zfAlloc(ZFArrayEditable);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadTaskRequestDataHolder)
{
    zfdelete(_ZFP_ZFThread_wrappedTaskCallback);
    _ZFP_ZFThread_wrappedTaskCallback = zfnull;
    zfdelete(_ZFP_ZFThread_mergeCallbackIgnoreOldTask);
    _ZFP_ZFThread_mergeCallbackIgnoreOldTask = zfnull;
    zfdelete(_ZFP_ZFThread_mergeCallbackIgnoreNewTask);
    _ZFP_ZFThread_mergeCallbackIgnoreNewTask = zfnull;
    zfdelete(_ZFP_ZFThread_mergeCallbackDoNotMerge);
    _ZFP_ZFThread_mergeCallbackDoNotMerge = zfnull;
    _ZFP_ZFThread_taskRunning = zffalse;
    zfRelease(_ZFP_ZFThread_taskDatas);
    _ZFP_ZFThread_taskDatas = zfnull;
}
public:
    ZFIdentityGenerator taskIdHolder;
ZF_GLOBAL_INITIALIZER_END(ZFThreadTaskRequestDataHolder)
#define _ZFP_ZFThreadTaskRequestTaskIdHolder (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadTaskRequestDataHolder)->taskIdHolder)

static void _ZFP_ZFThreadTaskRequest_taskStop(ZF_IN ZFThreadTaskRequestData *taskData)
{
    _ZFP_ZFThreadTaskRequestTaskIdHolder.idRelease(taskData->taskId());
}

static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestCallback_action)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }
    if(_ZFP_ZFThread_taskDatas != zfnull && !_ZFP_ZFThread_taskDatas->isEmpty())
    {
        // take and remove a task
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->getFirst<ZFThreadTaskRequestData *>();
        zfRetain(taskData);
        _ZFP_ZFThread_taskDatas->removeFirst();

        // run
        if(lockAvailable)
        {
            zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
        }
        taskData->taskCallback().execute(
            ZFListenerData().param0Set(taskData->taskParam0()).param1Set(taskData->taskParam1()),
            taskData->taskUserData());
        if(lockAvailable)
        {
            zfsynchronizeLock(_ZFP_ZFThread_mutex);
        }

        // cleanup
        _ZFP_ZFThreadTaskRequest_taskStop(taskData);
        zfRelease(taskData);

        // schedule next task or quit
        if(!_ZFP_ZFThread_taskDatas->isEmpty())
        {
            ZFPROTOCOL_ACCESS(ZFThreadTaskRequest)->taskRequest(*_ZFP_ZFThread_wrappedTaskCallback, zfnull, zfnull);
        }
        else
        {
            _ZFP_ZFThread_taskRunning = zffalse;
        }
        if(lockAvailable)
        {
            zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
        }
    }
    else
    {
        _ZFP_ZFThread_taskRunning = zffalse;
        if(lockAvailable)
        {
            zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
        }
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask_action)
{
    ZFThreadTaskRequestMergeCallbackData *mergeCallbackData = listenerData.param0->to<ZFThreadTaskRequestMergeCallbackData *>();
    mergeCallbackData->taskRequestDataMerged = zfRetain(mergeCallbackData->taskRequestDataNew);
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask_action)
{
    ZFThreadTaskRequestMergeCallbackData *mergeCallbackData = listenerData.param0->to<ZFThreadTaskRequestMergeCallbackData *>();
    mergeCallbackData->taskRequestDataMerged = zfRetain(mergeCallbackData->taskRequestDataOld);
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge_action)
{
    // leave empty merged data, nothing to do
}

const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreOldTask(void)
{
    return *_ZFP_ZFThread_mergeCallbackIgnoreOldTask;
}
const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackIgnoreNewTask(void)
{
    return *_ZFP_ZFThread_mergeCallbackIgnoreNewTask;
}
const ZFListener &_ZFP_ZFThreadTaskRequestMergeCallbackDoNotMerge(void)
{
    return *_ZFP_ZFThread_mergeCallbackDoNotMerge;
}
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFListener, ZFThreadTaskRequestMergeCallbackIgnoreOldTask, ZFThreadTaskRequestMergeCallbackIgnoreOldTask())
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFListener, ZFThreadTaskRequestMergeCallbackIgnoreNewTask, ZFThreadTaskRequestMergeCallbackIgnoreNewTask())
ZFEXPORT_VAR_READONLY_VALUEREF_DEFINE(ZFListener, ZFThreadTaskRequestMergeCallbackDoNotMerge, ZFThreadTaskRequestMergeCallbackDoNotMerge())
ZFEXPORT_VAR_READONLY_ALIAS_DEFINE(ZFListener, ZFThreadTaskRequestMergeCallbackDefault, ZFThreadTaskRequestMergeCallbackIgnoreOldTask)

ZFMETHOD_FUNC_DEFINE_6(zfidentity, ZFThreadTaskRequest,
                       ZFMP_IN(const ZFListener &, taskCallback),
                       ZFMP_IN_OPT(ZFObject *, taskUserData, zfnull),
                       ZFMP_IN_OPT(ZFObject *, taskParam0, zfnull),
                       ZFMP_IN_OPT(ZFObject *, taskParam1, zfnull),
                       ZFMP_IN_OPT(ZFObject *, taskOwner, zfnull),
                       ZFMP_IN_OPT(const ZFListener &, taskMergeCallback, ZFThreadTaskRequestMergeCallbackDefault()))
{
    zfblockedAlloc(ZFThreadTaskRequestData, taskRequestData);
    taskRequestData->taskCallbackSet(taskCallback);
    taskRequestData->taskUserDataSet(taskUserData);
    taskRequestData->taskParam0Set(taskParam0);
    taskRequestData->taskParam1Set(taskParam1);
    taskRequestData->taskOwnerSet(taskOwner);
    return ZFThreadTaskRequest(taskRequestData, taskMergeCallback);
}
ZFMETHOD_FUNC_DEFINE_2(zfidentity, ZFThreadTaskRequest,
                       ZFMP_IN(ZFThreadTaskRequestData *, taskRequestData),
                       ZFMP_IN_OPT(const ZFListener &, mergeCallback, ZFThreadTaskRequestMergeCallbackDefault()))
{
    if(taskRequestData == zfnull || !taskRequestData->taskCallback().callbackIsValid())
    {
        return zfidentityInvalid();
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }

    zfindex oldTaskIndex = zfindexMax();
    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *existing = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(existing->taskCallback().objectCompare(taskRequestData->taskCallback()) == ZFCompareTheSame
            && existing->taskOwner() == taskRequestData->taskOwner())
        {
            oldTaskIndex = i;
            break;
        }
    }
    zfidentity taskId = zfidentityInvalid();
    if(oldTaskIndex != zfindexMax() && mergeCallback != ZFThreadTaskRequestMergeCallbackDoNotMerge())
    {
        zfblockedAlloc(ZFThreadTaskRequestMergeCallbackData, mergeCallbackData);
        mergeCallbackData->taskRequestDataOld = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(oldTaskIndex);
        mergeCallbackData->taskRequestDataNew = taskRequestData;
        mergeCallback.execute(ZFListenerData().param0Set(mergeCallbackData));
        if(mergeCallbackData->taskRequestDataMerged != zfnull)
        {
            taskRequestData = mergeCallbackData->taskRequestDataMerged;
            taskId = mergeCallbackData->taskRequestDataMerged->taskId();

            _ZFP_ZFThread_taskDatas->remove(oldTaskIndex);
            _ZFP_ZFThread_taskDatas->add(mergeCallbackData->taskRequestDataMerged);
            zfRelease(mergeCallbackData->taskRequestDataMerged);
            mergeCallbackData->taskRequestDataMerged = zfnull;
        }
        else
        {
            taskId = _ZFP_ZFThreadTaskRequestTaskIdHolder.idAcquire();
            _ZFP_ZFThread_taskDatas->add(taskRequestData);
        }
    }
    else
    {
        taskId = _ZFP_ZFThreadTaskRequestTaskIdHolder.idAcquire();
        _ZFP_ZFThread_taskDatas->add(taskRequestData);
    }
    ZFPropertyAccess(ZFThreadTaskRequestData, taskId)->setterMethod()->execute<void, zfidentity const &>(taskRequestData, taskId);

    if(!_ZFP_ZFThread_taskRunning)
    {
        _ZFP_ZFThread_taskRunning = zftrue;
        ZFPROTOCOL_ACCESS(ZFThreadTaskRequest)->taskRequest(*_ZFP_ZFThread_wrappedTaskCallback, zfnull, zfnull);
    }
    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }
    return taskId;
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFThreadTaskCancel,
                       ZFMP_IN(zfidentity, taskId))
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(taskData->taskId() == taskId)
        {
            _ZFP_ZFThreadTaskRequest_taskStop(taskData);
            _ZFP_ZFThread_taskDatas->remove(i);
            break;
        }
    }
    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }
}
ZFMETHOD_FUNC_DEFINE_4(void, ZFThreadTaskCancelExactly,
                       ZFMP_IN(const ZFListener &, task),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                       ZFMP_IN_OPT(ZFObject *, param0, zfnull),
                       ZFMP_IN_OPT(ZFObject *, param1, zfnull))
{
    if(!task.callbackIsValid())
    {
        return ;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(taskData->taskCallback().objectCompare(task) == ZFCompareTheSame
            && ZFObjectCompare(taskData->taskUserData(), userData) == ZFCompareTheSame
            && ZFObjectCompare(taskData->taskParam0(), param0) == ZFCompareTheSame
            && ZFObjectCompare(taskData->taskParam1(), param1) == ZFCompareTheSame)
        {
            _ZFP_ZFThreadTaskRequest_taskStop(taskData);
            _ZFP_ZFThread_taskDatas->remove(i);
            break;
        }
    }
    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFThreadTaskCancel,
                       ZFMP_IN(const ZFListener &, task))
{
    if(!task.callbackIsValid())
    {
        return ;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(taskData->taskCallback().objectCompare(task) == ZFCompareTheSame)
        {
            _ZFP_ZFThreadTaskRequest_taskStop(taskData);
            _ZFP_ZFThread_taskDatas->remove(i);
            --i;
        }
    }
    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFThreadTaskCancelWithOwner,
                       ZFMP_IN(ZFObject *, owner))
{
    if(owner == zfnull)
    {
        return ;
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < _ZFP_ZFThread_taskDatas->count(); ++i)
    {
        ZFThreadTaskRequestData *taskData = _ZFP_ZFThread_taskDatas->get<ZFThreadTaskRequestData *>(i);
        if(taskData->taskOwner() == owner)
        {
            _ZFP_ZFThreadTaskRequest_taskStop(taskData);
            _ZFP_ZFThread_taskDatas->remove(i);
            --i;
        }
    }
    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }
}

ZF_NAMESPACE_GLOBAL_END

