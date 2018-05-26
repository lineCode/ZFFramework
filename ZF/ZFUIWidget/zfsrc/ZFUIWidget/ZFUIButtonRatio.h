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
 * @file ZFUIButtonRatio.h
 * @brief check box button
 */

#ifndef _ZFI_ZFUIButtonRatio_h_
#define _ZFI_ZFUIButtonRatio_h_

#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonRatio
/**
 * @brief check box button, see #ZFUIButtonBasic
 */
zfclass ZF_ENV_EXPORT ZFUIButtonRatio : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZFUIButtonRatio, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonRatio)

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
        propertyValue.to<ZFUIImageView *>()->imageSet(zfRes(zfText("ZFUIWidget/ZFUIButtonRatio_IconNormal.png")));
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonIconStyleHighlighted)
    {
        propertyValue.to<ZFUIImageView *>()->imageSet(zfRes(zfText("ZFUIWidget/ZFUIButtonRatio_IconHighlighted.png")));
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonIconStyleChecked)
    {
        propertyValue.to<ZFUIImageView *>()->imageSet(zfRes(zfText("ZFUIWidget/ZFUIButtonRatio_IconChecked.png")));
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonIconStyleCheckedHighlighted)
    {
        propertyValue.to<ZFUIImageView *>()->imageSet(zfRes(zfText("ZFUIWidget/ZFUIButtonRatio_IconCheckedHighlighted.png")));
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonRatio_h_

