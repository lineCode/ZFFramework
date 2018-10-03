/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIOnScreenKeyboardAutoFit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG 0

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIOnScreenKeyboardAutoFitLayout)

// ============================================================
// scroll view
zfclass _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutScrollView : zfextends ZFUIScrollView
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIOnScreenKeyboardAutoFitLayoutScrollView, ZFUIScrollView)

protected:
    // disable scroll thumb
    zfoverride
    virtual void scrollThumbHorizontalOnInit(void)
    {
    }
    zfoverride
    virtual void scrollThumbVerticalOnInit(void)
    {
    }
};

// ============================================================
// _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate
zfclassNotPOD _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate
{
public:
    ZFUIOnScreenKeyboardAutoFitLayout *pimplOwner;
    ZFUIScrollView *scrollView;
    zfbool scrollEnableFlag;
    ZFUIView *autoFitFocusedView;
    ZFUIMargin autoFitMargin;
    ZFListener onScreenKeyboardStateOnChangeListener;
    ZFListener viewFocusOnChangeListener;
    ZFListener scrollFocusedViewToVisibleDelayListener;
    zfbool autoFitFocusedViewNeedUpdate;

public:
    void autoFitEnableSet(ZF_IN zfbool value)
    {
        if(value)
        {
            ZFObjectGlobalEventObserver().observerAdd(
                ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(),
                this->onScreenKeyboardStateOnChangeListener,
                this->pimplOwner->objectHolder());

            this->pimplOwner->layoutRequest();
        }
        else
        {
            this->scrollEnableFlag = zffalse;
            this->scrollView->scrollEnableSet(this->scrollEnableFlag && this->pimplOwner->autoFitScrollEnable());
            ZFObjectGlobalEventObserver().observerRemove(
                ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(),
                this->onScreenKeyboardStateOnChangeListener);

            this->scrollView->scrollContentFrameSet(ZFUIRectGetBounds(this->pimplOwner->layoutedFrame()));
            if(this->autoFitMargin == ZFUIMarginZero() || this->pimplOwner->layoutParam() == zfnull)
            {
                return ;
            }
            this->autoFitMargin = ZFUIMarginZero();
            #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
                zfLogTrimT() << "[ZFUIOnScreenKeyboardAutoFitLayout] margin changed to" << this->autoFitMargin;
            #endif
            this->scrollView->layoutParam()->layoutMarginSet(this->autoFitMargin);
        }
    }
    void autoFitFocusedViewToVisibleSet(ZF_IN zfbool value)
    {
        if(value)
        {
            ZFObjectGlobalEventObserver().observerAdd(
                ZFUIView::EventViewFocusOnChange(),
                this->viewFocusOnChangeListener,
                this->pimplOwner->objectHolder());
        }
        else
        {
            ZFObjectGlobalEventObserver().observerRemove(
                ZFUIView::EventViewFocusOnChange(),
                this->viewFocusOnChangeListener);
            #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
                zfLogTrimT() << "[ZFUIOnScreenKeyboardAutoFitLayout] focused view changed to" << (void *)zfnull;
            #endif
            this->autoFitFocusedView = zfnull;
        }
    }

public:
    void autoFitUpdateFrame(void)
    {
        if(this->pimplOwner->layoutParam() == zfnull)
        {
            return ;
        }

        ZFUIRect orgRect = ZFUIViewPositionOnScreen(this->pimplOwner);
        if(orgRect.size != this->pimplOwner->layoutedFrame().size)
        {
            return ;
        }

        this->autoFitMarginCalc(this->autoFitMargin, orgRect, ZFUIOnScreenKeyboardState::instanceForView(this->pimplOwner));
        #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
            zfLogTrimT() << "[ZFUIOnScreenKeyboardAutoFitLayout] margin changed to" << this->autoFitMargin;
        #endif
        this->scrollView->layoutParam()->layoutMarginSet(this->autoFitMargin);
    }
    void autoFitMarginCalc(ZF_OUT ZFUIMargin &ret,
                           ZF_IN const ZFUIRect &orgRect,
                           ZF_IN ZFUIOnScreenKeyboardState *keyboardState)
    {
        ret = ZFUIMarginZero();
        if(keyboardState->keyboardShowing())
        {
            ZFUIRect clientFrame = keyboardState->keyboardFixClientFrame();
            if(ZFUIRectGetLeft(clientFrame) > ZFUIRectGetLeft(orgRect))
            {
                ret.left += ZFUIRectGetLeft(clientFrame) - ZFUIRectGetLeft(orgRect);
            }
            if(ZFUIRectGetTop(clientFrame) > ZFUIRectGetTop(orgRect))
            {
                ret.top += ZFUIRectGetTop(clientFrame) - ZFUIRectGetTop(orgRect);
            }
            if(ZFUIRectGetRight(clientFrame) < ZFUIRectGetRight(orgRect))
            {
                ret.right += ZFUIRectGetRight(orgRect) - ZFUIRectGetRight(clientFrame);
            }
            if(ZFUIRectGetBottom(clientFrame) < ZFUIRectGetBottom(orgRect))
            {
                ret.bottom += ZFUIRectGetBottom(orgRect) - ZFUIRectGetBottom(clientFrame);
            }
        }
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(onScreenKeyboardStateOnChange)
    {
        ZFUIOnScreenKeyboardState *keyboardState = listenerData.sender->to<ZFUIOnScreenKeyboardState *>();
        ZFUIOnScreenKeyboardAutoFitLayout *layout = userData->objectHolded();
        if(keyboardState->keyboardShowing())
        {
            if(layout->autoFitFocusedView() != zfnull)
            {
                layout->d->autoFitFocusedViewNeedUpdate = zftrue;
            }
            layout->d->scrollEnableFlag = zftrue;
            layout->d->scrollView->scrollEnableSet(layout->d->scrollEnableFlag && layout->autoFitScrollEnable());
            layout->layoutRequest();
        }
        else
        {
            layout->d->scrollEnableFlag = zffalse;
            layout->d->scrollView->scrollEnableSet(layout->d->scrollEnableFlag && layout->autoFitScrollEnable());
            #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
                zfLogTrimT() << "[ZFUIOnScreenKeyboardAutoFitLayout] margin changed to" << ZFUIMarginZero();
            #endif
            layout->d->scrollView->layoutParam()->layoutMarginSet(ZFUIMarginZero());
            layout->d->scrollView->scrollContentFrameSet(ZFUIRectGetBounds(layout->layoutedFrame()));
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(viewFocusOnChange)
    {
        ZFUIOnScreenKeyboardAutoFitLayout *layout = userData->objectHolded();
        ZFUIView *view = listenerData.sender->to<ZFUIView *>();
        if(!view->viewFocused())
        {
            if(view == layout->d->autoFitFocusedView)
            {
                layout->d->autoFitFocusedView = zfnull;
            }
            return ;
        }

        {
            ZFUIView *parent = view;
            while(parent != zfnull && parent != layout)
            {
                parent = parent->viewParentVirtual();
            }
            if(parent == zfnull)
            {
                return ;
            }
        }

        #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
            zfLogTrimT() << "[ZFUIOnScreenKeyboardAutoFitLayout] focused view changed to" << view;
        #endif
        layout->d->autoFitFocusedView = view;
        layout->d->autoFitFocusedViewNeedUpdate = zftrue;
        layout->layoutRequest();
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(scrollFocusedViewToVisibleDelay)
    {
        ZFUIOnScreenKeyboardAutoFitLayout *layout = userData->objectHolded();
        if(layout->autoFitFocusedView() != zfnull)
        {
            #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
                zfLogTrimT() << "[ZFUIOnScreenKeyboardAutoFitLayout] scrollFocusedViewToVisible" << layout->autoFitFocusedView();
            #endif
            layout->d->scrollView->scrollChildToVisible(layout->autoFitFocusedView());
        }
    }

public:
    _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate(void)
    : pimplOwner(zfnull)
    , scrollView(zfnull)
    , scrollEnableFlag(zffalse)
    , autoFitFocusedView(zfnull)
    , autoFitMargin(ZFUIMarginZero())
    , onScreenKeyboardStateOnChangeListener(ZFCallbackForFunc(_ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate::onScreenKeyboardStateOnChange))
    , viewFocusOnChangeListener(ZFCallbackForFunc(_ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate::viewFocusOnChange))
    , scrollFocusedViewToVisibleDelayListener(ZFCallbackForFunc(_ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate::scrollFocusedViewToVisibleDelay))
    , autoFitFocusedViewNeedUpdate(zffalse)
    {
    }
};

// ============================================================
// ZFUIOnScreenKeyboardAutoFitLayout
ZFOBJECT_REGISTER(ZFUIOnScreenKeyboardAutoFitLayout)

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIOnScreenKeyboardAutoFitLayout, zfbool, autoFitEnable)
{
    if(this->autoFitEnable() != propertyValueOld)
    {
        d->autoFitEnableSet(this->autoFitEnable());
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIOnScreenKeyboardAutoFitLayout, zfbool, autoFitFocusedViewToVisible)
{
    if(this->autoFitFocusedViewToVisible() != propertyValueOld)
    {
        d->autoFitFocusedViewToVisibleSet(this->autoFitFocusedViewToVisible());
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIOnScreenKeyboardAutoFitLayout, zfbool, autoFitScrollEnable)
{
    if(this->autoFitFocusedViewToVisible() != propertyValueOld)
    {
        d->scrollView->scrollEnableSet(d->scrollEnableFlag && this->autoFitScrollEnable());
    }
}

ZFUIView *ZFUIOnScreenKeyboardAutoFitLayout::autoFitFocusedView(void)
{
    return d->autoFitFocusedView;
}

void ZFUIOnScreenKeyboardAutoFitLayout::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate);
    d->pimplOwner = this;
    zfblockedAlloc(_ZFP_ZFUIOnScreenKeyboardAutoFitLayoutScrollView, scrollView);
    d->scrollView = scrollView;
    d->scrollEnableFlag = zffalse;
    d->scrollView->scrollEnableSet(d->scrollEnableFlag && this->autoFitScrollEnable());
    this->viewDelegateSet(d->scrollView);
}
void ZFUIOnScreenKeyboardAutoFitLayout::objectOnDealloc(void)
{
    ZFThreadTaskCancel(d->scrollFocusedViewToVisibleDelayListener);

    d->autoFitEnableSet(zffalse);
    d->autoFitFocusedViewToVisibleSet(zffalse);

    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIOnScreenKeyboardAutoFitLayout::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    d->autoFitEnableSet(this->autoFitEnable());
    d->autoFitFocusedViewToVisibleSet(this->autoFitFocusedViewToVisible());
}

void ZFUIOnScreenKeyboardAutoFitLayout::viewDelegateLayoutOnMeasure(ZF_OUT ZFUISize &ret,
                                                                    ZF_IN const ZFUISize &sizeHint,
                                                                    ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = ZFUISizeZero();
    ZFUISize sizeHintTmp = ZFUIViewLayoutParam::sizeHintOffset(sizeHint,
        ZFUISizeMake(ZFUIMarginGetWidth(d->autoFitMargin), ZFUIMarginGetHeight(d->autoFitMargin)));
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        ZFUIView *child = this->childAtIndex(i);
        child->layoutMeasure(sizeHintTmp, child->layoutParam()->sizeParam());
        ret.width = zfmMax(ret.width, child->layoutMeasuredSize().width + ZFUIMarginGetWidth(child->layoutParam()->layoutMargin()));
        ret.height = zfmMax(ret.height, child->layoutMeasuredSize().height + ZFUIMarginGetWidth(child->layoutParam()->layoutMargin()));
    }
}
void ZFUIOnScreenKeyboardAutoFitLayout::viewDelegateLayoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::viewDelegateLayoutOnLayoutPrepare(bounds);
    if(!this->autoFitEnable()
        || this->layoutedFrame().size == ZFUISizeZero() || this->layoutedFrame().size != bounds.size
        || !d->scrollEnableFlag)
    {
        d->scrollView->scrollContentFrameSet(bounds);
    }
    else
    {
        ZFUIRect scrollContentFrame = d->scrollView->scrollContentFrame();
        scrollContentFrame.size = bounds.size;
        d->scrollView->scrollContentFrameSetWhileAnimating(scrollContentFrame);
        d->autoFitUpdateFrame();
        if(d->autoFitFocusedViewNeedUpdate)
        {
            d->autoFitFocusedViewNeedUpdate = zffalse;
            ZFThreadTaskRequest(d->scrollFocusedViewToVisibleDelayListener, this->objectHolder());
        }
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFUIOnScreenKeyboardAutoFitLayout *, ZFUIOnScreenKeyboardAutoFitStart,
                       ZFMP_IN(ZFUIWindow *, window))
{
    if(window != zfnull)
    {
        zfCoreAssertWithMessageTrim(window->viewDelegate() == zfnull,
            "[ZFUIOnScreenKeyboardAutoFitStart] you must not set window's viewDelegate when using auto fit");
        window->viewDelegateClassSet(ZFUIOnScreenKeyboardAutoFitLayout::ClassData()->classNameFull());
        return window->viewDelegate<ZFUIOnScreenKeyboardAutoFitLayout *>();
    }
    return zfnull;
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFUIOnScreenKeyboardAutoFitStop,
                       ZFMP_IN(ZFUIWindow *, window))
{
    if(window != zfnull)
    {
        zfCoreAssertWithMessageTrim(
            window->viewDelegate() == zfnull
            || ZFCastZFObject(ZFUIOnScreenKeyboardAutoFitLayout *, window->viewDelegate()) != zfnull,
            "[ZFUIOnScreenKeyboardAutoFitStop] stopped with unknown viewDelegate: %s",
            window->viewDelegate()->objectInfoOfInstance().cString());
        window->viewDelegateClassSet("");
    }
}

ZF_NAMESPACE_GLOBAL_END

