/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIPage.h"
#include "ZFUIPageManager.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIPageResumeReason)
ZFENUM_DEFINE(ZFUIPagePauseReason)

ZFOBJECT_REGISTER(ZFUIPage)

ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageOnCreate)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageOnResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageOnAttach)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageOnDetach)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageOnPause)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageOnDestroy)

ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAfterCreate)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageBeforeResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAfterResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageBeforePause)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAfterPause)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageBeforeDestroy)

ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAniOnUpdate)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAniOnUpdateAniTarget)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAniOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFUIPage, PageAniOnStop)

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFUIPage, ZFUIPageManager *, pageManager)

ZFMETHOD_DEFINE_0(ZFUIPage, void, pageDestroy)
{
    this->pageManager()->requestPageDestroy(this);
}

// ============================================================
// pageDelayDetach
void ZFUIPage::pageDelayDetachOnCheck(ZF_IN ZFUIPagePauseReasonEnum reason)
{
    // nothing to do
}
void ZFUIPage::pageDelayDetachNotifyFinish(void)
{
    --(this->_ZFP_ZFUIPage_pageDelayDetachFlag);
    if(this->_ZFP_ZFUIPage_pageDelayDetachFlag == 0)
    {
        this->pageManager()->_ZFP_ZFUIPageManager_pageDelayDetachNotifyFinish(this);
    }
}

// ============================================================
// pageDelayDestroy
void ZFUIPage::pageDelayDestroyOnCheck(void)
{
    if(this->pageAni() != zfnull && this->pageAni()->aniRunning())
    {
        ZFLISTENER_LOCAL(pageAniOnFinish, {
                ZFUIPage *page = userData->objectHolded();
                page->_ZFP_pageDelayDestroyNotifyFinish();
            })
        this->pageAni()->observerAdd(ZFObserverAddParam()
                .eventIdSet(ZFAnimation::EventAniOnStop())
                .observerSet(pageAniOnFinish)
                .userDataSet(this->toObject()->objectHolder())
                .autoRemoveAfterActivateSet(zftrue)
            );
        this->pageDelayDestroyEnable();
    }
}
void ZFUIPage::pageDelayDestroyNotifyFinish(void)
{
    --(this->_ZFP_ZFUIPage_pageDelayDestroyFlag);
    if(this->_ZFP_ZFUIPage_pageDelayDestroyFlag == 0)
    {
        this->pageManager()->_ZFP_ZFUIPageManager_pageDelayDestroyNotifyFinish(this);
    }
}

// ============================================================
// page ani
ZFMETHOD_DEFINE_1(ZFUIPage, void, pageAniSet,
                  ZFMP_IN(ZFAnimation *, pageAni))
{
    zfCoreAssertWithMessage(this->_ZFP_ZFUIPage_pageAniCanChange,
        "you can only change pageAni during"
        " Page::pageAniOnUpdate"
        "or Manager::pageAniOnUpdate");
    zfRetainChange(_ZFP_ZFUIPage_pageAni, pageAni);
}

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIPage, zfbool, pageAniPriorityNeedHigher)

void ZFUIPage::pageAniOnUpdate(ZF_IN ZFUIPage *resumePageOrNull,
                               ZF_IN ZFUIPage *pausePageOrNull,
                               ZF_IN ZFUIPageResumeReasonEnum reason)
{
    switch(reason)
    {
        case ZFUIPageResumeReason::e_ByManagerResume:
            break;
        case ZFUIPageResumeReason::e_ByRequest:
            if(resumePageOrNull == this)
            {
                if(pausePageOrNull != zfnull)
                {
                    pausePageOrNull->pageAniSet(pausePageOrNull->pageAniPauseToBackground());
                }
                this->pageAniSet(this->pageAniResumeByRequest());
            }
            break;
        case ZFUIPageResumeReason::e_FromBackground:
            if(pausePageOrNull == this)
            {
                this->pageAniPriorityNeedHigher = zftrue;
                this->pageAniSet(this->pageAniPauseBeforeDestroy());
                if(resumePageOrNull != zfnull)
                {
                    resumePageOrNull->pageAniSet(resumePageOrNull->pageAniResumeFromBackground());
                }
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
}

// ============================================================
// private
void ZFUIPage::_ZFP_ZFUIPage_pageCreate(void)
{
    zfCoreAssert(!this->_ZFP_ZFUIPage_pageCreated);
    this->_ZFP_ZFUIPage_pageCreated = zftrue;

    this->_ZFP_ZFUIPage_pageOnCreateCalled = zffalse;
    this->pageOnCreate();
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnCreateCalled, "[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnCreate");
}
void ZFUIPage::_ZFP_ZFUIPage_pageResume(ZF_IN ZFUIPageResumeReasonEnum reason)
{
    zfCoreAssert(!this->_ZFP_ZFUIPage_pageResumed);
    this->_ZFP_ZFUIPage_pageResumed = zftrue;

    this->_ZFP_ZFUIPage_pageOnResumeCalled = zffalse;
    this->pageOnResume(reason);
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnResumeCalled, "[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnResume");
}
void ZFUIPage::_ZFP_ZFUIPage_pageAttach(ZF_IN ZFUIPageResumeReasonEnum reason)
{
    zfCoreAssert(!this->_ZFP_ZFUIPage_pageAttached);
    this->_ZFP_ZFUIPage_pageAttached = zftrue;

    this->_ZFP_ZFUIPage_pageOnAttachCalled = zffalse;
    this->pageOnAttach(reason);
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnAttachCalled, "[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnAttach");
}
void ZFUIPage::_ZFP_ZFUIPage_pageDetach(ZF_IN ZFUIPagePauseReasonEnum reason)
{
    zfCoreAssert(this->_ZFP_ZFUIPage_pageAttached);
    this->_ZFP_ZFUIPage_pageAttached = zffalse;

    this->_ZFP_ZFUIPage_pageOnDetachCalled = zffalse;
    this->pageOnDetach(reason);
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnDetachCalled, "[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnDetach");
}
void ZFUIPage::_ZFP_ZFUIPage_pagePause(ZF_IN ZFUIPagePauseReasonEnum reason)
{
    zfCoreAssert(this->_ZFP_ZFUIPage_pageResumed);
    this->_ZFP_ZFUIPage_pageResumed = zffalse;

    this->_ZFP_ZFUIPage_pageOnPauseCalled = zffalse;
    this->pageOnPause(reason);
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnPauseCalled, "[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnPause");
}
void ZFUIPage::_ZFP_ZFUIPage_pageDestroy(void)
{
    zfCoreAssert(this->_ZFP_ZFUIPage_pageCreated);
    this->_ZFP_ZFUIPage_pageCreated = zffalse;

    this->_ZFP_ZFUIPage_pageOnDestroyCalled = zffalse;
    this->pageOnDestroy();
    zfCoreAssertWithMessageTrim(this->_ZFP_ZFUIPage_pageOnDestroyCalled, "[ZFUIPage] you must call zfsuperI(ZFUIPage)::pageOnDestroy");

    zfRetainChange(this->_ZFP_ZFUIPage_pageCreateParam, zfnull);
}

ZF_NAMESPACE_GLOBAL_END

