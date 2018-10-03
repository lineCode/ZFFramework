/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUISysWindow.h"
#include "protocol/ZFProtocolZFUISysWindow.h"
#include "ZFUIRootView.h"
#include "ZFUIWindow.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(SysWindowMainWindowOnAttach)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
zfclassNotPOD _ZFP_ZFUISysWindowPrivate
{
public:
    ZFUISysWindowEmbedImpl *embedImpl; // auto retain
    void *nativeWindow;
    ZFUIRootView *windowRootView;
    ZFUIOrientationFlags sysWindowOrientationFlags;
    ZFUISysWindow *modalWindowOwner;
    ZFUISysWindow *modalWindowShowing; // manually retain
    ZFUIViewLayoutParam *sysWindowLayoutParam;
    ZFListener sysWindowLayoutParamOnChangeListener;
    zfbool nativeWindowCreated;
    zfbool nativeWindowResumed;
    ZFUIMargin sysWindowMargin;

public:
    _ZFP_ZFUISysWindowPrivate(void)
    : embedImpl(zfnull)
    , nativeWindow(zfnull)
    , windowRootView(zfnull)
    , sysWindowOrientationFlags(ZFUIOrientation::e_Top)
    , modalWindowOwner(zfnull)
    , modalWindowShowing(zfnull)
    , sysWindowLayoutParam(zfnull)
    , sysWindowLayoutParamOnChangeListener(ZFCallbackForFunc(_ZFP_ZFUISysWindowPrivate::sysWindowLayoutParamOnChange))
    , nativeWindowCreated(zffalse)
    , nativeWindowResumed(zffalse)
    , sysWindowMargin()
    {
    }
    ~_ZFP_ZFUISysWindowPrivate(void)
    {
        zfRelease(this->embedImpl);
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(sysWindowLayoutParamOnChange)
    {
        ZFUISysWindow *sysWindow = userData->objectHolded();
        if(sysWindow->nativeWindowEmbedImpl() != zfnull)
        {
            sysWindow->nativeWindowEmbedImpl()->sysWindowLayoutParamOnChange(sysWindow);
        }
        else
        {
            ZFPROTOCOL_ACCESS(ZFUISysWindow)->sysWindowLayoutParamOnChange(sysWindow);
        }
    }
};

ZFOBJECT_REGISTER(ZFUISysWindow)

ZFOBSERVER_EVENT_REGISTER(ZFUISysWindow, SysWindowOnCreate)
ZFOBSERVER_EVENT_REGISTER(ZFUISysWindow, SysWindowOnDestroy)
ZFOBSERVER_EVENT_REGISTER(ZFUISysWindow, SysWindowOnResume)
ZFOBSERVER_EVENT_REGISTER(ZFUISysWindow, SysWindowOnPause)
ZFOBSERVER_EVENT_REGISTER(ZFUISysWindow, SysWindowOnRotate)
ZFOBSERVER_EVENT_REGISTER(ZFUISysWindow, SysWindowMarginOnUpdate)

zfautoObject ZFUISysWindow::nativeWindowEmbed(ZF_IN ZFUISysWindowEmbedImpl *embedImpl)
{
    zfautoObject tmp = ZFUISysWindow::ClassData()->newInstance();
    ZFUISysWindow *ret = tmp.to<ZFUISysWindow *>();
    ret->d->embedImpl = zfRetain(embedImpl);
    embedImpl->_ZFP_ownerZFUISysWindow = ret;
    embedImpl->sysWindowLayoutParamOnInit(ret);
    return tmp;
}
ZFUISysWindowEmbedImpl *ZFUISysWindow::nativeWindowEmbedImpl(void)
{
    return d->embedImpl;
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, void, nativeWindowEmbedImplDestroy)
{
    if(this->nativeWindowEmbedImpl() != zfnull)
    {
        if(this->nativeWindowIsResumed())
        {
            this->nativeWindowEmbedImpl()->notifyOnPause(this);
        }
        if(this->nativeWindowIsCreated())
        {
            this->nativeWindowEmbedImpl()->notifyOnDestroy(this);
        }
    }
}

// ============================================================
static ZFUISysWindow *_ZFP_ZFUISysWindow_mainWindowRegistered = zfnull;
static ZFUISysWindow *_ZFP_ZFUISysWindow_mainWindowBuiltin = zfnull;
static ZFUISysWindow *_ZFP_ZFUISysWindow_mainWindow = zfnull;
static ZFObjectHolder *_ZFP_ZFUISysWindow_keyWindow = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUISysWindowMainWindowCleanup, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUISysWindowMainWindowCleanup)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUISysWindow) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUISysWindow);
    if(impl != zfnull)
    {
        impl->mainWindowOnCleanup();
    }
    zfblockedRelease(_ZFP_ZFUISysWindow_mainWindowRegistered);
    zfblockedRelease(_ZFP_ZFUISysWindow_keyWindow);

    _ZFP_ZFUISysWindow_mainWindowRegistered = zfnull;
    _ZFP_ZFUISysWindow_mainWindowBuiltin = zfnull;
    _ZFP_ZFUISysWindow_mainWindow = zfnull;
    _ZFP_ZFUISysWindow_keyWindow = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFUISysWindowMainWindowCleanup)

void ZFUISysWindow::mainWindowRegister(ZF_IN ZFUISysWindow *window)
{
    zfCoreAssertWithMessage(_ZFP_ZFUISysWindow_mainWindow == zfnull,
        "[ZFUISysWindow] mainWindowRegister must be called before accessing mainWindow");
    zfCoreAssertWithMessage(window != zfnull,
        "[ZFUISysWindow] mainWindowRegister called with null window");

    zfblockedRelease(_ZFP_ZFUISysWindow_mainWindowRegistered);
    _ZFP_ZFUISysWindow_mainWindowRegistered = zfRetain(window);
    _ZFP_ZFUISysWindow_mainWindow = _ZFP_ZFUISysWindow_mainWindowRegistered;

    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventSysWindowMainWindowOnAttach());
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, ZFUISysWindow *, mainWindow)
{
    if(_ZFP_ZFUISysWindow_mainWindow == zfnull)
    {
        _ZFP_ZFUISysWindow_mainWindowBuiltin = ZFPROTOCOL_ACCESS(ZFUISysWindow)->mainWindow();
        _ZFP_ZFUISysWindow_mainWindow = _ZFP_ZFUISysWindow_mainWindowBuiltin;
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventSysWindowMainWindowOnAttach());
    }
    return _ZFP_ZFUISysWindow_mainWindow;
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, zfbool, mainWindowAttached)
{
    return (_ZFP_ZFUISysWindow_mainWindow != zfnull);
}

ZFMETHOD_DEFINE_1(ZFUISysWindow, void, keyWindowSet,
                  ZFMP_IN(ZFUISysWindow *, window))
{
    if(window == zfnull)
    {
        zfRetainChange(_ZFP_ZFUISysWindow_keyWindow, zfnull);
    }
    else
    {
        zfRetainChange(_ZFP_ZFUISysWindow_keyWindow, window->objectHolder());
    }
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, ZFUISysWindow *, keyWindow)
{
    ZFUISysWindow *ret = zfnull;
    if(_ZFP_ZFUISysWindow_keyWindow != zfnull)
    {
        ret = _ZFP_ZFUISysWindow_keyWindow->objectHolded();
    }
    if(ret != zfnull)
    {
        return ret;
    }
    else
    {
        return ZFUISysWindow::mainWindow();
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFUISysWindow, const ZFUIMargin &, sysWindowMargin)
{
    return d->sysWindowMargin;
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_sysWindowMarginSet(ZF_IN const ZFUIMargin &sysWindowMargin)
{
    ZFUIMargin sysWindowMarginOld = d->sysWindowMargin;
    d->sysWindowMargin = ZFUIMarginApplyScaleReversely(sysWindowMargin, this->rootView()->scaleFixed());
    if(d->sysWindowMargin != sysWindowMarginOld)
    {
        this->sysWindowMarginOnUpdate(sysWindowMarginOld);
    }
}
void ZFUISysWindow::sysWindowMarginOnUpdate(ZF_IN const ZFUIMargin &sysWindowMarginOld)
{
    if(this->observerHasAdd(ZFUISysWindow::EventSysWindowMarginOnUpdate()))
    {
        this->observerNotify(ZFUISysWindow::EventSysWindowMarginOnUpdate(), zflineAlloc(v_ZFUIMargin, sysWindowMarginOld));
    }
}

void ZFUISysWindow::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUISysWindowPrivate);
    d->windowRootView = zfRetain(ZFUIRootView::ClassData()->newInstance().to<ZFUIRootView *>());
    d->sysWindowLayoutParam = zfAlloc(ZFUIViewLayoutParam);
    d->sysWindowLayoutParam->sizeParamSet(ZFUISizeParamFillFill());
}
void ZFUISysWindow::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->sysWindowLayoutParamOnInit(this);
    d->sysWindowLayoutParam->observerAdd(ZFUILayoutParam::EventLayoutParamOnChange(), d->sysWindowLayoutParamOnChangeListener, this->objectHolder());
}
void ZFUISysWindow::objectOnDeallocPrepare(void)
{
    if(d->nativeWindowResumed)
    {
        if(d->embedImpl != zfnull)
        {
            d->embedImpl->notifyOnPause(this);
        }
        else
        {
            ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnPause(this);
        }
    }
    if(d->nativeWindowCreated)
    {
        if(d->embedImpl != zfnull)
        {
            d->embedImpl->notifyOnDestroy(this);
        }
        else
        {
            ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnDestroy(this);
        }
    }

    if(d->embedImpl != zfnull)
    {
        d->embedImpl->nativeWindowOnCleanup(this);
    }
    else
    {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->nativeWindowOnCleanup(this);
    }
    zfsuper::objectOnDeallocPrepare();
}
void ZFUISysWindow::objectOnDealloc(void)
{
    zfRelease(d->sysWindowLayoutParam);
    zfRelease(d->windowRootView);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, void *, nativeWindow)
{
    return d->nativeWindow;
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, zfbool, nativeWindowIsCreated)
{
    return d->nativeWindowCreated;
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, zfbool, nativeWindowIsResumed)
{
    return d->nativeWindowResumed;
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, ZFUIOrientationEnum, sysWindowOrientation)
{
    if(d->nativeWindowCreated)
    {
        if(d->embedImpl != zfnull)
        {
            return d->embedImpl->sysWindowOrientation(this);
        }
        else
        {
            return ZFPROTOCOL_ACCESS(ZFUISysWindow)->sysWindowOrientation(this);
        }
    }
    else
    {
        return ZFUIOrientation::e_Top;
    }
}
ZFMETHOD_DEFINE_1(ZFUISysWindow, void, sysWindowOrientationFlagsSet,
                  ZFMP_IN(const ZFUIOrientationFlags &, sysWindowOrientationFlags))
{
    zfuint tmp = 0;
    if(ZFBitTest(sysWindowOrientationFlags.enumValue(), ZFUIOrientation::e_Left))
    {
        ZFBitSet(tmp, ZFUIOrientation::e_Left);
    }
    if(ZFBitTest(sysWindowOrientationFlags.enumValue(), ZFUIOrientation::e_Top))
    {
        ZFBitSet(tmp, ZFUIOrientation::e_Top);
    }
    if(ZFBitTest(sysWindowOrientationFlags.enumValue(), ZFUIOrientation::e_Right))
    {
        ZFBitSet(tmp, ZFUIOrientation::e_Right);
    }
    if(ZFBitTest(sysWindowOrientationFlags.enumValue(), ZFUIOrientation::e_Bottom))
    {
        ZFBitSet(tmp, ZFUIOrientation::e_Bottom);
    }
    if(d->sysWindowOrientationFlags != tmp)
    {
        d->sysWindowOrientationFlags = tmp;
        if(d->embedImpl != zfnull)
        {
            d->embedImpl->sysWindowOrientationFlagsSet(this, tmp);
        }
        else
        {
            if(this->nativeWindow() != zfnull)
            {
                ZFPROTOCOL_ACCESS(ZFUISysWindow)->sysWindowOrientationFlagsSet(this, tmp);
            }
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, const ZFUIOrientationFlags &, sysWindowOrientationFlags)
{
    return d->sysWindowOrientationFlags;
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, ZFUISysWindow *, modalWindowShow)
{
    zfCoreAssertWithMessage(d->modalWindowShowing == zfnull, "already has a showing modal window, you must finish it first");

    zfautoObject modalWindow;
    if(d->embedImpl != zfnull)
    {
        modalWindow = d->embedImpl->modalWindowShow(this);
    }
    else
    {
        zfCoreAssertWithMessage(this->nativeWindow() != zfnull,
            "you can only create modal window after ZFUISysWindow created, see ZFUISysWindow::nativeWindowIsCreated");

        modalWindow = ZFPROTOCOL_ACCESS(ZFUISysWindow)->modalWindowShow(this);
    }

    d->modalWindowShowing = zfRetain(modalWindow);
    d->modalWindowShowing->d->modalWindowOwner = this;
    return modalWindow;
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, void, modalWindowFinish)
{
    zfCoreAssertWithMessage(d->modalWindowOwner != zfnull, "not a modal window");

    if(d->embedImpl != zfnull)
    {
        d->embedImpl->modalWindowFinish(d->modalWindowOwner, this);
    }
    else
    {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->modalWindowFinish(d->modalWindowOwner, this);
    }
    zfblockedRelease(d->modalWindowOwner->d->modalWindowShowing);
    d->modalWindowOwner->d->modalWindowShowing = zfnull;
    d->modalWindowOwner = zfnull;

    zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, ZFUISysWindow *, modalWindowGetShowing)
{
    return d->modalWindowShowing;
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, ZFUISysWindow *, modalWindowGetOwner)
{
    return d->modalWindowOwner;
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, ZFUIViewLayoutParam *, sysWindowLayoutParam)
{
    return d->sysWindowLayoutParam;
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, ZFUIRootView *, rootView)
{
    return d->windowRootView;
}

ZFUIRect ZFUISysWindow::_ZFP_ZFUISysWindow_measureWindow(ZF_IN const ZFUIRect &rootRefRect)
{
    return ZFUIRectApplyScale(ZFUIViewLayoutParam::layoutParamApply(
            ZFUIRectApplyScaleReversely(rootRefRect, this->rootView()->scaleFixed()),
            this->rootView(),
            this->sysWindowLayoutParam()),
        this->rootView()->scaleFixed());
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onCreate(ZF_IN void *nativeWindow)
{
    zfCoreAssertWithMessage(!d->nativeWindowCreated, "window already created");

    d->nativeWindow = nativeWindow;
    d->nativeWindowCreated = zftrue;

    void *nativeParentView = zfnull;
    if(d->embedImpl != zfnull)
    {
        d->embedImpl->sysWindowOrientationFlagsSet(this, d->sysWindowOrientationFlags);
        d->embedImpl->nativeWindowRootViewOnAdd(this, nativeParentView);
    }
    else
    {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->sysWindowOrientationFlagsSet(this, d->sysWindowOrientationFlags);
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->nativeWindowRootViewOnAdd(this, nativeParentView);
    }
    ZFUIView::_ZFP_ZFUIView_nativeViewNotifyAdd(this->rootView(), nativeParentView);

    this->observerNotify(ZFUISysWindow::EventSysWindowOnCreate());
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onDestroy(void)
{
    if(d->nativeWindowResumed)
    {
        this->_ZFP_ZFUISysWindow_onPause();
    }
    if(!d->nativeWindowCreated)
    {
        return ;
    }
    d->nativeWindowCreated = zffalse;

    if(d->embedImpl != zfnull)
    {
        d->embedImpl->nativeWindowRootViewOnRemove(this);
    }
    else
    {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->nativeWindowRootViewOnRemove(this);
    }

    ZFUIView::_ZFP_ZFUIView_nativeViewNotifyRemove(this->rootView());

    this->observerNotify(ZFUISysWindow::EventSysWindowOnDestroy());

    if(this == _ZFP_ZFUISysWindow_mainWindowBuiltin)
    {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->mainWindowOnDestroy();
    }
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onResume(void)
{
    zfCoreAssertWithMessage(d->nativeWindowCreated, "window not created");
    zfCoreAssertWithMessage(!d->nativeWindowResumed, "window already resumed");

    d->nativeWindowResumed = zftrue;
    this->observerNotify(ZFUISysWindow::EventSysWindowOnResume());

    for(zfindex i = this->rootView()->childCount() - 1; i != zfindexMax(); --i)
    {
        ZFUIWindow *window = ZFCastZFObject(ZFUIWindow *, this->rootView()->childAtIndex(i));
        if(window != zfnull)
        {
            window->windowOwnerSysWindowOnResume();
        }
    }
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onPause(void)
{
    if(!d->nativeWindowResumed)
    {
        return ;
    }

    d->nativeWindowResumed = zffalse;
    this->observerNotify(ZFUISysWindow::EventSysWindowOnPause());

    for(zfindex i = this->rootView()->childCount() - 1; i != zfindexMax(); --i)
    {
        ZFUIWindow *window = ZFCastZFObject(ZFUIWindow *, this->rootView()->childAtIndex(i));
        if(window != zfnull)
        {
            window->windowOwnerSysWindowOnPause();
        }
    }
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onRotate(void)
{
    zfCoreAssertWithMessage(d->nativeWindowCreated, "window not created");
    zfCoreAssertWithMessage(d->nativeWindowResumed, "window not resumed");
    this->observerNotify(ZFUISysWindow::EventSysWindowOnRotate());

    for(zfindex i = this->rootView()->childCount() - 1; i != zfindexMax(); --i)
    {
        ZFUIWindow *window = ZFCastZFObject(ZFUIWindow *, this->rootView()->childAtIndex(i));
        if(window != zfnull)
        {
            window->windowOwnerSysWindowOnRotate();
        }
    }
}

// ============================================================
ZFOBJECT_REGISTER(ZFUISysWindowEmbedImpl)

void ZFUISysWindowEmbedImpl::nativeWindowRootViewOnAdd(ZF_IN ZFUISysWindow *sysWindow,
                                                       ZF_OUT void *&nativeParentView)
{
}
void ZFUISysWindowEmbedImpl::nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow)
{
}

zfautoObject ZFUISysWindowEmbedImpl::modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner)
{
    return ZFPROTOCOL_ACCESS(ZFUISysWindow)->modalWindowShow(sysWindowOwner);
}
void ZFUISysWindowEmbedImpl::modalWindowFinish(ZF_IN ZFUISysWindow *sysWindowOwner,
                                               ZF_IN ZFUISysWindow *sysWindowToFinish)
{
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->modalWindowFinish(sysWindowOwner, sysWindowToFinish);
}

ZF_NAMESPACE_GLOBAL_END

