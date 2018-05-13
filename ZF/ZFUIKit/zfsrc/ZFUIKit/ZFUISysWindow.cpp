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

zfclassNotPOD _ZFP_ZFUISysWindowPrivate
{
public:
    ZFUISysWindowEmbedImpl *embedImpl;
    void *nativeWindow;
    ZFUIRootView *windowRootView;
    ZFUIOrientationFlags sysWindowOrientationFlags;
    ZFUISysWindow *modalWindowOwner;
    ZFUISysWindow *modalWindowShowing;
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
    ret->d->embedImpl = embedImpl;
    embedImpl->_ZFP_ownerZFUISysWindow = ret;
    embedImpl->sysWindowLayoutParamOnInit(ret);
    return tmp;
}
ZFUISysWindowEmbedImpl *ZFUISysWindow::nativeWindowEmbedImpl(void)
{
    return d->embedImpl;
}

static ZFUISysWindow *_ZFP_ZFUISysWindow_mainWindowRegistered = zfnull;
static ZFUISysWindow *_ZFP_ZFUISysWindow_mainWindow = zfnull;
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
    _ZFP_ZFUISysWindow_mainWindowRegistered = zfnull;
    _ZFP_ZFUISysWindow_mainWindow = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFUISysWindowMainWindowCleanup)
void ZFUISysWindow::mainWindowRegister(ZF_IN ZFUISysWindow *window)
{
    zfCoreAssertWithMessage(_ZFP_ZFUISysWindow_mainWindow == zfnull,
        zfTextA("mainWindowRegister must be called before accessing mainWindow"));
    _ZFP_ZFUISysWindow_mainWindowRegistered = window;
}

ZFMETHOD_DEFINE_0(ZFUISysWindow, ZFUISysWindow *, mainWindow)
{
    if(_ZFP_ZFUISysWindow_mainWindow == zfnull)
    {
        if(_ZFP_ZFUISysWindow_mainWindowRegistered != zfnull)
        {
            _ZFP_ZFUISysWindow_mainWindow = _ZFP_ZFUISysWindow_mainWindowRegistered;
        }
        else
        {
            _ZFP_ZFUISysWindow_mainWindow = ZFPROTOCOL_ACCESS(ZFUISysWindow)->mainWindow();
        }
    }
    return _ZFP_ZFUISysWindow_mainWindow;
}

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
        ZFPointerHolder *param0 = ZFPointerHolder::cacheGet();
        param0->holdedData = &sysWindowMarginOld;
        this->observerNotify(ZFUISysWindow::EventSysWindowMarginOnUpdate(), param0);
        ZFPointerHolder::cacheAdd(param0);
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
    zfCoreAssertWithMessage(d->modalWindowShowing == zfnull, zfTextA("already has a showing modal window, you must finish it first"));

    ZFUISysWindow *modalWindow = zfnull;
    if(d->embedImpl != zfnull)
    {
        modalWindow = d->embedImpl->modalWindowShow(this);
    }
    else
    {
        zfCoreAssertWithMessage(this->nativeWindow() != zfnull,
            zfTextA("you can only create modal window after ZFUISysWindow created, see ZFUISysWindow::nativeWindowIsCreated"));

        modalWindow = ZFPROTOCOL_ACCESS(ZFUISysWindow)->modalWindowShow(this);
    }

    d->modalWindowShowing = modalWindow;
    modalWindow->d->modalWindowOwner = this;
    return modalWindow;
}
ZFMETHOD_DEFINE_0(ZFUISysWindow, void, modalWindowFinish)
{
    zfCoreAssertWithMessage(d->modalWindowOwner != zfnull, zfTextA("not a modal window"));

    if(d->embedImpl != zfnull)
    {
        d->embedImpl->modalWindowFinish(d->modalWindowOwner, this);
    }
    else
    {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->modalWindowFinish(d->modalWindowOwner, this);
    }
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
    ZFUIRect ret = ZFUIRectApplyScale(ZFUIViewLayoutParam::layoutParamApply(
            ZFUIRectApplyScaleReversely(rootRefRect, this->rootView()->scaleFixed()),
            this->rootView(),
            this->sysWindowLayoutParam()),
        this->rootView()->scaleFixed());
    return ret;
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onCreate(ZF_IN void *nativeWindow)
{
    zfCoreAssertWithMessage(!d->nativeWindowCreated, zfTextA("window already created"));

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

    if(this == _ZFP_ZFUISysWindow_mainWindow)
    {
        ZFPROTOCOL_ACCESS(ZFUISysWindow)->mainWindowOnDestroy();
    }
}
void ZFUISysWindow::_ZFP_ZFUISysWindow_onResume(void)
{
    zfCoreAssertWithMessage(d->nativeWindowCreated, zfTextA("window not created"));
    zfCoreAssertWithMessage(!d->nativeWindowResumed, zfTextA("window already resumed"));

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
    zfCoreAssertWithMessage(d->nativeWindowCreated, zfTextA("window not created"));
    zfCoreAssertWithMessage(d->nativeWindowResumed, zfTextA("window not resumed"));
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

ZFUISysWindow *ZFUISysWindowEmbedImpl::modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner)
{
    return ZFPROTOCOL_ACCESS(ZFUISysWindow)->modalWindowShow(sysWindowOwner);
}
void ZFUISysWindowEmbedImpl::modalWindowFinish(ZF_IN ZFUISysWindow *sysWindowOwner,
                                               ZF_IN ZFUISysWindow *sysWindowToFinish)
{
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->modalWindowFinish(sysWindowOwner, sysWindowToFinish);
}

void ZFUISysWindowEmbedImpl::sysWindowLayoutParamOnInit(ZF_IN ZFUISysWindow *sysWindow)
{
}
void ZFUISysWindowEmbedImpl::sysWindowLayoutParamOnChange(ZF_IN ZFUISysWindow *sysWindow)
{
}

ZFUIOrientationEnum ZFUISysWindowEmbedImpl::sysWindowOrientation(ZF_IN ZFUISysWindow *sysWindow)
{
    return ZFUIOrientation::e_Top;
}
void ZFUISysWindowEmbedImpl::sysWindowOrientationFlagsSet(ZF_IN ZFUISysWindow *sysWindow,
                                                          ZF_IN const ZFUIOrientationFlags &flags)
{
}

ZF_NAMESPACE_GLOBAL_END

