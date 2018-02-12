/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIViewCapture.h"
#include "protocol/ZFProtocolZFUIViewCapture.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_DEFINE_1(zfautoObject, ZFUIViewCapture,
                       ZFMP_IN(ZFUIView *, view))
{
    if(view == zfnull)
    {
        return zfautoObjectNull();
    }
    zfautoObject ret = ZFUIImage::ClassData()->newInstance();
    if(!ZFPROTOCOL_ACCESS(ZFUIViewCapture)->viewCapture(view, ret.to<ZFUIImage *>()))
    {
        return zfautoObjectNull();
    }
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

