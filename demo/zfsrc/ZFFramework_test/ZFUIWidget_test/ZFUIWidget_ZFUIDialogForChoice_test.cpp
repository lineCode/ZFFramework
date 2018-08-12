/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUIDialogForChoice_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUIDialogForChoice_test, ZFFramework_test_TestCase)

    ZFPROPERTY_RETAIN_READONLY(ZFUIDialogForChoice *, dialog, zflineAlloc(ZFUIDialogForChoice))

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        this->dialog()->dialogApplyAutoHide(this->dialog()->dialogButtonCancel());
        this->dialog()->dialogButtonCancelTextSet(zfText("cancel"));
        this->dialog()->dialogApplyAutoHide(this->dialog()->dialogButtonYes());
        this->dialog()->dialogButtonYesTextSet(zfText("confirm"));
        ZFLISTENER_LOCAL(choiceOnConfirm, {
            ZFUIDialogForChoice *dialog = userData->objectHolded();
            zfLogT() << zfText("onConfirm") << dialog->choiceSelectedNameList();
        })
        ZFLISTENER_LOCAL(choiceOnChange, {
            ZFUIDialogForChoice *dialog = userData->objectHolded();
            zfLogT() << zfText("onChange") << dialog->choiceSelectedNameList();
        })
        this->dialog()->observerAdd(ZFUIDialogForChoice::EventChoiceOnConfirm(), choiceOnConfirm, this->dialog()->objectHolder());
        this->dialog()->observerAdd(ZFUIDialogForChoice::EventChoiceOnChange(), choiceOnChange, this->dialog()->objectHolder());
        this->dialog()->dialogTitleTextSet(zfText("i'm title"));

        this->dialog()->choiceAdd(zfText("id 0"), zfText("choice 0"));
        this->dialog()->choiceAdd(zfText("id 1"), zfText("choice 1"));
        this->dialog()->choiceAdd(zfText("id 2"), zfText("choice 2"));

        zfblockedAlloc(ZFUIKit_test_Button, showButton);
        container->childAdd(showButton);
        showButton->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
        ZFLISTENER_LOCAL(showButtonOnClick, {
            userData->objectHolded<ZFUIDialogForChoice *>()->dialogShow();
        })
        showButton->observerAdd(ZFUIButton::EventButtonOnClick(), showButtonOnClick, this->dialog()->objectHolder());

        this->prepareSettingButton(window, this->dialog());
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIDialogForChoice *dialog)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUIDialogForChoice_test)

ZF_NAMESPACE_GLOBAL_END

