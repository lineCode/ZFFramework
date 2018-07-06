/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFOutputDefault.h"
#include "ZFObjectImpl.h"
#include "ZFGlobalEventCenter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFOutputDefaultOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFOutput _ZFP_ZFOutputDefault;

void ZFOutputDefaultSet(ZF_IN const ZFOutput &v)
{
    _ZFP_ZFOutputDefault = v;
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFOutputDefaultOnChange());
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(const ZFOutput &, ZFOutputDefault)
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFOutputDefaultSet, ZFMP_IN(const ZFOutput &, v))

ZF_NAMESPACE_GLOBAL_END
#endif

