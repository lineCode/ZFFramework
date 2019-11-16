/**
 * @file ZFUIViewStateAni_alpha.h
 * @brief alpha animation for #ZFUIViewStateAniStart
 */

#ifndef _ZFI_ZFUIViewStateAni_alpha_h_
#define _ZFI_ZFUIViewStateAni_alpha_h_

#include "ZFUIViewStateAni.h"
#include "ZFAnimationNativeView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief alpha animation for #ZFUIViewStateAniStart
 *
 * if a view's #ZFUIView::layoutedFramePrev's size is zero but #ZFUIView::layoutedFrame is not,
 * a alpha animation of #ZFAnimationNativeView would be applied
 */
zfclass ZF_ENV_EXPORT ZFUIViewStateAniSettingForAlpha : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUIViewStateAniSettingForAlpha, ZFStyleableObject)
    ZFOBJECT_SINGLETON_DECLARE_DETAIL(private, public, ZFUIViewStateAniSettingForAlpha, instance)

    /**
     * @brief whether enable this ani
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, aniEnable, zftrue)
    /**
     * @brief filter to ignore views from being auto animated, empty by default
     */
    ZFFilterForZFObject aniFilter;
    /**
     * @brief minimum offset to apply the animation, #ZFUIGlobalStyle::itemSpace by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniToleranceX, ZFUIGlobalStyle::DefaultStyle()->itemSpace())
    /**
     * @brief minimum offset to apply the animation, #ZFUIGlobalStyle::itemSpace by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, aniToleranceY, ZFUIGlobalStyle::DefaultStyle()->itemSpace())
    /**
     * @brief ani duration, 0 to use #ZFAnimationDurationDefault, 0 by default
     */
    ZFPROPERTY_ASSIGN(zftimet, aniDuration)
    /**
     * @brief ani curve, #ZFAnimationNativeViewCurve::e_EaseOut by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFAnimationNativeViewCurveEnum, aniCurve, ZFAnimationNativeViewCurve::e_EaseOut)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewStateAni_alpha_h_

