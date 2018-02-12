/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIWebKit_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWebKit_ZFUIWebView_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWebKit_ZFUIWebView_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIWebView, testView);
        container->childAdd(testView);
        testView->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
        testView->layoutParam()->layoutMarginSet(ZFUIMarginMake(40));
        testView->viewBackgroundColorSet(ZFUIColorRed());

        this->prepareSettingButton(window, testView);

        ZFLISTENER_LOCAL(loadStateOnChange, {
            zfLogTrimT() << zfText("webLoadingOnChange") << listenerData.sender->to<ZFUIWebView *>()->webLoading();
        })
        testView->observerAdd(ZFUIWebView::EventWebLoadStateOnChange(), loadStateOnChange);

        testView->webLoadUrl(zfText("http://www.baidu.com"));
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIWebView *testView)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIWebKit_ZFUIWebView_test)

ZF_NAMESPACE_GLOBAL_END

