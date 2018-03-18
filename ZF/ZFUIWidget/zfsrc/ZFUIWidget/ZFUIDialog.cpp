/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIDialog.h"
#include "ZFUIButton.h"
#include "ZFUIViewLayout.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFObjectCreator ZFUIDialogDefaultLayoutParamCreator = ZFUIDialogDefaultLayoutParamCreatorDefault;
zfautoObject ZFUIDialogDefaultLayoutParamCreatorDefault(void)
{
    zfblockedAlloc(ZFUIViewLayoutParam, lp);
    lp->layoutAlignSet(ZFUIAlign::e_Center);
    lp->layoutMarginSet(ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemMargin()));
    return lp;
}
ZFObjectCreator ZFUIDialogDefaultAniShowCreator = ZFUIDialogDefaultAniShowCreatorDefault;
zfautoObject ZFUIDialogDefaultAniShowCreatorDefault(void)
{
    zfblockedAlloc(ZFAnimationNativeView, ani);
    ani->aniAlphaFromSet(0);
    return ani;
}
ZFObjectCreator ZFUIDialogDefaultAniHideCreator = ZFUIDialogDefaultAniHideCreatorDefault;
zfautoObject ZFUIDialogDefaultAniHideCreatorDefault(void)
{
    zfblockedAlloc(ZFAnimationNativeView, ani);
    ani->aniAlphaToSet(0);
    return ani;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogDefaultCreatorReset, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIDialogDefaultCreatorReset)
{
    ZFUIDialogDefaultLayoutParamCreator = ZFUIDialogDefaultLayoutParamCreatorDefault;
    ZFUIDialogDefaultAniShowCreator = ZFUIDialogDefaultAniShowCreatorDefault;
    ZFUIDialogDefaultAniHideCreator = ZFUIDialogDefaultAniHideCreatorDefault;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogDefaultCreatorReset)

// ============================================================
// ZFUIDialog
ZFSTYLE_DEFAULT_DEFINE(ZFUIDialog)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogAllDialog, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIDialogAllDialog)
{
    ZFCoreArrayPOD<ZFUIDialog *> tmp;
    tmp.copyFrom(this->allDialog);
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->dialogShowing())
        {
            tmp[i]->dialogHideForce();
        }
    }
}
public:
    ZFCoreArrayPOD<ZFUIDialog *> allDialog;
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogAllDialog)
#define _ZFP_ZFUIDialogAllDialog ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIDialogAllDialog)->allDialog

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIDialogAutoHide, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIDialogAutoHide)
{
    ZFCoreArrayPOD<ZFUIDialog *> tmp;
    tmp.copyFrom(_ZFP_ZFUIDialogAllDialog);
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->dialogShowing())
        {
            tmp[i]->dialogHideForce();
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIDialogAutoHide)

// ============================================================
// _ZFP_I_ZFUIDialogPrivate
/*
 * memo:
 *
 * _ZFP_I_ZFUIDialogPrivate (ZFUIWindow, handle all key event)
 * ^ dialogWindowBg (internal bg, window ani target)
 *   dialogClickMask (internal bg, focused if dialogContainer has no focusable child)
 *   _ZFP_ZFUIDialogContentHolder (Ui disable, supply layout logic)
 *   ^ dialogBg
 *     ^ dialogContainer (dialogInternalContainer)
 *       ^ dialogView
 *         subclass impl views
 */
zfclass _ZFP_I_ZFUIDialogPrivate : zfextends ZFUIWindow
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIDialogPrivate, ZFUIWindow)

public:
    ZFUIDialog *pimplOwner;
    ZFUIView *dialogWindowBg;
    ZFAnimationNativeView *dialogWindowAniShow;
    ZFAnimationNativeView *dialogWindowAniHide;
    ZFUIButton *dialogClickMask;
    ZFUIImageView *dialogBg;
    ZFUIView *dialogContainer;
    ZFListener aniShowOnStopListener;
    ZFListener aniHideOnStopListener;

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->windowLevelSet(ZFUIWindowLevel::e_AppHigh);
        this->sysWindowMarginShouldApplySet(zffalse);
    }
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds)
    {
        if(this->pimplOwner->sysWindowMarginShouldApply())
        {
            zfsuper::layoutOnLayout(ZFUIRectApplyMargin(bounds, this->windowOwnerSysWindow()->sysWindowMargin()));
        }
        else
        {
            zfsuper::layoutOnLayout(bounds);
        }
    }

public:
    void aniStop(void)
    {
        if(this->dialogWindowAniShow != zfnull)
        {
            this->dialogWindowAniShow->aniStop();
        }
        if(this->dialogWindowAniHide != zfnull)
        {
            this->dialogWindowAniHide->aniStop();
        }
        if(this->pimplOwner->dialogAniShow() != zfnull)
        {
            this->pimplOwner->dialogAniShow()->aniStop();
        }
        if(this->pimplOwner->dialogAniHide() != zfnull)
        {
            this->pimplOwner->dialogAniHide()->aniStop();
        }
    }

public:
    ZFLISTENER_INLINE(dialogClickMaskOnClick)
    {
        if(this->pimplOwner->dialogHideWhenTouchOutside())
        {
            this->pimplOwner->dialogHide();
        }
    }
    ZFLISTENER_INLINE(aniShowOnStop)
    {
        if(!this->dialogWindowAniShow->aniRunning()
            && !this->dialogWindowAniHide->aniRunning()
            && (this->pimplOwner->dialogAniShow() == zfnull || !this->pimplOwner->dialogAniShow()->aniRunning())
            && (this->pimplOwner->dialogAniHide() == zfnull || !this->pimplOwner->dialogAniHide()->aniRunning())
            )
        {
            this->viewUIEnableTreeSet(zftrue);
            ZFUIViewBlinkWhenFocusAutoApplyPause();
            if(this->pimplOwner->dialogFocusAutomatically())
            {
                ZFUIView *v = this->pimplOwner->dialogFocusOnUpdate();
                if(this->pimplOwner->observerHasAdd(ZFUIDialog::EventDialogFocusOnUpdate()))
                {
                    zfblockedAlloc(ZFObjectHolder, param0, v);
                    this->pimplOwner->observerNotify(ZFUIDialog::EventDialogFocusOnUpdate(), param0);
                }
                if(v != zfnull)
                {
                    v->viewFocusRequest(zftrue);
                }
                else
                {
                    this->dialogClickMask->viewFocusRequest(zftrue);
                }
            }
            else
            {
                this->dialogClickMask->viewFocusRequest(zftrue);
            }
            ZFUIViewBlinkWhenFocusAutoApplyResume();
            this->pimplOwner->dialogAfterShow();
        }
    }
    ZFLISTENER_INLINE(aniHideOnStop)
    {
        if(!this->dialogWindowAniShow->aniRunning()
            && !this->dialogWindowAniHide->aniRunning()
            && (this->pimplOwner->dialogAniShow() == zfnull || !this->pimplOwner->dialogAniShow()->aniRunning())
            && (this->pimplOwner->dialogAniHide() == zfnull || !this->pimplOwner->dialogAniHide()->aniRunning())
            )
        {
            this->viewUIEnableTreeSet(zftrue);
            this->windowHide();
            this->pimplOwner->dialogAfterHide();
            zfRelease(this->pimplOwner);
        }
    }

public:
    void layoutDialogPrepare(ZF_IN const ZFUIRect &bounds)
    {
        if(this->pimplOwner->dialogBackgroundImage() != zfnull)
        {
            this->dialogContainer->layoutParam()->layoutMarginSet(this->pimplOwner->dialogBackgroundImage()->imageNinePatch());
        }
    }
    void layoutDialog(ZF_IN const ZFUIRect &bounds)
    {
        ZFUIMargin dialogMargin = this->pimplOwner->dialogLayoutParam()->layoutMargin();
        const ZFUIMargin &contentMargin = this->dialogContainer->layoutParam()->layoutMargin();

        ZFUIMargin totalMargin = ZFUIMarginInc(dialogMargin, contentMargin);

        this->dialogContainer->layoutMeasure(
            ZFUISizeApplyMargin(bounds.size, totalMargin),
            this->pimplOwner->dialogLayoutParam()->sizeParam());
        ZFUISize dialogContainerSize = this->dialogContainer->layoutMeasuredSize();

        ZFUISize dialogBgSize = dialogContainerSize;
        dialogBgSize.width += ZFUIMarginGetWidth(contentMargin);
        dialogBgSize.height += ZFUIMarginGetHeight(contentMargin);
        this->dialogBg->layout(ZFUIAlignApply(
            this->pimplOwner->dialogLayoutParam()->layoutAlign(),
            bounds,
            dialogBgSize,
            dialogMargin));
    }

    // event handle
protected:
    zfoverride
    virtual void viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
    {
        // dialog would always resolve key event
        keyEvent->eventResolvedSet(zftrue);

        if(!this->pimplOwner->dialogHideWhenClickBack()
            || keyEvent->keyAction != ZFUIKeyAction::e_KeyUp)
        {
            return ;
        }

        switch(keyEvent->keyCode)
        {
            case ZFUIKeyCode::e_kPhoneBack:
            case ZFUIKeyCode::e_kEsc:
                this->pimplOwner->dialogHide();
                break;
            default:
                break;
        }
    }
};
ZFOBJECT_REGISTER(_ZFP_I_ZFUIDialogPrivate)

zfclass _ZFP_ZFUIDialogContentHolder : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(_ZFP_ZFUIDialogContentHolder, ZFUIView)

public:
    _ZFP_I_ZFUIDialogPrivate *pimplOwner;

protected:
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
    {
        this->pimplOwner->layoutDialogPrepare(bounds);
    }
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds)
    {
        this->pimplOwner->layoutDialog(bounds);
    }
};

zfclass _ZFP_I_ZFUIDialog_DialogClickMask : zfextends ZFUIButton
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIDialog_DialogClickMask, ZFUIButton)
};

// ============================================================
// ZFUIDialog
ZFOBJECT_REGISTER(ZFUIDialog)

ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogBeforeShow)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogAfterShow)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogBeforeHide)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogAfterHide)
ZFOBSERVER_EVENT_REGISTER(ZFUIDialog, DialogFocusOnUpdate)

ZFPROPERTY_OVERRIDE_ON_UPDATE_DEFINE(ZFUIDialog, ZFUIColor, dialogWindowColor)
{
    d->dialogWindowBg->viewBackgroundColorSet(this->dialogWindowColor());
}
ZFPROPERTY_OVERRIDE_ON_UPDATE_DEFINE(ZFUIDialog, ZFUIImage *, dialogBackgroundImage)
{
    d->dialogBg->imageSet(this->dialogBackgroundImage());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIDialog, ZFUIView *, dialogView)
{
    if(this->dialogView() != zfnull)
    {
        d->dialogContainer->childAdd(this->dialogView());
        this->dialogView()->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
    }
}
ZFPROPERTY_OVERRIDE_ON_DETACH_DEFINE(ZFUIDialog, ZFUIView *, dialogView)
{
    if(this->dialogView() != zfnull)
    {
        d->dialogContainer->childRemove(this->dialogView());
    }
}

ZFMETHOD_DEFINE_0(ZFUIDialog, zfbool, dialogShowing)
{
    return d->windowShowing();
}
ZFMETHOD_DEFINE_0(ZFUIDialog, void, dialogShow)
{
    if(d->windowShowing())
    {
        return ;
    }
    zfRetain(this);
    d->aniStop();
    d->windowShow();

    if(this->dialogWindowAutoResize())
    {
        ZFUIOnScreenKeyboardAutoResizeStart(this->dialogWindow());
    }
    if(this->dialogWindowAutoFit())
    {
        ZFUIOnScreenKeyboardAutoFitStart(this->dialogWindow());
    }

    d->viewUIEnableTreeSet(zffalse);
    this->dialogBeforeShow();
    if(this->dialogWindowColor() != ZFUIColorTransparent() && this->dialogWindowAutoDim())
    {
        d->dialogWindowAniShow->aniTargetSet(d->dialogWindowBg);
        d->dialogWindowAniShow->observerAdd(ZFObserverAddParam()
            .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
            .observerSet(d->aniShowOnStopListener)
            .autoRemoveAfterActivateSet(zftrue)
        );
        d->dialogWindowAniShow->aniStart();
    }
    if(this->dialogAniShow() != zfnull)
    {
        this->dialogAniShow()->aniTargetSet(d->dialogBg);
        this->dialogAniShow()->observerAdd(ZFObserverAddParam()
            .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
            .observerSet(d->aniShowOnStopListener)
            .autoRemoveAfterActivateSet(zftrue)
        );
        this->dialogAniShow()->aniStart();
    }
}
ZFMETHOD_DEFINE_0(ZFUIDialog, void, dialogHide)
{
    if(!d->windowShowing())
    {
        return ;
    }
    d->aniStop();
    if(!d->windowShowing())
    {
        return ;
    }

    if(this->dialogWindowAutoResize())
    {
        ZFUIOnScreenKeyboardAutoResizeStop(this->dialogWindow());
    }
    if(this->dialogWindowAutoFit())
    {
        ZFUIOnScreenKeyboardAutoFitStop(this->dialogWindow());
    }

    d->viewUIEnableTreeSet(zffalse);
    this->dialogBeforeHide();
    if(this->dialogWindowColor() != ZFUIColorTransparent() && this->dialogWindowAutoDim())
    {
        d->dialogWindowAniHide->aniTargetSet(d->dialogWindowBg);
        d->dialogWindowAniHide->observerAdd(ZFObserverAddParam()
            .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
            .observerSet(d->aniHideOnStopListener)
            .autoRemoveAfterActivateSet(zftrue)
        );
        d->dialogWindowAniHide->aniStart();
    }
    if(this->dialogAniHide() != zfnull)
    {
        this->dialogAniHide()->aniTargetSet(d->dialogBg);
        this->dialogAniHide()->observerAdd(ZFObserverAddParam()
            .eventIdSet(ZFAnimation::EventAniOnStopOrOnInvalid())
            .observerSet(d->aniHideOnStopListener)
            .autoRemoveAfterActivateSet(zftrue)
        );
        this->dialogAniHide()->aniStart();
    }
}
ZFMETHOD_DEFINE_0(ZFUIDialog, void, dialogHideForce)
{
    if(d->windowShowing())
    {
        zfRetain(this);
        d->aniStop();
        if(d->windowShowing())
        {
            this->dialogBeforeHide();
            d->windowHide();
            this->dialogAfterHide();
        }
        zfRelease(this);
    }
}

ZFMETHOD_DEFINE_1(ZFUIDialog, void, dialogApplyAutoHide,
                  ZFMP_IN(ZFUIButton *, button))
{
    if(button == zfnull)
    {
        return ;
    }
    ZFLISTENER_LOCAL(buttonOnClick, {
        userData->to<ZFObjectHolder *>()->holdedObj.to<ZFUIDialog *>()->dialogHide();
    })
    button->observerAdd(ZFObserverAddParam()
            .eventIdSet(ZFUIButton::EventButtonOnClick())
            .observerSet(buttonOnClick)
            .userDataSet(this->objectHolder())
            .observerLevelSet(ZFLevelZFFrameworkPostNormal)
        );
}

ZFMETHOD_DEFINE_0(ZFUIDialog, ZFUIWindow *, dialogWindow)
{
    return d;
}

ZFUIView *ZFUIDialog::dialogInternalContainer(void)
{
    return d->dialogContainer;
}
ZFUIView *ZFUIDialog::dialogInternalBackgroundContainer(void)
{
    return d->dialogBg;
}

ZFUIView *ZFUIDialog::dialogFocusOnUpdate(void)
{
    return ZFUIViewFocusNextMove(this->dialogInternalContainer(),
        ZFUIViewFocusNextParam().focusDirectionSet(ZFUIOrientation::e_Left | ZFUIOrientation::e_Top));
}

void ZFUIDialog::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAlloc(_ZFP_I_ZFUIDialogPrivate);
    d->pimplOwner = this;

    d->dialogWindowBg = zfAlloc(ZFUIView);
    d->internalBgViewAdd(d->dialogWindowBg);
    d->dialogWindowBg->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());

    d->dialogClickMask = zfAlloc(_ZFP_I_ZFUIDialog_DialogClickMask);
    d->internalBgViewAdd(d->dialogClickMask);
    d->dialogClickMask->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
    d->dialogClickMask->observerAdd(ZFUIButton::EventButtonOnClick(),
        ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_I_ZFUIDialogPrivate, dialogClickMaskOnClick)));

    zfblockedAlloc(_ZFP_ZFUIDialogContentHolder, dialogContentHolder);
    d->childAdd(dialogContentHolder);
    dialogContentHolder->pimplOwner = d;
    dialogContentHolder->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
    dialogContentHolder->viewUIEnableSet(zffalse);

    d->dialogBg = zfAlloc(ZFUIImageView);
    dialogContentHolder->childAdd(d->dialogBg);
    d->dialogBg->viewUIEnableTreeSet(zftrue);
    d->dialogBg->viewUIEnableSet(zftrue);

    d->dialogContainer = zfAlloc(ZFUIViewLayout);
    d->dialogBg->childAdd(d->dialogContainer);
    d->dialogContainer->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());

    d->dialogWindowAniShow = zfAlloc(ZFAnimationNativeView);
    d->dialogWindowAniShow->aniAlphaFromSet(0);
    d->dialogWindowAniHide = zfAlloc(ZFAnimationNativeView);
    d->dialogWindowAniHide->aniAlphaToSet(0);

    d->aniShowOnStopListener = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_I_ZFUIDialogPrivate, aniShowOnStop));
    d->aniHideOnStopListener = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_I_ZFUIDialogPrivate, aniHideOnStop));

    d->dialogWindowBg->viewBackgroundColorSet(this->dialogWindowColor());
    d->dialogBg->imageSet(this->dialogBackgroundImage());

    _ZFP_ZFUIDialogAllDialog.add(this);
}
void ZFUIDialog::objectOnDealloc(void)
{
    _ZFP_ZFUIDialogAllDialog.removeElement(this);

    zfRetainChange(d->dialogWindowAniShow, zfnull);
    zfRetainChange(d->dialogWindowAniHide, zfnull);

    zfRelease(d->dialogContainer);
    zfRelease(d->dialogBg);
    zfRelease(d->dialogClickMask);
    zfRelease(d->dialogWindowBg);
    zfRelease(d);
    d = zfnull;

    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

