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
 * @file ZFUIRootView.h
 * @brief root view for holding a ZFUIView's tree
 */

#ifndef _ZFI_ZFUIRootView_h_
#define _ZFI_ZFUIRootView_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief root view for holding a ZFUIView's tree
 *
 * internally used to manage view's,
 * usually you should not use this class
 */
zffinal zfclass ZF_ENV_EXPORT ZFUIRootView : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIRootView, ZFUIView)
    ZFCLASS_PRIVATE_ALLOC("can only be created by owner ZFUISysWindow")

public:
    /**
     * @brief set scale for root view, as well as all of its children
     */
    ZFMETHOD_DECLARE_1(void, scaleForAppSet,
                       ZFMP_IN(zffloat, scale))

protected:
    zfoverride
    virtual void viewOnAddToParent(ZF_IN ZFUIView *parent);
    zfoverride
    virtual void viewOnRemoveFromParent(ZF_IN ZFUIView *parent);

protected:
    /**
     * @brief for a root view, this method would always return sizeHint
     */
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIRootView_h_

