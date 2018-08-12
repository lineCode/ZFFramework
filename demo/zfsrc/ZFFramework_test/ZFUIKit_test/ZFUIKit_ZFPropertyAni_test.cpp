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

zfclass ZFUIKit_ZFPropertyAni_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIKit_ZFPropertyAni_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();
        ZFFramework_test_protocolCheck(ZFUIView);
        ZFUIWindow *window = zfnull;
        ZFUIView *container = zfnull;
        ZFUIKit_test_prepareTestWindow(window, container, this);

        zfblockedAlloc(ZFUIView, testContainer);
        container->childAdd(testContainer);
        testContainer->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
        testContainer->layoutParam()->layoutMarginSet(ZFUIMarginMake(0, 50, 0, 0));
        testContainer->tagSet(zfText("owner"), this->objectHolder());

        for(zfindex i = 0; i < 4; ++i)
        {
            zfblockedAlloc(ZFUIView, child);
            testContainer->childAdd(child);
            child->viewSizePreferedSet(ZFUISizeMake(zfmRand(20, 80), zfmRand(20, 80)));
            child->viewBackgroundColorSet(ZFUIColorRandom(128));
            child->layoutParam()->layoutAlignSet(ZFUIAlign::e_LeftInner | ZFUIAlign::e_TopInner);
        }

        ZFLISTENER_LOCAL(startOnClick, {
            ZFPropertyAniBlock(setting);

            ZFUIView *testContainer = userData->objectHolded();
            ZFUIKit_ZFPropertyAni_test *owner = testContainer->tagGet(zfText("owner"))->objectHolded();
            zfint xMargin = testContainer->layoutedFrame().size.width - 80;
            zfint yMargin = testContainer->layoutedFrame().size.height - 80;

            setting->aniDurationSet(owner->aniDuration());
            setting->aniIntervalSet(owner->aniInterval());
            setting->aniCurveSet(owner->aniCurve());

            for(zfindex i = 0; i < testContainer->childCount(); ++i)
            {
                ZFUIView *child = testContainer->childAtIndex(i);
                child->layoutParam()->layoutMarginSet(ZFUIMarginMake(zfmRand(xMargin), zfmRand(yMargin), 0, 0));
            }
        })
        zfblockedAlloc(ZFUIKit_test_Button, startButton);
        container->childAdd(startButton);
        startButton->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner | ZFUIAlign::e_RightInner);
        startButton->buttonLabelTextSet(zfText("start"));
        startButton->observerAdd(ZFUIButton::EventButtonOnClick(), startOnClick, testContainer->objectHolder());
        startButton->buttonSimulateClick();

        this->prepareSettingButton(window);
    }

    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniDuration, 0)
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniInterval, 35)
    ZFPROPERTY_RETAIN(ZFTimeLineCurve *, aniCurve)
private:
    void prepareSettingButton(ZF_IN ZFUIWindow *window)
    {
        zfblockedAlloc(ZFArrayEditable, settings);

        ZFUIKit_test_prepareSettingForNormalProperty(settings, this, zftimet, ZFPropertyAccess(zfself, aniDuration),
            ZFCoreArrayPODCreate(zftimet
                , (zftimet)0
                , (zftimet)3000
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, this, zftimet, ZFPropertyAccess(zfself, aniInterval),
            ZFCoreArrayPODCreate(zftimet
                , (zftimet)35
                , (zftimet)1000
                ));

        ZFUIKit_test_prepareSettingForNormalProperty(settings, this, ZFTimeLineCurve *, ZFPropertyAccess(zfself, aniCurve),
            ZFCoreArrayPODCreate(ZFTimeLineCurve *
                , ZFTimeLineCurveEaseIn::instance()
                , ZFTimeLineCurveEaseOut::instance()
                , ZFTimeLineCurveEaseInOut::instance()
                , ZFTimeLineCurveBounceIn::instance()
                , ZFTimeLineCurveBounceOut::instance()
                , ZFTimeLineCurveBounceInOut::instance()
                ));

        ZFUIKit_test_prepareSettingButtonWithTestWindow(window, settings);
    }
};
ZFOBJECT_REGISTER(ZFUIKit_ZFPropertyAni_test)

ZF_NAMESPACE_GLOBAL_END

