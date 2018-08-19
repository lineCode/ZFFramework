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
 * @file ZFUIImageView.h
 * @brief image view
 */

#ifndef _ZFI_ZFUIImageView_h_
#define _ZFI_ZFUIImageView_h_

#include "ZFUIView.h"
#include "ZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIImageView
/**
 * @brief native image view
 */
zfclass ZF_ENV_EXPORT ZFUIImageView : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIImageView, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIImageView)

public:
    // ============================================================
    // properties
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(zfbool, viewUIEnable)
    {
        propertyValue = zffalse;
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(zfbool, viewUIEnableTree)
    {
        propertyValue = zffalse;
    }

    /**
     * @brief image for this view
     */
    ZFPROPERTY_RETAIN(ZFUIImage *, image)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUIImage *, image)
    ZFPROPERTY_OVERRIDE_ON_DETACH_DECLARE(ZFUIImage *, image)

    /**
     * @brief image scale type, fill by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIContentScaleTypeEnum, imageScaleType,
                                ZFUIContentScaleType::EnumDefault())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_INLINE(ZFUIContentScaleTypeEnum, imageScaleType)
    {
        if(propertyValue != propertyValueOld)
        {
            this->layoutRequest();
        }
    }

    /**
     * @brief margin of the content image
     */
    ZFPROPERTY_ASSIGN(ZFUIMargin, imageMargin)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_INLINE(ZFUIMargin, imageMargin)
    {
        if(propertyValue != propertyValueOld)
        {
            this->nativeImplViewMarginUpdate();
        }
    }

public:
    /**
     * @brief measure image view accorrding to current image
     */
    ZFMETHOD_DECLARE_1(void, measureImageView,
                       ZFMP_OUT(ZFUISize &, ret))

protected:
    /**
     * @brief init with image
     */
    ZFOBJECT_ON_INIT_INLINE_1(ZFMP_IN(ZFUIImage *, image))
    {
        this->objectOnInit();
        this->imageSet(image);
    }

    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

protected:
    zfoverride
    virtual void nativeImplViewMarginImplUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin);
    zfoverride
    virtual void nativeImplViewOnLayout(ZF_OUT ZFUIRect &ret,
                                        ZF_IN const ZFUIRect &bounds,
                                        ZF_IN const ZFUIMargin &nativeImplViewMargin);
    /**
     * @brief image view would measure accorrding image size
     */
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIImageView_h_

