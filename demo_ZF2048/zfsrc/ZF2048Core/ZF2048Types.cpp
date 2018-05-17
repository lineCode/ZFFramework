/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZF2048Types.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZF2048Point ZF2048PointZero = {0, 0};

ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZF2048Value, ZF2048Value, {
        return zfsToIntT(v, src, srcLen);
    }, {
        return zfsFromIntT(s, v);
    })

ZF_NAMESPACE_GLOBAL_END

