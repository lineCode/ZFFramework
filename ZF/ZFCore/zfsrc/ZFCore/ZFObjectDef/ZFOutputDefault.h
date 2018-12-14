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
 * @file ZFOutputDefault.h
 * @brief default callback for output
 */

#ifndef _ZFI_ZFOutputDefault_h_
#define _ZFI_ZFOutputDefault_h_

#include "ZFObjectObserver.h"
#include "ZFIOCallback.h"
ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT const ZFOutput &_ZFP_ZFOutputDefault(void);
/**
 * @brief default output callback, output source must be zfchar *string
 *
 * you may change this at runtime by #ZFOutputDefaultAdd/#ZFOutputDefaultRemove
 */
#define ZFOutputDefault() _ZFP_ZFOutputDefault()

/** @brief see #ZFOutputDefault  */
extern ZF_ENV_EXPORT void ZFOutputDefaultAdd(ZF_IN const ZFOutput &v);
/** @brief see #ZFOutputDefault  */
extern ZF_ENV_EXPORT void ZFOutputDefaultRemove(ZF_IN const ZFOutput &v);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputDefault_h_

