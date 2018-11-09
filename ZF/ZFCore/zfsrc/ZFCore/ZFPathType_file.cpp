/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFPathType_file.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPATHTYPE_DEFINE(file)

// ============================================================
ZFPATHTYPE_FILEIO_REGISTER(file, ZFPathType_file()
        , ZFFileFileIsExist
        , ZFFileFileIsDir
        , ZFFilePathInfoCallbackGetFileNameDefault
        , ZFFilePathInfoCallbackToChildDefault
        , ZFFilePathInfoCallbackToParentDefault
        , ZFFileFilePathCreate
        , ZFFileFileRemove
        , ZFFileFileFindFirst
        , ZFFileFileFindNext
        , ZFFileFileFindClose
        , ZFFileFileOpen
        , ZFFileFileClose
        , ZFFileFileTell
        , ZFFileFileSeek
        , ZFFileFileRead
        , ZFFileFileWrite
        , ZFFileFileFlush
        , ZFFileFileIsEof
        , ZFFileFileIsError
        , ZFFileFileSize
    )

// ============================================================
// ZFInputForFile
ZFMETHOD_FUNC_DEFINE_INLINE_2(ZFInput, ZFInputForFile,
                              ZFMP_IN(const zfchar *, filePath),
                              ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read))

// ============================================================
// ZFOutputForFile
ZFMETHOD_FUNC_DEFINE_INLINE_2(ZFOutput, ZFOutputForFile,
                              ZFMP_IN(const zfchar *, filePath),
                              ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))

ZF_NAMESPACE_GLOBAL_END

