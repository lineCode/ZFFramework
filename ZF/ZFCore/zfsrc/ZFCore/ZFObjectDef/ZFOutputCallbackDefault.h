/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
/**
 * @file ZFOutputCallbackDefault.h
 * @brief default callback for output
 */

#ifndef _ZFI_ZFOutputCallbackDefault_h_
#define _ZFI_ZFOutputCallbackDefault_h_

#include "ZFObjectObserver.h"
#include "ZFIOCallback.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFOutputCallbackDefault changed,
 * params and result has no use and is always null
 */
ZFOBSERVER_EVENT_GLOBAL(ZFOutputCallbackDefaultOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

extern ZF_ENV_EXPORT ZFOutputCallback _ZFP_ZFOutputCallbackDefault;
/**
 * @brief default output callback, output source must be zfchar *string
 *
 * you may change this at runtime by #ZFOutputCallbackDefaultSet,
 * and #ZFGlobalEvent::EventZFOutputCallbackDefaultOnChange would be fired
 */
#define ZFOutputCallbackDefault() ((const ZFOutputCallback &)_ZFP_ZFOutputCallbackDefault)
/**
 * @brief see #ZFOutputCallbackDefault
 */
extern ZF_ENV_EXPORT void ZFOutputCallbackDefaultSet(ZF_IN const ZFOutputCallback &v);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputCallbackDefault_h_

