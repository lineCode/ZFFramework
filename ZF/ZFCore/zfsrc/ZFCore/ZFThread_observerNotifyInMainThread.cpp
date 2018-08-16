/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFThread_observerNotifyInMainThread.h"

#include "ZFCore/ZFSTLWrapper/zfstl_list.h"
#include "ZFCore/ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObserverNotifyInMainThreadCacheHolder, ZFLevelZFFrameworkHigh)
{
}
zfstllist<zfautoObject> l;
ZF_GLOBAL_INITIALIZER_END(ZFObserverNotifyInMainThreadCacheHolder)

zfclass _ZFP_I_ZFObserverNotifyInMainThreadTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFObserverNotifyInMainThreadTaskData, ZFObject)

public:
    static zfautoObject cacheGet(void)
    {
        zfCoreMutexLocker();
        ZF_GLOBAL_INITIALIZER_CLASS(ZFObserverNotifyInMainThreadCacheHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObserverNotifyInMainThreadCacheHolder);
        if(d->l.empty())
        {
            return zflineAlloc(zfself);
        }
        else
        {
            zfautoObject ret = *(d->l.begin());
            d->l.pop_front();
            return ret;
        }
    }
    static void cacheAdd(ZF_IN zfself *taskData)
    {
        zfCoreMutexLocker();
        taskData->removeAll();
        ZF_GLOBAL_INITIALIZER_CLASS(ZFObserverNotifyInMainThreadCacheHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFObserverNotifyInMainThreadCacheHolder);
        if(d->l.size() < 10)
        {
            d->l.push_back(taskData);
        }
    }

public:
    _ZFP_I_ZFObserverNotifyInMainThreadTaskData(void)
    : obj(zfnull)
    , customSender(zfnull)
    , param0(zfnull)
    , param1(zfnull)
    {
    }

public: // use direct pointer for performance
    ZFObject *obj;
    inline void objSet(ZF_IN ZFObject *obj) {zfRetain(obj); zfRelease(this->obj); this->obj = obj;}

    ZFObject *customSender;
    inline void customSenderSet(ZF_IN ZFObject *customSender) {zfRetain(customSender); zfRelease(this->customSender); this->customSender = customSender;}

    zfidentity eventId;

    ZFObject *param0;
    inline void param0Set(ZF_IN ZFObject *param0) {zfRetain(param0); zfRelease(this->param0); this->param0 = param0;}

    ZFObject *param1;
    inline void param1Set(ZF_IN ZFObject *param1) {zfRetain(param1); zfRelease(this->param1); this->param1 = param1;}

private:
    inline void removeAll(void)
    {
        this->objSet(zfnull);
        this->customSenderSet(zfnull);
        this->param0Set(zfnull);
        this->param1Set(zfnull);
    }

public:
    zfoverride
    virtual void objectOnDealloc(void)
    {
        this->removeAll();
        zfsuper::objectOnDealloc();
    }
};
ZFOBJECT_REGISTER(_ZFP_I_ZFObserverNotifyInMainThreadTaskData)

// ============================================================
static ZFListener *_ZFP_ZFObserverNotifyInMainThreadCallback = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObserverNotifyInMainThreadDataHolder, ZFLevelZFFrameworkEssential)
{
    this->callback = ZFCallbackForFunc(zfself::callbackAction);
    _ZFP_ZFObserverNotifyInMainThreadCallback = &this->callback;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFObserverNotifyInMainThreadDataHolder)
{
    _ZFP_ZFObserverNotifyInMainThreadCallback = zfnull;
}
private:
    ZFListener callback;
    static ZFLISTENER_PROTOTYPE_EXPAND(callbackAction)
    {
        _ZFP_I_ZFObserverNotifyInMainThreadTaskData *taskData = userData->to<_ZFP_I_ZFObserverNotifyInMainThreadTaskData *>();
        taskData->obj->observerNotifyWithCustomSender(
            taskData->customSender,
            taskData->eventId,
            taskData->param0,
            taskData->param1);
        _ZFP_I_ZFObserverNotifyInMainThreadTaskData::cacheAdd(taskData);
    }
ZF_GLOBAL_INITIALIZER_END(ZFObserverNotifyInMainThreadDataHolder)

ZFMETHOD_FUNC_DEFINE_5(zfidentity, ZFObserverNotifyInMainThreadWithCustomSender,
                       ZFMP_IN(ZFObject *, customSender),
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_IN(zfidentity, eventId),
                       ZFMP_IN_OPT(ZFObject *, param0, zfnull),
                       ZFMP_IN_OPT(ZFObject *, param1, zfnull))
{
    if(obj == zfnull)
    {
        return zfidentityInvalid();
    }
    if(_ZFP_ZFObserverNotifyInMainThreadCallback)
    {
        zfautoObject taskDataHolder = _ZFP_I_ZFObserverNotifyInMainThreadTaskData::cacheGet();
        _ZFP_I_ZFObserverNotifyInMainThreadTaskData *taskData = taskDataHolder;
        taskData->objSet(obj);
        taskData->customSenderSet(customSender);
        taskData->eventId = eventId;
        taskData->param0Set(param0);
        taskData->param1Set(param1);
        return ZFThreadTaskRequest(*_ZFP_ZFObserverNotifyInMainThreadCallback,
            taskData,
            zfnull,
            zfnull,
            zfnull,
            ZFThreadTaskRequestMergeCallbackDoNotMerge());
    }
    else
    {
        obj->observerNotifyWithCustomSender(customSender, eventId, param0, param1);
        return zfidentityInvalid();
    }
}
ZFMETHOD_FUNC_DEFINE_INLINE_4(zfidentity, ZFObserverNotifyInMainThread,
                              ZFMP_IN(ZFObject *, obj),
                              ZFMP_IN(zfidentity, eventId),
                              ZFMP_IN_OPT(ZFObject *, param0, zfnull),
                              ZFMP_IN_OPT(ZFObject *, param1, zfnull))
ZFMETHOD_FUNC_DEFINE_1(void, ZFObserverNotifyInMainThreadCancel,
                       ZFMP_IN(zfidentity, taskId))
{
    ZFThreadTaskCancel(taskId);
}

ZF_NAMESPACE_GLOBAL_END

