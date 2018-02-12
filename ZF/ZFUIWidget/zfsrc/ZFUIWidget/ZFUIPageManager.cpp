/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIPageManager.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassPOD _ZFP_ZFUIPageManagerPageDelayDetachData
{
public:
    ZFUIPage *page;
    ZFUIPagePauseReasonEnum reason;
};
zfclassNotPOD _ZFP_ZFUIPageManagerPrivate
{
public:
    ZFUIPageManager *pimplOwner;
    ZFCoreArrayPOD<ZFUIPage *> pageList;
    zfbool managerResumed;
    zfindex managerUIBlocked;
    zfindex requestBlocked;
    ZFCoreQueuePOD<ZFUIPageRequest *> requestQueue;
    zfbool requestRunning;
    ZFListener requestOnResolveListener;
    ZFCoreArrayPOD<_ZFP_ZFUIPageManagerPageDelayDetachData> pageDelayDetachList;
    ZFCoreArrayPOD<ZFUIPage *> pageDelayDestroyList;
    zfbool movePageFlag;

    zfbool pageAniOverrideForOnce;
    ZFAnimation *pageAniOverrideForOnceResumeAni;
    ZFAnimation *pageAniOverrideForOncePauseAni;
    zfbool pageAniOverrideForOncePauseAniHasHigherPriority;

    ZFUIPage *pageLastResume;

    ZFAnimation *pageAniLastResume;
    ZFAnimation *pageAniLastPause;

    ZFListener pageAniOnStopOrOnInvalidListener;
    ZFListener pageAniOnStartListener;
    ZFListener pageAniOnStopListener;

public:
    void requestDoPost(ZF_IN ZFObject *owner)
    {
        zfsynchronizedObject(owner);
        if(this->requestBlocked > 0 || this->requestQueue.isEmpty() || this->requestRunning)
        {
            return ;
        }
        this->requestRunning = zftrue;
        ZFThreadTaskRequest(
            this->requestOnResolveListener,
            zfHint("userData")owner,
            zfHint("param0")zfnull,
            zfHint("param1")zfnull,
            zfHint("owner")owner,
            ZFThreadTaskRequestMergeCallbackIgnoreNewTask());
    }
private:
    static ZFLISTENER_PROTOTYPE_EXPAND(requestOnResolve)
    {
        ZFUIPageManager *manager = userData->to<ZFUIPageManager *>();
        zfsynchronizedObject(manager->toObject());
        if(manager->d->requestBlocked > 0 || manager->d->requestQueue.isEmpty())
        {
            manager->d->requestRunning = zffalse;
            return ;
        }

        manager->requestBlockedSet(zftrue);
        ZFUIPageRequest *request = manager->d->requestQueue.queueTake();

        if(!request->requestResolved())
        {
            manager->requestOnResolvePrepare(request);
        }
        if(!request->requestResolved())
        {
            manager->requestOnResolve(request);
        }
        manager->managerUIBlockedSet(zffalse);
        zfCoreAssertWithMessageTrim(request->requestResolved(),
            zfTextA("[ZFUIPageManager] unresolved request: %s"),
            zfsCoreZ2A(request->objectInfo().cString()));
        manager->requestOnResolveFinish(request);
        zfRelease(request);
        manager->requestBlockedSet(zffalse);
        manager->d->requestRunning = zffalse;

        if(manager->d->requestQueue.isEmpty())
        {
            ZFUIPage *foregroundPage = manager->pageForeground();
            if(foregroundPage != zfnull && !foregroundPage->pageResumed())
            {
                manager->d->pageResume(foregroundPage, ZFUIPageResumeReason::e_ByManagerResume);
            }
            if(manager->d->requestQueue.isEmpty())
            {
                manager->requestOnResolveFinishAll();
            }
        }
        else
        {
            manager->d->requestDoPost(manager->toObject());
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(pageAniOnStopOrOnInvalid)
    {
        ZFUIPage *page = userData->tagGet<ZFUIPage *>(zfText("page"));
        ZFEnum *reason = userData->tagGet<ZFEnum *>(zfText("reason"));
        ZFUIPageManager *pageManager = page->pageManager();

        zfRetain(userData);
        zfblockedRelease(userData);
        page->pageManager()->d->pageAniListenerDetach(page);

        if(reason->classData()->classIsTypeOf(ZFUIPageResumeReason::ClassData()))
        {
            pageManager->d->pageAniLastResume = zfnull;
        }
        else if(reason->classData()->classIsTypeOf(ZFUIPagePauseReason::ClassData()))
        {
            pageManager->d->pageAniLastPause = zfnull;
        }

        page->_ZFP_ZFUIPage_pageAniCanChange = zftrue;
        page->pageAniSet(zfnull);
        page->_ZFP_ZFUIPage_pageAniCanChange = zffalse;

        if(reason->classData()->classIsTypeOf(ZFUIPagePauseReason::ClassData()))
        {
            page->pageManager()->pageDelayDetachNotifyFinish(page);
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(pageAniOnStart)
    {
        ZFUIPage *page = userData->tagGet<ZFUIPage *>(zfText("page"));
        ZFEnum *reason = userData->tagGet<ZFEnum *>(zfText("reason"));

        page->pageManager()->managerUIBlockedSet(zftrue);
        ZFAnimation *pageAni = listenerData.sender->to<ZFAnimation *>();
        page->pageAniOnStart(pageAni, reason);
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(pageAniOnStop)
    {
        ZFUIPage *page = userData->tagGet<ZFUIPage *>(zfText("page"));
        ZFEnum *reason = userData->tagGet<ZFEnum *>(zfText("reason"));

        ZFAnimation *pageAni = listenerData.sender->to<ZFAnimation *>();
        page->pageAniOnStop(pageAni, reason);

        page->pageManager()->managerUIBlockedSet(zffalse);
    }

public:
    void pageCreate(ZF_IN ZFUIPage *page, ZF_IN const ZFUIPageRequestPageCreateParam &createParam)
    {
        page->pageCreateParamSet(createParam.pageCreateParam());
        page->pageGroupIdSet(createParam.pageGroupId());

        page->_ZFP_ZFUIPage_pageCreate();
        this->pimplOwner->pageOnCreate(page, createParam);
        page->pageAfterCreate();
        this->pimplOwner->pageAfterCreate(page);
    }
    void pageResume(ZF_IN ZFUIPage *page, ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->pimplOwner->pageBeforeResume(page, reason);
        page->pageBeforeResume(reason);
        this->pageAttach(page, reason);
        page->_ZFP_ZFUIPage_pageResume(reason);
        this->pimplOwner->pageOnResume(page, reason);
        page->pageAfterResume(reason);
        this->pimplOwner->pageAfterResume(page, reason);

        this->pageAniUpdate(page, (this->pageLastResume == page) ? zfnull : this->pageLastResume, reason);
        this->pageLastResume = page;
    }
    void pageAttach(ZF_IN ZFUIPage *page, ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        if(!page->pageAttached())
        {
            this->pimplOwner->pageOnAttach(page, reason);
            page->_ZFP_ZFUIPage_pageAttach(reason);
        }
    }
    void pageDoDetach(ZF_IN ZFUIPage *page, ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        if(page->pageAttached() && this->pimplOwner->pageOnDetachCheckRequire(page, reason))
        {
            page->_ZFP_ZFUIPage_pageDetach(reason);
            this->pimplOwner->pageOnDetach(page, reason);
        }
    }
    void pageDetach(ZF_IN ZFUIPage *page, ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        page->pageDelayDetachOnCheck(reason);
        this->pimplOwner->pageDelayDetachOnCheck(page, reason);
        if(page->_ZFP_ZFUIPage_pageDelayDetachFlag > 0 || page->_ZFP_ZFUIPage_pageDelayDetachByManagerFlag > 0)
        {
            _ZFP_ZFUIPageManagerPageDelayDetachData data;
            data.page = page;
            data.reason = reason;
            this->pageDelayDetachList.add(data);
        }
        else
        {
            this->pageDoDetach(page, reason);
        }
    }
    void pagePause(ZF_IN ZFUIPage *page, ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->pimplOwner->pageBeforePause(page, reason);
        page->pageBeforePause(reason);
        this->pimplOwner->pageOnPause(page, reason);
        page->_ZFP_ZFUIPage_pagePause(reason);
        this->pageDetach(page, reason);
        page->pageAfterPause(reason);
        this->pimplOwner->pageAfterPause(page, reason);
    }
    void pageDestroy(ZF_IN ZFUIPage *page)
    {
        zfCoreAssert(page->_ZFP_ZFUIPage_pageDelayDetachFlag == 0 && page->_ZFP_ZFUIPage_pageDelayDetachByManagerFlag == 0);
        if(page->pageAttached())
        {
            this->pimplOwner->pageOnDetach(page, ZFUIPagePauseReason::e_BeforeDestroy);
            page->_ZFP_ZFUIPage_pageDetach(ZFUIPagePauseReason::e_BeforeDestroy);
        }

        this->pimplOwner->pageBeforeDestroy(page);
        page->pageBeforeDestroy();
        this->pimplOwner->pageOnDestroy(page);
        page->_ZFP_ZFUIPage_pageDestroy();
    }

public:
    void pageDelayDetachCheckFinish(ZF_IN ZFUIPage *page)
    {
        if(page->_ZFP_ZFUIPage_pageDelayDetachFlag > 0 || page->_ZFP_ZFUIPage_pageDelayDetachByManagerFlag > 0)
        {
            return ;
        }
        for(zfindex i = 0; i < this->pageDelayDetachList.count(); ++i)
        {
            _ZFP_ZFUIPageManagerPageDelayDetachData data = this->pageDelayDetachList[i];
            if(data.page == page)
            {
                this->pageDelayDetachList.remove(i);
                this->pageDoDetach(data.page, data.reason);
                break;
            }
        }
    }
    void pageDelayDestroyCheckFinish(ZF_IN ZFUIPage *page)
    {
        if(page->_ZFP_ZFUIPage_pageDelayDestroyFlag > 0 || page->_ZFP_ZFUIPage_pageDelayDestroyByManagerFlag > 0)
        {
            return ;
        }
        zfindex index = this->pageDelayDestroyList.find(page);
        if(index == zfindexMax())
        {
            return ;
        }
        this->pageDelayDestroyList.remove(index);
        this->pageDestroy(page);
        zfRelease(page);
    }

public:
    void pageAniUpdate(ZF_IN ZFUIPage *resumePageOrNull,
                       ZF_IN ZFUIPage *pausePageOrNull,
                       ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        // cleanup old animation
        if(this->pageAniLastResume != zfnull)
        {
            this->pageAniLastResume->aniStop();
            zfRetainChange(this->pageAniLastResume, zfnull);
        }
        if(this->pageAniLastPause != zfnull)
        {
            this->pageAniLastPause->aniStop();
            zfRetainChange(this->pageAniLastPause, zfnull);
        }
        if(resumePageOrNull != zfnull && resumePageOrNull->pageAni() != zfnull)
        {
            resumePageOrNull->pageAni()->aniStop();
        }
        if(pausePageOrNull != zfnull && pausePageOrNull->pageAni() != zfnull)
        {
            pausePageOrNull->pageAni()->aniStop();
        }

        // update new animation
        if(resumePageOrNull != zfnull)
        {
            resumePageOrNull->_ZFP_ZFUIPage_pageAniCanChange = zftrue;
            resumePageOrNull->pageAniPriorityNeedHigher = zffalse;
        }
        if(pausePageOrNull != zfnull)
        {
            pausePageOrNull->_ZFP_ZFUIPage_pageAniCanChange = zftrue;
            pausePageOrNull->pageAniPriorityNeedHigher = zffalse;
        }
        this->pimplOwner->pageAniOnUpdate(resumePageOrNull, pausePageOrNull, reason);
        if(pausePageOrNull != zfnull)
        {
            pausePageOrNull->toObject()->observerNotify(ZFUIPage::EventPageAniOnUpdate());
        }
        if(resumePageOrNull != zfnull)
        {
            resumePageOrNull->toObject()->observerNotify(ZFUIPage::EventPageAniOnUpdate());
        }

        // finally start new animation
        zfbool pausePageHasHigherPriority = zffalse;
        if(pausePageOrNull != zfnull && pausePageOrNull->pageAniPriorityNeedHigher)
        {
            pausePageHasHigherPriority = zftrue;
        }
        if(resumePageOrNull != zfnull && resumePageOrNull->pageAniPriorityNeedHigher)
        {
            pausePageHasHigherPriority = zffalse;
        }
        if(pausePageOrNull != zfnull && pausePageOrNull->pageAni() != zfnull)
        {
            pausePageOrNull->pageAniOnUpdateAniTarget(pausePageOrNull->pageAni());
            pausePageOrNull->toObject()->observerNotify(ZFUIPage::EventPageAniOnUpdateAniTarget(), pausePageOrNull->pageAni());
            this->pageAniLastPause = pausePageOrNull->pageAni();
        }
        if(resumePageOrNull != zfnull && resumePageOrNull->pageAni() != zfnull)
        {
            resumePageOrNull->pageAniOnUpdateAniTarget(resumePageOrNull->pageAni());
            resumePageOrNull->toObject()->observerNotify(ZFUIPage::EventPageAniOnUpdateAniTarget(), resumePageOrNull->pageAni());
            this->pageAniLastResume = resumePageOrNull->pageAni();
        }
        if(resumePageOrNull != zfnull)
        {
            resumePageOrNull->_ZFP_ZFUIPage_pageAniCanChange = zffalse;
        }
        if(pausePageOrNull != zfnull)
        {
            pausePageOrNull->_ZFP_ZFUIPage_pageAniCanChange = zffalse;
        }
        this->pimplOwner->pageAniOnUpdatePriority(resumePageOrNull, pausePageOrNull, pausePageHasHigherPriority);
        if(pausePageHasHigherPriority)
        {
            this->pimplOwner->toObject()->observerNotify(
                ZFUIPageManager::EventPageAniOnUpdatePriority(),
                ZFObjectToObject(pausePageOrNull),
                ZFObjectToObject(resumePageOrNull));
        }
        else
        {
            this->pimplOwner->toObject()->observerNotify(
                ZFUIPageManager::EventPageAniOnUpdatePriority(),
                ZFObjectToObject(resumePageOrNull),
                ZFObjectToObject(pausePageOrNull));
        }

        if(pausePageOrNull != zfnull)
        {
            ZFUIPagePauseReasonEnum pauseReason = ZFUIPagePauseReason::e_ByManagerPause;
            switch(reason)
            {
                case ZFUIPageResumeReason::e_ByManagerResume:
                    pauseReason = ZFUIPagePauseReason::e_ByManagerPause;
                    break;
                case ZFUIPageResumeReason::e_ByRequest:
                    pauseReason = ZFUIPagePauseReason::e_ToBackground;
                    break;
                case ZFUIPageResumeReason::e_FromBackground:
                    pauseReason = ZFUIPagePauseReason::e_BeforeDestroy;
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
            if(pausePageOrNull->pageAni() != zfnull)
            {
                this->pimplOwner->pageDelayDetachEnable(pausePageOrNull);
                this->pageAniListenerAttach(pausePageOrNull, zflineAlloc(ZFUIPagePauseReason, pauseReason));
                pausePageOrNull->pageAni()->aniStart();
            }
            else
            {
                // detach immediately
            }
        }
        if(resumePageOrNull != zfnull && resumePageOrNull->pageAni() != zfnull)
        {
            this->pageAniListenerAttach(resumePageOrNull, zflineAlloc(ZFUIPageResumeReason, reason));
            resumePageOrNull->pageAni()->aniStart();
        }
    }
    void pageAniListenerAttach(ZF_IN ZFUIPage *page,
                               ZF_IN ZFEnum *reason)
    {
        zfblockedAlloc(ZFObject, userData);
        userData->tagSet(zfText("page"), page->toObject());
        userData->tagSet(zfText("reason"), reason);
        page->pageAni()->observerAdd(ZFAnimation::EventAniOnStopOrOnInvalid(), this->pageAniOnStopOrOnInvalidListener, userData);
        page->pageAni()->observerAdd(ZFAnimation::EventAniOnStart(), this->pageAniOnStartListener, userData);
        page->pageAni()->observerAdd(ZFAnimation::EventAniOnStop(), this->pageAniOnStopListener, userData);
    }
    void pageAniListenerDetach(ZF_IN ZFUIPage *page)
    {
        page->pageAni()->observerRemove(ZFAnimation::EventAniOnStopOrOnInvalid(), this->pageAniOnStopOrOnInvalidListener);
        page->pageAni()->observerRemove(ZFAnimation::EventAniOnStart(), this->pageAniOnStartListener);
        page->pageAni()->observerRemove(ZFAnimation::EventAniOnStop(), this->pageAniOnStopListener);
    }

public:
    _ZFP_ZFUIPageManagerPrivate(void)
    : pimplOwner(zfnull)
    , pageList()
    , managerResumed(zffalse)
    , managerUIBlocked(0)
    , requestBlocked(0)
    , requestQueue()
    , requestRunning(zffalse)
    , requestOnResolveListener()
    , pageDelayDetachList()
    , pageDelayDestroyList()
    , movePageFlag(zffalse)
    , pageAniOverrideForOnce(zffalse)
    , pageAniOverrideForOnceResumeAni(zfnull)
    , pageAniOverrideForOncePauseAni(zfnull)
    , pageAniOverrideForOncePauseAniHasHigherPriority(zffalse)
    , pageLastResume(zfnull)
    , pageAniLastResume(zfnull)
    , pageAniLastPause(zfnull)
    , pageAniOnStopOrOnInvalidListener(ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerPrivate::pageAniOnStopOrOnInvalid))
    , pageAniOnStartListener(ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerPrivate::pageAniOnStart))
    , pageAniOnStopListener(ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerPrivate::pageAniOnStop))
    {
        this->requestOnResolveListener = ZFCallbackForRawFunction(_ZFP_ZFUIPageManagerPrivate::requestOnResolve);
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIPageManager)

ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerOnCreate)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerOnResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerOnPause)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerOnDestroy)

ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerAfterCreate)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerAfterResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerBeforePause)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerBeforeDestroy)

ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, ManagerUIBlockedOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, RequestBlockedOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, RequestOnResolvePrepare)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, RequestOnResolve)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, RequestOnResolveFinish)
ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, RequestOnResolveFinishAll)

ZFOBSERVER_EVENT_REGISTER(ZFUIPageManager, PageAniOnUpdatePriority)

// ============================================================
// embeded logic
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, embededCreate)
{
    zfCoreAssertWithMessageTrim(d == zfnull, zfTextA("[ZFUIPageManager] already created"));

    zfCoreMutexLock();
    d = zfpoolNew(_ZFP_ZFUIPageManagerPrivate);
    zfCoreMutexUnlock();

    d->pimplOwner = this;
    zfRetain(this);
    this->managerOnCreate();
    this->managerAfterCreate();
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, embededResume)
{
    zfCoreAssertWithMessageTrim(d != zfnull, zfTextA("[ZFUIPageManager] resume called before create"));
    zfCoreAssertWithMessageTrim(!d->managerResumed, zfTextA("[ZFUIPageManager] already resumed"));
    d->managerResumed = zftrue;

    ZFUIPage *foregroundPage = this->pageForeground();
    if(foregroundPage != zfnull)
    {
        d->pageAniUpdate(foregroundPage, zfnull, ZFUIPageResumeReason::e_ByManagerResume);
    }

    this->managerOnResume();

    if(!d->pageList.isEmpty())
    {
        ZFUIPage *foregroundPage = d->pageList.getLast();
        if(!foregroundPage->pageResumed())
        {
            d->pageResume(foregroundPage, ZFUIPageResumeReason::e_ByManagerResume);
        }
    }

    this->managerAfterResume();
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, embededPause)
{
    zfCoreAssertWithMessageTrim(d->managerResumed, zfTextA("[ZFUIPageManager] already paused"));
    d->managerResumed = zffalse;

    this->managerBeforePause();

    if(!d->pageList.isEmpty())
    {
        ZFUIPage *foregroundPage = d->pageList.getLast();
        if(foregroundPage->pageResumed())
        {
            d->pagePause(foregroundPage, ZFUIPagePauseReason::e_ByManagerPause);
        }
    }
    this->managerOnPause();
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, embededDestroy)
{
    zfCoreAssertWithMessageTrim(d != zfnull, zfTextA("[ZFUIPageManager] already destroyed"));
    zfCoreAssertWithMessageTrim(!d->managerResumed, zfTextA("[ZFUIPageManager] destroy called before pause"));
    zfCoreAssertWithMessageTrim(d->pageDelayDestroyList.isEmpty(), zfTextA("[ZFUIPageManager] you must not destroy the manager if there are pages delaying detach or destroy"));

    this->managerBeforeDestroy();

    if(!d->pageList.isEmpty())
    {
        ZFCoreArrayPOD<ZFUIPage *> pageList;
        pageList.copyFrom(d->pageList);
        d->pageList.removeAll();
        for(zfindex i = pageList.count() - 1; i != zfindexMax(); --i)
        {
            ZFUIPage *page = pageList[i];
            d->pageDestroy(page);
            zfRelease(page);
        }
    }

    if(!d->requestQueue.isEmpty())
    {
        ZFCoreArrayPOD<ZFUIPageRequest *> tmp;
        while(!d->requestQueue.isEmpty())
        {
            tmp.add(d->requestQueue.queueTake());
        }
        for(zfindex i = 0; i < tmp.count(); ++i)
        {
            this->managerUIBlockedSet(zffalse);
            zfRelease(tmp[i]);
        }
    }

    this->managerOnDestroy();

    ZFThreadTaskCancelWithOwner(this->toObject());

    zfCoreMutexLock();
    zfpoolDelete(d);
    d = zfnull;
    zfCoreMutexUnlock();

    zfRelease(this);
}

ZFMETHOD_DEFINE_0(ZFUIPageManager, zfbool, managerCreated)
{
    return (d != zfnull);
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfbool, managerResumed)
{
    return (d != zfnull && d->managerResumed);
}

// ============================================================
// manager control
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, managerUIBlockedSet,
                  ZFMP_IN(zfbool, value))
{
    zfCoreAssertWithMessage(d != zfnull, zfTextA("[ZFUIPageManager] manager not created"));

    zfsynchronizedObjectLock(this->toObject());
    if(value)
    {
        ++(d->managerUIBlocked);
        if(d->managerUIBlocked == 1)
        {
            zfsynchronizedObjectUnlock(this->toObject());
            this->managerUIBlockedOnChange();
        }
        else
        {
            zfsynchronizedObjectUnlock(this->toObject());
        }
    }
    else
    {
        --(d->managerUIBlocked);
        if(d->managerUIBlocked == 0)
        {
            zfsynchronizedObjectUnlock(this->toObject());
            this->managerUIBlockedOnChange();
        }
        else
        {
            zfsynchronizedObjectUnlock(this->toObject());
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfindex, managerUIBlocked)
{
    return (d ? d->managerUIBlocked : zffalse);
}

// ============================================================
// page access
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfindex, pageCount)
{
    zfCoreAssertWithMessage(d != zfnull, zfTextA("[ZFUIPageManager] manager not created"));
    return d->pageList.count();
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, ZFCoreArrayPOD<ZFUIPage *> &, pageList)
{
    zfCoreAssertWithMessage(d != zfnull, zfTextA("[ZFUIPageManager] manager not created"));
    return d->pageList;
}

// ============================================================
// page request
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, requestPageCreate,
                  ZFMP_IN(const ZFUIPageRequestPageCreateParam &, createParam))
{
    zfblockedAlloc(ZFUIPageRequestPageCreate, request);
    request->createParam = createParam;
    this->requestPost(request);
}
ZFMETHOD_DEFINE_3(ZFUIPageManager, void, requestPageCreate,
                  ZFMP_IN(ZFUIPage *, page),
                  ZFMP_IN_OPT(ZFObject *, pageCreateParam, zfnull),
                  ZFMP_IN_OPT(zfbool, pageAutoResume, zftrue))
{
    if(page == zfnull)
    {
        return ;
    }
    zfblockedAlloc(ZFUIPageRequestPageCreate, request);
    request->createParam
        .pageSet(page)
        .pageCreateParamSet(pageCreateParam)
        .pageAutoResumeSet(pageAutoResume);
    this->requestPost(request);
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, requestPageResume,
                  ZFMP_IN(ZFUIPage *, page))
{
    if(page == zfnull)
    {
        return ;
    }
    zfblockedAlloc(ZFUIPageRequestPageResume, request);
    request->pageSet(page);
    this->requestPost(request);
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, requestPageGroupResume,
                  ZFMP_IN(const zfchar *, pageGroupId))
{
    zfblockedAlloc(ZFUIPageRequestPageGroupResume, request);
    request->pageGroupIdSet(zfstring(pageGroupId));
    this->requestPost(request);
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, requestPageDestroy,
                  ZFMP_IN(ZFUIPage *, page))
{
    if(page == zfnull)
    {
        return ;
    }
    zfblockedAlloc(ZFUIPageRequestPageDestroy, request);
    request->pageSet(page);
    this->requestPost(request);
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, requestPost,
                  ZFMP_IN(ZFUIPageRequest *, request))
{
    zfCoreAssertWithMessage(d != zfnull, zfTextA("[ZFUIPageManager] manager not created"));
    if(request != zfnull)
    {
        zfsynchronizedObject(this->toObject());
        this->managerUIBlockedSet(zftrue);
        d->requestQueue.queuePut(zfRetain(request));
        d->requestDoPost(this->toObject());
    }
}
ZFMETHOD_DEFINE_1(ZFUIPageManager, void, requestBlockedSet,
                  ZFMP_IN(zfbool, value))
{
    zfCoreAssertWithMessage(d != zfnull, zfTextA("[ZFUIPageManager] manager not created"));
    zfsynchronizedObjectLock(this->toObject());
    if(value)
    {
        ++(d->requestBlocked);
        if(d->requestBlocked == 1)
        {
            zfsynchronizedObjectUnlock(this->toObject());
            this->requestBlockedOnChange();
        }
        else
        {
            zfsynchronizedObjectUnlock(this->toObject());
        }
    }
    else
    {
        --(d->requestBlocked);
        if(d->requestBlocked == 0)
        {
            zfsynchronizedObjectUnlock(this->toObject());
            this->requestBlockedOnChange();
            d->requestDoPost(this->toObject());
        }
        else
        {
            zfsynchronizedObjectUnlock(this->toObject());
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, zfindex, requestBlocked)
{
    return (d ? d->requestBlocked : 0);
}

// ============================================================
// request resolve
void ZFUIPageManager::requestOnResolve(ZF_IN ZFUIPageRequest *request)
{
    this->toObject()->observerNotify(ZFUIPageManager::EventRequestOnResolve(), this->toObject());
    this->toObject()->observerNotifyWithCustomSender(this->toObject(), ZFUIPageManager::EventRequestOnResolve(), this->toObject());

    if(request->requestResolved())
    {
        return ;
    }

    const ZFClass *cls = request->classData();
    if(cls->classIsTypeOf(ZFUIPageRequestPageResume::ClassData()))
    {
        this->requestOnResolvePageResume(request->to<ZFUIPageRequestPageResume *>());
    }
    else if(cls->classIsTypeOf(ZFUIPageRequestPageCreate::ClassData()))
    {
        this->requestOnResolvePageCreate(request->to<ZFUIPageRequestPageCreate *>());
    }
    else if(cls->classIsTypeOf(ZFUIPageRequestPageDestroy::ClassData()))
    {
        this->requestOnResolvePageDestroy(request->to<ZFUIPageRequestPageDestroy *>());
    }
    else if(cls->classIsTypeOf(ZFUIPageRequestPageGroupResume::ClassData()))
    {
        this->requestOnResolvePageGroupResume(request->to<ZFUIPageRequestPageGroupResume *>());
    }
    else if(cls->classIsTypeOf(ZFUIPageRequestCustom::ClassData()))
    {
        this->requestOnResolveCustom(request->to<ZFUIPageRequestCustom *>());
    }
}
void ZFUIPageManager::requestOnResolvePageCreate(ZF_IN ZFUIPageRequestPageCreate *request)
{
    this->resolvePageCreate(request);
}
void ZFUIPageManager::resolvePageCreate(ZF_IN ZFUIPageRequestPageCreate *request)
{
    const ZFUIPageRequestPageCreateParam &createParam = request->createParam;
    zfCoreAssertWithMessageTrim(createParam.page() != zfnull, zfTextA("[ZFUIPageManager] page is nil when requestPageCreate"));
    zfCoreAssertWithMessageTrim(!createParam.page()->pageCreated(), zfTextA("[ZFUIPageManager] page already created when requestPageCreate"));

    ZFUIPage *page = createParam.page();
    page->_ZFP_ZFUIPage_pageManager = this;
    zfRetain(page);
    d->pageList.add(0, page);

    d->pageCreate(page, createParam);

    if(createParam.pageAutoResume())
    {
        this->requestPageResume(page);
    }

    request->requestResolvedSet(zftrue);
}
void ZFUIPageManager::requestOnResolvePageResume(ZF_IN ZFUIPageRequestPageResume *request)
{
    this->resolvePageResume(request);
}
void ZFUIPageManager::resolvePageResume(ZF_IN ZFUIPageRequestPageResume *request)
{
    ZFUIPage *page = request->page();
    zfindex index = d->pageList.find(page);
    if(index == zfindexMax() || page->pageResumed())
    {
        return ;
    }
    if(index != d->pageList.count() - 1)
    {
        d->pageList.remove(index);
        d->pageList.add(page);
    }

    d->pageResume(page, ZFUIPageResumeReason::e_ByRequest);

    if(d->pageList.count() >= 2)
    {
        ZFUIPage *old = d->pageList[d->pageList.count() - 2];
        if(old->pageResumed())
        {
            d->pagePause(old, ZFUIPagePauseReason::e_ToBackground);
        }
    }

    request->requestResolvedSet(zftrue);
}
void ZFUIPageManager::requestOnResolvePageGroupResume(ZF_IN ZFUIPageRequestPageGroupResume *request)
{
    this->resolvePageGroupResume(request);
}
void ZFUIPageManager::resolvePageGroupResume(ZF_IN ZFUIPageRequestPageGroupResume *request)
{
    const zfchar *pageGroupId = request->pageGroupId();
    this->movePageBegin();
    ZFCoreArrayPOD<ZFUIPage *> &pageList = this->pageList();
    ZFCoreArrayPOD<ZFUIPage *> pageListTmp;
    for(zfindex i = 0; i < pageList.count(); ++i)
    {
        ZFUIPage *page = pageList[i];
        if(page->pageGroupId().compare(pageGroupId) == 0)
        {
            pageList.remove(i);
            --i;
            pageListTmp.add(page);
        }
    }
    pageList.addFrom(pageListTmp);
    this->movePageEnd();

    request->requestResolvedSet(zftrue);
}
void ZFUIPageManager::requestOnResolvePageDestroy(ZF_IN ZFUIPageRequestPageDestroy *request)
{
    this->resolvePageDestroy(request);
}
void ZFUIPageManager::resolvePageDestroy(ZF_IN ZFUIPageRequestPageDestroy *request)
{
    ZFUIPage *page = request->page();
    zfindex index = d->pageList.find(page);
    if(index == zfindexMax())
    {
        return ;
    }
    if(index == d->pageList.count() - 1)
    {
        d->pageList.remove(index);

        if(!d->pageList.isEmpty())
        {
            ZFUIPage *foregroundPage = d->pageList.getLast();
            if(!foregroundPage->pageResumed())
            {
                d->pageResume(foregroundPage, ZFUIPageResumeReason::e_FromBackground);
            }
        }

        if(page->pageResumed())
        {
            d->pagePause(page, ZFUIPagePauseReason::e_BeforeDestroy);
        }

        page->pageDelayDestroyOnCheck();
        this->pageDelayDestroyOnCheck(page);
        if(page->_ZFP_ZFUIPage_pageDelayDestroyFlag > 0 || page->_ZFP_ZFUIPage_pageDelayDestroyByManagerFlag > 0)
        {
            d->pageDelayDestroyList.add(page);
        }
        else
        {
            d->pageDestroy(page);
            zfRelease(page);
        }
    }
    else
    {
        d->pageList.remove(index);

        if(!d->pageList.isEmpty())
        {
            ZFUIPage *foregroundPage = d->pageList.getLast();
            if(!foregroundPage->pageResumed())
            {
                d->pageResume(foregroundPage, ZFUIPageResumeReason::e_FromBackground);
            }
        }

        d->pageDestroy(page);
        zfRelease(page);
    }

    request->requestResolvedSet(zftrue);
}

void ZFUIPageManager::requestOnResolveCustom(ZF_IN ZFUIPageRequestCustom *request)
{
    this->resolveCustom(request);
}
void ZFUIPageManager::resolveCustom(ZF_IN ZFUIPageRequestCustom *request)
{
    request->listener().execute(
        ZFListenerData(zfidentityInvalid(), this->toObject(), request->param0(), request->param1()),
        request->userData());

    request->requestResolvedSet(zftrue);
}

// ============================================================
// pageDelayDetach
void ZFUIPageManager::pageDelayDetachOnCheck(ZF_IN ZFUIPage *page,
                                             ZF_IN ZFUIPagePauseReasonEnum reason)
{
    // nothing to do
}
void ZFUIPageManager::pageDelayDetachEnable(ZF_IN ZFUIPage *page)
{
    zfCoreAssertWithMessage(d != zfnull, zfTextA("[ZFUIPageManager] manager not created"));
    ++(page->_ZFP_ZFUIPage_pageDelayDetachByManagerFlag);
    this->pageDelayDestroyEnable(page);
}
void ZFUIPageManager::pageDelayDetachNotifyFinish(ZF_IN ZFUIPage *page)
{
    --(page->_ZFP_ZFUIPage_pageDelayDetachByManagerFlag);
    d->pageDelayDetachCheckFinish(page);
    this->pageDelayDestroyNotifyFinish(page);
}
void ZFUIPageManager::_ZFP_ZFUIPageManager_pageDelayDetachNotifyFinish(ZF_IN ZFUIPage *page)
{
    d->pageDelayDetachCheckFinish(page);
    this->pageDelayDestroyNotifyFinish(page);
}

// ============================================================
// pageDelayDestroy
void ZFUIPageManager::pageDelayDestroyOnCheck(ZF_IN ZFUIPage *page)
{
    // nothing to do
}
void ZFUIPageManager::pageDelayDestroyEnable(ZF_IN ZFUIPage *page)
{
    zfCoreAssertWithMessage(d != zfnull, zfTextA("[ZFUIPageManager] manager not created"));
    ++(page->_ZFP_ZFUIPage_pageDelayDestroyByManagerFlag);
}
void ZFUIPageManager::pageDelayDestroyNotifyFinish(ZF_IN ZFUIPage *page)
{
    --(page->_ZFP_ZFUIPage_pageDelayDestroyByManagerFlag);
    d->pageDelayDestroyCheckFinish(page);
}
void ZFUIPageManager::_ZFP_ZFUIPageManager_pageDelayDestroyNotifyFinish(ZF_IN ZFUIPage *page)
{
    d->pageDelayDestroyCheckFinish(page);
}

// ============================================================
// page move
void ZFUIPageManager::movePageBegin(void)
{
    zfCoreAssertWithMessage(d != zfnull, zfTextA("[ZFUIPageManager] manager not created"));
    zfCoreAssertWithMessageTrim(!d->movePageFlag, zfTextA("[ZFUIPageManager] move page not paired, have you forgot movePageEnd?"));
    d->movePageFlag = zftrue;
}
void ZFUIPageManager::movePageEnd(void)
{
    zfCoreAssertWithMessageTrim(d->movePageFlag, zfTextA("[ZFUIPageManager] move page not paired, have you forgot movePageBegin?"));
    d->movePageFlag = zffalse;

    ZFUIPage *pageToPause = zfnull;
    for(zfindex i = d->pageList.count() - 1; i != zfindexMax(); --i)
    {
        ZFUIPage *page = d->pageList[i];
        if(page->pageResumed())
        {
            if(i != d->pageList.count() - 1)
            {
                pageToPause = page;
            }
            break;
        }
    }

    if(!d->pageList.isEmpty())
    {
        ZFUIPage *page = d->pageList.getLast();
        if(!page->pageResumed())
        {
            d->pageResume(page, ZFUIPageResumeReason::e_ByRequest);
        }
    }

    if(pageToPause != zfnull)
    {
        d->pagePause(pageToPause, ZFUIPagePauseReason::e_ToBackground);
    }
}

// ============================================================
// page ani
ZFMETHOD_DEFINE_3(ZFUIPageManager, void, pageAniOverrideForOnce,
                  ZFMP_IN(ZFAnimation *, pageAniResume),
                  ZFMP_IN(ZFAnimation *, pageAniPause),
                  ZFMP_IN_OPT(zfbool, pageAniPauseHasHigherPriority, zffalse))
{
    zfCoreAssertWithMessage(d != zfnull, zfTextA("[ZFUIPageManager] manager not created"));
    d->pageAniOverrideForOnce = zftrue;
    zfRetainChange(d->pageAniOverrideForOnceResumeAni, pageAniResume);
    zfRetainChange(d->pageAniOverrideForOncePauseAni, pageAniPause);
    d->pageAniOverrideForOncePauseAniHasHigherPriority = pageAniPauseHasHigherPriority;
}
ZFMETHOD_DEFINE_0(ZFUIPageManager, void, pageAniOverrideForOnceCancel)
{
    if(d != zfnull)
    {
        d->pageAniOverrideForOnce = zffalse;
        zfRetainChange(d->pageAniOverrideForOnceResumeAni, zfnull);
        zfRetainChange(d->pageAniOverrideForOncePauseAni, zfnull);
    }
}
zfbool ZFUIPageManager::pageAniOverrideForOnceCheckUpdate(ZF_IN ZFUIPage *resumePageOrNull,
                                                          ZF_IN ZFUIPage *pausePageOrNull,
                                                          ZF_IN ZFUIPageResumeReasonEnum reason)
{
    if(!d->pageAniOverrideForOnce)
    {
        return zffalse;
    }
    d->pageAniOverrideForOnce = zffalse;
    if(resumePageOrNull != zfnull)
    {
        resumePageOrNull->pageAniSet(d->pageAniOverrideForOnceResumeAni);
        resumePageOrNull->pageAniPriorityNeedHigher = zffalse;
    }
    if(pausePageOrNull != zfnull)
    {
        pausePageOrNull->pageAniSet(d->pageAniOverrideForOncePauseAni);
        pausePageOrNull->pageAniPriorityNeedHigher = d->pageAniOverrideForOncePauseAniHasHigherPriority;
    }
    zfRetainChange(d->pageAniOverrideForOnceResumeAni, zfnull);
    zfRetainChange(d->pageAniOverrideForOncePauseAni, zfnull);
    return zftrue;
}
void ZFUIPageManager::pageAniOnUpdate(ZF_IN ZFUIPage *resumePageOrNull,
                                      ZF_IN ZFUIPage *pausePageOrNull,
                                      ZF_IN ZFUIPageResumeReasonEnum reason)
{
    if(this->pageAniOverrideForOnceCheckUpdate(resumePageOrNull, pausePageOrNull, reason))
    {
        return ;
    }

    switch(reason)
    {
        case ZFUIPageResumeReason::e_ByManagerResume:
            break;
        case ZFUIPageResumeReason::e_ByRequest:
            if(pausePageOrNull != zfnull)
            {
                pausePageOrNull->pageAniSet(pausePageOrNull->pageAniPauseToBackground());
            }
            if(resumePageOrNull != zfnull)
            {
                resumePageOrNull->pageAniSet(resumePageOrNull->pageAniResumeByRequest());
            }
            break;
        case ZFUIPageResumeReason::e_FromBackground:
            if(pausePageOrNull != zfnull)
            {
                pausePageOrNull->pageAniPriorityNeedHigher = zftrue;
                pausePageOrNull->pageAniSet(pausePageOrNull->pageAniPauseBeforeDestroy());
            }
            if(resumePageOrNull != zfnull)
            {
                resumePageOrNull->pageAniSet(resumePageOrNull->pageAniResumeFromBackground());
            }
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }

    if(pausePageOrNull != zfnull)
    {
        pausePageOrNull->pageAniOnUpdate(resumePageOrNull, pausePageOrNull, reason);
    }
    if(resumePageOrNull != zfnull)
    {
        resumePageOrNull->pageAniOnUpdate(resumePageOrNull, pausePageOrNull, reason);
    }
}

ZF_NAMESPACE_GLOBAL_END

