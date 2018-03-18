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
 * @file ZFUIPage.h
 * @brief abstract page
 */

#ifndef _ZFI_ZFUIPage_h_
#define _ZFI_ZFUIPage_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief page resume reason
 */
ZFENUM_BEGIN(ZFUIPageResumeReason)
    ZFENUM_VALUE(ByManagerResume)
    ZFENUM_VALUE(ByRequest)
    ZFENUM_VALUE(FromBackground)
ZFENUM_SEPARATOR(ZFUIPageResumeReason)
    ZFENUM_VALUE_REGISTER(ByManagerResume)
    ZFENUM_VALUE_REGISTER(ByRequest)
    ZFENUM_VALUE_REGISTER(FromBackground)
ZFENUM_END(ZFUIPageResumeReason)

/**
 * @brief page pause reason
 */
ZFENUM_BEGIN(ZFUIPagePauseReason)
    ZFENUM_VALUE(ByManagerPause)
    ZFENUM_VALUE(ToBackground)
    ZFENUM_VALUE(BeforeDestroy)
ZFENUM_SEPARATOR(ZFUIPagePauseReason)
    ZFENUM_VALUE_REGISTER(ByManagerPause)
    ZFENUM_VALUE_REGISTER(ToBackground)
    ZFENUM_VALUE_REGISTER(BeforeDestroy)
ZFENUM_END(ZFUIPagePauseReason)

/**
 * @brief invalid page group id, see #ZFUIPageManager::requestPageGroupResume for more info
 */
#define ZFUIPageGroupIdInvalid zfText("ZFUIPageGroupIdInvalid")

// ============================================================
zfclassFwd ZFUIPageManager;
zfclassFwd _ZFP_ZFUIPageManagerPrivate;
/**
 * @brief abstract page, see #ZFUIPageManager for how to use
 */
zfinterface ZF_ENV_EXPORT ZFUIPage : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE_WITH_CUSTOM_CTOR(ZFUIPage, ZFInterface)

    // ============================================================
    // observers
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page created\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageOnCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPageResumeReason\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageOnResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page attached to manager,
     * param0 is #ZFUIPageResumeReason\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageOnAttach)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page detached from manager,
     * param0 is #ZFUIPagePauseReason\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageOnDetach)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPagePauseReason\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageOnPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page destroyed\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageOnDestroy)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page created\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageAfterCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPageResumeReason\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageBeforeResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPageResumeReason\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageAfterResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPagePauseReason\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageBeforePause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page resumed,
     * param0 is #ZFUIPagePauseReason\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageAfterPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page destroyed\n
     * this event would also be fired to it's owner #pageManager
     */
    ZFOBSERVER_EVENT(PageBeforeDestroy)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to update page animation before start
     */
    ZFOBSERVER_EVENT(PageAniOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to update page animation's target before start,
     * param0 is the animation (ensured not null)
     */
    ZFOBSERVER_EVENT(PageAniOnUpdateAniTarget)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page about to start animation,
     * ensured called even if no animation to start for convenient\n
     * param0 is the animation to start or null if no animation to start,
     * param1 is the #ZFUIPagePauseReason or #ZFUIPageResumeReason
     */
    ZFOBSERVER_EVENT(PageAniOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when page about to stop animation,
     * ensured called even if no animation to start for convenient\n
     * param0 is the animation to stop or null if no animation to stop,
     * param1 is the #ZFUIPagePauseReason or #ZFUIPageResumeReason
     */
    ZFOBSERVER_EVENT(PageAniOnStop)

    // ============================================================
    // manager spec
public:
    /**
     * @brief owner page manager
     */
    virtual ZFUIPageManager *pageManager(void)
    {
        return this->_ZFP_ZFUIPage_pageManager;
    }
    /**
     * @brief owner page manager
     */
    template<typename T_ZFUIPageManager>
    T_ZFUIPageManager pageManager(void)
    {
        return ZFCastZFObjectUnchecked(T_ZFUIPageManager, this->pageManager());
    }

protected:
    /**
     * @brief set the #pageCreateParam, usually set by owner page manager only
     */
    ZFMETHOD_INLINE_DETAIL_1(protected, ZFMethodIsVirtual,
                             void, pageCreateParamSet,
                             ZFMP_IN(ZFObject * const &, pageCreateParam))
    {
        zfRetainChange(this->_ZFP_ZFUIPage_pageCreateParam, pageCreateParam);
    }
public:
    /**
     * @brief page create param passed from #ZFUIPageManager::requestPageCreate
     */
    ZFMETHOD_INLINE_0(ZFObject * const &, pageCreateParam)
    {
        return _ZFP_ZFUIPage_pageCreateParam;
    }

    // ============================================================
    // page property
public:
    /**
     * @brief page group id, see #ZFUIPageManager::requestPageGroupResume for more info
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfstring, pageGroupId, ZFUIPageGroupIdInvalid)
    /**
     * @brief page's animation
     *
     * we have these animations for page to setup:
     * -  pageAniResumeByRequest / pagePauseAniToBackground:
     *   used when a page is resumed by user request,
     *   and the previous resume page would be sent to background
     * -  pageAniResumeFromBackground / pagePauseAniBeforeDestroy:
     *   used when a foreground page is destroyed by user request,
     *   and background page would result to be moved to foreground
     *
     * see #ZFUIPageManager::pageAniOnUpdate for more info
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, pageAniResumeByRequest)
    /**
     * @brief page's animation, see #pageAniResumeByRequest
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, pageAniResumeFromBackground)
    /**
     * @brief page's animation, see #pageAniResumeByRequest
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, pageAniPauseToBackground)
    /**
     * @brief page's animation, see #pageAniResumeByRequest
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, pageAniPauseBeforeDestroy)

    // ============================================================
    // page state
public:
    /**
     * @brief true if page created (true after #pageOnCreate called)
     */
    ZFMETHOD_INLINE_0(zfbool, pageCreated)
    {
        return _ZFP_ZFUIPage_pageCreated;
    }
    /**
     * @brief true if page resumed (true after #pageOnResume called)
     */
    ZFMETHOD_INLINE_0(zfbool, pageResumed)
    {
        return _ZFP_ZFUIPage_pageResumed;
    }
    /**
     * @brief true if page attached (true after #pageOnAttach and false before #pageOnDetach)
     */
    ZFMETHOD_INLINE_0(zfbool, pageAttached)
    {
        return _ZFP_ZFUIPage_pageAttached;
    }

    /**
     * @brief equal to invoke #ZFUIPageManager::requestPageDestroy
     */
    ZFMETHOD_DECLARE_0(void, pageDestroy)

    // ============================================================
    // page life cycle
protected:
    /**
     * @brief see #ZFUIPageManager
     */
    virtual inline void pageOnCreate(void)
    {
        this->_ZFP_ZFUIPage_pageOnCreateCalled = zftrue;
        this->pageAniOnInit();
        this->toObject()->observerNotify(zfself::EventPageOnCreate());
    }
    /**
     * @brief see #ZFUIPageManager
     */
    virtual inline void pageOnResume(ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->_ZFP_ZFUIPage_pageOnResumeCalled = zftrue;
        this->toObject()->observerNotify(zfself::EventPageOnResume(), zflineAlloc(ZFUIPageResumeReason, reason));
    }
    /**
     * @brief see #ZFUIPageManager
     */
    virtual inline void pageOnAttach(ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->_ZFP_ZFUIPage_pageOnAttachCalled = zftrue;
        this->toObject()->observerNotify(zfself::EventPageOnAttach(), zflineAlloc(ZFUIPageResumeReason, reason));
    }
    /**
     * @brief see #ZFUIPageManager
     */
    virtual inline void pageOnDetach(ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->_ZFP_ZFUIPage_pageOnDetachCalled = zftrue;
        this->toObject()->observerNotify(zfself::EventPageOnDetach(), zflineAlloc(ZFUIPagePauseReason, reason));
    }
    /**
     * @brief see #ZFUIPageManager
     */
    virtual inline void pageOnPause(ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->_ZFP_ZFUIPage_pageOnPauseCalled = zftrue;
        this->toObject()->observerNotify(zfself::EventPageOnPause(), zflineAlloc(ZFUIPagePauseReason, reason));
    }
    /**
     * @brief see #ZFUIPageManager
     */
    virtual inline void pageOnDestroy(void)
    {
        this->_ZFP_ZFUIPage_pageOnDestroyCalled = zftrue;
        this->_ZFP_ZFUIPage_pageAniCanChange = zftrue;
        this->pageAniSet(zfnull);
        this->_ZFP_ZFUIPage_pageAniCanChange = zffalse;
        this->toObject()->observerNotify(zfself::EventPageOnDestroy());
    }

    // ============================================================
    // page events
protected:
    /** @brief see #EventPageAfterCreate */
    virtual inline void pageAfterCreate(void)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageAfterCreate());
    }
    /** @brief see #EventPageBeforeResume */
    virtual inline void pageBeforeResume(ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageBeforeResume(),
                                         zflineAlloc(ZFUIPageResumeReason, reason));
    }
    /** @brief see #EventPageAfterResume */
    virtual inline void pageAfterResume(ZF_IN ZFUIPageResumeReasonEnum reason)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageAfterResume(),
                                         zflineAlloc(ZFUIPageResumeReason, reason));
    }
    /** @brief see #EventPageBeforePause */
    virtual inline void pageBeforePause(ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageBeforePause(),
                                         zflineAlloc(ZFUIPagePauseReason, reason));
    }
    /** @brief see #EventPageAfterPause */
    virtual inline void pageAfterPause(ZF_IN ZFUIPagePauseReasonEnum reason)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageAfterPause(),
                                         zflineAlloc(ZFUIPagePauseReason, reason));
    }
    /** @brief see #EventPageBeforeDestroy */
    virtual inline void pageBeforeDestroy(void)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageBeforeDestroy());
    }

    // ============================================================
    // pageDelayDetach
protected:
    /** @brief see #ZFUIPageManager::pageDelayDetachOnCheck */
    virtual void pageDelayDetachOnCheck(ZF_IN ZFUIPagePauseReasonEnum reason);
    /** @brief see #ZFUIPageManager::pageDelayDetachOnCheck */
    zffinal void pageDelayDetachEnable(void)
    {
        ++(this->_ZFP_ZFUIPage_pageDelayDetachFlag);
    }
    /** @brief see #ZFUIPageManager::pageDelayDetachOnCheck */
    zffinal void pageDelayDetachNotifyFinish(void);

    // ============================================================
    // pageDelayDestroy
protected:
    /** @brief see #ZFUIPageManager::pageDelayDestroyOnCheck */
    virtual void pageDelayDestroyOnCheck(void);
    /** @brief see #ZFUIPageManager::pageDelayDestroyOnCheck */
    zffinal void pageDelayDestroyEnable(void)
    {
        ++(this->_ZFP_ZFUIPage_pageDelayDestroyFlag);
    }
    /** @brief see #ZFUIPageManager::pageDelayDestroyOnCheck */
    zffinal void pageDelayDestroyNotifyFinish(void);
public:
    zffinal void _ZFP_pageDelayDestroyNotifyFinish(void)
    {
        this->pageDelayDestroyNotifyFinish();
    }

    // ============================================================
    // page ani
public:
    /**
     * @brief the final animation being used when page stack changed, null to disable animation
     *
     * this value can only be updated by #pageAniOnUpdate,
     * you may override the method to override the animation\n
     * this value would be reset to null when animation stopped or page destroyed\n
     */
    ZFMETHOD_DECLARE_1(void, pageAniSet,
                       ZFMP_IN(ZFAnimation *, pageAni))
    /**
     * @brief see #pageAniSet
     */
    ZFMETHOD_INLINE_0(ZFAnimation *, pageAni)
    {
        return _ZFP_ZFUIPage_pageAni;
    }

public:
    /**
     * @brief whether this page need higher priority for animation,
     *   typically a higher priority animation would have its view on the top
     */
    zfbool pageAniPriorityNeedHigher;
protected:
    /**
     * @brief called only once to init page animation during #pageOnCreate,
     *   do nothing by default,
     *   you may override without calling super to override default animation
     */
    virtual void pageAniOnInit(void)
    {
    }
    /**
     * @brief for page to update it's final animation, see #ZFUIPageManager::pageAniOnUpdate
     */
    virtual void pageAniOnUpdate(ZF_IN ZFUIPage *resumePageOrNull,
                                 ZF_IN ZFUIPage *pausePageOrNull,
                                 ZF_IN ZFUIPageResumeReasonEnum reason);
    /**
     * @brief called to update animation's target, do nothing by default
     */
    virtual void pageAniOnUpdateAniTarget(ZF_IN ZFAnimation *pageAni)
    {
    }

    // ============================================================
    // event
protected:
    /** @brief see #EventPageAniOnStart */
    virtual void pageAniOnStart(ZF_IN ZFAnimation *pageAni,
                                ZF_IN ZFEnum *pagePauseReasonOrResumeReason)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageAniOnStart(),
            pageAni, pagePauseReasonOrResumeReason);
    }
    /** @brief see #EventPageAniOnStop */
    virtual void pageAniOnStop(ZF_IN ZFAnimation *pageAni,
                               ZF_IN ZFEnum *pagePauseReasonOrResumeReason)
    {
        this->toObject()->observerNotify(ZFUIPage::EventPageAniOnStop(),
            pageAni, pagePauseReasonOrResumeReason);
    }

    // ============================================================
private:
    ZFUIPageManager *_ZFP_ZFUIPage_pageManager;
    ZFObject *_ZFP_ZFUIPage_pageCreateParam;
    zfbool _ZFP_ZFUIPage_pageCreated;
    zfbool _ZFP_ZFUIPage_pageResumed;
    zfbool _ZFP_ZFUIPage_pageAttached;
    zfbool _ZFP_ZFUIPage_pageOnCreateCalled;
    zfbool _ZFP_ZFUIPage_pageOnResumeCalled;
    zfbool _ZFP_ZFUIPage_pageOnAttachCalled;
    zfbool _ZFP_ZFUIPage_pageOnDetachCalled;
    zfbool _ZFP_ZFUIPage_pageOnPauseCalled;
    zfbool _ZFP_ZFUIPage_pageOnDestroyCalled;
    zfuint _ZFP_ZFUIPage_pageDelayDetachFlag;
    zfuint _ZFP_ZFUIPage_pageDelayDetachByManagerFlag;
    zfuint _ZFP_ZFUIPage_pageDelayDestroyFlag;
    zfuint _ZFP_ZFUIPage_pageDelayDestroyByManagerFlag;
    ZFAnimation *_ZFP_ZFUIPage_pageAni;
    zfbool _ZFP_ZFUIPage_pageAniCanChange;
protected:
    /** @cond ZFPrivateDoc */
    ZFUIPage(void)
    : _ZFP_ZFUIPage_pageManager(zfnull)
    , _ZFP_ZFUIPage_pageCreateParam(zfnull)
    , _ZFP_ZFUIPage_pageCreated(zffalse)
    , _ZFP_ZFUIPage_pageResumed(zffalse)
    , _ZFP_ZFUIPage_pageAttached(zffalse)
    , _ZFP_ZFUIPage_pageOnCreateCalled(zffalse)
    , _ZFP_ZFUIPage_pageOnResumeCalled(zffalse)
    , _ZFP_ZFUIPage_pageOnAttachCalled(zffalse)
    , _ZFP_ZFUIPage_pageOnDetachCalled(zffalse)
    , _ZFP_ZFUIPage_pageOnPauseCalled(zffalse)
    , _ZFP_ZFUIPage_pageOnDestroyCalled(zffalse)
    , _ZFP_ZFUIPage_pageDelayDetachFlag(0)
    , _ZFP_ZFUIPage_pageDelayDetachByManagerFlag(0)
    , _ZFP_ZFUIPage_pageDelayDestroyFlag(0)
    , _ZFP_ZFUIPage_pageDelayDestroyByManagerFlag(0)
    , _ZFP_ZFUIPage_pageAni(zfnull)
    , _ZFP_ZFUIPage_pageAniCanChange(zffalse)
    {
    }
    /** @endcond */
private:
    zffinal void _ZFP_ZFUIPage_pageCreate(void);
    zffinal void _ZFP_ZFUIPage_pageResume(ZF_IN ZFUIPageResumeReasonEnum reason);
    zffinal void _ZFP_ZFUIPage_pageAttach(ZF_IN ZFUIPageResumeReasonEnum reason);
    zffinal void _ZFP_ZFUIPage_pageDetach(ZF_IN ZFUIPagePauseReasonEnum reason);
    zffinal void _ZFP_ZFUIPage_pagePause(ZF_IN ZFUIPagePauseReasonEnum reason);
    zffinal void _ZFP_ZFUIPage_pageDestroy(void);
private:
    friend zfclassFwd ZFUIPageManager;
    friend zfclassFwd _ZFP_ZFUIPageManagerPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPage_h_

