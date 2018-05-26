/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
/**
 * @file ZFUIButtonCheckBox.h
 * @brief check box button
 */

#ifndef _ZFI_ZFUIButtonCheckBox_h_
#define _ZFI_ZFUIButtonCheckBox_h_

#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonCheckBox
/**
 * @brief check box button, see #ZFUIButtonBasic
 */
zfclass ZF_ENV_EXPORT ZFUIButtonCheckBox : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZFUIButtonCheckBox, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonCheckBox)

public:
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(zfbool, buttonCheckable)
    {
        propertyValue = zftrue;
    }

    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIAlignFlags, buttonContentAlign)
    {
        propertyValue = ZFUIAlign::e_LeftInner;
    }

    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonIconStyleNormal)
    {
        propertyValue.to<ZFUIImageView *>()->imageSet(zfRes(zfText("ZFUIWidget/ZFUIButtonCheckBox_IconNormal.png")));
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonIconStyleHighlighted)
    {
        propertyValue.to<ZFUIImageView *>()->imageSet(zfRes(zfText("ZFUIWidget/ZFUIButtonCheckBox_IconHighlighted.png")));
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonIconStyleChecked)
    {
        propertyValue.to<ZFUIImageView *>()->imageSet(zfRes(zfText("ZFUIWidget/ZFUIButtonCheckBox_IconChecked.png")));
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonIconStyleCheckedHighlighted)
    {
        propertyValue.to<ZFUIImageView *>()->imageSet(zfRes(zfText("ZFUIWidget/ZFUIButtonCheckBox_IconCheckedHighlighted.png")));
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonCheckBox_h_

