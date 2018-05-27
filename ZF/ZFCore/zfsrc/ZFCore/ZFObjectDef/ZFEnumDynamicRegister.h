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
 * @file ZFEnumDynamicRegister.h
 * @brief dynamic register #ZFEnum
 */

#ifndef _ZFI_ZFEnumDynamicRegister_h_
#define _ZFI_ZFEnumDynamicRegister_h_

#include "ZFEnum.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief dynamic register #ZFEnum
 *
 * @note dynamic registered contents would be removed automatically
 *   during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkHigh
 */
extern ZF_ENV_EXPORT const ZFClass *ZFEnumDynamicRegister(ZF_IN const zfchar *enumClassName,
                                                          ZF_IN const ZFCoreArrayPOD<zfidentity> &enumValues,
                                                          ZF_IN const ZFCoreArray<zfstring> &enumNames,
                                                          ZF_IN zfuint enumDefault = ZFEnumInvalid(),
                                                          ZF_IN zfbool enumIsFlags = zffalse,
                                                          ZF_OUT_OPT zfstring *errorHint = zfnull);
/**
 * @brief see #ZFEnumDynamicRegister
 */
extern ZF_ENV_EXPORT void ZFEnumDynamicUnregister(ZF_IN const ZFClass *enumClass);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFEnumDynamicRegister_h_

