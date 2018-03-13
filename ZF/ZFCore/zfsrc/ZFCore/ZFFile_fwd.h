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
 * @file ZFFile_fwd.h
 * @brief file utility
 */

#ifndef _ZFI_ZFFile_fwd_h_
#define _ZFI_ZFFile_fwd_h_

#include "ZFFileBOM.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFFileSeparator
extern ZF_ENV_EXPORT const zfchar _ZFP_ZFFileSeparator;
extern ZF_ENV_EXPORT const zfchar *_ZFP_ZFFileSeparatorString;

/**
 * @brief file separator (ensured to be '/' for now)
 */
ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(zfchar, ZFFileSeparator, _ZFP_ZFFileSeparator)

/**
 * @brief string version of #ZFFileSeparator
 */
ZFEXPORT_VAR_READONLY_VALUEREF_DECLARE(const zfchar *, ZFFileSeparatorString, _ZFP_ZFFileSeparatorString)

// ============================================================
// ZFFileOpenOption
/**
 * @brief open falgs, similar to fopen's flags,
 *   while we have binary mode only
 *
 * usage:
 * -  ZFFileOpenOptionCreate\n
 *   create a new file for write only
 * -  ZFFileOpenOptionCreate | ZFFileOpenOption::e_Read\n
 *   create a new file for read/write
 * -  ZFFileOpenOption::e_Read\n
 *   open a existing file for read only, fail if file not exist
 * -  ZFFileOpenOption::e_Read | ZFFileOpenOptionWrite\n
 *   open a existing file for read/write, fail if file not exist
 * -  ZFFileOpenOptionWrite\n
 *   same as ZFFileOpenOptionCreate, create a new file for write only
 * -  ZFFileOpenOptionAppend\n
 *   open a file for append or create new if not exist, for write only
 * -  ZFFileOpenOptionAppend | ZFFileOpenOption::e_Read\n
 *   open a file for append or create new if not exist, for read/write
 */
ZFENUM_BEGIN(ZFFileOpenOption)
    ZFENUM_VALUE_WITH_INIT(Create, 1) /**< @brief create for read write */
    ZFENUM_VALUE_WITH_INIT(Read, 2) /**< @brief readonly */
    ZFENUM_VALUE_WITH_INIT(Write, 4) /**< @brief write only */
    ZFENUM_VALUE_WITH_INIT(Append, 8) /**< @brief append only */
ZFENUM_SEPARATOR(ZFFileOpenOption)
    ZFENUM_VALUE_REGISTER(Create)
    ZFENUM_VALUE_REGISTER(Read)
    ZFENUM_VALUE_REGISTER(Write)
    ZFENUM_VALUE_REGISTER(Append)
ZFENUM_END_FLAGS(ZFFileOpenOption, ZFFileOpenOptionFlags)

// ============================================================
zfclassFwd _ZFP_ZFFileFindDataPrivate;
/**
 * @brief data used by ZFFile when finding files
 * @see ZFFileFileFindFirst, ZFFileFileFindNext, ZFFileFileFindClose
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFFileFindData
{
public:
    /** @cond ZFPrivateDoc */
    ZFFileFindData(void);
    ZFFileFindData(ZF_IN ZFFileFindData const &ref);
    ZFFileFindData &operator = (ZF_IN ZFFileFindData const &ref);
    zfbool operator == (ZF_IN ZFFileFindData const &ref) const;
    zfbool operator != (ZF_IN ZFFileFindData const &ref) const
    {
        return !this->operator == (ref);
    }
    /** @endcond */
    ~ZFFileFindData(void);

public:
    /**
     * @brief return path of file
     */
    const zfchar *filePath(void) const
    {
        return this->impl().filePath;
    }
    /**
     * @brief return file name of file
     */
    const zfchar *fileName(void) const
    {
        return this->impl().fileName;
    }
    /**
     * @brief return true if is a directory
     */
    zfbool fileIsDir(void) const
    {
        return this->impl().fileIsDir;
    }

public:
    /**
     * @brief get a short info
     */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #objectInfoT */
    inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    /** @brief for impl to achieve custom find logic */
    zffinal zfclassNotPOD Impl
    {
    public:
        zfstring filePath; /**< @brief file path */
        const zfchar *fileName; /**< @brief file path */
        zfbool fileIsDir; /**< @brief whether directory */
        void *nativeFd; /**< @brief for impl to store native find data */
    public:
        /** @cond ZFPrivateDoc */
        Impl(void)
        : filePath()
        , fileName(zfnull)
        , fileIsDir(zffalse)
        , nativeFd(zfnull)
        {
        }
        /** @endcond */
    public:
        /** @brief util method for impl to parse #fileName from #filePath */
        void fileNameParse(void)
        {
            zfindex t = zfstringFindReversely(filePath, ZFFileSeparator());
            if(t == zfindexMax())
            {
                this->fileName = this->filePath.cString();
            }
            else
            {
                this->fileName = this->filePath.cString() + t + 1;
            }
        }
    };
public:
    /** @brief the impl */
    Impl &impl(void) const;
    /** @brief name for the impl */
    const zfchar *implName(void) const;
    /** @brief user data passed from #implAttach */
    void *implUserData(void) const;
    /** @brief begin first find */
    void implAttach(ZF_IN const zfchar *implName,
                    ZF_IN_OPT void *implUserData = zfnull);
    /** @brief close find */
    void implDetach(void);
    /** @brief check whether impl matches, assert fail if not match, return the #implUserData */
    void *implCheck(ZF_IN const zfchar *implName) const;
private:
    _ZFP_ZFFileFindDataPrivate *d;
};
ZFPROPERTY_TYPE_ACCESS_ONLY_DECLARE(ZFFileFindData, ZFFileFindData)
ZFOUTPUT_TYPE(ZFFileFindData, {output << v.objectInfo();})

// ============================================================
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFFileCallback_pathInfo zfText("pathInfo")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFFileCallback_flags zfText("flags")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFFileCallback_autoSkipBOMTable zfText("autoSkipBOMTable")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFFileCallback_filePath zfText("filePath")

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFile_fwd_h_

