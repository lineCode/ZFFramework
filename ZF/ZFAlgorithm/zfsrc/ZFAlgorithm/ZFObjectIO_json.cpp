/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFObjectIO_json.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECTIO_DEFINE(json, {
        return zfscmpTheSame(fileExt, ZFObjectIO_json);
    }, {
        return ZFObjectFromJson(ret, input, outErrorHint);
    }, {
        return ZFObjectToJson(output, obj, outErrorHint);
    })

ZF_NAMESPACE_GLOBAL_END

