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

zfclass ZFUIKit_scale_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_scale_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUITextView, view);
        container->childAdd(view);
        view->textSet("test text");
        view->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);

        this->prepareSettingButton(window);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        { // scaleForApp
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFStringEditable *text = listenerData.param0->to<ZFStringEditable *>();
                text->stringValueSet(zfstringWithFormat(
                    "scale: %d",
                    (zfint)ZFUISysWindow::mainWindow()->rootView()->scaleForApp()));
            })
            setting->buttonTextGetterSet(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIRootView *rootView = ZFUISysWindow::mainWindow()->rootView();
                rootView->scaleForAppSet(((zfint)(rootView->scaleForApp()) % 2) + 1);
            })
            setting->buttonClickListenerSet(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_scale_test)

ZF_NAMESPACE_GLOBAL_END

