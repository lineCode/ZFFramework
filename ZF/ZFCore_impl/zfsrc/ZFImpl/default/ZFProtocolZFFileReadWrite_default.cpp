/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFFileReadWrite.h"
#include "ZFCore/ZFString.h"
#include <stdio.h>

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFFileReadWriteImpl_default, ZFFileReadWrite, ZFProtocolLevel::e_Default)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("C:FILE"))
public:
    virtual ZFToken fileOpen(ZF_IN const zfchar *filePath,
                             ZF_IN_OPT ZFFileOpenOptionFlags flag = ZFFileOpenOption::e_Read)
    {
        const zfcharA *sFlag = zfnull;
        if(ZFBitTest(flag, ZFFileOpenOption::e_Append))
        {
            if(ZFBitTest(flag, ZFFileOpenOption::e_Read))
            {
                sFlag = zfTextA("a+b");
            }
            else
            {
                sFlag = zfTextA("ab");
            }
        }
        else if(ZFBitTest(flag, ZFFileOpenOption::e_Create))
        {
            if(ZFBitTest(flag, ZFFileOpenOption::e_Read))
            {
                sFlag = zfTextA("w+b");
            }
            else
            {
                sFlag = zfTextA("wb");
            }
        }
        else if(ZFBitTest(flag, ZFFileOpenOption::e_Write))
        {
            if(ZFBitTest(flag, ZFFileOpenOption::e_Read))
            {
                sFlag = zfTextA("r+b");
            }
            else
            {
                sFlag = zfTextA("wb");
            }
        }
        else if(ZFBitTest(flag, ZFFileOpenOption::e_Read))
        {
            sFlag = zfTextA("rb");
        }
        else
        {
            zfCoreCriticalShouldNotGoHere();
            return ZFTokenInvalid();
        }

        #if ZF_ENV_sys_Windows
            zfstringW _filePathSaved = ZFStringZ2W(filePath);
            DWORD _fileAttrSaved = GetFileAttributesW(_filePathSaved.cString());
            SetFileAttributesW(_filePathSaved.cString(), FILE_ATTRIBUTE_NORMAL);
        #endif

        ZFToken ret = (ZFToken)(fopen(ZFStringZ2A(filePath), sFlag));

        #if ZF_ENV_sys_Windows
            SetFileAttributesW(_filePathSaved.cString(), _fileAttrSaved);
        #endif

        return ret;
    }
    virtual zfbool fileClose(ZF_IN ZFToken token)
    {
        if(token == ZFTokenInvalid())
        {
            return zftrue;
        }
        return (fclose((FILE *)token) == 0);
    }

    virtual zfindex fileTell(ZF_IN ZFToken token)
    {
        if(token == ZFTokenInvalid())
        {
            return zfindexMax();
        }
        long result = ftell((FILE *)token);
        if(result == -1)
        {
            return zfindexMax();
        }
        return (zfindex)result;
    }
    virtual zfbool fileSeek(ZF_IN ZFToken token,
                            ZF_IN zfindex byteSize,
                            ZF_IN_OPT ZFSeekPos position = ZFSeekPosBegin)
    {
        if(token == ZFTokenInvalid())
        {
            return zffalse;
        }
        int tmpPos = 0;
        long seekSize = (long)byteSize;
        switch(position)
        {
            case ZFSeekPosBegin:
                tmpPos = SEEK_SET;
                break;
            case ZFSeekPosCur:
                tmpPos = SEEK_CUR;
                break;
            case ZFSeekPosCurReversely:
                tmpPos = SEEK_CUR;
                seekSize = -seekSize;
                break;
            case ZFSeekPosEnd:
                tmpPos = SEEK_END;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                break;
        }
        return (fseek((FILE *)token, seekSize, tmpPos) == 0);
    }

    virtual zfindex fileRead(ZF_IN ZFToken token,
                             ZF_IN void *buf,
                             ZF_IN zfindex maxByteSize)
    {
        if(token == ZFTokenInvalid())
        {
            return 0;
        }
        else
        {
            return (zfindex)fread(buf, 1, (size_t)maxByteSize, (FILE *)token);
        }
    }

    virtual zfindex fileWrite(ZF_IN ZFToken token,
                              ZF_IN const void *src,
                              ZF_IN zfindex maxByteSize)
    {
        if(token == ZFTokenInvalid())
        {
            return 0;
        }
        else
        {
            return (zfindex)fwrite(src, 1, (size_t)maxByteSize, (FILE *)token);
        }
    }
    virtual void fileFlush(ZF_IN ZFToken token)
    {
        if(token != ZFTokenInvalid())
        {
            fflush((FILE *)token);
        }
    }
    virtual zfbool fileIsEof(ZF_IN ZFToken token)
    {
        return (feof((FILE *)token) != 0);
    }
    virtual zfbool fileIsError(ZF_IN ZFToken token)
    {
        return (ferror((FILE *)token) != 0);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFFileReadWriteImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFFileReadWriteImpl_default)

ZF_NAMESPACE_GLOBAL_END

