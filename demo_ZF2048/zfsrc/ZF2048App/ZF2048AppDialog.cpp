#include "ZF2048AppDialog.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZF2048AppDialogPrivate
{
public:
    ZFUILinearLayout *dialogContainer;
    ZFUITextView *dialogTitleView;

public:
    _ZFP_ZF2048AppDialogPrivate(void)
    : dialogContainer(zfnull)
    , dialogTitleView(zfnull)
    {
    }
};

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZF2048AppDialog, zfstring, dialogTitle)
{
    d->dialogTitleView->text(this->dialogTitle());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZF2048AppDialog, ZFUIView *, dialogContent)
{
    if(this->dialogContent() != zfnull)
    {
        d->dialogContainer->childAdd(this->dialogContent());
    }
}
ZFPROPERTY_OVERRIDE_ON_DETACH_DEFINE(ZF2048AppDialog, ZFUIView *, dialogContent)
{
    if(this->dialogContent() != zfnull)
    {
        this->dialogContent()->viewRemoveFromParent();
    }
}

void ZF2048AppDialog::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfnew(_ZFP_ZF2048AppDialogPrivate);

    d->dialogContainer = zfAlloc(ZFUILinearLayout);
    this->dialogInternalContainer()->childAdd(d->dialogContainer);
    d->dialogContainer->layoutOrientation(ZFUIOrientation::e_Top);
    d->dialogContainer->layoutChildSpace(ZFUIGlobalStyle::DefaultStyle()->itemSpace());
    d->dialogContainer->viewSizeMin(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeDialog()));

    d->dialogTitleView = zfAlloc(ZFUITextView);
    d->dialogContainer->childAdd(d->dialogTitleView);
    d->dialogTitleView->viewSizeMin(ZFUISizeMake(0, ZFUIGlobalStyle::DefaultStyle()->itemSizeDialog()));
    d->dialogTitleView->textAlign(ZFUIAlign::e_Center);
}
void ZF2048AppDialog::objectOnDealloc(void)
{
    zfRetainChange(d->dialogContainer, zfnull);
    zfRetainChange(d->dialogTitleView, zfnull);

    zfdelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

ZF_NAMESPACE_GLOBAL_END

