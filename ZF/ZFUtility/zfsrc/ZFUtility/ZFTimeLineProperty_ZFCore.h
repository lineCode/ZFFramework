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
 * @file ZFTimeLineProperty_ZFCore.h
 * @brief see #ZFTimeLineProperty
 */

#ifndef _ZFI_ZFTimeLineProperty_ZFCore_h_
#define _ZFI_ZFTimeLineProperty_ZFCore_h_

#include "ZFTimeLineProperty.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFTIMELINEPROPERTY_DECLARE_BY_VALUE_DETAIL(ZFTimeLineProperty_zfbool, zfbool, {
    return ((progress <= 0.5f) ? fromValue : toValue);
})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLineProperty_ZFCore_h_

