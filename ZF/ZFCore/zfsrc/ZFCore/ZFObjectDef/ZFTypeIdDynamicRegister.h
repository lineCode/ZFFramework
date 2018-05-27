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
 * @file ZFTypeIdDynamicRegister.h
 * @brief dynamic register #ZFTypeIdBase
 */

#ifndef _ZFI_ZFTypeIdDynamicRegister_h_
#define _ZFI_ZFTypeIdDynamicRegister_h_

#include "ZFTypeId.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief dynamic register #ZFTypeIdBase
 *
 * @note dynamic registered contents would be removed automatically
 *   during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkHigh
 */
extern ZF_ENV_EXPORT zfbool ZFTypeIdDynamicRegister(ZF_IN const zfchar *typeIdName,
                                                    ZF_IN const ZFCorePointerForObject<ZFTypeIdBase *> &typeIdData,
                                                    ZF_OUT_OPT zfstring *errorHint = zfnull);
/**
 * @brief see #ZFTypeIdDynamicRegister
 */
extern ZF_ENV_EXPORT void ZFTypeIdDynamicUnregister(ZF_IN const zfchar *typeIdName);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTypeIdDynamicRegister_h_

