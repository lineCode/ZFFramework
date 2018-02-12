/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIWheelEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIWheelEvent)
ZFCACHEABLE_DEFINE(ZFUIWheelEvent, ZFUIWheelEvent)

void ZFUIWheelEvent::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    ret += zfText("wheel ");
    zfsFromIntT(ret, this->wheelX);
    ret += zfText(", ");
    zfsFromIntT(ret, this->wheelY);

    if(this->eventResolved())
    {
        ret += zfText(" (resolved)");
    }
}

void ZFUIWheelEvent::cacheableOnReset(void)
{
    zfsuper::cacheableOnReset();
    this->wheelX = 0;
    this->wheelY = 0;
}

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWheelEvent, zfint, wheelX)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIWheelEvent, zfint, wheelY)

ZF_NAMESPACE_GLOBAL_END

