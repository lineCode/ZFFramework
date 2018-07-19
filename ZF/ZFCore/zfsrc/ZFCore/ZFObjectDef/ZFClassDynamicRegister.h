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
 * @file ZFClassDynamicRegister.h
 * @brief dynamic register a ZFClass
 */

#ifndef _ZFI_ZFClassDynamicRegister_h_
#define _ZFI_ZFClassDynamicRegister_h_

#include "ZFClass.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief dynamic register a class
 *
 * parent class must be object type that can be allocated\n
 * return null if parent class not valid
 * or class name already exists
 *
 * @note dynamic registered contents would be removed automatically
 *   during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkHigh
 */
extern ZF_ENV_EXPORT const ZFClass *ZFClassDynamicRegister(ZF_IN const zfchar *classNameFull,
                                                           ZF_IN_OPT const ZFClass *parent = zfnull,
                                                           ZF_IN_OPT ZFObject *classDynamicRegisterUserData = zfnull,
                                                           ZF_OUT_OPT zfstring *errorHint = zfnull);
/**
 * @brief unregister class that registered by #ZFClassDynamicRegister
 */
extern ZF_ENV_EXPORT void ZFClassDynamicUnregister(ZF_IN const ZFClass *cls);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFClassDynamicRegister_h_

