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
 * @file ZFUIViewStateAni.h
 * @brief advanced animation that apply by state
 */

#ifndef _ZFI_ZFUIViewStateAni_h_
#define _ZFI_ZFUIViewStateAni_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief filter to exclude certain view to be animated by #ZFUIViewStateAniStart, empty by default
 */
ZFEXPORT_VAR_DECLARE(ZFFilterForZFObject, ZFUIViewStateAniFilter)

// ============================================================
/**
 * @brief advanced animation that apply by view state
 *
 * for app level users, simply call this method and change your view settings,
 * animations would be applied automatically\n
 * \n
 * this method use native animation to performan view animations,
 * typically has better performance but may cause strange animation\n
 * you may also use #ZFPropertyAniBegin/#ZFPropertyAniEnd to perform property change animation,
 * which are implemented by update property value accorrding to timeline,
 * best compatibility but may has worse performance,
 * and only the properties that supply #ZFTYPEID_PROGRESS_DECLARE are supported\n
 * \n
 * \n
 * \n
 * for those who want to care about implementations:\n
 * state animation are implemented by:
 * -# attach observer to #ZFUIView::EventViewLayoutOnLayoutRequest
 * -# when the view layout finished (#ZFUIView::EventViewLayoutOnLayoutFinish),
 *   compare view's state, if changed, apply animation accorrding to the change
 *
 * \n
 * for animation impl, attach observer to these events and apply your animation:
 * -  #ZFUIViewStateAniImpl::EventStateAniOnInit
 * -  #ZFUIViewStateAniImpl::EventStateAniOnDealloc
 * -  #ZFUIViewStateAniImpl::EventStateAniViewAttach
 * -  #ZFUIViewStateAniImpl::EventStateAniViewDetach
 * -  #ZFUIViewStateAniImpl::EventStateAniViewAniPrepare
 * -  #ZFUIViewStateAniImpl::EventStateAniViewAniStart
 * -  #ZFUIViewStateAniImpl::EventStateAniViewAniStop
 *
 * use #ZFUIViewStateAniImplRegister is recommended
 */
ZFMETHOD_FUNC_DECLARE_0(void, ZFUIViewStateAniStart)
/**
 * @brief whether #ZFUIViewStateAniStart started
 */
ZFMETHOD_FUNC_DECLARE_0(zfbool, ZFUIViewStateAniStarted)

// ============================================================
ZF_NAMESPACE_BEGIN(ZFUIViewStateAniImpl)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFUIViewStateAniStart actually started,
 * you may do your own initialize step here
 */
ZFOBSERVER_EVENT_GLOBAL_WITH_NS(ZFUIViewStateAniImpl, StateAniOnInit)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFUIViewStateAniStart all tasks stopped,
 * you may do your own cleanup step here
 */
ZFOBSERVER_EVENT_GLOBAL_WITH_NS(ZFUIViewStateAniImpl, StateAniOnDealloc)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFUIViewStateAniStart started and a view invoked #ZFUIView::layoutRequest,
 * you may store the attached view for future use\n
 * sender is the view
 */
ZFOBSERVER_EVENT_GLOBAL_WITH_NS(ZFUIViewStateAniImpl, StateAniViewAttach)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when a view attached by #ZFUIViewStateAniImpl::EventStateAniViewAttach
 * would finish all of its task,
 * you may do your own cleanup step here\n
 * sender is the view
 */
ZFOBSERVER_EVENT_GLOBAL_WITH_NS(ZFUIViewStateAniImpl, StateAniViewDetach)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified to check whether the view needs animation\n
 * sender is the view,
 * param0 is a #ZFArrayEditable to store #ZFAnimation,
 * if more than one #ZFAnimation added to param0,
 * only the first one would be used,
 * if nothing added to param0,
 * no animation would be applied to the view
 */
ZFOBSERVER_EVENT_GLOBAL_WITH_NS(ZFUIViewStateAniImpl, StateAniViewAniPrepare)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified before start the animation for each view that prepared by
 * #ZFUIViewStateAniImpl::EventStateAniViewAniPrepare\n
 * sender is the view,
 * param0 is the #ZFAnimation
 */
ZFOBSERVER_EVENT_GLOBAL_WITH_NS(ZFUIViewStateAniImpl, StateAniViewAniStart)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when stop the animation for each view that prepared by
 * #ZFUIViewStateAniImpl::EventStateAniViewAniPrepare\n
 * sender is the view,
 * param0 is the #ZFAnimation
 */
ZFOBSERVER_EVENT_GLOBAL_WITH_NS(ZFUIViewStateAniImpl, StateAniViewAniStop)
ZF_NAMESPACE_END(ZFUIViewStateAniImpl)

// ============================================================
/**
 * @brief util type for impl to implement #ZFUIViewStateAniStart, use #ZFUIViewStateAniImplRegister to register it
 */
zfinterface ZF_ENV_EXPORT ZFUIViewStateAniImplProtocol : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFUIViewStateAniImplProtocol, ZFInterface)

public:
    /** @brief see #ZFUIViewStateAniImpl::EventStateAniOnInit */
    virtual void stateAniOnInit(void) {}
    /** @brief see #ZFUIViewStateAniImpl::EventStateAniOnDealloc */
    virtual void stateAniOnDealloc(void) {}
    /** @brief see #ZFUIViewStateAniImpl::EventStateAniViewAttach */
    virtual void stateAniViewAttach(ZF_IN ZFUIView *view) {}
    /** @brief see #ZFUIViewStateAniImpl::EventStateAniViewDetach */
    virtual void stateAniViewDetach(ZF_IN ZFUIView *view) {}
    /** @brief see #ZFUIViewStateAniImpl::EventStateAniViewAniPrepare */
    virtual void stateAniViewAniPrepare(ZF_IN ZFUIView *view, ZF_IN_OUT ZFArrayEditable *aniList) {}
    /** @brief see #ZFUIViewStateAniImpl::EventStateAniViewAniStart */
    virtual void stateAniViewAniStart(ZF_IN ZFUIView *view, ZF_IN ZFAnimation *ani) {}
    /** @brief see #ZFUIViewStateAniImpl::EventStateAniViewAniStop */
    virtual void stateAniViewAniStop(ZF_IN ZFUIView *view, ZF_IN ZFAnimation *ani) {}
};

/**
 * @brief util method to register impl for #ZFUIViewStateAniStart,
 *   use #ZF_GLOBAL_INITIALIZER_INIT is recommended,
 *   remember to unregister by #ZFUIViewStateAniImplUnregister
 */
extern ZF_ENV_EXPORT void ZFUIViewStateAniImplRegister(ZF_IN ZFUIViewStateAniImplProtocol *impl);
/** @brief see #ZFUIViewStateAniImplRegister */
extern ZF_ENV_EXPORT void ZFUIViewStateAniImplUnregister(ZF_IN ZFUIViewStateAniImplProtocol *impl);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewStateAni_h_

