/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFObjectIO_zfsd.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(zfsd, {
        return ZFObjectIOImplCheck(pathInfo, "zfsd");
    }, {
        return ZFObjectFromZfsd(ret, input, outErrorHint);
    }, {
        return ZFObjectToZfsd(output, obj, outErrorHint);
    })

ZF_NAMESPACE_GLOBAL_END

