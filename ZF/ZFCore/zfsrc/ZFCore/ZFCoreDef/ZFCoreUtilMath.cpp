/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFCoreUtilMath.h"
#include "ZFCoreStaticInitializer.h"
#include <time.h>
#include <stdlib.h>

ZF_NAMESPACE_GLOBAL_BEGIN

const zffloat zffloatEpsilonT<zffloat>::v = zffloatEpsilon;
const zfdouble zffloatEpsilonT<zfdouble>::v = zfdoubleEpsilon;
const zflongdouble zffloatEpsilonT<zflongdouble>::v = zflongdoubleEpsilon;

// ============================================================
ZF_STATIC_REGISTER_INIT(ZFCoreUtilMathDataHolder)
{
    srand((zfuint)time(zfnull));
}
ZF_STATIC_REGISTER_END(ZFCoreUtilMathDataHolder)

zfuint zfmRand(void)
{
    return (zfuint)rand();
}

ZF_NAMESPACE_GLOBAL_END

