/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#ifndef _ZFI_ZFImpl_sys_iOS_ZFUIKit_impl_h_
#define _ZFI_ZFImpl_sys_iOS_ZFUIKit_impl_h_

#include "ZFImpl.h"

#include "ZFImpl/sys_iOS/ZFImpl_sys_iOS_ZF_impl.h"
#include "ZFUIKit/ZFUITypeDef.h"

#if ZF_ENV_sys_iOS

#include <UIKit/UIKit.h>
ZF_NAMESPACE_GLOBAL_BEGIN

// convert utility
extern ZF_ENV_EXPORT void ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIPointToCGPoint(ZF_OUT CGPoint &ret, ZF_IN const ZFUIPoint &point);
inline CGPoint ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIPointToCGPoint(ZF_IN const ZFUIPoint &point)
{
    CGPoint ret = CGPointZero;
    ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIPointToCGPoint(ret, point);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIPointFromCGPoint(ZF_OUT ZFUIPoint &ret, ZF_IN const CGPoint &cgPoint);
inline ZFUIPoint ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIPointFromCGPoint(ZF_IN const CGPoint &cgPoint)
{
    ZFUIPoint ret = ZFUIPointZero();
    ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIPointFromCGPoint(ret, cgPoint);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_iOS_ZFUIKit_impl_ZFUISizeToCGSize(ZF_OUT CGSize &ret, ZF_IN const ZFUISize &size);
inline CGSize ZFImpl_sys_iOS_ZFUIKit_impl_ZFUISizeToCGSize(ZF_IN const ZFUISize &size)
{
    CGSize ret = CGSizeZero;
    ZFImpl_sys_iOS_ZFUIKit_impl_ZFUISizeToCGSize(ret, size);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_iOS_ZFUIKit_impl_ZFUISizeFromCGSize(ZF_OUT ZFUISize &ret, ZF_IN const CGSize &cgSize);
inline ZFUISize ZFImpl_sys_iOS_ZFUIKit_impl_ZFUISizeFromCGSize(ZF_IN const CGSize &cgSize)
{
    ZFUISize ret = ZFUISizeZero();
    ZFImpl_sys_iOS_ZFUIKit_impl_ZFUISizeFromCGSize(ret, cgSize);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectToCGRect(ZF_OUT CGRect &ret, ZF_IN const ZFUIRect &rect);
inline CGRect ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectToCGRect(ZF_IN const ZFUIRect &rect)
{
    CGRect ret = CGRectZero;
    ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectToCGRect(ret, rect);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectFromCGRect(ZF_OUT ZFUIRect &ret, ZF_IN const CGRect &cgRect);
inline ZFUIRect ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectFromCGRect(ZF_IN const CGRect &cgRect)
{
    ZFUIRect ret = ZFUIRectZero();
    ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectFromCGRect(ret, cgRect);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIMarginToUIEdgeInsets(ZF_OUT UIEdgeInsets &ret, ZF_IN const ZFUIMargin &margin);
inline UIEdgeInsets ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIMarginToUIEdgeInsets(ZF_IN const ZFUIMargin &margin)
{
    UIEdgeInsets ret = UIEdgeInsetsZero;
    ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIMarginToUIEdgeInsets(ret, margin);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIMarginFromUIEdgeInsets(ZF_OUT ZFUIMargin &ret, ZF_IN const UIEdgeInsets &edgeInsets);
inline ZFUIMargin ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIMarginFromUIEdgeInsets(ZF_IN const UIEdgeInsets &edgeInsets)
{
    ZFUIMargin ret = ZFUIMarginZero();
    ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIMarginFromUIEdgeInsets(ret, edgeInsets);
    return ret;
}

extern ZF_ENV_EXPORT UIColor *ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIColorToUIColor(ZF_IN const ZFUIColor &color);
extern ZF_ENV_EXPORT void ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIColorFromUIColor(ZF_OUT ZFUIColor &ret, ZF_IN UIColor *uiColor);
inline ZFUIColor ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIColorFromUIColor(ZF_IN UIColor *uiColor)
{
    ZFUIColor ret = ZFUIColorZero();
    ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIColorFromUIColor(ret, uiColor);
    return ret;
}

// ============================================================
// util method
extern ZF_ENV_EXPORT zfbool ZFImpl_sys_iOS_ZFUIKit_impl_UIColorToARGB(ZF_IN_OUT zfuint *pARGB, ZF_IN UIColor *color);

extern ZF_ENV_EXPORT void ZFImpl_sys_iOS_ZFUIKit_impl_UIColorGetInfo(ZF_OUT zfstring &ret, ZF_IN UIColor *color);
inline zfstring ZFImpl_sys_iOS_ZFUIKit_impl_UIColorGetInfo(ZF_IN UIColor *color)
{
    zfstring ret;
    ZFImpl_sys_iOS_ZFUIKit_impl_UIColorGetInfo(ret, color);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_iOS_ZFUIKit_impl_UIViewGetViewTree(ZF_OUT zfstring &ret, ZF_IN UIView *view);
inline zfstring ZFImpl_sys_iOS_ZFUIKit_impl_UIViewGetViewTree(ZF_IN UIView *view)
{
    zfstring ret;
    ZFImpl_sys_iOS_ZFUIKit_impl_UIViewGetViewTree(ret, view);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_iOS
#endif // #ifndef _ZFI_ZFImpl_sys_iOS_ZFUIKit_impl_h_

