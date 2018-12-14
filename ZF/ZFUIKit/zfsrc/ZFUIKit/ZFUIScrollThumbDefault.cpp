/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIScrollThumbDefault.h"
#include "ZFUIWindow.h"
#include "ZFAnimationNativeView.h"

#include "ZFUIKit/protocol/ZFProtocolZFAnimationNativeView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIScrollThumbDefault)

// ============================================================
zfclass _ZFP_ZFUIScrollThumbDefault_HideThumbAni : zfextends ZFAnimationTimeLine
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIScrollThumbDefault_HideThumbAni, ZFAnimationTimeLine)

private:
    ZFUIImageView *thumbView;

protected:
    virtual void objectOnInit(ZF_IN ZFUIImageView *thumbView)
    {
        this->objectOnInit();
        this->thumbView = thumbView;
    }
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
    }

protected:
    zfoverride
    virtual void aniTimeLineOnUpdate(ZF_IN zffloat progress)
    {
        zfsuper::aniTimeLineOnUpdate(progress);
        if(this->thumbView->viewVisible())
        {
            this->thumbView->viewAlphaSet(1 - progress);
        }
    }
};
zfclass _ZFP_ZFUIScrollThumbDefault_HideThumbAniTypeNativeView : zfextends ZFAnimationNativeView
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIScrollThumbDefault_HideThumbAniTypeNativeView, ZFAnimationNativeView)

protected:
    virtual void objectOnInit(ZF_IN ZFUIImageView *thumbView)
    {
        this->objectOnInit();
        this->aniTargetSet(thumbView);
        this->aniAlphaToSet(0);
    }
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
    }
};
zfclassNotPOD _ZFP_ZFUIScrollThumbDefaultPrivate
{
public:
    ZFUIImageView *thumbView;
    ZFAnimation *thumbHideAni;
    ZFListener thumbHideAniAutoStopListener;
    zfint lastPos;
    zfint lastSize;
    zftimet lastVisibleTime;
public:
    _ZFP_ZFUIScrollThumbDefaultPrivate(void)
    : thumbView(zfnull)
    , thumbHideAni(zfnull)
    , thumbHideAniAutoStopListener(ZFCallbackForFunc(thumbHideAniAutoStop))
    , lastPos(0)
    , lastSize(0)
    , lastVisibleTime(ZFTime::timestamp() - 1000)
    {
    }
public:
    static ZFLISTENER_PROTOTYPE_EXPAND(aniOnStop)
    {
        ZFUIScrollThumbDefault *owner = userData->objectHolded();
        if(owner != zfnull)
        {
            owner->d->thumbView->viewAlphaSet(1);
            owner->d->thumbView->viewVisibleSet(zffalse);
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(thumbHideAniAutoStop)
    {
        ZFUIScrollThumbDefault *owner = userData->objectHolded();
        if(owner != zfnull)
        {
            owner->d->thumbHideAni->aniStop();
        }
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIScrollThumbDefault)

void ZFUIScrollThumbDefault::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIScrollThumbDefaultPrivate);

    d->thumbView = zfAlloc(ZFUIImageView);

    if(ZFPROTOCOL_IS_AVAILABLE(ZFAnimationNativeView))
    {
        d->thumbHideAni = zfAlloc(_ZFP_ZFUIScrollThumbDefault_HideThumbAniTypeNativeView, d->thumbView);
    }
    else
    {
        d->thumbHideAni = zfAlloc(_ZFP_ZFUIScrollThumbDefault_HideThumbAni, d->thumbView);
    }
}
void ZFUIScrollThumbDefault::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();

    d->thumbHideAni->observerAdd(
        ZFAnimation::EventAniOnStop(),
        ZFCallbackForFunc(_ZFP_ZFUIScrollThumbDefaultPrivate::aniOnStop),
        this->objectHolder());

    ZFObjectGlobalEventObserver().observerAdd(
        ZFUIWindow::EventWindowOnHide(),
        d->thumbHideAniAutoStopListener,
        this->objectHolder());
}
void ZFUIScrollThumbDefault::objectOnDeallocPrepare(void)
{
    ZFObjectGlobalEventObserver().observerRemove(ZFUIWindow::EventWindowOnHide(), d->thumbHideAniAutoStopListener);
    zfsuper::objectOnDeallocPrepare();
}
void ZFUIScrollThumbDefault::objectOnDealloc(void)
{
    d->thumbHideAni->aniStop();
    zfRelease(d->thumbHideAni);
    zfRelease(d->thumbView);
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIScrollThumbDefault, ZFUIImage *, scrollThumbImageHorizontal)
{
    if(this->scrollThumbHorizontal())
    {
        d->thumbView->imageSet(this->scrollThumbImageHorizontal());
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIScrollThumbDefault, ZFUIImage *, scrollThumbImageVertical)
{
    if(!this->scrollThumbHorizontal())
    {
        d->thumbView->imageSet(this->scrollThumbImageVertical());
    }
}

void ZFUIScrollThumbDefault::scrollThumbInit(void)
{
    d->lastPos = 0;
    d->lastSize = 0;

    this->scrollView()->internalFgViewAdd(d->thumbView);

    if(this->scrollThumbHorizontal())
    {
        d->thumbView->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner | ZFUIAlign::e_BottomInner);
        d->thumbView->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
        d->thumbView->imageSet(this->scrollThumbImageHorizontal());
    }
    else
    {
        d->thumbView->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner | ZFUIAlign::e_RightInner);
        d->thumbView->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
        d->thumbView->imageSet(this->scrollThumbImageVertical());
    }

    d->thumbView->viewVisibleSet(zffalse);
}
void ZFUIScrollThumbDefault::scrollThumbDealloc(void)
{
    d->thumbHideAni->aniStop();

    this->scrollView()->internalFgViewRemove(d->thumbView);
}

void ZFUIScrollThumbDefault::scrollThumbUpdate(void)
{
    if(this->scrollView()->scrollViewState() != ZFUIScrollViewState::e_Idle)
    {
        d->thumbHideAni->aniStop();
        d->thumbView->viewAlphaSet(1);
    }

    if(d->thumbView->image() == zfnull)
    {
        d->thumbView->viewVisibleSet(zffalse);
    }
    else
    {
        ZFUISize imageSize = d->thumbView->image()->imageSize();
        zfint pos = 0;
        zfint size = 0;
        zfint sizeRange = 0;
        zfbool viewVisibleNew = zffalse;
        if(this->scrollThumbHorizontal())
        {
            sizeRange = this->scrollView()->layoutedFrame().size.width
                - ZFUIMarginGetWidth(this->scrollView()->nativeImplViewMargin());
            ZFUIScrollThumb::scrollThumbPosFromViewPos(
                pos,
                size,
                sizeRange,
                this->scrollView()->scrollArea().size.width,
                this->scrollView()->scrollContentFrame().point.x,
                this->scrollView()->scrollContentFrame().size.width,
                imageSize.width,
                0,
                imageSize.width);
            sizeRange -= imageSize.width;
            pos += this->scrollView()->nativeImplViewMargin().left;
            if(size != 0)
            {
                viewVisibleNew = zftrue;
                d->thumbView->layoutParam()->layoutMarginSet(ZFUIMarginMake(
                    pos, 0, 0, this->scrollView()->nativeImplViewMargin().bottom));
                d->thumbView->layoutParam()->sizeHintSet(ZFUISizeMake(size, imageSize.height));
            }
        }
        else
        {
            sizeRange = this->scrollView()->layoutedFrame().size.height
                - ZFUIMarginGetHeight(this->scrollView()->nativeImplViewMargin());
            ZFUIScrollThumb::scrollThumbPosFromViewPos(
                pos,
                size,
                sizeRange,
                this->scrollView()->scrollArea().size.height,
                this->scrollView()->scrollContentFrame().point.y,
                this->scrollView()->scrollContentFrame().size.height,
                imageSize.height,
                0,
                0);
            pos += this->scrollView()->nativeImplViewMargin().top;
            if(size != 0)
            {
                viewVisibleNew = zftrue;
                d->thumbView->layoutParam()->layoutMarginSet(ZFUIMarginMake(
                    0, pos, this->scrollView()->nativeImplViewMargin().right, 0));
                d->thumbView->layoutParam()->sizeHintSet(ZFUISizeMake(imageSize.width, size));
            }
        }

        if(d->lastPos == 0 && d->lastSize == 0)
        {
            d->lastPos = pos;
            d->lastSize = size;
        }

        zftimet curTime = ZFTime::timestamp();
        if(viewVisibleNew)
        {
            if(!d->thumbView->viewVisible()
                && pos == d->lastPos && size == d->lastSize
                && (this->scrollView()->scrollViewState() == ZFUIScrollViewState::e_Idle || size == sizeRange))
            {
                viewVisibleNew = zffalse;
            }
            else
            {
                if(curTime - d->lastVisibleTime >= 100
                    && this->scrollView()->scrollViewState() == ZFUIScrollViewState::e_Idle
                    && size == sizeRange)
                {
                    viewVisibleNew = zffalse;
                }
                d->lastPos = pos;
                d->lastSize = size;
            }
        }

        if(viewVisibleNew)
        {
            d->lastVisibleTime = curTime;
        }
        else
        {
            d->thumbHideAni->aniStop();
        }
        d->thumbView->viewVisibleSet(viewVisibleNew);
    }

    // auto hide scroll thumb
    if(d->thumbView->viewVisible()
        && this->scrollView()->scrollViewState() == ZFUIScrollViewState::e_Idle)
    {
        d->thumbHideAni->aniStop();
        d->thumbView->viewVisibleSet(zftrue);
        zftimet autoHideTime = (this->scrollThumbHorizontal() ? this->scrollThumbAutoHideDurationHorizontal() : this->scrollThumbAutoHideDurationVertical());
        if(autoHideTime > 0)
        {
            d->thumbHideAni->aniDelaySet(this->scrollThumbHorizontal() ? this->scrollThumbAutoHideDelayHorizontal() : this->scrollThumbAutoHideDelayVertical());
            d->thumbHideAni->aniDurationSet(autoHideTime);
            d->thumbHideAni->aniStart();
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

