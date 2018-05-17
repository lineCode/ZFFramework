/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFClassSerializable.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFTYPEID_ACCESS_ONLY_DEFINE(ZFClassInstanceObserverAddParam, ZFClassInstanceObserverAddParam)

ZFTYPEID_DEFINE_BY_STRING_CONVERTER(ZFClass, const ZFClass *, {
        v = ZFClass::classForName((srcLen == zfindexMax()) ? src : zfstring(src, srcLen).cString());
        return (v != zfnull);
    }, {
        if(v)
        {
            s += v->className();
        }
        return zftrue;
    })

ZF_NAMESPACE_GLOBAL_END

