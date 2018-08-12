/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_ZFUITextEdit_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUITextEdit_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        ZFLISTENER_LOCAL(onClickReturn, {
            ZFUITextEdit *textEdit = listenerData.sender->toAny();
            textEdit->viewFocusRequest(zffalse);
        })

        zfblockedAlloc(ZFUITextEdit, view);
        container->childAdd(view);
        view->layoutParam()->sizeParamSet(ZFUISizeParamFillWrap());
        view->layoutParam()->layoutMarginSet(ZFUIMarginMake(40));
        view->viewBackgroundColorSet(ZFUIColorRed());
        view->observerAdd(ZFUITextEdit::EventTextOnReturnClick(), onClickReturn);
        view->textPlaceHolder()->textSet(zfText("first text edit"));

        zfblockedAlloc(ZFUITextEdit, view2);
        container->childAdd(view2);
        view2->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
        view2->layoutParam()->layoutMarginSet(ZFUIMarginMake(40, 100, 40, 40));
        view2->viewBackgroundColorSet(ZFUIColorBlue());
        view2->observerAdd(ZFUITextEdit::EventTextOnReturnClick(), onClickReturn);
        view2->textPlaceHolder()->textSet(zfText("second text edit"));

        this->prepareSettingButton(window, view);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUITextEdit *view)
    {
        zfblockedAlloc(ZFArrayEditable, settings);
        ZFUIKit_test_prepareSettingForBoolProperty(settings, view, ZFPropertyAccess(ZFUITextEdit, textEditEnable));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUITextEdit_test)

ZF_NAMESPACE_GLOBAL_END

