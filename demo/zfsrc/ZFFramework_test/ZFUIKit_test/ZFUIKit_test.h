/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#ifndef _ZFI_ZFUIKit_test_h_
#define _ZFI_ZFUIKit_test_h_

#include "../../ZFFramework_test_global.h"
#include "ZFUIKit.h"
#include "ZFUIWidget/ZFUIButtonBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIKit_test_Window : zfextends ZFUIWindow
{
    ZFOBJECT_DECLARE(ZFUIKit_test_Window, ZFUIWindow)

    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIColor, viewBackgroundColor)
    {
        propertyValue = ZFUIColorWhite();
    }
};

zfclass ZFUIKit_test_Button : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZFUIKit_test_Button, ZFUIButtonBasic)

    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonBackgroundStyleNormal)
    {
        propertyValue.to<ZFUIImageView *>()->viewBackgroundColorSet(ZFUIColorGreen());
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonBackgroundStyleHighlighted)
    {
        propertyValue.to<ZFUIImageView *>()->viewBackgroundColorSet(ZFUIColorBlue());
    }
};

zfclass ZFUIKit_test_ListView : zfextends ZFUIScrollView
{
    ZFOBJECT_DECLARE(ZFUIKit_test_ListView, ZFUIScrollView)

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->scrollBounceVerticalAlwaysSet(zftrue);
    }

    zfoverride
    virtual void layoutParamOnUpdate(ZF_IN ZFUIViewLayoutParam *layoutParam)
    {
        zfsuper::layoutParamOnUpdate(layoutParam);

        layoutParam->sizeParamSet(ZFUISizeParamFillWrap());
        layoutParam->layoutAlignSet(ZFUIAlign::e_TopInner);
    }
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
    {
        zfsuper::layoutOnLayoutPrepare(bounds);

        zfint contentHeight = 0;
        zfint space = 4;
        ZFUISize childMeasureSizeHint = ZFUISizeMake(this->scrollArea().size.width, -1);
        ZFUISizeParam childMeasureSizeParam = ZFUISizeParamFillWrap();

        for(zfindex i = 0; i < this->childCount(); ++i)
        {
            contentHeight += space;

            ZFUIView *child = this->childAtIndex(i);
            child->layoutMeasure(childMeasureSizeHint, childMeasureSizeParam);
            child->layoutParam()->layoutMarginSet(ZFUIMarginMake(0, contentHeight, 0, 0));

            contentHeight += child->layoutMeasuredSize().height;
        }
        contentHeight += space;

        this->scrollContentFrameSetWhileAnimating(ZFUIRectMake(
            this->scrollContentFrame().point.x, this->scrollContentFrame().point.y,
            this->scrollArea().size.width, contentHeight));
    }
};

extern void ZFUIKit_test_prepareTestWindow(ZF_OUT ZFUIWindow *&window,
                                           ZF_OUT ZFUIView *&container,
                                           ZF_IN ZFTestCase *testCaseToStop);

zfclass ZFUIKit_test_SettingData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIKit_test_SettingData, ZFObject)

public:
    ZFOBSERVER_EVENT(SettingOnChange)

public:
    /**
     * param0 is a #ZFStringEditable to get the button's text
     */
    ZFPROPERTY_ASSIGN(ZFListener, buttonTextGetter)
    ZFPROPERTY_ASSIGN(ZFListener, buttonClickListener)
    ZFPROPERTY_RETAIN(ZFObject *, userData)

public:
    zffinal void settingUpdate(void)
    {
        this->observerNotify(zfself::EventSettingOnChange());
    }

protected:
    virtual void objectOnInit(ZF_IN const ZFListener &buttonTextGetter,
                              ZF_IN const ZFListener &buttonClickListener,
                              ZF_IN_OPT ZFObject *userData = zfnull)
    {
        this->objectOnInit();
        this->buttonTextGetterSet(buttonTextGetter);
        this->buttonClickListenerSet(buttonClickListener);
        this->userDataSet(userData);
    }
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
    }
};
extern zfautoObject ZFUIKit_test_prepareSettingButton(ZF_IN ZFArray *settings);
extern void ZFUIKit_test_prepareSettingButtonWithTestWindow(ZF_IN ZFUIWindow *window,
                                                            ZF_IN ZFArray *settings);
extern void ZFUIKit_test_prepareSettingForProperty(ZF_IN_OUT ZFArrayEditable *settings,
                                                   ZF_IN ZFObject *obj,
                                                   ZF_IN const ZFProperty *property,
                                                   ZF_IN const ZFListener &nextCallback,
                                                   ZF_IN ZFObject *userData);

// ============================================================
extern void ZFUIKit_test_prepareSettingForBoolProperty(ZF_IN_OUT ZFArrayEditable *settings,
                                                       ZF_IN ZFObject *obj,
                                                       ZF_IN const ZFProperty *property);

zfclass _ZFP_I_ZFUIKit_test_prepareSettingForNormalProperty_Holder : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFUIKit_test_prepareSettingForNormalProperty_Holder, ZFObject)
public:
    ZFObject *obj;
    const ZFProperty *property;
    ZFCoreArrayBase *propertyValues;
    zfindex index;
public:
    _ZFP_I_ZFUIKit_test_prepareSettingForNormalProperty_Holder(void)
    : obj(zfnull)
    , property(zfnull)
    , propertyValues(zfnull)
    , index(0)
    {
    }
protected:
    virtual void objectOnDeallocPrepare(void)
    {
        if(this->propertyValues)
        {
            this->propertyValues->refDelete();
        }
        zfsuper::objectOnDeallocPrepare();
    }
};
#define ZFUIKit_test_prepareSettingForNormalProperty(settings, obj_, PropertyType, property_, propertyValues_) \
    do { \
        zfblockedAlloc(_ZFP_I_ZFUIKit_test_prepareSettingForNormalProperty_Holder, userData); \
        userData->obj = obj_; \
        userData->property = property_; \
        userData->propertyValues = propertyValues_.refNew(); \
        ZFLISTENER_LOCAL(nextCallback, { \
                _ZFP_I_ZFUIKit_test_prepareSettingForNormalProperty_Holder *holder = userData->toAny(); \
                ZFCoreArray<PropertyType> const &propertyValues = *(const ZFCoreArray<PropertyType> *)holder->propertyValues; \
                ++(holder->index); \
                if(holder->index >= propertyValues.count()) \
                { \
                    holder->index = 0; \
                } \
                holder->property->setterMethod()->execute<void ZFM_COMMA() PropertyType const &>( \
                    holder->obj, propertyValues[holder->index]); \
            }) \
        ZFUIKit_test_prepareSettingForProperty(settings, userData->obj, userData->property, nextCallback, userData); \
    } while(zffalse)

extern void ZFUIKit_test_prepareSettingForLayoutRequest(ZF_IN_OUT ZFArrayEditable *settings,
                                                        ZF_IN ZFUIView *view);

extern void ZFUIKit_test_prepareSettingForResetProperty(ZF_IN_OUT ZFArrayEditable *settings,
                                                        ZF_IN ZFObject *obj,
                                                        ZF_IN const ZFCoreArrayPOD<const ZFProperty *> &propertyList);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFUIKit_test_h_

