/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFString.h"
#include "ZFCore/ZFString.h"
#include "ZFImpl/tools/UTFCodeUtil/UTFCodeUtil.h"

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFStringImpl_sys_iOS, ZFString, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("iOS:NSString"))
public:
    virtual void *nativeStringCreate(ZF_IN_OPT const zfchar *s = zfnull)
    {
        NSString *ret = nil;
        if(s == zfnull)
        {
            ret = [NSString string];
        }
        else
        {
            ret = [NSString stringWithCString:ZFStringZ2A(s) encoding:NSUTF8StringEncoding];
        }
        return (__bridge_retained void *)ret;
    }

    virtual void *nativeStringRetain(ZF_IN void *nativeString)
    {
        NSString *tmp = (__bridge NSString *)nativeString;
        return (__bridge_retained void *)tmp;
    }
    virtual void nativeStringRelease(ZF_IN void *nativeString)
    {
        NSString *tmp = (__bridge_transfer NSString *)nativeString;
        tmp = nil;
    }

    virtual void stringValueAccess(ZF_IN void *nativeString,
                                   ZF_OUT const zfchar *&stringValue,
                                   ZF_OUT void *&stringValueToken)
    {
#if ZF_ENV_ZFCHAR_USE_CHAR_A
        stringValue = ((__bridge NSString *)nativeString).UTF8String;
#endif
#if ZF_ENV_ZFCHAR_USE_CHAR_A
        zfstring *token = zfnew(zfstring);
        ZFString::toZFChar(*token, ((__bridge NSString *)nativeString).UTF8String, ZFStringEncoding::e_UTF8);
        stringValue = token->cString();
        stringValueToken = token;
#endif
    }
    virtual void stringValueAccessCleanup(ZF_IN void *nativeString,
                                          ZF_IN const zfchar *stringValue,
                                          ZF_IN void *stringValueToken)
    {
#if ZF_ENV_ZFCHAR_USE_CHAR_A
        // noting to do
#endif
#if ZF_ENV_ZFCHAR_USE_CHAR_A
        zfdelete((zfstring *)stringValueToken);
#endif
    }

    virtual zfbool toUTF8(ZF_OUT zfstringA &result,
                          ZF_IN const void *s,
                          ZF_IN ZFStringEncodingEnum srcEncoding)
    {
        switch(srcEncoding)
        {
            case ZFStringEncoding::e_UTF8:
                result += (const zfcharA *)s;
                return zftrue;
            case ZFStringEncoding::e_UTF16LE:
            {
                zfcharA *sUTF8 = UTFCodeUtil::UTF16ToUTF8((const zfcharW *)s);
                if(sUTF8 == zfnull) {return zffalse;}
                result += sUTF8;
                zffree(sUTF8);
                return zftrue;
            }
            case ZFStringEncoding::e_UTF16BE:
            {
                zfcharW *sUTF16 = UTFCodeUtil::UTF16BEToUTF16((const zfcharW *)s);
                if(sUTF16 == zfnull) {return zffalse;}
                zfcharA *sUTF8 = UTFCodeUtil::UTF16ToUTF8(sUTF16);
                zffree(sUTF16);
                if(sUTF8 == zfnull) {return zffalse;}
                result += sUTF8;
                zffree(sUTF8);
                return zftrue;
            }
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    virtual zfbool toUTF16(ZF_OUT zfstringW &result,
                           ZF_IN const void *s,
                           ZF_IN ZFStringEncodingEnum srcEncoding)
    {
        switch(srcEncoding)
        {
            case ZFStringEncoding::e_UTF8:
            {
                zfcharW *sUTF16 = UTFCodeUtil::UTF8ToUTF16((const zfcharA *)s);
                if(sUTF16 == zfnull) {return zffalse;}
                result += sUTF16;
                zffree(sUTF16);
                return zftrue;
            }
            case ZFStringEncoding::e_UTF16LE:
                result += (const zfcharW *)s;
                return zftrue;
            case ZFStringEncoding::e_UTF16BE:
            {
                zfcharW *sUTF16 = UTFCodeUtil::UTF16BEToUTF16((const zfcharW *)s);
                if(sUTF16 == zfnull) {return zffalse;}
                result += sUTF16;
                zffree(sUTF16);
                return zftrue;
            }
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    virtual zfbool toUTF16BE(ZF_OUT zfstringW &result,
                             ZF_IN const void *s,
                             ZF_IN ZFStringEncodingEnum srcEncoding)
    {
        switch(srcEncoding)
        {
            case ZFStringEncoding::e_UTF8:
            {
                zfcharW *sUTF16 = UTFCodeUtil::UTF8ToUTF16((const zfcharA *)s);
                if(sUTF16 == zfnull) {return zffalse;}
                zfcharW *sUTF16BE = UTFCodeUtil::UTF16ToUTF16BE(sUTF16);
                zffree(sUTF16);
                if(sUTF16BE == zfnull) {return zffalse;}
                result += sUTF16BE;
                zffree(sUTF16BE);
                return zftrue;
            }
            case ZFStringEncoding::e_UTF16LE:
            {
                zfcharW *sUTF16BE = UTFCodeUtil::UTF16ToUTF16BE((const zfcharW *)s);
                if(sUTF16BE == zfnull) {return zffalse;}
                result += sUTF16BE;
                zffree(sUTF16BE);
                return zftrue;
            }
            case ZFStringEncoding::e_UTF16BE:
                result += (const zfcharW *)s;
                return zftrue;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
    }
    virtual zfindex wordCountOfUTF8(ZF_IN const zfcharA *utf8String)
    {
        return UTFCodeUtil::UTF8GetWordCount(utf8String);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFStringImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFStringImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

