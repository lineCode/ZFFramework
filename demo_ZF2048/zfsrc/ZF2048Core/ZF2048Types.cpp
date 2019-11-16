#include "ZF2048Types.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZF2048Point ZF2048PointZero = {0, 0};

ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZF2048Value, ZF2048Value, {
        return zfsToIntT(v, src, srcLen);
    }, {
        return zfsFromIntT(s, v);
    })

ZF_NAMESPACE_GLOBAL_END

