/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZF2048AppSettingDialog.h"
#include "ZF2048AppButton.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZF2048AppDataWidthMin ((zfindex)2)
#define _ZFP_ZF2048AppDataWidthMax ((zfindex)10)

zfclass _ZFP_ZF2048AppSettingDialogPrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZF2048AppSettingDialogPrivate, ZFObject)

public:
    ZF2048AppSettingDialog *owner;

public:
    ZFPROPERTY_RETAIN(ZFUIView *, contentView)

public:
    ZFUITextView *dataWidthHint;
    ZFUIButtonBasic *dataWidthIncrease;
    ZFUITextEdit *dataWidthEdit;
    ZFUIButtonBasic *dataWidthDecrease;

    ZFUITextView *dataHeightHint;
    ZFUIButtonBasic *dataHeightIncrease;
    ZFUITextEdit *dataHeightEdit;
    ZFUIButtonBasic *dataHeightDecrease;

    ZFUIButtonBasic *confirmButton;
    ZFUIButtonBasic *cancelButton;

public:
    static zfindex readData(ZF_IN ZFUITextEdit *edit)
    {
        zfindex ret = 4;
        zfindexFromString(ret, edit->text());
        if(ret < _ZFP_ZF2048AppDataWidthMin || ret > _ZFP_ZF2048AppDataWidthMax)
        {
            ret = 4;
        }
        return ret;
    }

protected:
    virtual void objectOnInit(ZF_IN ZF2048AppSettingDialog *owner)
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

        this->loadContent();
        this->prepareLogic();

        this->owner->styleKeySet("dialogTitle", "ZF2048SettingDialog_title");

        this->dataWidthHint->styleKeySet("text", "ZF2048SettingDialog_widthHint");
        this->dataHeightHint->styleKeySet("text", "ZF2048SettingDialog_heightHint");

        this->dataWidthIncrease->buttonLabelStyleNormal()->styleKeySet("text", "ZF2048SettingDialog_dataIncrease");
        this->dataWidthDecrease->buttonLabelStyleNormal()->styleKeySet("text", "ZF2048SettingDialog_dataDecrease");

        this->dataHeightIncrease->buttonLabelStyleNormal()->styleKeySet("text", "ZF2048SettingDialog_dataIncrease");
        this->dataHeightDecrease->buttonLabelStyleNormal()->styleKeySet("text", "ZF2048SettingDialog_dataDecrease");

        this->confirmButton->buttonLabelStyleNormal()->styleKeySet("text", "ZF2048SettingDialog_confirm");
        this->cancelButton->buttonLabelStyleNormal()->styleKeySet("text", "ZF2048SettingDialog_cancel");
    }

private:
    void loadContent(void)
    {
        if(zftrue)
        { // load from xml
            ZFCoreStatisticInvokeTimeAccurateLoggerOneTime("ZF2048_serializePerformanceTest");

            ZFInput input = ZFInputForResFile("ZF2048/app/settingdialog.xml");
            ZFSerializableData data = ZFSerializableDataFromXml(input);
            zfautoObject obj = ZFObjectFromData(data);
            this->contentViewSet(obj);
            zfCoreAssert(this->contentView() != zfnull);
        }
        else
        { // load by code
            zfblockedAlloc(ZFUILinearLayout, mainView);
            this->contentViewSet(mainView);
            mainView->layoutOrientationSet(ZFUIOrientation::e_Top);
            mainView->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

            zfblockedAlloc(ZFUILinearLayout, settingContainer);
            mainView->childAdd(settingContainer);
            settingContainer->layoutOrientationSet(ZFUIOrientation::e_Left);
            settingContainer->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

            {
                zfblockedAlloc(ZFUITextView, dataWidthHintTmp);
                settingContainer->childAdd(dataWidthHintTmp);
                dataWidthHintTmp->viewIdSet("dataWidthHint");

                zfblockedAlloc(ZFUILinearLayout, dataWidthContainerTmp);
                settingContainer->childAdd(dataWidthContainerTmp);
                dataWidthContainerTmp->layoutOrientationSet(ZFUIOrientation::e_Top);

                zfblockedAlloc(ZF2048AppButton, dataWidthIncreaseTmp);
                dataWidthContainerTmp->childAdd(dataWidthIncreaseTmp);
                dataWidthIncreaseTmp->viewIdSet("dataWidthIncrease");

                zfblockedAlloc(ZFUITextEditWidget, dataWidthEditTmp);
                dataWidthContainerTmp->childAdd(dataWidthEditTmp);
                dataWidthEditTmp->viewIdSet("dataWidthEdit");

                zfblockedAlloc(ZF2048AppButton, dataWidthDecreaseTmp);
                dataWidthContainerTmp->childAdd(dataWidthDecreaseTmp);
                dataWidthDecreaseTmp->viewIdSet("dataWidthDecrease");
            }
            {
                zfblockedAlloc(ZFUITextView, dataHeightHintTmp);
                settingContainer->childAdd(dataHeightHintTmp);
                dataHeightHintTmp->viewIdSet("dataHeightHint");

                zfblockedAlloc(ZFUILinearLayout, dataHeightContainerTmp);
                settingContainer->childAdd(dataHeightContainerTmp);
                dataHeightContainerTmp->layoutOrientationSet(ZFUIOrientation::e_Top);

                zfblockedAlloc(ZF2048AppButton, dataHeightIncreaseTmp);
                dataHeightContainerTmp->childAdd(dataHeightIncreaseTmp);
                dataHeightIncreaseTmp->viewIdSet("dataHeightIncrease");

                zfblockedAlloc(ZFUITextEditWidget, dataHeightEditTmp);
                dataHeightContainerTmp->childAdd(dataHeightEditTmp);
                dataHeightEditTmp->viewIdSet("dataHeightEdit");

                zfblockedAlloc(ZF2048AppButton, dataHeightDecreaseTmp);
                dataHeightContainerTmp->childAdd(dataHeightDecreaseTmp);
                dataHeightDecreaseTmp->viewIdSet("dataHeightDecrease");
            }

            zfblockedAlloc(ZFUILinearLayout, buttonLayout);
            mainView->childAdd(buttonLayout);
            buttonLayout->layoutChildSpaceSet(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

            zfblockedAlloc(ZF2048AppButton, confirmButtonTmp);
            buttonLayout->childAdd(confirmButtonTmp);
            confirmButtonTmp->viewIdSet("confirmButton");

            zfblockedAlloc(ZF2048AppButton, cancelButtonTmp);
            buttonLayout->childAdd(cancelButtonTmp);
            cancelButtonTmp->viewIdSet("cancelButton");
        }

        this->dataWidthHint = this->contentView()->childFindById("dataWidthHint")->toAny();
        this->dataWidthIncrease = this->contentView()->childFindById("dataWidthIncrease")->toAny();
        this->dataWidthEdit = this->contentView()->childFindById("dataWidthEdit")->toAny();
        this->dataWidthDecrease = this->contentView()->childFindById("dataWidthDecrease")->toAny();

        this->dataHeightHint = this->contentView()->childFindById("dataHeightHint")->toAny();
        this->dataHeightIncrease = this->contentView()->childFindById("dataHeightIncrease")->toAny();
        this->dataHeightEdit = this->contentView()->childFindById("dataHeightEdit")->toAny();
        this->dataHeightDecrease = this->contentView()->childFindById("dataHeightDecrease")->toAny();

        this->confirmButton = this->contentView()->childFindById("confirmButton")->toAny();
        this->cancelButton = this->contentView()->childFindById("cancelButton")->toAny();
    }
    void prepareLogic(void)
    {
        ZFLISTENER_LOCAL(increaseOnClick, {
            ZFUIButtonBasic *increase = userData->tagGet("increase")->objectHolded();
            ZFUITextEdit *edit = userData->tagGet("edit")->objectHolded();
            ZFUIButtonBasic *decrease = userData->tagGet("decrease")->objectHolded();
            zfindex value = _ZFP_ZF2048AppSettingDialogPrivate::readData(edit);
            ++value;
            value = zfmApplyRange<zfindex>(value, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
            edit->textSet(zfindexToString(value));
            increase->buttonEnableSet(value != _ZFP_ZF2048AppDataWidthMax);
            decrease->buttonEnableSet(value != _ZFP_ZF2048AppDataWidthMin);
        })
        ZFLISTENER_LOCAL(decreaseOnClick, {
            ZFUIButtonBasic *increase = userData->tagGet("increase")->objectHolded();
            ZFUITextEdit *edit = userData->tagGet("edit")->objectHolded();
            ZFUIButtonBasic *decrease = userData->tagGet("decrease")->objectHolded();
            zfindex value = _ZFP_ZF2048AppSettingDialogPrivate::readData(edit);
            --value;
            value = zfmApplyRange<zfindex>(value, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
            edit->textSet(zfindexToString(value));
            increase->buttonEnableSet(value != _ZFP_ZF2048AppDataWidthMax);
            decrease->buttonEnableSet(value != _ZFP_ZF2048AppDataWidthMin);
        })
        ZFLISTENER_LOCAL(editOnChange, {
            ZFUIButtonBasic *increase = userData->tagGet("increase")->objectHolded();
            ZFUITextEdit *edit = userData->tagGet("edit")->objectHolded();
            ZFUIButtonBasic *decrease = userData->tagGet("decrease")->objectHolded();
            zfindex value = _ZFP_ZF2048AppSettingDialogPrivate::readData(edit);
            value = zfmApplyRange<zfindex>(value, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
            increase->buttonEnableSet(value != _ZFP_ZF2048AppDataWidthMax);
            decrease->buttonEnableSet(value != _ZFP_ZF2048AppDataWidthMin);
        })
        ZFLISTENER_LOCAL(editOnClickReturn, {
            ZFUITextEdit *textEdit = listenerData.sender->toAny();
            textEdit->viewFocusRequest(zffalse);
        })

        zfblockedAlloc(ZFObject, userDataForWidth);
        userDataForWidth->tagSet("increase", this->dataWidthIncrease->objectHolder());
        userDataForWidth->tagSet("edit", this->dataWidthEdit->objectHolder());
        userDataForWidth->tagSet("decrease", this->dataWidthDecrease->objectHolder());

        zfblockedAlloc(ZFObject, userDataForHeight);
        userDataForHeight->tagSet("increase", this->dataHeightIncrease->objectHolder());
        userDataForHeight->tagSet("edit", this->dataHeightEdit->objectHolder());
        userDataForHeight->tagSet("decrease", this->dataHeightDecrease->objectHolder());

        zfblockedAlloc(ZFRegExp, textFilter);
        textFilter->regExpCompile(zfstringWithFormat("\\d{1,2}"));
        this->dataWidthEdit->textEditFilterSet(textFilter);
        this->dataHeightEdit->textEditFilterSet(textFilter);

        this->dataWidthIncrease->observerAdd(ZFUIButton::EventButtonOnClick(), increaseOnClick, userDataForWidth);
        this->dataWidthEdit->observerAdd(ZFUITextEdit::EventTextOnChange(), editOnChange, userDataForWidth);
        this->dataWidthEdit->observerAdd(ZFUITextEdit::EventTextOnReturnClick(), editOnClickReturn, userDataForWidth);
        this->dataWidthDecrease->observerAdd(ZFUIButton::EventButtonOnClick(), decreaseOnClick, userDataForWidth);

        this->dataHeightIncrease->observerAdd(ZFUIButton::EventButtonOnClick(), increaseOnClick, userDataForHeight);
        this->dataHeightEdit->observerAdd(ZFUITextEdit::EventTextOnChange(), editOnChange, userDataForHeight);
        this->dataHeightEdit->observerAdd(ZFUITextEdit::EventTextOnReturnClick(), editOnClickReturn, userDataForHeight);
        this->dataHeightDecrease->observerAdd(ZFUIButton::EventButtonOnClick(), decreaseOnClick, userDataForHeight);

        this->confirmButton->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, confirmOnClick)));
        this->cancelButton->observerAdd(ZFUIButton::EventButtonOnClick(), ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, cancelOnClick)));
    }
    ZFLISTENER_INLINE(confirmOnClick)
    {
        this->owner->dataWidth = zfself::readData(this->dataWidthEdit);
        this->owner->dataHeight = zfself::readData(this->dataHeightEdit);
        this->owner->settingOnChange();
        this->owner->dialogHide();
    }
    ZFLISTENER_INLINE(cancelOnClick)
    {
        this->owner->dialogHide();
    }
};

ZFOBJECT_REGISTER(ZF2048AppSettingDialog)

ZFOBSERVER_EVENT_REGISTER(ZF2048AppSettingDialog, SettingOnChange)

void ZF2048AppSettingDialog::objectOnInit(ZF_IN zfindex dataWidth,
                                          ZF_IN zfindex dataHeight)
{
    this->objectOnInit();
    this->dataWidth = zfmApplyRange(dataWidth, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
    this->dataHeight = zfmApplyRange(dataHeight, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
}
void ZF2048AppSettingDialog::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAlloc(_ZFP_ZF2048AppSettingDialogPrivate, this);
    this->dataWidth = 4;
    this->dataHeight = 4;
    this->dialogContentSet(d->contentView());
}
void ZF2048AppSettingDialog::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    d->dataWidthEdit->textSet(zfindexToString(this->dataWidth));
    d->dataHeightEdit->textSet(zfindexToString(this->dataHeight));
}
void ZF2048AppSettingDialog::objectOnDealloc(void)
{
    zfRetainChange(d, zfnull);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

