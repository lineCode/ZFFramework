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

zfclass ZFUIKit_ZFUIImage_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImage_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIImageView, view);
        container->childAdd(view);
        view->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());

        // encode image to binary and load it again to test
        zfautoObject imageHolder = zfRes(zfText("test_normal.png"));
        ZFIOBufferedCallbackUsingBuffer io;
        ZFUIImageEncodeToFile(io, imageHolder.toAny());
        zfautoObject imageNew = ZFUIImageEncodeFromFile(io);
        view->imageSet(imageNew.toAny());

        this->prepareSettingButton(window, view);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIImageView *view)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIImage_test)

ZF_NAMESPACE_GLOBAL_END

