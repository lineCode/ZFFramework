/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFEvent.h"

ZF_NAMESPACE_GLOBAL_BEGIN
ZFOBJECT_REGISTER(ZFEvent)

void ZFEvent::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    if(this->eventResolved())
    {
        ret += zfText(" (resolved)");
    }
    ZFClassUtil::objectPropertyInfo(ret, this);
}

ZF_NAMESPACE_GLOBAL_END

