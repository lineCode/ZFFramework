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
 * @file ZFPathType_res.h
 * @brief #ZFPathInfo impl as res file
 */

#ifndef _ZFI_ZFPathType_res_h_
#define _ZFI_ZFPathType_res_h_

#include "ZFFile.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPathInfo
 *
 * pathData is the res path
 * @note path must be well formed, use #ZFFilePathFormat if necessary
 */
#define ZFPathType_res "res"

// ============================================================
// ZFInputForResFile
/**
 * @brief util to create a resource file input callback,
 *   see #ZFFileResOpen for what resource file is
 *
 * param:
 * -  (const zfchar *)resFilePath: resource file path to use
 *
 * auto open and auto close files, may return a null callback if open file error
 */
ZFMETHOD_FUNC_DECLARE_INLINE_1(ZFInput, ZFInputForResFile,
                               ZFMP_IN(const zfchar *, resFilePath))
{
    ZFInput ret;
    ZFInputForPathInfoT(ret, ZFPathType_res, resFilePath, ZFFileOpenOption::e_Read);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPathType_res_h_

