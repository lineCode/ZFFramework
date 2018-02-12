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

zfclass ZFUIKit_ZFAnimationNativeView_multiple_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFAnimationNativeView_multiple_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView)
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIView, view);
        container->childAdd(view);
        view->viewSizePreferedSet(ZFUISizeMake(80, 60));
        view->viewBackgroundColorSet(ZFUIColorRandom());
        view->layoutParam()->layoutAlignSet(ZFUIAlign::e_Center);

        ZFLISTENER_LOCAL(startOnClick, {
            ZFUIView *view = userData->toAny();

            zfblockedAlloc(ZFAnimationGroup, aniGroup);
            aniGroup->aniTargetSet(view);

            zfblockedAlloc(ZFAnimationNativeView, ani0);
            aniGroup->childAniAdd(ani0);
            ani0->aniTranslateXToSet(1);
            ani0->aniDurationSet(3000);

            zfblockedAlloc(ZFAnimationNativeView, ani1);
            aniGroup->childAniAdd(ani1);
            ani1->aniRotateZToSet(180);
            ani1->aniDurationSet(3000);

            aniGroup->aniStart();
        })
        zfblockedAlloc(ZFUIKit_test_Button, startButton);
        startButton->buttonLabelTextSet(zfText("start"));
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), startOnClick, view);
        container->childAdd(startButton);
        startButton->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner | ZFUIAlign::e_RightInner);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFAnimationNativeView_multiple_test)

ZF_NAMESPACE_GLOBAL_END

