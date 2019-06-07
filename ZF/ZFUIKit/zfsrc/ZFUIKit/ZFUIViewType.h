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
 * @file ZFUIViewType.h
 * @brief additional class for ZFUIView
 */

#ifndef _ZFI_ZFUIViewType_h_
#define _ZFI_ZFUIViewType_h_

#include "ZFUIGlobalStyle.h"
#include "ZFUILayoutParam.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief three layer for ZFUIView
 */
ZFENUM_BEGIN(ZFUIViewChildLayer)
    ZFENUM_VALUE(Normal) /**< @brief added by #ZFUIView::childAdd */
    ZFENUM_VALUE(InternalImpl) /**< @brief added by #ZFUIView::internalImplViewAdd */
    ZFENUM_VALUE(InternalBg) /**< @brief added by #ZFUIView::internalBgViewAdd */
    ZFENUM_VALUE(InternalFg) /**< @brief added by #ZFUIView::internalFgViewAdd */
ZFENUM_SEPARATOR(ZFUIViewChildLayer)
    ZFENUM_VALUE_REGISTER(Normal)
    ZFENUM_VALUE_REGISTER(InternalImpl)
    ZFENUM_VALUE_REGISTER(InternalBg)
    ZFENUM_VALUE_REGISTER(InternalFg)
ZFENUM_END(ZFUIViewChildLayer)

// ============================================================
// ZFUIViewMeasureResult
/**
 * @brief data used by #ZFUIView::EventViewLayoutOnMeasureFinish,
 *   you may modify the #ZFUIViewMeasureResult::measuredSize
 *   to override the measured result
 */
zfclass ZF_ENV_EXPORT ZFUIViewMeasureResult : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIViewMeasureResult, ZFObject)

public:
    /** @brief see #ZFUIViewMeasureResult */
    ZFUISize sizeHint;
    /** @brief see #ZFUIViewMeasureResult */
    ZFUISizeParam sizeParam;
    /** @brief see #ZFUIViewMeasureResult */
    ZFUISize measuredSize;

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }
};

// ============================================================
// ZFUIViewLayoutParam
zfclassFwd ZFUIView;
/**
 * @brief base class of layout params for ZFUIView
 *
 * ZFUIView supply basic but useful dynamic layout mechanism,
 * which use align, margin and measure logic to supply dynamic layout\n
 * while layout a child, ZFUIView would do those things:
 * -  measure child to see it's size depends on #sizeParam
 * -  layout child depends on #ZFUIAlignApply
 *
 * you should always use #ZFUIView::layoutParamCreate to create a layout param\n
 * subclass may override or define new params to supply more complex layout mechanism\n
 * \n
 * dynamic layout logic depends on #ZFUIViewLayoutParam::sizeHint and #ZFUIViewLayoutParam::sizeParam
 */
zfclass ZF_ENV_EXPORT ZFUIViewLayoutParam : zfextends ZFUILayoutParam
{
    ZFOBJECT_DECLARE(ZFUIViewLayoutParam, ZFUILayoutParam)

public:
    /** @brief see #layoutParamApply */
    static void layoutParamApplyT(ZF_OUT ZFUIRect &ret,
                                  ZF_IN const ZFUIRect &rect,
                                  ZF_IN ZFUIView *child,
                                  ZF_IN ZFUIViewLayoutParam *lp);
    /**
     * @brief calculate single child's frame using logic of #ZFUIViewLayoutParam
     *
     * this is not necessary for subclass to declare its own one,
     * it's declared for convenient for subclass to layout child
     * using parent's layout logic
     */
    static ZFUIRect layoutParamApply(ZF_IN const ZFUIRect &rect,
                                     ZF_IN ZFUIView *child,
                                     ZF_IN ZFUIViewLayoutParam *lp)
    {
        ZFUIRect ret = ZFUIRectZero();
        ZFUIViewLayoutParam::layoutParamApplyT(ret, rect, child, lp);
        return ret;
    }

    /**
     * @brief util method to apply sizeHint accorrding sizeParam
     */
    ZFMETHOD_DECLARE_DETAIL_4(public, ZFMethodTypeStatic,
                              void, sizeHintApply,
                              ZFMP_OUT(zfint &, ret),
                              ZFMP_IN(zfint, size),
                              ZFMP_IN(zfint, sizeHint),
                              ZFMP_IN(ZFUISizeTypeEnum, sizeParam))
    /** @brief see #sizeHintApply */
    ZFMETHOD_INLINE_DETAIL_3(public, ZFMethodTypeStatic,
                             zfint, sizeHintApply,
                             ZFMP_IN(zfint, size),
                             ZFMP_IN(zfint, sizeHint),
                             ZFMP_IN(ZFUISizeTypeEnum, sizeParam))
    {
        zfint ret = size;
        ZFUIViewLayoutParam::sizeHintApply(ret, size, sizeHint, sizeParam);
        return ret;
    }

    /**
     * @brief util method to apply sizeHint accorrding sizeParam
     */
    ZFMETHOD_DECLARE_DETAIL_4(public, ZFMethodTypeStatic,
                              void, sizeHintApply,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, size),
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(const ZFUISizeParam &, sizeParam))
    /** @brief see #sizeHintApply */
    ZFMETHOD_INLINE_DETAIL_3(public, ZFMethodTypeStatic,
                             ZFUISize, sizeHintApply,
                             ZFMP_IN(const ZFUISize &, size),
                             ZFMP_IN(const ZFUISize &, sizeHint),
                             ZFMP_IN(const ZFUISizeParam &, sizeParam))
    {
        ZFUISize ret = ZFUISizeZero();
        ZFUIViewLayoutParam::sizeHintApply(ret, size, sizeHint, sizeParam);
        return ret;
    }

    /**
     * @brief merge two size hint
     */
    ZFMETHOD_DECLARE_DETAIL_3(public, ZFMethodTypeStatic,
                              void, sizeHintMerge,
                              ZFMP_OUT(zfint &, ret),
                              ZFMP_IN(zfint, sizeHint0),
                              ZFMP_IN(zfint, sizeHint1))
    /** @brief see #sizeHintMerge */
    ZFMETHOD_INLINE_DETAIL_2(public, ZFMethodTypeStatic,
                             zfint, sizeHintMerge,
                             ZFMP_IN(zfint, sizeHint0),
                             ZFMP_IN(zfint, sizeHint1))
    {
        zfint ret = 0;
        ZFUIViewLayoutParam::sizeHintMerge(ret, sizeHint0, sizeHint1);
        return ret;
    }

    /**
     * @brief merge two size hint
     */
    ZFMETHOD_DECLARE_DETAIL_3(public, ZFMethodTypeStatic,
                              void, sizeHintMerge,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, sizeHint0),
                              ZFMP_IN(const ZFUISize &, sizeHint1))
    /** @brief see #sizeHintMerge */
    ZFMETHOD_INLINE_DETAIL_2(public, ZFMethodTypeStatic,
                             ZFUISize, sizeHintMerge,
                             ZFMP_IN(const ZFUISize &, sizeHint0),
                             ZFMP_IN(const ZFUISize &, sizeHint1))
    {
        ZFUISize ret = ZFUISizeZero();
        ZFUIViewLayoutParam::sizeHintMerge(ret, sizeHint0, sizeHint1);
        return ret;
    }

    /**
     * @brief safely increase or decrease size hint, do nothing if old one is no limit
     */
    ZFMETHOD_DECLARE_DETAIL_3(public, ZFMethodTypeStatic,
                              void, sizeHintOffset,
                              ZFMP_OUT(zfint &, ret),
                              ZFMP_IN(zfint, sizeHint),
                              ZFMP_IN(zfint, offset))
    /** @brief see #sizeHintOffset */
    ZFMETHOD_INLINE_DETAIL_2(public, ZFMethodTypeStatic,
                             zfint, sizeHintOffset,
                             ZFMP_IN(zfint, sizeHint),
                             ZFMP_IN(zfint, offset))
    {
        zfint ret = 0;
        ZFUIViewLayoutParam::sizeHintOffset(ret, sizeHint, offset);
        return ret;
    }

    /**
     * @brief safely increase or decrease size hint, do nothing if old one is no limit
     */
    ZFMETHOD_DECLARE_DETAIL_3(public, ZFMethodTypeStatic,
                              void, sizeHintOffset,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(const ZFUISize &, offset))
    /** @brief see #sizeHintOffset */
    ZFMETHOD_INLINE_DETAIL_2(public, ZFMethodTypeStatic,
                             ZFUISize, sizeHintOffset,
                             ZFMP_IN(const ZFUISize &, sizeHint),
                             ZFMP_IN(const ZFUISize &, offset))
    {
        ZFUISize ret = ZFUISizeZero();
        ZFUIViewLayoutParam::sizeHintOffset(ret, sizeHint, offset);
        return ret;
    }

    /**
     * @brief safely increase or decrease size hint, do nothing if old one is no limit
     */
    ZFMETHOD_DECLARE_DETAIL_3(public, ZFMethodTypeStatic,
                              void, sizeHintOffset,
                              ZFMP_OUT(ZFUISize &, ret),
                              ZFMP_IN(const ZFUISize &, sizeHint),
                              ZFMP_IN(zfint, offset))
    /** @brief see #sizeHintOffset */
    ZFMETHOD_INLINE_DETAIL_2(public, ZFMethodTypeStatic,
                             ZFUISize, sizeHintOffset,
                             ZFMP_IN(const ZFUISize &, sizeHint),
                             ZFMP_IN(zfint, offset))
    {
        ZFUISize ret = ZFUISizeZero();
        ZFUIViewLayoutParam::sizeHintOffset(ret, sizeHint, offset);
        return ret;
    }

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }

public:
    /**
     * @brief see #ZFUIViewLayoutParam, default is #ZFUISizeInvalid
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISize, sizeHint,
                                ZFUISizeInvalid())
    /**
     * @brief see #ZFUIViewLayoutParam, default is #ZFUISizeParamWrapWrap
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISizeParam, sizeParam,
                                ZFUISizeParamWrapWrap())
    /**
     * @brief see #ZFUIViewLayoutParam, default is (ZFUIAlign::e_LeftInner | ZFUIAlign::e_TopInner)
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIAlignFlags, layoutAlign,
                                ZFUIAlign::e_LeftInner | ZFUIAlign::e_TopInner)
    /**
     * @brief see #ZFUIViewLayoutParam, default is (0, 0, 0, 0)
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIMargin, layoutMargin,
                                ZFUIMarginZero())
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIViewType_h_

