/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFString.h"
#include "ZFCore/ZFString.h"
#include "ZFImpl/tools/UTFCodeUtil/UTFCodeUtil.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFStringImpl_sys_Android, ZFString, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:String"))
public:
    virtual void *nativeStringCreate(ZF_IN_OPT const zfchar *s = zfnull)
    {
        // should always be global ref
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jstring ret = zfnull;
        if(s == zfnull)
        {
            jobject tmp = JNIUtilNewString(jniEnv, zfnull, 0);
            ret = (jstring)JNIUtilNewGlobalRef(jniEnv, tmp);
            JNIUtilDeleteLocalRef(jniEnv, tmp);
        }
        else
        {
            jobject tmp = JNIUtilNewStringUTF(jniEnv, ZFStringZ2A(s));
            ret = (jstring)JNIUtilNewGlobalRef(jniEnv, tmp);
            JNIUtilDeleteLocalRef(jniEnv, tmp);
        }
        return ZFCastStatic(void *, ret);
    }

    virtual void *nativeStringRetain(ZF_IN void *nativeString)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        return ZFCastStatic(void *, JNIUtilNewGlobalRef(jniEnv, ZFCastStatic(jstring, nativeString)));
    }
    virtual void nativeStringRelease(ZF_IN void *nativeString)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, ZFCastStatic(jstring, nativeString));
    }

    virtual void stringValueAccess(ZF_IN void *nativeString,
                                   ZF_OUT const zfchar *&stringValue,
                                   ZF_OUT void *&stringValueToken)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        const zfcharA *tmp = JNIUtilGetStringUTFChars(jniEnv, ZFCastStatic(jstring, nativeString), zfnull);
#if ZF_ENV_ZFCHAR_USE_CHAR_A
        stringValue = tmp;
#endif
#if ZF_ENV_ZFCHAR_USE_CHAR_W
        zfstring *token = zfnew(zfstring);
        ZFString::toZFChar(*token, tmp, ZFStringEncoding::e_UTF8);
        stringValue = token->cString();
        stringValueToken = token;
#endif
    }
    virtual void stringValueAccessCleanup(ZF_IN void *nativeString,
                                          ZF_IN const zfchar *stringValue,
                                          ZF_IN void *stringValueToken)
    {
#if ZF_ENV_ZFCHAR_USE_CHAR_A
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilReleaseStringUTFChars(jniEnv, ZFCastStatic(jstring, nativeString), stringValue);
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
ZFPROTOCOL_IMPLEMENTATION_END(ZFStringImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFStringImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

