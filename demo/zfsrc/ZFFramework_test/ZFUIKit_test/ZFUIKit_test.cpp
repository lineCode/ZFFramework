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

ZFOBJECT_REGISTER(ZFUIKit_test_Window)
ZFOBJECT_REGISTER(ZFUIKit_test_Button)
ZFOBJECT_REGISTER(ZFUIKit_test_ListView)

void ZFUIKit_test_prepareTestWindow(ZF_OUT ZFUIWindow *&window,
                                    ZF_OUT ZFUIView *&container,
                                    ZF_IN ZFTestCase *testCaseToStop)
{
    window = zfAlloc(ZFUIKit_test_Window);
    zfblockedRelease(window);
    window->windowShow();

    // close button
    zfblockedAlloc(ZFUIKit_test_Button, closeButton);
    window->childAdd(closeButton);
    closeButton->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner | ZFUIAlign::e_RightInner);
    closeButton->buttonLabelTextSet("close");
    ZFLISTENER_LOCAL(onClickCloseButton, {
        ZFUIWindow *window = userData->tagGet("window")->objectHolded();
        ZFTestCase *testCase = userData->tagGet("testCaseToStop")->objectHolded();
        window->windowHide();
        testCase->testCaseStop();
    })
    zfblockedAlloc(ZFObject, closeButtonUserData);
    closeButtonUserData->tagSet("window", window->objectHolder());
    closeButtonUserData->tagSet("testCaseToStop", testCaseToStop->objectHolder());
    closeButton->observerAdd(ZFUIButton::EventButtonOnClick(), onClickCloseButton, closeButtonUserData);

    // container
    container = zfAlloc(ZFUIView);
    zfblockedRelease(container);
    window->childAdd(container);
    container->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
    container->layoutParam()->layoutMarginSet(ZFUIMarginMake(0, 50, 0, 0));
}

zfautoObject ZFUIKit_test_prepareSettingButton(ZF_IN ZFArray *settings)
{
    zfblockedAlloc(ZFUIKit_test_Button, settingsButton);
    settingsButton->buttonLabelTextSet("settings");
    settingsButton->tagSet("settingsHolder", settings);

    zfblockedAlloc(ZFUIKit_test_Window, window);
    settingsButton->tagSet("setting window", window);
    window->viewAlphaSet(0.8f);
    ZFLISTENER_LOCAL(onClickSetting, {
        userData->objectHolded<ZFUIWindow *>()->windowShow();
    })
    settingsButton->observerAdd(ZFUIButton::EventButtonOnClick(), onClickSetting, window->objectHolder());

    zfblockedAlloc(ZFUIKit_test_Button, closeButton);
    window->childAdd(closeButton);
    closeButton->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner);
    closeButton->buttonLabelStyle()->textSet("done");
    ZFLISTENER_LOCAL(onClickCloseButton, {
        userData->objectHolded<ZFUIWindow *>()->windowHide();
    })
    closeButton->observerAdd(ZFUIButton::EventButtonOnClick(), onClickCloseButton, window->objectHolder());

    zfblockedAlloc(ZFUIKit_test_ListView, listView);
    window->childAdd(listView);
    listView->layoutParam()->sizeParamSet(ZFUISizeParamFillFill());
    listView->layoutParam()->layoutMarginSet(ZFUIMarginMake(0, 50, 0, 0));
    for(zfindex i = 0; i < settings->count(); ++i)
    {
        ZFUIKit_test_SettingData *setting = settings->get<ZFUIKit_test_SettingData *>(i);
        zfCoreAssert(setting->buttonTextGetter().callbackIsValid());
        zfCoreAssert(setting->buttonClickListener().callbackIsValid());

        zfblockedAlloc(ZFUIKit_test_Button, button);
        listView->childAdd(button);
        ZFLISTENER_LOCAL(onButtonClick, {
            ZFUIKit_test_SettingData *setting = ZFAny(userData);
            ZFUIButtonBasic *button = ZFAny(listenerData.sender);

            setting->buttonClickListener().execute(ZFListenerData().senderSet(button), setting->userData());
            setting->settingUpdate();
        })
        button->observerAdd(ZFUIButton::EventButtonOnClick(), onButtonClick, setting);

        zfblockedAlloc(ZFObject, settingChangeUserData);
        settingChangeUserData->tagSet("setting", setting);
        settingChangeUserData->tagSet("button", button);
        ZFLISTENER_LOCAL(settingOnChange, {
            ZFUIKit_test_SettingData *setting = userData->tagGet<ZFUIKit_test_SettingData *>("setting");
            ZFUIButtonBasic *button = userData->tagGet<ZFUIButtonBasic *>("button");

            zfblockedAlloc(ZFStringEditable, buttonText);
            setting->buttonTextGetter().execute(
                ZFListenerData().senderSet(button).param0Set(buttonText),
                setting->userData());
            button->buttonLabelTextSet(buttonText->stringValue());
        })
        setting->observerAdd(ZFUIKit_test_SettingData::EventSettingOnChange(), settingOnChange, settingChangeUserData);

        zfblockedAlloc(ZFStringEditable, buttonText);
        setting->buttonTextGetter().execute(
            ZFListenerData().senderSet(button).param0Set(buttonText),
            setting->userData());
        button->buttonLabelTextSet(buttonText->stringValue());
    }

    return settingsButton;
}

void ZFUIKit_test_prepareSettingButtonWithTestWindow(ZF_IN ZFUIWindow *window,
                                                     ZF_IN ZFArray *settings)
{
    zfautoObject buttonHolder = ZFUIKit_test_prepareSettingButton(settings);
    ZFUIButton *button = buttonHolder;
    window->childAdd(button);
    button->layoutParam()->layoutAlignSet(ZFUIAlign::e_TopInner);
}

void ZFUIKit_test_prepareSettingForProperty(ZF_IN_OUT ZFArrayEditable *settings,
                                            ZF_IN ZFObject *obj,
                                            ZF_IN const ZFProperty *property,
                                            ZF_IN const ZFListener &nextCallback,
                                            ZF_IN ZFObject *userData)
{
    zfCoreAssert(settings != zfnull);
    zfCoreAssert(nextCallback.callbackIsValid());

    zfblockedAlloc(ZFObject, holder);
    holder->tagSet("obj", obj->objectHolder());
    holder->tagSet("property", zflineAlloc(v_ZFProperty, property));
    holder->tagSet("nextCallback", zflineAlloc(ZFTypeHolder, zfnew(ZFListener, nextCallback), ZFTypeHolderTypeObject<ZFListener *>));
    holder->tagSet("userData", userData);

    ZFLISTENER_LOCAL(buttonTextGetter, {
        ZFObject *obj = userData->tagGet("obj")->objectHolded();
        const ZFProperty *property = userData->tagGet<v_ZFProperty *>("property")->zfv;

        ZFStringEditable *text = listenerData.param0->to<ZFStringEditable *>();
        text->stringValueSet(zfstringWithFormat("%s : %s", property->propertyName(), ZFPropertyGetInfo(property, obj).cString()));
    })
    ZFLISTENER_LOCAL(buttonClickListener, {
        const ZFListener &nextCallback = userData->tagGet<ZFTypeHolder *>("nextCallback")->holdedDataRef<const ZFListener &>();
        nextCallback.execute(ZFListenerData(), userData->tagGet("userData"));
    })

    settings->add(zflineAlloc(ZFUIKit_test_SettingData, buttonTextGetter, buttonClickListener, holder));
}

void ZFUIKit_test_prepareSettingForBoolProperty(ZF_IN_OUT ZFArrayEditable *settings,
                                                ZF_IN ZFObject *obj,
                                                ZF_IN const ZFProperty *property)
{
    zfCoreAssert(settings != zfnull);
    zfCoreAssert(obj != zfnull && property != zfnull);
    zfCoreAssert(obj->classData()->classIsTypeOf(property->propertyOwnerClass()));
    zfCoreAssert(zfscmpTheSame(property->propertyTypeId(), ZFTypeId_zfbool()));

    zfbool value = property->getterMethod()->execute<zfbool const &>(obj);
    ZFUIKit_test_prepareSettingForNormalProperty(settings, obj, zfbool, property, ZFCoreArrayPODCreate(zfbool, value, !value));
}

void ZFUIKit_test_prepareSettingForLayoutRequest(ZF_IN_OUT ZFArrayEditable *settings,
                                                 ZF_IN ZFUIView *view)
{
    zfCoreAssert(settings != zfnull);
    zfCoreAssert(view != zfnull);

    ZFLISTENER_LOCAL(buttonTextGetter, {
        ZFStringEditable *text = listenerData.param0->to<ZFStringEditable *>();
        text->stringValueSet("layoutRequest");
    })
    ZFLISTENER_LOCAL(buttonClickListener, {
        userData->objectHolded<ZFUIView *>()->layoutRequest();
    })

    settings->add(zflineAlloc(ZFUIKit_test_SettingData, buttonTextGetter, buttonClickListener, view->objectHolder()));
}

void ZFUIKit_test_prepareSettingForResetProperty(ZF_IN_OUT ZFArrayEditable *settings,
                                                 ZF_IN ZFObject *obj,
                                                 ZF_IN const ZFCoreArrayPOD<const ZFProperty *> &propertyList)
{
    zfblockedAlloc(ZFUIKit_test_SettingData, setting);
    settings->add(setting);
    setting->userDataSet(zflineAlloc(ZFObject));
    setting->userData()->tagSet("obj", obj->objectHolder());
    setting->userData()->tagSet("settings", settings->objectHolder());
    zfblockedAlloc(ZFTypeHolder, propertyListHolder,
        zfnew(ZFCoreArrayPOD<const ZFProperty *>, propertyList),
        ZFTypeHolderTypeObject<ZFCoreArrayPOD<const ZFProperty *> *>);
    setting->userData()->tagSet("propertyList", propertyListHolder);

    ZFLISTENER_LOCAL(buttonTextGetter, {
        ZFStringEditable *text = listenerData.param0->to<ZFStringEditable *>();
        text->stringValueSet("reset setting");
    })
    setting->buttonTextGetterSet(buttonTextGetter);

    ZFLISTENER_LOCAL(buttonClickListener, {
        ZFObject *obj = userData->tagGet("obj")->objectHolded();
        ZFArray *settings = userData->tagGet("settings")->objectHolded();
        const ZFCoreArrayPOD<const ZFProperty *> &toReset = userData->tagGet<ZFTypeHolder *>("propertyList")->holdedDataRef<const ZFCoreArrayPOD<const ZFProperty *> &>();

        if(obj == zfnull || settings == zfnull || toReset.isEmpty())
        {
            return ;
        }

        zfautoObject fromObj = obj->classData()->newInstance();
        for(zfindex i = 0; i < toReset.count(); ++i)
        {
            ZFPropertyCopy(toReset[i], obj, fromObj);
        }
        for(zfindex i = 0; i < settings->count(); ++i)
        {
            settings->get<ZFUIKit_test_SettingData *>(i)->settingUpdate();
        }
    })
    setting->buttonClickListenerSet(buttonClickListener);
}

ZF_NAMESPACE_GLOBAL_END

