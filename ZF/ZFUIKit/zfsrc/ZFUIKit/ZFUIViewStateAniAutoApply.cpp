#include "ZFUIViewStateAniAutoApply.h"
#include "ZFUIViewBlink.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ViewStateAniAutoApplyStart)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ViewStateAniAutoApplyStop)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ViewStateAniAutoApplyPause)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ViewStateAniAutoApplyResume)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
ZFEXPORT_VAR_DEFINE(zfbool, ZFUIViewStateAniAutoApply, zffalse)
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniAutoApply_settingInit, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniAutoApply_settingInit)
{
    ZFUIViewStateAniAutoApply(zffalse);
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniAutoApply_settingInit)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniAutoApply_autoStart, ZFLevelAppLow)
{
    if(ZFUIViewStateAniAutoApply())
    {
        ZFUIViewStateAniAutoApplyStart();
    }
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniAutoApply_autoStart)
{
    ZFUIViewStateAniAutoApplyStop();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniAutoApply_autoStart)

// ============================================================
static zfbool _ZFP_ZFUIViewStateAniAutoApply_started = zffalse;
static zfint _ZFP_ZFUIViewStateAniAutoApply_paused = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniAutoApplyDataHolder, ZFLevelZFFrameworkEssential)
{
    this->taskStartListener = ZFCallbackForFunc(zfself::taskStart);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniAutoApplyDataHolder)
{
}
public:
    ZFListener taskStartListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(taskStart)
    {
        if(_ZFP_ZFUIViewStateAniAutoApply_paused == 0)
        {
            ZFUIViewStateAniStart();
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniAutoApplyDataHolder)

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(void, ZFUIViewStateAniAutoApplyStart)
{
    ZFUIViewStateAniAutoApplyStop();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniAutoApplyDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniAutoApplyDataHolder);

    _ZFP_ZFUIViewStateAniAutoApply_started = zftrue;

    ZFObjectGlobalEventObserver().observerAdd(ZFUIView::EventViewLayoutOnLayoutRequest(), d->taskStartListener);
    ZFObjectGlobalEventObserver().observerAdd(ZFUIView::EventViewOnAddToParent(), d->taskStartListener);
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewStateAniAutoApplyStart());
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFUIViewStateAniAutoApplyStop)
{
    ZFUIViewStateAniAutoApplyPauseForTimeCancel();
    if(!_ZFP_ZFUIViewStateAniAutoApply_started)
    {
        return ;
    }
    _ZFP_ZFUIViewStateAniAutoApply_started = zffalse;

    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniAutoApplyDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniAutoApplyDataHolder);
    ZFObjectGlobalEventObserver().observerRemove(ZFUIView::EventViewLayoutOnLayoutRequest(), d->taskStartListener);
    ZFObjectGlobalEventObserver().observerRemove(ZFUIView::EventViewOnAddToParent(), d->taskStartListener);
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewStateAniAutoApplyStop());
}
ZFMETHOD_FUNC_DEFINE_0(zfbool, ZFUIViewStateAniAutoApplyStarted)
{
    return _ZFP_ZFUIViewStateAniAutoApply_started;
}

ZFMETHOD_FUNC_DEFINE_0(void, ZFUIViewStateAniAutoApplyPause)
{
    ++_ZFP_ZFUIViewStateAniAutoApply_paused;
    if(_ZFP_ZFUIViewStateAniAutoApply_paused == 1)
    {
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewStateAniAutoApplyPause());
    }
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFUIViewStateAniAutoApplyResume)
{
    zfCoreAssert(_ZFP_ZFUIViewStateAniAutoApply_paused > 0);
    --_ZFP_ZFUIViewStateAniAutoApply_paused;
    if(_ZFP_ZFUIViewStateAniAutoApply_paused == 0)
    {
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventViewStateAniAutoApplyResume());
    }
}
ZFMETHOD_FUNC_DEFINE_0(zfindex, ZFUIViewStateAniAutoApplyPaused)
{
    return (!_ZFP_ZFUIViewStateAniAutoApply_started || _ZFP_ZFUIViewStateAniAutoApply_paused);
}

// ============================================================
// ZFUIViewStateAniAutoApplyPauseForTime
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder, ZFLevelZFFrameworkEssential)
{
    this->started = zffalse;
    this->delayTimer = zfnull;
    this->doStopListener = ZFCallbackForFunc(zfself::doStop);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder)
{
    zfRelease(this->delayTimer);
}
public:
    zfbool started;
    ZFTimer *delayTimer;
    zftimet endTime;
    ZFListener doStopListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(doStop)
    {
        ZFUIViewStateAniAutoApplyPauseForTimeCancel();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder)

ZFMETHOD_FUNC_DEFINE_1(void, ZFUIViewStateAniAutoApplyPauseForTime,
                       ZFMP_IN_OPT(zftimet, time, zftimetZero()))
{
    if(ZFUIViewStateAniAutoApplyPaused())
    {
        return ;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder);

    time = zfmMax(time, (zftimet)100);
    zftimet endTime = ZFTime::timestamp() + time;
    if(d->started && endTime <= d->endTime + 100)
    {
        return ;
    }

    if(d->delayTimer == zfnull)
    {
        d->delayTimer = zfAlloc(ZFTimer);
        d->delayTimer->observerAdd(ZFTimer::EventTimerOnActivate(), d->doStopListener);
        d->delayTimer->timerActivateInMainThread(zftrue);
    }
    d->delayTimer->timerStop();

    d->endTime = endTime;
    d->delayTimer->timerInterval(time);
    ZFUIViewStateAniAutoApplyPause();
    d->started = zftrue;
    d->delayTimer->timerStart();
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFUIViewStateAniAutoApplyPauseForTimeCancel)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewStateAniAutoApplyPauseForTimeDataHolder);
    if(!d->started)
    {
        return ;
    }
    d->started = zffalse;
    d->delayTimer->timerStop();
    ZFUIViewStateAniAutoApplyResume();
}

// ZFUIViewStateAniAutoApplyPauseForTime auto cleanup
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewStateAniAutoApplyPauseForTimeAutoCleanup, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewStateAniAutoApplyPauseForTimeAutoCleanup)
{
    ZFUIViewStateAniAutoApplyPauseForTimeCancel();
}
ZF_GLOBAL_INITIALIZER_END(ZFUIViewStateAniAutoApplyPauseForTimeAutoCleanup)

ZF_NAMESPACE_GLOBAL_END

