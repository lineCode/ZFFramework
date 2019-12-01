#ifndef _ZFI_ZF2048AppButton_h_
#define _ZFI_ZF2048AppButton_h_

#include "ZFUIWidget.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZF2048AppButton : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZF2048AppButton, ZFUIButtonBasic)

    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUISize, viewSizeMin)
    {
        propertyValue = ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeButton());
    }

    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUITextView *, buttonLabelStyleNormal)
    {
        propertyValue.to<ZFUITextView *>()->textColor(ZFUIColorBlue());
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUITextView *, buttonLabelStyleHighlighted)
    {
        propertyValue.to<ZFUITextView *>()->textColor(ZFUIColorBlue());
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUITextView *, buttonLabelStyleDisabled)
    {
        propertyValue.to<ZFUITextView *>()->textColor(ZFUIColorBlue());
    }

    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonBackgroundStyleNormal)
    {
        propertyValue.to<ZFUIImageView *>()->image(zfRes("ZF2048/app/button_normal.xml"));
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonBackgroundStyleHighlighted)
    {
        propertyValue.to<ZFUIImageView *>()->image(zfRes("ZF2048/app/button_highlighted.xml"));
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonBackgroundStyleDisabled)
    {
        propertyValue.to<ZFUIImageView *>()->image(zfRes("ZF2048/app/button_disabled.xml"));
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZF2048AppButton_h_

