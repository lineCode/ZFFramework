/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIWindow.h"
#include "ZFUIRootView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIWindowLevel)

ZFSTYLE_DEFAULT_DEFINE(ZFUIWindow)

// ============================================================
// _ZFP_ZFUIWindowPrivate
zfclassNotPOD _ZFP_ZFUIWindowPrivate
{
public:
    ZFUISysWindow *windowOwnerSysWindow;
    ZFUIViewLayoutParam *windowLayoutParam;
    zfbool windowRemoveOverrideFlag;

public:
    _ZFP_ZFUIWindowPrivate(void)
    : windowOwnerSysWindow(zfnull)
    , windowLayoutParam(zfnull)
    , windowRemoveOverrideFlag(zffalse)
    {
    }
};

// ============================================================
// ZFUIWindow
ZFOBJECT_REGISTER(ZFUIWindow)

ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOnShow)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOnHide)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnPause)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnRotate)

ZFMETHOD_DEFINE_1(ZFUIWindow, ZFUIWindow *, windowForView,
                  ZFMP_IN(ZFUIView *, forView))
{
    while(forView != zfnull && !forView->classData()->classIsTypeOf(ZFUIWindow::ClassData()))
    {
        forView = forView->viewParent();
    }
    if(forView != zfnull)
    {
        return ZFCastZFObject(ZFUIWindow *, forView);
    }
    return zfnull;
}

ZFMETHOD_DEFINE_1(ZFUIWindow, ZFUISysWindow *, sysWindowForView,
                  ZFMP_IN(ZFUIView *, view))
{
    ZFUIWindow *window = ZFUIWindow::windowForView(view);
    return ((window != zfnull) ? window->windowOwnerSysWindow() : zfnull);
}

void ZFUIWindow::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIWindowPrivate);
    d->windowOwnerSysWindow = ZFUISysWindow::mainWindow();
    d->windowLayoutParam = zfAlloc(ZFUIViewLayoutParam);
    d->windowLayoutParam->sizeParamSet(ZFUISizeParamFillFill());
}
void ZFUIWindow::objectOnDealloc(void)
{
    zfRelease(d->windowLayoutParam);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

// ============================================================
// properties
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFUIWindow, ZFUIWindowLevelEnum, windowLevel)
{
    zfCoreAssertWithMessage(!this->windowShowing(), zfTextA("you must not change window level while it's showing"));
}

ZFMETHOD_DEFINE_1(ZFUIWindow, void, windowOwnerSysWindowSet,
                  ZFMP_IN(ZFUISysWindow *, windowOwnerSysWindow))
{
    if(d->windowOwnerSysWindow != windowOwnerSysWindow)
    {
        zfCoreAssertWithMessage(!this->windowShowing(), zfTextA("you must not change window's owner while it's showing"));
        zfCoreAssertWithMessage(windowOwnerSysWindow != zfnull, zfTextA("null owner sys window"));

        ZFUISysWindow *oldSysWindow = d->windowOwnerSysWindow;
        d->windowOwnerSysWindow = windowOwnerSysWindow;
        this->windowOwnerSysWindowOnChange(oldSysWindow);
    }
}
ZFMETHOD_DEFINE_0(ZFUIWindow, ZFUISysWindow *, windowOwnerSysWindow)
{
    return d->windowOwnerSysWindow;
}

ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowShow)
{
    zfindex addToIndex = 0;
    ZFCoreArrayPOD<ZFUIView *> tmpArray = this->windowOwnerSysWindow()->rootView()->childArray();
    for(zfindex i = 0; i < tmpArray.count(); ++i)
    {
        ZFUIWindow *tmpWindow = ZFCastZFObject(ZFUIWindow *, tmpArray.get(i));
        if(tmpWindow != zfnull)
        {
            if(tmpWindow->windowLevel() <= this->windowLevel())
            {
                addToIndex = i + 1;
            }
            else
            {
                break;
            }
        }
    }
    this->windowOwnerSysWindow()->rootView()->childAdd(this, d->windowLayoutParam, addToIndex);
}
ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowHide)
{
    zfRetain(this);
    d->windowRemoveOverrideFlag = zftrue;
    this->viewRemoveFromParent();
    d->windowRemoveOverrideFlag = zffalse;
    zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFUIWindow, zfbool, windowShowing)
{
    return (this->viewParent() != zfnull);
}

ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowMoveToTop)
{
    if(!this->windowShowing())
    {
        return ;
    }

    ZFCoreArrayPOD<ZFUIView *> tmpArray = this->windowOwnerSysWindow()->rootView()->childArray();
    ZFUIWindowLevelEnum selfWindowLevel = this->windowLevel();
    zfindex topIndex = zfindexMax();
    zfindex selfIndex = zfindexMax();
    for(zfindex i = tmpArray.count() - 1; i != zfindexMax(); --i)
    {
        ZFUIWindow *tmpWindow = ZFCastZFObject(ZFUIWindow *, tmpArray.get(i));
        if(tmpWindow != zfnull)
        {
            if(topIndex == zfindexMax() && tmpWindow->windowLevel() == selfWindowLevel)
            {
                topIndex = i;
            }
            if(tmpWindow == this)
            {
                selfIndex = i;
                break;
            }
        }
    }
    if(topIndex != selfIndex && topIndex != zfindexMax() && selfIndex != zfindexMax())
    {
        this->windowOwnerSysWindow()->rootView()->childMove(selfIndex, topIndex);
    }
}
ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowMoveToBottom)
{
    if(!this->windowShowing())
    {
        return ;
    }

    ZFCoreArrayPOD<ZFUIView *> tmpArray = this->windowOwnerSysWindow()->rootView()->childArray();
    ZFUIWindowLevelEnum selfWindowLevel = this->windowLevel();
    zfindex bottomIndex = zfindexMax();
    zfindex selfIndex = zfindexMax();
    for(zfindex i = 0; i < tmpArray.count(); ++i)
    {
        ZFUIWindow *tmpWindow = ZFCastZFObject(ZFUIWindow *, tmpArray.get(i));
        if(tmpWindow != zfnull)
        {
            if(bottomIndex == zfindexMax() && tmpWindow->windowLevel() == selfWindowLevel)
            {
                bottomIndex = i;
            }
            if(tmpWindow == this)
            {
                selfIndex = i;
                break;
            }
        }
    }
    if(bottomIndex != selfIndex && bottomIndex != zfindexMax() && selfIndex != zfindexMax())
    {
        this->windowOwnerSysWindow()->rootView()->childMove(selfIndex, bottomIndex);
    }
}

ZFMETHOD_DEFINE_0(ZFUIWindow, ZFUIViewLayoutParam *, windowLayoutParam)
{
    return d->windowLayoutParam;
}

void ZFUIWindow::viewOnAddToParent(ZF_IN ZFUIView *parent)
{
    zfCoreAssertWithMessage(parent->classData()->classIsTypeOf(ZFUIRootView::ClassData()), zfTextA("you must not add a window to another view"));
    zfsuper::viewOnAddToParent(parent);

    this->windowOnShow();
}
void ZFUIWindow::viewOnRemoveFromParent(ZF_IN ZFUIView *parent)
{
    // should not check remove here,
    // since it's all right to remove this window when owner sys window deallocated or when hide window
    zfsuper::viewOnRemoveFromParent(parent);

    if(d->windowRemoveOverrideFlag)
    {
        this->windowOnHide();
    }
    else
    {
        this->windowHide();
    }
}

ZF_NAMESPACE_GLOBAL_END

