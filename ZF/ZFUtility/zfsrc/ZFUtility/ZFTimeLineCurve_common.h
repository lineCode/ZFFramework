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
 * @file ZFTimeLineCurve_common.h
 * @brief see #ZFTimeLineCurve
 */

#ifndef _ZFI_ZFTimeLineCurve_common_h_
#define _ZFI_ZFTimeLineCurve_common_h_

#include "ZFTimeLineCurve.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief linear time line, see #ZFTimeLineCurve
 */
zfclass ZF_ENV_EXPORT ZFTimeLineCurveLinear : zfextends ZFTimeLineCurve
{
    ZFOBJECT_DECLARE(ZFTimeLineCurveLinear, ZFTimeLineCurve)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

protected:
    zfoverride
    virtual zffloat progressOnUpdate(ZF_IN zffloat time)
    {
        return time;
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTimeLineCurve_common_h_

