/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIScrollView.h"
#include "ZFUIScroller.h"
#include "ZFUIScrollThumb.h"
#include "protocol/ZFProtocolZFUIScrollView.h"

#include "ZFUIViewPositionOnScreen.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFUIScrollView_DEBUG_logEvent 0

ZFENUM_DEFINE(ZFUIScrollViewState)

ZFSTYLE_DEFAULT_DEFINE(ZFUIScrollView)

// ============================================================
typedef enum {
    _ZFP_ZFUIScrollViewActionDragBegin,
    _ZFP_ZFUIScrollViewActionDrag,
    _ZFP_ZFUIScrollViewActionDragEnd,
    _ZFP_ZFUIScrollViewActionScrollBegin,
    _ZFP_ZFUIScrollViewActionScroll,
    _ZFP_ZFUIScrollViewActionScrollEnd,
    _ZFP_ZFUIScrollViewActionUpdate,
} _ZFP_ZFUIScrollViewAction;

// ============================================================
// _ZFP_ZFUIScrollViewPrivate
zfclass _ZFP_ZFUIScrollViewPrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_ZFUIScrollViewPrivate, ZFObject)

public:
    ZFUIScrollView *pimplOwner;
    ZFUIScrollViewStateEnum state;
    zfbool scrollContentFrameOverrideFlag;
    zfbool scrollAniTimerStarted;
    zftimet scrollAniLastTime;

    ZFUIScroller *xScroll;
    zfbool xScrollEnable;
    zfint xScrollDragPrevPos;
    zfint xScrollDragCurPos;
    zfbool xScrollAniTimerStarted;

    ZFUIScroller *yScroll;
    zfbool yScrollEnable;
    zfint yScrollDragPrevPos;
    zfint yScrollDragCurPos;
    zfbool yScrollAniTimerStarted;

    ZFCoreQueuePOD<_ZFP_ZFUIScrollViewAction> scrollerActions;
    zfbool scrollerActionRunning;

    zfbool scrollThumbNeedUpdate;
    ZFUIRect scrollArea;
    ZFUIMargin scrollAreaMargin;

    const ZFClass *xScrollThumbClass;
    ZFUIScrollThumb *xScrollThumb;

    const ZFClass *yScrollThumbClass;
    ZFUIScrollThumb *yScrollThumb;

    zfint autoScrollSpeedX;
    zfint autoScrollSpeedY;
    zfbool autoScrollStartFlag;

    zfint scrollOverrideFlag;

protected:
    _ZFP_ZFUIScrollViewPrivate(void)
    : pimplOwner(zfnull)
    , state(ZFUIScrollViewState::e_Idle)
    , scrollContentFrameOverrideFlag(zffalse)
    , scrollAniTimerStarted(zffalse)
    , scrollAniLastTime(0)
    , xScroll(zfnull)
    , xScrollEnable(zftrue)
    , xScrollDragPrevPos(0)
    , xScrollDragCurPos(0)
    , xScrollAniTimerStarted(zffalse)
    , yScroll(zfnull)
    , yScrollEnable(zftrue)
    , yScrollDragPrevPos(0)
    , yScrollDragCurPos(0)
    , yScrollAniTimerStarted(zffalse)
    , scrollerActions()
    , scrollerActionRunning(zffalse)
    , scrollThumbNeedUpdate(zftrue)
    , scrollAreaMargin(ZFUIMarginZero())
    , xScrollThumbClass(zfnull)
    , xScrollThumb(zfnull)
    , yScrollThumbClass(zfnull)
    , yScrollThumb(zfnull)
    , autoScrollSpeedX(0)
    , autoScrollSpeedY(0)
    , autoScrollStartFlag(zffalse)
    , scrollOverrideFlag(0)
    {
    }

    // ============================================================
    // scroller logic
public:
    void scrollerInit(void)
    {
        this->xScroll->scrollAniStartCallback = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, x_scrollAniStart));
        this->xScroll->scrollAniStopCallback = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, x_scrollAniStop));
        this->yScroll->scrollAniStartCallback = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, y_scrollAniStart));
        this->yScroll->scrollAniStopCallback = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, y_scrollAniStop));
    }

public:
    void scrollAreaUpdate(void)
    {
        this->scrollArea = this->pimplOwner->layoutedFrame();
        this->scrollArea.point = ZFUIPointZero();
        ZFUIRectApplyMargin(this->scrollArea, this->scrollArea, this->pimplOwner->nativeImplViewMargin());
        ZFUIRectApplyMargin(this->scrollArea, this->scrollArea, this->scrollAreaMargin);
    }

public:
    void scrollerUpdate(void)
    {
        this->scrollContentFrameUpdate();
        this->scrollerActionRun();
    }

public:
    void scrollBounceChanged(void)
    {
        ZFPROTOCOL_ACCESS(ZFUIScrollView)->scrollViewScrollBounceSet(this->pimplOwner,
            this->pimplOwner->scrollBounceHorizontal(),
            this->pimplOwner->scrollBounceVertical(),
            this->pimplOwner->scrollBounceHorizontalAlways(),
            this->pimplOwner->scrollBounceVerticalAlways());
        this->xScroll->scrollBounceChanged(this->pimplOwner->scrollBounceHorizontal(), this->pimplOwner->scrollBounceHorizontalAlways());
        this->yScroll->scrollBounceChanged(this->pimplOwner->scrollBounceVertical(), this->pimplOwner->scrollBounceVerticalAlways());
    }

public:
    void scrollAlignToPageChanged(void)
    {
        this->xScroll->scrollAlignToPageChanged(this->pimplOwner->scrollAlignToPageHorizontal());
        this->yScroll->scrollAlignToPageChanged(this->pimplOwner->scrollAlignToPageVertical());
    }

public:
    void processDragBegin(ZF_IN const ZFUIPoint &mousePos,
                          ZF_IN zftimet mouseTime)
    {
        // reset state before drag begin
        this->xScrollAniTimerStarted = zffalse;
        this->yScrollAniTimerStarted = zffalse;
        this->notifyScrollAniTimerStop();
        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionDragEnd);

        if(!this->pimplOwner->scrollEnable())
        {
            return ;
        }

        this->xScrollEnable = (this->pimplOwner->scrollBounceHorizontalAlways()
            || this->pimplOwner->scrollContentFrame().size.width > this->pimplOwner->scrollArea().size.width);
        this->yScrollEnable = (this->pimplOwner->scrollBounceVerticalAlways()
            || this->pimplOwner->scrollContentFrame().size.height > this->pimplOwner->scrollArea().size.height);

        if(this->xScroll->scrollRequireFocus())
        {
            this->yScrollEnable = zffalse;
        }
        else if(this->yScroll->scrollRequireFocus())
        {
            this->xScrollEnable = zffalse;
        }

        if(this->xScrollEnable)
        {
            this->xScrollDragPrevPos = mousePos.x;
            this->xScrollDragCurPos = mousePos.x;
            this->xScroll->scrollOnDragBegin(mousePos.x, mouseTime);
        }
        if(this->yScrollEnable)
        {
            this->yScrollDragPrevPos = mousePos.y;
            this->yScrollDragCurPos = mousePos.y;
            this->yScroll->scrollOnDragBegin(mousePos.y, mouseTime);
        }

        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionDragBegin);
        this->scrollerActionRun();
    }
    void processDrag(ZF_IN const ZFUIPoint &mousePos,
                     ZF_IN zftimet mouseTime)
    {
        if(this->state != ZFUIScrollViewState::e_Dragging)
        {
            return ;
        }

        zfbool alignToAxis = (this->pimplOwner->scrollAlignToAxis() || this->pimplOwner->scrollAlignToPageHorizontal() || this->pimplOwner->scrollAlignToPageVertical());
        if(alignToAxis && this->xScrollEnable && this->yScrollEnable)
        {
            this->xScrollDragPrevPos = this->xScrollDragCurPos;
            this->xScrollDragCurPos = mousePos.x;
            this->yScrollDragPrevPos = this->yScrollDragCurPos;
            this->yScrollDragCurPos = mousePos.y;

            zfint xOffset = zfmAbs(this->xScrollDragCurPos - this->xScrollDragPrevPos);
            zfint yOffset = zfmAbs(this->yScrollDragCurPos - this->yScrollDragPrevPos);
            if(xOffset > yOffset)
            {
                this->yScrollEnable = zffalse;
            }
            else if(yOffset >= xOffset)
            {
                this->xScrollEnable = zffalse;
            }
        }

        if(this->xScrollEnable)
        {
            this->xScroll->scrollOnDrag(mousePos.x, mouseTime);
        }
        if(this->yScrollEnable)
        {
            this->yScroll->scrollOnDrag(mousePos.y, mouseTime);
        }

        this->scrollContentFrameUpdate();
        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionDrag);
        this->scrollerActionRun();
    }
    void processDragEnd(ZF_IN zftimet mouseTime,
                        ZF_IN zfbool needScrollAni)
    {
        if(this->state != ZFUIScrollViewState::e_Dragging)
        {
            // this may be an error state, reset by end scroll
            this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionScrollEnd);
            this->scrollerActionRun();
            return ;
        }

        if(this->xScrollEnable)
        {
            this->xScroll->scrollOnDragEnd(mouseTime, needScrollAni);
        }
        if(this->yScrollEnable)
        {
            this->yScroll->scrollOnDragEnd(mouseTime, needScrollAni);
        }

        this->xScrollEnable = (this->pimplOwner->scrollBounceHorizontalAlways()
            || this->pimplOwner->scrollContentFrame().size.width > this->pimplOwner->scrollArea().size.width);
        this->yScrollEnable = (this->pimplOwner->scrollBounceVerticalAlways()
            || this->pimplOwner->scrollContentFrame().size.height > this->pimplOwner->scrollArea().size.height);

        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionDragEnd);
        this->scrollerActionRun();
    }
    void processScrollAnimation(ZF_IN zftimet relativeTimeInMiliseconds)
    {
        this->scrollAniLastTime = relativeTimeInMiliseconds;

        if(this->xScrollAniTimerStarted)
        {
            this->xScroll->scrollAniOnUpdate(relativeTimeInMiliseconds);
        }
        if(this->yScrollAniTimerStarted)
        {
            this->yScroll->scrollAniOnUpdate(relativeTimeInMiliseconds);
        }

        if(this->autoScrollSpeedX != 0)
        {
            if((this->autoScrollSpeedX > 0 && this->xScroll->scrollContentOffset() >= 0)
                || (this->autoScrollSpeedX < 0
                    && this->xScroll->scrollContentOffset() + this->xScroll->scrollContentSize() <= this->pimplOwner->scrollArea().size.width))
            {
                this->pimplOwner->scrollOverrideSet(zftrue);
                this->pimplOwner->autoScrollStopX();
                this->pimplOwner->scrollOverrideSet(zffalse);
            }
            else
            {
                this->xScroll->scrollBySpeed(this->autoScrollSpeedX);
            }
        }
        if(this->autoScrollSpeedY != 0)
        {
            if((this->autoScrollSpeedY > 0 && this->yScroll->scrollContentOffset() >= 0)
                || (this->autoScrollSpeedY < 0
                    && this->yScroll->scrollContentOffset() + this->yScroll->scrollContentSize() <= this->pimplOwner->scrollArea().size.height))
            {
                this->pimplOwner->scrollOverrideSet(zftrue);
                this->pimplOwner->autoScrollStopY();
                this->pimplOwner->scrollOverrideSet(zffalse);
            }
            else
            {
                this->yScroll->scrollBySpeed(this->autoScrollSpeedY);
            }
        }

        this->scrollContentFrameUpdate();
        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionScroll);
        this->scrollerActionRun();
    }

public:
    void scrollContentFrameUpdateForImpl(void)
    {
        ZFUIRect frame = this->pimplOwner->scrollContentFrame();
        frame.point.x += this->pimplOwner->scrollAreaMargin().left;
        frame.point.y += this->pimplOwner->scrollAreaMargin().top;
        ZFPROTOCOL_ACCESS(ZFUIScrollView)->scrollViewScrollContentFrameSet(
            this->pimplOwner,
            ZFUIRectApplyScale(frame, this->pimplOwner->scaleFixed()));
    }
private:
    void scrollContentFrameUpdate(void)
    {
        ZFUIRect frame = ZFUIRectMake(
            this->xScroll->scrollContentOffset(),
            this->yScroll->scrollContentOffset(),
            this->xScroll->scrollContentSize(),
            this->yScroll->scrollContentSize());
        this->scrollContentFrameOverrideFlag = zftrue;
        this->pimplOwner->scrollContentFrameSet(frame);
        this->scrollContentFrameOverrideFlag = zffalse;
        this->scrollContentFrameUpdateForImpl();

        this->notifyScrollContentFrameOnChange();
        this->scrollThumbNeedUpdate = zftrue;
    }

private:
    ZFMETHOD_INLINE_0(zftimet, x_scrollAniStart)
    {
        if(!this->xScrollAniTimerStarted)
        {
            this->xScrollAniTimerStarted = zftrue;
            this->notifyScrollAniTimerStart();
        }
        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionScrollBegin);
        return this->scrollAniLastTime;
    }
    ZFMETHOD_INLINE_0(zftimet, y_scrollAniStart)
    {
        if(!this->yScrollAniTimerStarted)
        {
            this->yScrollAniTimerStarted = zftrue;
            this->notifyScrollAniTimerStart();
        }
        this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionScrollBegin);
        return this->scrollAniLastTime;
    }
    ZFMETHOD_INLINE_0(void, x_scrollAniStop)
    {
        if(this->xScrollAniTimerStarted)
        {
            this->xScrollAniTimerStarted = zffalse;
            if(!this->yScrollAniTimerStarted)
            {
                this->notifyScrollAniTimerStop();
                this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionScrollEnd);
            }
        }
    }
    ZFMETHOD_INLINE_0(void, y_scrollAniStop)
    {
        if(this->yScrollAniTimerStarted)
        {
            this->yScrollAniTimerStarted = zffalse;
            if(!this->xScrollAniTimerStarted)
            {
                this->notifyScrollAniTimerStop();
                this->scrollerActionAdd(_ZFP_ZFUIScrollViewActionScrollEnd);
            }
        }
    }

private:
    void notifyScrollAniTimerStart(void)
    {
        if(!this->scrollAniTimerStarted)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollAniTimerStart");
            #endif
            this->scrollAniTimerStarted = zftrue;
            static const zftimet recommendTimerInterval = zftimet(17); // nearly 60fps
            this->scrollAniLastTime = ZFPROTOCOL_ACCESS(ZFUIScrollView)->scrollViewScrollAnimationStart(this->pimplOwner, recommendTimerInterval);
        }
    }
    void notifyScrollAniTimerStop(void)
    {
        if(this->scrollAniTimerStarted)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollAniTimerStop");
            #endif
            this->scrollAniTimerStarted = zffalse;
            ZFPROTOCOL_ACCESS(ZFUIScrollView)->scrollViewScrollAnimationStop(this->pimplOwner);
        }
    }
    void notifyScrollOnDragBegin(void)
    {
        if(this->state != ZFUIScrollViewState::e_Dragging)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollOnDragBegin");
            #endif
            this->notifyScrollAniTimerStop();
            this->notifyScrollOnScrollEnd();

            this->state = ZFUIScrollViewState::e_Dragging;
            this->pimplOwner->scrollOnDragBegin();
            this->scrollThumbUpdate();
        }
    }
    void notifyScrollOnDrag(void)
    {
        if(this->state == ZFUIScrollViewState::e_Dragging)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollOnDrag");
            #endif
            this->pimplOwner->scrollOnDrag();
            this->scrollThumbUpdate();
        }
    }
    void notifyScrollOnDragEnd(void)
    {
        if(this->state == ZFUIScrollViewState::e_Dragging)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollOnDragEnd");
            #endif
            this->state = ZFUIScrollViewState::e_Idle;
            this->pimplOwner->scrollOnDragEnd();
            this->scrollThumbNeedUpdate = zftrue;
            this->scrollThumbUpdate();
        }
    }
    void notifyScrollOnScrollBegin(void)
    {
        if(this->state != ZFUIScrollViewState::e_Scrolling)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollOnScrollBegin");
            #endif
            this->notifyScrollOnDragEnd();

            this->state = ZFUIScrollViewState::e_Scrolling;
            this->pimplOwner->scrollOnScrollBegin();
            this->scrollThumbUpdate();
        }
    }
    void notifyScrollOnScroll(void)
    {
        if(this->state == ZFUIScrollViewState::e_Scrolling)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollOnScroll");
            #endif
            this->pimplOwner->scrollOnScroll();
            this->scrollThumbUpdate();
        }
    }
    void notifyScrollOnScrollEnd(void)
    {
        if(this->state == ZFUIScrollViewState::e_Scrolling)
        {
            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollOnScrollEnd");
            #endif
            this->notifyScrollOnScroll();

            this->state = ZFUIScrollViewState::e_Idle;
            this->notifyScrollAniTimerStop();
            this->pimplOwner->scrollOnScrollEnd();
            this->scrollThumbNeedUpdate = zftrue;
            this->scrollThumbUpdate();
        }
    }
    void notifyScrollContentFrameOnChange(void)
    {
        this->pimplOwner->scrollContentFrameOnChange();
    }
    void notifyScrollAutoScrollOnStart(void)
    {
        if(!this->autoScrollStartFlag)
        {
            this->autoScrollStartFlag = zftrue;

            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollAutoScrollOnStart");
            #endif
            this->pimplOwner->scrollAutoScrollOnStart();
        }
    }
    void notifyScrollAutoScrollOnStop(void)
    {
        if(this->autoScrollStartFlag && this->autoScrollSpeedX == 0 && this->autoScrollSpeedY == 0)
        {
            this->autoScrollStartFlag = zffalse;

            #if _ZFP_ZFUIScrollView_DEBUG_logEvent
                zfLogTrimT() << this->pimplOwner->objectInfoOfInstance() << zfText("scrollAutoScrollOnStop");
            #endif
            this->pimplOwner->scrollAutoScrollOnStop();
        }
    }

private:
    void scrollerActionRun(void)
    {
        if(this->scrollerActionRunning || this->scrollerActions.isEmpty())
        {
            return ;
        }

        this->scrollerActionRunning = zftrue;
        while(!this->scrollerActions.isEmpty())
        {
            _ZFP_ZFUIScrollViewAction scrollerAction = this->scrollerActions.queueTake();
            switch(scrollerAction)
            {
                case _ZFP_ZFUIScrollViewActionDragBegin:
                    this->notifyScrollOnDragBegin();
                    break;
                case _ZFP_ZFUIScrollViewActionDrag:
                    this->notifyScrollOnDrag();
                    break;
                case _ZFP_ZFUIScrollViewActionDragEnd:
                    this->notifyScrollOnDragEnd();
                    break;
                case _ZFP_ZFUIScrollViewActionScrollBegin:
                    this->notifyScrollOnScrollBegin();
                    break;
                case _ZFP_ZFUIScrollViewActionScroll:
                    this->notifyScrollOnScroll();
                    break;
                case _ZFP_ZFUIScrollViewActionScrollEnd:
                    this->notifyScrollOnScrollEnd();
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
        }
        this->scrollerActionRunning = zffalse;
    }
    void scrollerActionAdd(ZF_IN _ZFP_ZFUIScrollViewAction scrollerAction)
    {
        this->scrollerActions.queuePut(scrollerAction);
    }

    // ============================================================
    // scroll thumb logic
public:
    void scrollThumbUpdate(void)
    {
        if(this->scrollThumbNeedUpdate)
        {
            if(this->xScrollThumb != zfnull)
            {
                this->xScrollThumb->scrollThumbUpdate();
                this->xScrollThumb->toObject()->observerNotify(ZFUIScrollThumb::EventScrollThumbOnUpdate());
            }
            if(this->yScrollThumb != zfnull)
            {
                this->yScrollThumb->scrollThumbUpdate();
                this->yScrollThumb->toObject()->observerNotify(ZFUIScrollThumb::EventScrollThumbOnUpdate());
            }
            this->scrollThumbNeedUpdate = zffalse;
        }
    }
};

// ============================================================
// ZFUIScrollView
ZFOBJECT_REGISTER(ZFUIScrollView)

ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnDragBegin)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnDrag)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnDragEnd)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnScrollBegin)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnScroll)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnScrollEnd)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollAreaMarginOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollContentFrameOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollAutoScrollOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollAutoScrollOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFUIScrollView, ScrollOnScrolledByUser)

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIScrollView, zfbool, scrollEnable)
{
    d->xScrollEnable = this->scrollEnable();
    d->yScrollEnable = this->scrollEnable();
    ZFPROTOCOL_ACCESS(ZFUIScrollView)->scrollViewScrollEnableSet(this, this->scrollEnable());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIScrollView, zfbool, scrollBounceHorizontal)
{
    d->scrollBounceChanged();
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIScrollView, zfbool, scrollBounceVertical)
{
    d->scrollBounceChanged();
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIScrollView, zfbool, scrollBounceHorizontalAlways)
{
    d->scrollBounceChanged();
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIScrollView, zfbool, scrollBounceVerticalAlways)
{
    d->scrollBounceChanged();
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIScrollView, zfbool, scrollAlignToPageHorizontal)
{
    d->scrollAlignToPageChanged();
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIScrollView, zfbool, scrollAlignToPageVertical)
{
    d->scrollAlignToPageChanged();
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIScrollView, ZFUIRect, scrollContentFrame)
{
    if(d->xScroll->scrollContentOffset() != propertyValueOld.point.x
        || d->xScroll->scrollContentSize() != propertyValue.size.width)
    {
        d->xScroll->scrollContentChanged(propertyValue.point.x, propertyValue.size.width);
    }
    if(d->yScroll->scrollContentOffset() != propertyValueOld.point.x
        || d->yScroll->scrollContentSize() != propertyValue.size.width)
    {
        d->yScroll->scrollContentChanged(propertyValue.point.y, propertyValue.size.height);
    }
    if(!d->scrollContentFrameOverrideFlag)
    {
        d->xScroll->scrollWithoutAnimation(propertyValue.point.x);
        d->yScroll->scrollWithoutAnimation(propertyValue.point.y);
        d->scrollerUpdate();

        if(d->scrollOverrideFlag == 0)
        {
            this->scrollOnScrolledByUser();
        }
    }
}
void ZFUIScrollView::_ZFP_ZFUIScrollView_scrollContentFrameSetByImpl(ZF_IN const ZFUIRect &rect)
{
    d->scrollContentFrameOverrideFlag = zftrue;
    this->scrollContentFrameSet(rect);
    d->scrollContentFrameOverrideFlag = zffalse;
}

void ZFUIScrollView::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfAlloc(_ZFP_ZFUIScrollViewPrivate);
    d->pimplOwner = this;
    d->xScroll = zfRetain(ZFCastZFObject(ZFUIScroller *, this->scrollerClass()->newInstance().toObject()));
    d->yScroll = zfRetain(ZFCastZFObject(ZFUIScroller *, this->scrollerClass()->newInstance().toObject()));
    zfCoreAssertWithMessage(d->xScroll != zfnull && d->yScroll != zfnull,
        zfTextA("scrollerClass must return a class type of %s"),
        zfsCoreZ2A(ZFUIScroller::ClassData()->classNameFull()));
    zfCoreAssert(d->xScroll != zfnull && d->yScroll != zfnull);
    d->scrollerInit();

    this->scrollThumbHorizontalOnInit();
    this->scrollThumbVerticalOnInit();

    zfclassNotPOD _ZFP_ZFUIScrollView_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUIScrollView)->nativeScrollViewDestroy(view->to<ZFUIScrollView *>(), nativeImplView);
        }
    };
    this->nativeImplViewSet(
        ZFPROTOCOL_ACCESS(ZFUIScrollView)->nativeScrollViewCreate(this),
        _ZFP_ZFUIScrollView_nativeImplViewDestroy::action);
}
void ZFUIScrollView::objectOnDealloc(void)
{
    zfRetainChange(d->xScroll, zfnull);
    zfRetainChange(d->yScroll, zfnull);

    if(d->xScrollThumb != zfnull)
    {
        d->xScrollThumb->scrollThumbDealloc();
        zfRelease(d->xScrollThumb);
        d->xScrollThumb = zfnull;
    }
    if(d->yScrollThumb != zfnull)
    {
        d->yScrollThumb->scrollThumbDealloc();
        zfRelease(d->yScrollThumb);
        d->yScrollThumb = zfnull;
    }

    zfRelease(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIScrollView::objectOnDeallocPrepare(void)
{
    // set content frame to stop scroll animation
    this->scrollContentFrameSet(this->scrollContentFrame());
    zfsuper::objectOnDeallocPrepare();
}

void ZFUIScrollView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    ret += zfText(" ");
    ZFUIRectToString(ret, this->scrollContentFrame());
    if(!this->scrollEnable())
    {
        ret += zfText(" ScrollDisabled");
    }
}

// ============================================================
// override ZFUIView
void ZFUIScrollView::implChildOnAdd(ZF_IN ZFUIView *child,
                                    ZF_IN zfindex virtualIndex,
                                    ZF_IN ZFUIViewChildLayerEnum childLayer,
                                    ZF_IN zfindex childLayerIndex)
{
    switch(child->viewLayer())
    {
        case ZFUIViewChildLayer::e_Normal:
            ZFPROTOCOL_ACCESS(ZFUIScrollView)->scrollChildAdd(this, child, childLayerIndex);
            break;
        case ZFUIViewChildLayer::e_InternalImpl:
            zfsuper::implChildOnAdd(child, virtualIndex, childLayer, childLayerIndex);
            break;
        case ZFUIViewChildLayer::e_InternalBg:
            zfsuper::implChildOnAdd(child, virtualIndex, childLayer, childLayerIndex);
            break;
        case ZFUIViewChildLayer::e_InternalFg:
            zfsuper::implChildOnAdd(child, virtualIndex - this->childCount(), childLayer, childLayerIndex);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}
void ZFUIScrollView::implChildOnRemove(ZF_IN ZFUIView *child,
                                       ZF_IN zfindex virtualIndex,
                                       ZF_IN ZFUIViewChildLayerEnum childLayer,
                                       ZF_IN zfindex childLayerIndex)
{
    switch(childLayer)
    {
        case ZFUIViewChildLayer::e_Normal:
            ZFPROTOCOL_ACCESS(ZFUIScrollView)->scrollChildRemove(this, child, childLayerIndex);
            break;
        case ZFUIViewChildLayer::e_InternalImpl:
            zfsuper::implChildOnRemove(child, virtualIndex, childLayer, childLayerIndex);
            break;
        case ZFUIViewChildLayer::e_InternalBg:
            zfsuper::implChildOnRemove(child, virtualIndex, childLayer, childLayerIndex);
            break;
        case ZFUIViewChildLayer::e_InternalFg:
            zfsuper::implChildOnRemove(child, virtualIndex - this->childCount(), childLayer, childLayerIndex);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}
void ZFUIScrollView::implChildOnRemoveAllForDealloc(void)
{
    zfsuper::implChildOnRemoveAllForDealloc();
    ZFPROTOCOL_ACCESS(ZFUIScrollView)->scrollChildRemoveAllForDealloc(this);
}

void ZFUIScrollView::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    d->scrollAreaUpdate();
    zfint xScrollOwnerSize = bounds.size.width - ZFUIMarginGetWidth(this->nativeImplViewMargin()) - ZFUIMarginGetWidth(d->scrollAreaMargin);
    zfint yScrollOwnerSize = bounds.size.height - ZFUIMarginGetHeight(this->nativeImplViewMargin()) - ZFUIMarginGetHeight(d->scrollAreaMargin);
    if(xScrollOwnerSize != d->xScroll->scrollOwnerSize()
        || yScrollOwnerSize != d->yScroll->scrollOwnerSize())
    {
        d->xScroll->scrollOwnerSizeChanged(xScrollOwnerSize);
        d->yScroll->scrollOwnerSizeChanged(yScrollOwnerSize);
        d->scrollerUpdate();

        ZFLISTENER_LOCAL(action, {
            ZFUIScrollView *scrollView = userData->objectHolded();
            if(scrollView == zfnull)
            {
                return ;
            }
            ZFUIView *focusedChild = scrollView->viewFocusFind();
            if(focusedChild != zfnull)
            {
                scrollView->scrollChildToVisible(
                    focusedChild,
                    ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemMargin()),
                    zffalse);
            }
        })
        ZFThreadTaskRequest(action, this->objectHolder());
    }

    d->scrollThumbUpdate();
    zfsuper::layoutOnLayoutPrepare(bounds);
}
void ZFUIScrollView::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayout(ZFUIRectGetBounds(this->scrollContentFrame()));
}
void ZFUIScrollView::layoutOnLayoutFinish(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayoutFinish(bounds);
    d->scrollContentFrameUpdateForImpl();
}
void ZFUIScrollView::layoutedFrameFixedOnUpdateForChild(ZF_OUT ZFUIRect &ret, ZF_IN const ZFUIRect &childFrame)
{
    ret = childFrame;
    ret.point.x += this->scrollContentFrame().point.x;
    ret.point.y += this->scrollContentFrame().point.y;
}

void ZFUIScrollView::viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent)
{
    zfsuper::viewEventOnMouseEvent(mouseEvent);
}
void ZFUIScrollView::viewEventOnWheelEvent(ZF_IN ZFUIWheelEvent *wheelEvent)
{
    zfsuper::viewEventOnWheelEvent(wheelEvent);
    if(wheelEvent->eventResolved()) {
        return ;
    }
    if(!this->scrollEnable())
    {
        return ;
    }

    zfint wheelXSaved = this->scrollByPointEndPoint().x - this->scrollContentFrame().point.x;
    zfint wheelYSaved = this->scrollByPointEndPoint().y - this->scrollContentFrame().point.y;
    const zfint initValue = 60;
    const zfint maxValue = 3000;
    zfint wheelX = wheelEvent->wheelX * initValue;
    zfint wheelY = wheelEvent->wheelY * initValue;
    if(this->scrollAlignToAxis())
    {
        if(zfmAbs(wheelY) >= zfmAbs(wheelX))
        {
            wheelX = 0;
            wheelXSaved = 0;
        }
        else
        {
            wheelY = 0;
            wheelYSaved = 0;
        }
    }

    if(!((wheelXSaved > 0 && wheelX < 0)
        || (wheelXSaved < 0 && wheelX > 0)))
    {
        wheelX = wheelXSaved + wheelX;
        if(wheelX > 0 && wheelX > maxValue)
        {
            wheelX = maxValue;
        }
        else if(wheelX < 0 && wheelX < -maxValue)
        {
            wheelX = -maxValue;
        }
    }

    if(!((wheelYSaved > 0 && wheelY < 0)
        || (wheelYSaved < 0 && wheelY > 0)))
    {
        wheelY = wheelYSaved + wheelY;
        if(wheelY > 0 && wheelY > maxValue)
        {
            wheelY = maxValue;
        }
        else if(wheelY < 0 && wheelY < -maxValue)
        {
            wheelY = -maxValue;
        }
    }

    if(wheelX != 0)
    {
        if(this->scrollAlignToPageHorizontal())
        {
            zfint pageSize = this->scrollArea().size.width;
            if(zfmAbs(wheelX) < zfmAbs(pageSize))
            {
                if(wheelX > 0)
                {
                    wheelX += pageSize / 2;
                }
                else
                {
                    wheelX -= pageSize / 2;
                }
            }
        }
        if((this->scrollContentOffsetLeft() <= 0 && wheelX > 0)
            || (this->scrollContentOffsetRight() <= 0 && wheelX < 0))
        {
            wheelX = 0;
        }
    }

    if(wheelY != 0)
    {
        if(this->scrollAlignToPageVertical())
        {
            zfint pageSize = this->scrollArea().size.height;
            if(zfmAbs(wheelY) < zfmAbs(pageSize))
            {
                if(wheelY > 0)
                {
                    wheelY += pageSize / 2;
                }
                else
                {
                    wheelY -= pageSize / 2;
                }
            }
        }
        if((this->scrollContentOffsetTop() <= 0 && wheelY > 0)
            || (this->scrollContentOffsetBottom() <= 0 && wheelY < 0))
        {
            wheelY = 0;
        }
    }

    if(wheelX != 0 || wheelY != 0)
    {
        this->scrollByPoint(this->scrollContentFrame().point.x + wheelX, this->scrollContentFrame().point.y + wheelY);
        wheelEvent->eventResolvedSet(zftrue);
    }
}

void ZFUIScrollView::nativeImplViewMarginOnChange(void)
{
    d->scrollAreaUpdate();
    zfsuper::nativeImplViewMarginOnChange();
}

static void _ZFP_ZFUIScrollView_scrollChildToVisible(ZF_OUT zfint &offset,
                                                     ZF_IN zfint childStart,
                                                     ZF_IN zfint childLength,
                                                     ZF_IN zfint selfStart,
                                                     ZF_IN zfint selfLength,
                                                     ZF_IN zfint contentOffset,
                                                     ZF_IN zfint contentSize,
                                                     ZF_IN zfint marginHead,
                                                     ZF_IN zfint marginTail)
{
    if(childLength + marginHead + marginTail >= selfLength)
    {
        // offset = selfStart + marginHead + (selfLength - marginHead - marginTail) / 2 - (childStart + childLength / 2);
        offset = (2 * selfStart + marginHead + selfLength - marginTail - 2 * childStart - childLength) / 2;
    }
    else
    {
        if(childStart < selfStart + marginHead)
        {
            offset = selfStart + marginHead - childStart;
        }
        else if(childStart + childLength > selfStart + selfLength - marginTail)
        {
            offset = (selfStart + selfLength - marginTail) - (childStart + childLength);
        }
    }
    if(contentOffset + offset > 0)
    {
        offset = -contentOffset;
    }
    if(contentSize >= selfLength && contentOffset + offset + contentSize < selfLength)
    {
        offset = selfLength - contentOffset - contentSize;
    }
}
ZFMETHOD_DEFINE_0(ZFUIScrollView, void, scrollToFitRange)
{
    d->xScroll->scrollToFitRange();
    d->yScroll->scrollToFitRange();
    d->scrollerUpdate();
}
ZFMETHOD_DEFINE_3(ZFUIScrollView, void, scrollChildToVisible,
                  ZFMP_IN(ZFUIView *, child),
                  ZFMP_IN_OPT(const ZFUIMargin &, margin, ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemMargin())),
                  ZFMP_IN_OPT(zfbool, scrollWithAni, zftrue))
{
    if(child == zfnull)
    {
        return ;
    }

    zfint offsetX = 0;
    zfint offsetY = 0;

    ZFUIRect selfRect = ZFUIViewPositionOnScreen(this);
    ZFUIRect childRect = ZFUIViewPositionOnScreen(child);
    const ZFUIRect &contentFrame = this->scrollContentFrame();
    _ZFP_ZFUIScrollView_scrollChildToVisible(
        offsetX,
        childRect.point.x, childRect.size.width,
        selfRect.point.x, selfRect.size.width,
        contentFrame.point.x, contentFrame.size.width,
        margin.left, margin.right);
    _ZFP_ZFUIScrollView_scrollChildToVisible(
        offsetY,
        childRect.point.y, childRect.size.height,
        selfRect.point.y, selfRect.size.height,
        contentFrame.point.y, contentFrame.size.height,
        margin.top, margin.bottom);

    if(offsetX != 0 || offsetY != 0)
    {
        ZFUIRect t = this->scrollContentFrame();
        t.point.x += offsetX;
        t.point.y += offsetY;
        if(scrollWithAni)
        {
            this->scrollByPoint(t.point.x, t.point.y);
        }
        else
        {
            this->scrollContentFrameSet(t);
        }
    }
}

ZFMETHOD_DEFINE_1(ZFUIScrollView, void, scrollAreaMarginAdd,
                  ZFMP_IN(const ZFUIMargin &, margin))
{
    if(margin != ZFUIMarginZero())
    {
        ZFUIMarginInc(d->scrollAreaMargin, d->scrollAreaMargin, margin);
        this->layoutRequest();
        this->scrollAreaMarginOnChange();
    }
}
ZFMETHOD_DEFINE_1(ZFUIScrollView, void, scrollAreaMarginRemove,
                  ZFMP_IN(const ZFUIMargin &, margin))
{
    if(margin != ZFUIMarginZero())
    {
        ZFUIMarginDec(d->scrollAreaMargin, d->scrollAreaMargin, margin);
        this->layoutRequest();
        this->scrollAreaMarginOnChange();
    }
}
ZFMETHOD_DEFINE_0(ZFUIScrollView, const ZFUIMargin &, scrollAreaMargin)
{
    return d->scrollAreaMargin;
}

ZFMETHOD_DEFINE_0(ZFUIScrollView, const ZFUIRect &, scrollArea)
{
    return d->scrollArea;
}

ZFMETHOD_DEFINE_1(ZFUIScrollView, void, scrollThumbHorizontalClassSet,
                  ZFMP_IN(const ZFClass *, cls))
{
    if(cls == zfnull || !cls->classIsTypeOf(ZFUIScrollThumb::ClassData()))
    {
        cls = zfnull;
    }

    d->xScrollThumbClass = cls;
    if(d->xScrollThumb != zfnull)
    {
        d->xScrollThumb->scrollThumbDealloc();
        zfRelease(d->xScrollThumb);
        d->xScrollThumb = zfnull;
    }
    if(cls != zfnull)
    {
        d->xScrollThumb = zfRetain(ZFCastZFObject(ZFUIScrollThumb *, cls->newInstance().toObject()));
        if(d->xScrollThumb != zfnull)
        {
            d->xScrollThumb->_scrollView = this;
            d->xScrollThumb->_horizontal = zftrue;
            d->xScrollThumb->scrollThumbInit();
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIScrollView, const ZFClass *, scrollThumbHorizontalClass)
{
    return d->xScrollThumbClass;
}
ZFMETHOD_DEFINE_1(ZFUIScrollView, void, scrollThumbVerticalClassSet,
                  ZFMP_IN(const ZFClass *, cls))
{
    if(cls == zfnull || !cls->classIsTypeOf(ZFUIScrollThumb::ClassData()))
    {
        cls = zfnull;
    }

    d->yScrollThumbClass = cls;
    if(d->yScrollThumb != zfnull)
    {
        d->yScrollThumb->scrollThumbDealloc();
        zfRelease(d->yScrollThumb);
        d->yScrollThumb = zfnull;
    }
    if(cls != zfnull)
    {
        d->yScrollThumb = zfRetain(ZFCastZFObject(ZFUIScrollThumb *, cls->newInstance().toObject()));
        if(d->yScrollThumb != zfnull)
        {
            d->yScrollThumb->_scrollView = this;
            d->yScrollThumb->_horizontal = zffalse;
            d->yScrollThumb->scrollThumbInit();
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIScrollView, const ZFClass *, scrollThumbVerticalClass)
{
    return d->yScrollThumbClass;
}

void ZFUIScrollView::scrollThumbHorizontalOnInit(void)
{
    this->scrollThumbHorizontalClassSet(ZFUIScrollThumbHorizontalClass());
}
void ZFUIScrollView::scrollThumbVerticalOnInit(void)
{
    this->scrollThumbVerticalClassSet(ZFUIScrollThumbVerticalClass());
}

// ============================================================
// scroll control
ZFMETHOD_DEFINE_0(ZFUIScrollView, const ZFClass *, scrollerClass)
{
    return ZFUIScrollerClass();
}

ZFMETHOD_DEFINE_1(ZFUIScrollView, void, scrollContentFrameSetAnimated,
                  ZFMP_IN(const ZFUIRect &, scrollContentFrame))
{
    this->scrollContentFrameSetWhileAnimating(ZFUIRectMake(
        this->scrollContentFrame().point.x,
        this->scrollContentFrame().point.y,
        scrollContentFrame.size.width,
        scrollContentFrame.size.height));
    if(d->state != ZFUIScrollViewState::e_Dragging)
    {
        this->scrollByPoint(scrollContentFrame.point.x, scrollContentFrame.point.y);
    }
    else
    {
        if(d->scrollOverrideFlag == 0)
        {
            this->scrollOnScrolledByUser();
        }
    }
}
ZFMETHOD_DEFINE_1(ZFUIScrollView, void, scrollContentFrameSetWhileAnimating,
                  ZFMP_IN(const ZFUIRect &, scrollContentFrame))
{
    d->xScroll->scrollContentChanged(scrollContentFrame.point.x, scrollContentFrame.size.width);
    d->yScroll->scrollContentChanged(scrollContentFrame.point.y, scrollContentFrame.size.height);
    d->scrollerUpdate();
}
ZFMETHOD_DEFINE_2(ZFUIScrollView, void, scrollByPoint,
                  ZFMP_IN(zfint, xPos),
                  ZFMP_IN(zfint, yPos))
{
    if(d->state != ZFUIScrollViewState::e_Dragging)
    {
        d->xScroll->scrollByPoint(xPos);
        d->yScroll->scrollByPoint(yPos);
        d->scrollerUpdate();
    }
    this->scrollOverrideSet(zftrue);
    this->autoScrollStopX();
    this->autoScrollStopY();
    this->scrollOverrideSet(zffalse);

    if(d->scrollOverrideFlag == 0)
    {
        this->scrollOnScrolledByUser();
    }
}
ZFMETHOD_DEFINE_0(ZFUIScrollView, ZFUIPoint, scrollByPointEndPoint)
{
    return ZFUIPointMake(d->xScroll->scrollByPointEndPoint(), d->yScroll->scrollByPointEndPoint());
}
ZFMETHOD_DEFINE_2(ZFUIScrollView, void, scrollBySpeed,
                  ZFMP_IN(zfint, xSpeedInPixelsPerSecond),
                  ZFMP_IN(zfint, ySpeedInPixelsPerSecond))
{
    if(d->state != ZFUIScrollViewState::e_Dragging)
    {
        d->xScroll->scrollBySpeed(xSpeedInPixelsPerSecond);
        d->yScroll->scrollBySpeed(ySpeedInPixelsPerSecond);
        d->scrollerUpdate();
    }

    if(d->scrollOverrideFlag == 0)
    {
        this->scrollOnScrolledByUser();
    }
}
ZFMETHOD_DEFINE_0(ZFUIScrollView, zfint, scrollBySpeedCurrentSpeedX)
{
    return d->xScroll->scrollBySpeedCurrentSpeed();
}
ZFMETHOD_DEFINE_0(ZFUIScrollView, zfint, scrollBySpeedCurrentSpeedY)
{
    return d->yScroll->scrollBySpeedCurrentSpeed();
}
ZFMETHOD_DEFINE_0(ZFUIScrollView, ZFUIPoint, scrollBySpeedEndPointPredicted)
{
    return ZFUIPointMake(d->xScroll->scrollBySpeedEndPointPredicted(), d->yScroll->scrollBySpeedEndPointPredicted());
}

ZFMETHOD_DEFINE_0(ZFUIScrollView, ZFUIPoint, scrollEndPointPredicted)
{
    return ZFUIPointMake(d->xScroll->scrollEndPointPredicted(), d->yScroll->scrollEndPointPredicted());
}

ZFMETHOD_DEFINE_1(ZFUIScrollView, void, autoScrollStartX,
                  ZFMP_IN(zfint, speedInPixelsPerSecond))
{
    if(d->state != ZFUIScrollViewState::e_Dragging)
    {
        d->autoScrollSpeedX = speedInPixelsPerSecond;
        if(d->xScrollEnable)
        {
            d->xScroll->scrollBySpeed(speedInPixelsPerSecond);
        }
    }

    if(d->scrollOverrideFlag == 0)
    {
        this->scrollOnScrolledByUser();
    }
}
ZFMETHOD_DEFINE_1(ZFUIScrollView, void, autoScrollStartY,
                  ZFMP_IN(zfint , speedInPixelsPerSecond))
{
    if(d->state != ZFUIScrollViewState::e_Dragging)
    {
        d->autoScrollSpeedY = speedInPixelsPerSecond;
        if(d->yScrollEnable)
        {
            d->yScroll->scrollBySpeed(speedInPixelsPerSecond);
        }
    }

    if(d->scrollOverrideFlag == 0)
    {
        this->scrollOnScrolledByUser();
    }
}
ZFMETHOD_DEFINE_0(ZFUIScrollView, void, autoScrollStopX)
{
    if(d->autoScrollSpeedX != 0)
    {
        d->autoScrollSpeedX = 0;
        d->xScroll->scrollStop();
        d->scrollerUpdate();

        if(d->scrollOverrideFlag == 0)
        {
            this->scrollOnScrolledByUser();
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIScrollView, void, autoScrollStopY)
{
    if(d->autoScrollSpeedY != 0)
    {
        d->autoScrollSpeedY = 0;
        d->yScroll->scrollStop();
        d->scrollerUpdate();

        if(d->scrollOverrideFlag == 0)
        {
            this->scrollOnScrolledByUser();
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIScrollView, zfint, autoScrollSpeedX)
{
    return d->autoScrollSpeedX;
}
ZFMETHOD_DEFINE_0(ZFUIScrollView, zfint, autoScrollSpeedY)
{
    return d->autoScrollSpeedY;
}

ZFMETHOD_DEFINE_2(ZFUIScrollView, void, scrollSimulateDragBegin,
                  ZFMP_IN(const ZFUIPoint &, mousePos),
                  ZFMP_IN(zftimet, mouseTime))
{
    this->_ZFP_ZFUIScrollView_notifyDragBegin(mousePos, mouseTime);
}
ZFMETHOD_DEFINE_2(ZFUIScrollView, void, scrollSimulateDrag,
                  ZFMP_IN(const ZFUIPoint &, mousePos),
                  ZFMP_IN(zftimet, mouseTime))
{
    this->_ZFP_ZFUIScrollView_notifyDrag(mousePos, mouseTime);
}
ZFMETHOD_DEFINE_2(ZFUIScrollView, void, scrollSimulateDragEnd,
                  ZFMP_IN(zftimet, mouseTime),
                  ZFMP_IN_OPT(zfbool, needScrollAni, zftrue))
{
    this->_ZFP_ZFUIScrollView_notifyDragEnd(mouseTime, needScrollAni);
}

void ZFUIScrollView::scrollOverrideSet(ZF_IN zfbool scrollOverride)
{
    if(scrollOverride)
    {
        ++(d->scrollOverrideFlag);
    }
    else
    {
        --(d->scrollOverrideFlag);
        zfCoreAssert(d->scrollOverrideFlag >= 0);
    }
}
zfbool ZFUIScrollView::scrollOverride(void)
{
    return (d->scrollOverrideFlag > 0);
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFUIScrollView, ZFUIScrollViewStateEnum, scrollViewState)
{
    return d->state;
}

// ============================================================
// scroll callbacks
void ZFUIScrollView::_ZFP_ZFUIScrollView_notifyDragBegin(ZF_IN const ZFUIPoint &mousePos,
                                                         ZF_IN zftimet mouseTime)
{
    this->scrollOverrideSet(zftrue);
    this->autoScrollStopX();
    this->autoScrollStopY();
    this->scrollOverrideSet(zffalse);
    d->processDragBegin(mousePos, mouseTime);

    if(d->scrollOverrideFlag == 0)
    {
        this->scrollOnScrolledByUser();
    }
}
void ZFUIScrollView::_ZFP_ZFUIScrollView_notifyDrag(ZF_IN const ZFUIPoint &mousePos,
                                                    ZF_IN zftimet mouseTime)
{
    this->scrollOverrideSet(zftrue);
    this->autoScrollStopX();
    this->autoScrollStopY();
    this->scrollOverrideSet(zffalse);
    d->processDrag(mousePos, mouseTime);
}
void ZFUIScrollView::_ZFP_ZFUIScrollView_notifyDragEnd(ZF_IN zftimet mouseTime,
                                                       ZF_IN zfbool needScrollAni)
{
    this->scrollOverrideSet(zftrue);
    this->autoScrollStopX();
    this->autoScrollStopY();
    this->scrollOverrideSet(zffalse);
    d->processDragEnd(mouseTime, needScrollAni);
}
void ZFUIScrollView::_ZFP_ZFUIScrollView_notifyScrollAnimation(ZF_IN zftimet relativeTimeInMiliseconds)
{
    d->processScrollAnimation(relativeTimeInMiliseconds);
}

ZF_NAMESPACE_GLOBAL_END

