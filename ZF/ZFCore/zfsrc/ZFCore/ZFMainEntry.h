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
 * @file ZFMainEntry.h
 * @brief app's main entry wrapper
 */

#ifndef _ZFI_ZFMainEntry_h_
#define _ZFI_ZFMainEntry_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

typedef zfint (*_ZFP_ZFMainFuncType)(ZF_IN_OPT ZFCoreArray<zfstring> const &params);
extern ZF_ENV_EXPORT void _ZFP_ZFMainRegister(ZF_IN _ZFP_ZFMainFuncType func);
/**
 * @brief app's main entry wrapper
 *
 * typical usage:
 * @code
 *   // in your main.cpp
 *   #include "ZFMainEntry.h"
 *   ZFMAIN_ENTRY(params)
 *   {
 *       // do your work,
 *       // which usually is creating your main window
 *
 *       // params hold the params from certain app routine,
 *       // it's not ensured each implementation would have same params format,
 *       // also note that params is not ensured keep alive during app life time
 *       return 0;
 *   }
 * @endcode
 */
#define ZFMAIN_ENTRY(paramName) \
    zfint _ZFMain(ZF_IN_OPT ZFCoreArray<zfstring> const &paramName); \
    ZF_STATIC_REGISTER_INIT(ZFMainEntryRegister) \
    { \
        _ZFP_ZFMainRegister(_ZFMain); \
    } \
    ZF_STATIC_REGISTER_END(ZFMainEntryRegister) \
    zfint _ZFMain(ZF_IN_OPT ZFCoreArray<zfstring> const &paramName)

/**
 * @brief enter main app entry
 *
 * used by implementation only
 */
extern ZF_ENV_EXPORT zfint ZFMainExecute(ZF_IN_OPT ZFCoreArray<zfstring> const &params = ZFCoreArray<zfstring>());

/**
 * @brief util method to call #ZFFrameworkInit, #ZFMainExecute and #ZFFrameworkCleanup
 */
extern ZF_ENV_EXPORT int ZFMainCommonEntry(ZF_IN int argc = 0, char **argv = zfnull);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMainEntry_h_

