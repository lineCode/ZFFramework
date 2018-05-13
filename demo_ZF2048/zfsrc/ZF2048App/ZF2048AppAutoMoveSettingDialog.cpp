/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZF2048AppAutoMoveSettingDialog.h"
#include "ZF2048AppButton.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZF2048AppAutoMoveActionItem : zfextends ZF2048AppButton
{
    ZFOBJECT_DECLARE(_ZFP_ZF2048AppAutoMoveActionItem, ZF2048AppButton)

    ZFPROPERTY_ASSIGN(ZF2048AppAutoMoveActionEnum, actionValue)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZF2048AppAutoMoveActionEnum, actionValue)
    {
        this->buttonLabelStyleNormal()->styleKeySet(zfText("text"),
            zfstringWithFormat(zfText("ZF2048AutoMove_%s"), ZF2048AppAutoMoveAction::EnumNameForValue(propertyValue)));
    }
};

zfclass _ZFP_ZF2048AppAutoMoveSettingDialogPrivate : zfextends ZFUILinearLayout
{
    ZFOBJECT_DECLARE(_ZFP_ZF2048AppAutoMoveSettingDialogPrivate, ZFUILinearLayout)

public:
    ZF2048AppAutoMoveSettingDialog *owner;

    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, hintText, zflineAlloc(ZFUITextView))
    ZFPROPERTY_RETAIN_READONLY(ZFUIListAdapterArray *, actionListAdapter, zflineAlloc(ZFUIListAdapterArray))
    ZFPROPERTY_RETAIN_READONLY(ZFUIListView *, actionList, zflineAlloc(ZFUIListView))
    ZFPROPERTY_RETAIN_READONLY(ZFUILinearLayout *, addActionLayout, zflineAlloc(ZFUILinearLayout))
    ZFPROPERTY_RETAIN_READONLY(_ZFP_ZF2048AppAutoMoveActionItem *, addAction_left, zflineAlloc(_ZFP_ZF2048AppAutoMoveActionItem))
    ZFPROPERTY_RETAIN_READONLY(_ZFP_ZF2048AppAutoMoveActionItem *, addAction_top, zflineAlloc(_ZFP_ZF2048AppAutoMoveActionItem))
    ZFPROPERTY_RETAIN_READONLY(_ZFP_ZF2048AppAutoMoveActionItem *, addAction_right, zflineAlloc(_ZFP_ZF2048AppAutoMoveActionItem))
    ZFPROPERTY_RETAIN_READONLY(_ZFP_ZF2048AppAutoMoveActionItem *, addAction_bottom, zflineAlloc(_ZFP_ZF2048AppAutoMoveActionItem))
    ZFPROPERTY_RETAIN_READONLY(_ZFP_ZF2048AppAutoMoveActionItem *, addAction_random, zflineAlloc(_ZFP_ZF2048AppAutoMoveActionItem))
    ZFPROPERTY_RETAIN_READONLY(ZFUILinearLayout *, buttonLayout, zflineAlloc(ZFUILinearLayout))
    ZFPROPERTY_RETAIN_READONLY(ZF2048AppButton *, confirmButton, zflineAlloc(ZF2048AppButton))
    ZFPROPERTY_RETAIN_READONLY(ZF2048AppButton *, cancelButton, zflineAlloc(ZF2048AppButton))

protected:
    virtual void objectOnInit(ZF_IN ZF2048AppAutoMoveSettingDialog *owner)
    {
        this->objectOnInit();
        this->owner = owner;
    }
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
    }
    zfoverride
    virtual void objectOnInitFinish(void)
    {
        zfsuper::objectOnInitFinish();

        this->layoutOrientationSet(ZFUIOrientation::e_Top);
        this->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

        this->childAdd(this->hintText());
        this->hintText()->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner);
        this->hintText()->styleKeySet(zfText("text"), zfText("ZF2048AutoMoveDialog_hint"));

        this->childAdd(this->actionList());
        this->actionList()->cellSizeHintSet(-1);
        this->actionList()->viewSizeMinSet(ZFUISizeMake(0, 120));
        this->actionList()->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
        this->actionList()->layoutParam()->sizeHintSet(ZFUISizeMake(200, 150));

        this->actionList()->listAdapterSet(this->actionListAdapter());
        this->actionList()->cellUpdater()->add(zflineAlloc(ZFUIListCellUpdaterBasic));

        this->childAdd(this->addActionLayout());
        this->addActionLayout()->layoutOrientationSet(ZFUIOrientation::e_Left);
        this->addActionLayout()->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());
        this->addActionLayout()->childAdd(this->addAction_left());
        this->addActionLayout()->childAdd(this->addAction_top());
        this->addActionLayout()->childAdd(this->addAction_right());
        this->addActionLayout()->childAdd(this->addAction_bottom());
        this->addActionLayout()->childAdd(this->addAction_random());

        this->childAdd(this->buttonLayout());
        this->buttonLayout()->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());
        this->buttonLayout()->layoutOrientationSet(ZFUIOrientation::e_Left);
        this->buttonLayout()->childAdd(this->confirmButton());
        this->confirmButton()->buttonLabelStyleNormal()->styleKeySet(zfText("text"), zfText("ZF2048AutoMoveDialog_confirm"));
        this->buttonLayout()->childAdd(this->cancelButton());
        this->cancelButton()->buttonLabelStyleNormal()->styleKeySet(zfText("text"), zfText("ZF2048AutoMoveDialog_cancel"));

        this->addActionButtonSetup();
        this->buttonSetup();
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfsuper::objectOnDealloc();
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(addActionOnClick)
    {
        ZF2048AppAutoMoveSettingDialog *owner = userData->objectHolded();
        _ZFP_ZF2048AppAutoMoveActionItem *clicked = listenerData.sender->toAny();

        owner->autoMoves.add(clicked->actionValue());
        zfblockedAlloc(_ZFP_ZF2048AppAutoMoveActionItem, actionItem);
        zfblockedAlloc(ZFUIListCell, cell);
        cell->cellViewSet(actionItem);
        owner->d->actionListAdapter()->cellAdd(cell);
        actionItem->actionValueSet(clicked->actionValue());
        actionItem->observerAdd(ZFUIButtonBasic::EventButtonOnClick(), ZFCallbackForFunc(zfself::actionItemOnClick), owner->objectHolder());
        owner->d->actionList()->listReload();

        owner->d->confirmButton()->buttonEnableSet(owner->d->actionListAdapter()->cellCount() > 0);
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(actionItemOnClick)
    {
        ZF2048AppAutoMoveSettingDialog *owner = userData->objectHolded();
        _ZFP_ZF2048AppAutoMoveActionItem *actionItem = listenerData.sender->toAny();

        zfindex index = owner->d->actionListAdapter()->cellArray()->find(actionItem->viewParent(), ZFComparerCheckEqual);
        zfCoreAssert(index != zfindexMax());
        owner->autoMoves.remove(index);
        owner->d->actionListAdapter()->cellRemove(index);
        owner->d->actionList()->listReload();

        owner->d->confirmButton()->buttonEnableSet(owner->d->actionListAdapter()->cellCount() > 0);
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(confirmButtonOnClick)
    {
        ZF2048AppAutoMoveSettingDialog *owner = userData->objectHolded();
        owner->autoMoveSettingOnChange();
        owner->dialogHide();
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(cancelButtonOnClick)
    {
        ZF2048AppAutoMoveSettingDialog *owner = userData->objectHolded();
        owner->dialogHide();
    }
private:
    void addActionButtonSetup(void)
    {
        ZFListener onClickListener = ZFCallbackForFunc(zfself::addActionOnClick);
        ZFObjectHolder *ownerHolder = this->owner->objectHolder();
        this->addAction_left()->actionValueSet(ZF2048AppAutoMoveAction::e_Left);
        this->addAction_left()->observerAdd(ZFUIButton::EventButtonOnClick(), onClickListener, ownerHolder);
        this->addAction_top()->actionValueSet(ZF2048AppAutoMoveAction::e_Top);
        this->addAction_top()->observerAdd(ZFUIButton::EventButtonOnClick(), onClickListener, ownerHolder);
        this->addAction_right()->actionValueSet(ZF2048AppAutoMoveAction::e_Right);
        this->addAction_right()->observerAdd(ZFUIButton::EventButtonOnClick(), onClickListener, ownerHolder);
        this->addAction_bottom()->actionValueSet(ZF2048AppAutoMoveAction::e_Bottom);
        this->addAction_bottom()->observerAdd(ZFUIButton::EventButtonOnClick(), onClickListener, ownerHolder);
        this->addAction_random()->actionValueSet(ZF2048AppAutoMoveAction::e_Random);
        this->addAction_random()->observerAdd(ZFUIButton::EventButtonOnClick(), onClickListener, ownerHolder);
    }
    void buttonSetup(void)
    {
        this->confirmButton()->buttonEnableSet(zffalse);
        this->confirmButton()->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForFunc(zfself::confirmButtonOnClick), this->owner->objectHolder());
        this->cancelButton()->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForFunc(zfself::cancelButtonOnClick), this->owner->objectHolder());
    }
};

ZFOBJECT_REGISTER(ZF2048AppAutoMoveSettingDialog)

ZFOBSERVER_EVENT_REGISTER(ZF2048AppAutoMoveSettingDialog, AutoMoveSettingOnChange)

void ZF2048AppAutoMoveSettingDialog::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAlloc(_ZFP_ZF2048AppAutoMoveSettingDialogPrivate, this);
    this->dialogContentSet(d);
    this->styleKeySet(zfText("dialogTitle"), zfText("ZF2048_autoMove"));

    d->addAction_left()->buttonSimulateClick();
    d->addAction_top()->buttonSimulateClick();
    d->addAction_left()->buttonSimulateClick();
    d->addAction_bottom()->buttonSimulateClick();
}
void ZF2048AppAutoMoveSettingDialog::objectOnDealloc(void)
{
    zfRetainChange(d, zfnull);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

