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

zfclass ZFUIKit_ZFUIImageNinePatch_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFUIImageNinePatch_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIImageView, imageView);
        container->childAdd(imageView);
        imageView->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);
        imageView->imageSet(zfRes("test_normal.png").to<ZFCopyable *>()->copy());
        imageView->image()->imageNinePatchSet(ZFUIMarginMake(
            imageView->image()->imageSize().width / 2,
            imageView->image()->imageSize().height / 2));

        this->prepareSettingButton(window, imageView);
    }

private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window,
                              ZF_IN ZFUIImageView *imageView)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        { // wrap or fill
            zfblockedAlloc(ZFUIKit_test_SettingData, setting);
            settings->add(setting);
            setting->userDataSet(zflineAlloc(ZFObject));
            setting->userData()->tagSet("imageView", imageView->objectHolder());
            ZFLISTENER_LOCAL(buttonTextGetter, {
                ZFUIImageView *imageView = userData->tagGet("imageView")->objectHolded();
                ZFStringEditable *text = listenerData.param0->to<ZFStringEditable *>();
                zfbool fill = (imageView->layoutParam()->sizeParam().width == ZFUISizeType::e_Fill);
                text->stringValueSet(fill ? "fill" : "wrap");
            })
            setting->buttonTextGetterSet(buttonTextGetter);
            ZFLISTENER_LOCAL(buttonClickListener, {
                ZFUIImageView *imageView = userData->tagGet("imageView")->objectHolded();
                zfbool fill = (imageView->layoutParam()->sizeParam().width == ZFUISizeType::e_Fill);
                imageView->layoutParam()->sizeParamSet(fill ? ZFUISizeParamWrapWrap() : ZFUISizeParamFillFill());
            })
            setting->buttonClickListenerSet(buttonClickListener);
        }

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFUIImageNinePatch_test)

ZF_NAMESPACE_GLOBAL_END

