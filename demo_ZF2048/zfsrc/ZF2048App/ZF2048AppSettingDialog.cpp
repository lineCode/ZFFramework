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

        this->owner->styleKeyForProperty("dialogTitle", "ZF2048SettingDialog_title");

        this->dataWidthHint->styleKeyForProperty("text", "ZF2048SettingDialog_widthHint");
        this->dataHeightHint->styleKeyForProperty("text", "ZF2048SettingDialog_heightHint");

        this->dataWidthIncrease->buttonLabelStyleNormal()->styleKeyForProperty("text", "ZF2048SettingDialog_dataIncrease");
        this->dataWidthDecrease->buttonLabelStyleNormal()->styleKeyForProperty("text", "ZF2048SettingDialog_dataDecrease");

        this->dataHeightIncrease->buttonLabelStyleNormal()->styleKeyForProperty("text", "ZF2048SettingDialog_dataIncrease");
        this->dataHeightDecrease->buttonLabelStyleNormal()->styleKeyForProperty("text", "ZF2048SettingDialog_dataDecrease");

        this->confirmButton->buttonLabelStyleNormal()->styleKeyForProperty("text", "ZF2048SettingDialog_confirm");
        this->cancelButton->buttonLabelStyleNormal()->styleKeyForProperty("text", "ZF2048SettingDialog_cancel");
    }

private:
    void loadContent(void)
    {
        if(zftrue)
        { // load from xml
            ZFCoreStatisticInvokeTimeLoggerOneTime("ZF2048_serializePerformanceTest");

            ZFInput input = ZFInputForResFile("ZF2048/app/settingdialog.xml");
            ZFSerializableData data = ZFSerializableDataFromXml(input);
            zfautoObject obj = ZFObjectFromData(data);
            this->contentView(obj);
            zfCoreAssert(this->contentView() != zfnull);
        }
        else
        { // load by code
            zfblockedAlloc(ZFUILinearLayout, mainView);
            this->contentView(mainView);
            mainView->layoutOrientation(ZFUIOrientation::e_Top);
            mainView->layoutChildSpace(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

            zfblockedAlloc(ZFUILinearLayout, settingContainer);
            mainView->childAdd(settingContainer);
            settingContainer->layoutOrientation(ZFUIOrientation::e_Left);
            settingContainer->layoutChildSpace(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

            {
                zfblockedAlloc(ZFUITextView, dataWidthHintTmp);
                settingContainer->childAdd(dataWidthHintTmp);
                dataWidthHintTmp->viewId("dataWidthHint");

                zfblockedAlloc(ZFUILinearLayout, dataWidthContainerTmp);
                settingContainer->childAdd(dataWidthContainerTmp);
                dataWidthContainerTmp->layoutOrientation(ZFUIOrientation::e_Top);

                zfblockedAlloc(ZF2048AppButton, dataWidthIncreaseTmp);
                dataWidthContainerTmp->childAdd(dataWidthIncreaseTmp);
                dataWidthIncreaseTmp->viewId("dataWidthIncrease");

                zfblockedAlloc(ZFUITextEditWidget, dataWidthEditTmp);
                dataWidthContainerTmp->childAdd(dataWidthEditTmp);
                dataWidthEditTmp->viewId("dataWidthEdit");

                zfblockedAlloc(ZF2048AppButton, dataWidthDecreaseTmp);
                dataWidthContainerTmp->childAdd(dataWidthDecreaseTmp);
                dataWidthDecreaseTmp->viewId("dataWidthDecrease");
            }
            {
                zfblockedAlloc(ZFUITextView, dataHeightHintTmp);
                settingContainer->childAdd(dataHeightHintTmp);
                dataHeightHintTmp->viewId("dataHeightHint");

                zfblockedAlloc(ZFUILinearLayout, dataHeightContainerTmp);
                settingContainer->childAdd(dataHeightContainerTmp);
                dataHeightContainerTmp->layoutOrientation(ZFUIOrientation::e_Top);

                zfblockedAlloc(ZF2048AppButton, dataHeightIncreaseTmp);
                dataHeightContainerTmp->childAdd(dataHeightIncreaseTmp);
                dataHeightIncreaseTmp->viewId("dataHeightIncrease");

                zfblockedAlloc(ZFUITextEditWidget, dataHeightEditTmp);
                dataHeightContainerTmp->childAdd(dataHeightEditTmp);
                dataHeightEditTmp->viewId("dataHeightEdit");

                zfblockedAlloc(ZF2048AppButton, dataHeightDecreaseTmp);
                dataHeightContainerTmp->childAdd(dataHeightDecreaseTmp);
                dataHeightDecreaseTmp->viewId("dataHeightDecrease");
            }

            zfblockedAlloc(ZFUILinearLayout, buttonLayout);
            mainView->childAdd(buttonLayout);
            buttonLayout->layoutChildSpace(ZFUIGlobalStyle::DefaultStyle()->itemSpace());

            zfblockedAlloc(ZF2048AppButton, confirmButtonTmp);
            buttonLayout->childAdd(confirmButtonTmp);
            confirmButtonTmp->viewId("confirmButton");

            zfblockedAlloc(ZF2048AppButton, cancelButtonTmp);
            buttonLayout->childAdd(cancelButtonTmp);
            cancelButtonTmp->viewId("cancelButton");
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
            ZFUIButtonBasic *increase = userData->objectTag("increase")->objectHolded();
            ZFUITextEdit *edit = userData->objectTag("edit")->objectHolded();
            ZFUIButtonBasic *decrease = userData->objectTag("decrease")->objectHolded();
            zfindex value = _ZFP_ZF2048AppSettingDialogPrivate::readData(edit);
            ++value;
            value = zfmApplyRange<zfindex>(value, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
            edit->text(zfindexToString(value));
            increase->buttonEnable(value != _ZFP_ZF2048AppDataWidthMax);
            decrease->buttonEnable(value != _ZFP_ZF2048AppDataWidthMin);
        })
        ZFLISTENER_LOCAL(decreaseOnClick, {
            ZFUIButtonBasic *increase = userData->objectTag("increase")->objectHolded();
            ZFUITextEdit *edit = userData->objectTag("edit")->objectHolded();
            ZFUIButtonBasic *decrease = userData->objectTag("decrease")->objectHolded();
            zfindex value = _ZFP_ZF2048AppSettingDialogPrivate::readData(edit);
            --value;
            value = zfmApplyRange<zfindex>(value, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
            edit->text(zfindexToString(value));
            increase->buttonEnable(value != _ZFP_ZF2048AppDataWidthMax);
            decrease->buttonEnable(value != _ZFP_ZF2048AppDataWidthMin);
        })
        ZFLISTENER_LOCAL(editOnChange, {
            ZFUIButtonBasic *increase = userData->objectTag("increase")->objectHolded();
            ZFUITextEdit *edit = userData->objectTag("edit")->objectHolded();
            ZFUIButtonBasic *decrease = userData->objectTag("decrease")->objectHolded();
            zfindex value = _ZFP_ZF2048AppSettingDialogPrivate::readData(edit);
            value = zfmApplyRange<zfindex>(value, _ZFP_ZF2048AppDataWidthMin, _ZFP_ZF2048AppDataWidthMax);
            increase->buttonEnable(value != _ZFP_ZF2048AppDataWidthMax);
            decrease->buttonEnable(value != _ZFP_ZF2048AppDataWidthMin);
        })
        ZFLISTENER_LOCAL(editOnClickReturn, {
            ZFUITextEdit *textEdit = listenerData.sender()->toAny();
            textEdit->viewFocusRequest(zffalse);
        })

        zfblockedAlloc(ZFObject, userDataForWidth);
        userDataForWidth->objectTag("increase", this->dataWidthIncrease->objectHolder());
        userDataForWidth->objectTag("edit", this->dataWidthEdit->objectHolder());
        userDataForWidth->objectTag("decrease", this->dataWidthDecrease->objectHolder());

        zfblockedAlloc(ZFObject, userDataForHeight);
        userDataForHeight->objectTag("increase", this->dataHeightIncrease->objectHolder());
        userDataForHeight->objectTag("edit", this->dataHeightEdit->objectHolder());
        userDataForHeight->objectTag("decrease", this->dataHeightDecrease->objectHolder());

        zfblockedAlloc(ZFRegExp, textFilter);
        textFilter->regExpCompile(zfstringWithFormat("\\d{1,2}"));
        this->dataWidthEdit->textEditFilter(textFilter);
        this->dataHeightEdit->textEditFilter(textFilter);

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
    this->dialogContent(d->contentView());
}
void ZF2048AppSettingDialog::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    d->dataWidthEdit->text(zfindexToString(this->dataWidth));
    d->dataHeightEdit->text(zfindexToString(this->dataHeight));
}
void ZF2048AppSettingDialog::objectOnDealloc(void)
{
    zfRetainChange(d, zfnull);
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

