#include "ZFImpl_sys_Qt_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFString.h"
#include "ZFCore/ZFString.h"
#include "ZFImpl/tools/UTFCodeUtil/UTFCodeUtil.h"

#if ZF_ENV_sys_Qt

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFStringImpl_sys_Qt, ZFString, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QString")
public:
    virtual void *nativeStringCreate(ZF_IN_OPT const zfchar *s = zfnull)
    {
        QString *ret = new QString();
        if(s != zfnull)
        {
            *ret = QString::fromUtf8(s);
        }
        return ZFCastStatic(void *, ret);
    }

    virtual void *nativeStringRetain(ZF_IN void *nativeString)
    {
        QString *nativeStringTmp = ZFCastStatic(QString *, nativeString);
        QString *ret = new QString(*nativeStringTmp);
        return ret;
    }
    virtual void nativeStringRelease(ZF_IN void *nativeString)
    {
        delete ZFCastStatic(QString *, nativeString);
    }

    virtual void stringValueAccess(ZF_IN void *nativeString,
                                   ZF_OUT const zfchar *&stringValue,
                                   ZF_OUT void *&stringValueToken)
    {
        QString *nativeStringTmp = ZFCastStatic(QString *, nativeString);
        QByteArray *stringValueTokenTmp = new QByteArray(nativeStringTmp->toUtf8());
        stringValue = stringValueTokenTmp->constData();
        stringValueToken = stringValueTokenTmp;
    }
    virtual void stringValueAccessCleanup(ZF_IN void *nativeString,
                                          ZF_IN const zfchar *stringValue,
                                          ZF_IN void *stringValueToken)
    {
        QByteArray *stringValueTokenTmp = (QByteArray *)stringValueToken;
        delete stringValueTokenTmp;
    }

    virtual zfbool toUTF8(ZF_OUT zfstring &result,
                          ZF_IN const void *s,
                          ZF_IN ZFStringEncodingEnum srcEncoding)
    {
        switch(srcEncoding)
        {
            case ZFStringEncoding::e_UTF8:
                result += (const zfchar *)s;
                return zftrue;
            case ZFStringEncoding::e_UTF16LE:
            {
                zfchar *sUTF8 = UTFCodeUtil::UTF16ToUTF8((const zfcharW *)s);
                if(sUTF8 == zfnull) {return zffalse;}
                result += sUTF8;
                zffree(sUTF8);
                return zftrue;
            }
            case ZFStringEncoding::e_UTF16BE:
            {
                zfcharW *sUTF16 = UTFCodeUtil::UTF16BEToUTF16((const zfcharW *)s);
                if(sUTF16 == zfnull) {return zffalse;}
                zfchar *sUTF8 = UTFCodeUtil::UTF16ToUTF8(sUTF16);
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
                zfcharW *sUTF16 = UTFCodeUtil::UTF8ToUTF16((const zfchar *)s);
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
                zfcharW *sUTF16 = UTFCodeUtil::UTF8ToUTF16((const zfchar *)s);
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
    virtual zfindex wordCountOfUTF8(ZF_IN const zfchar *utf8String)
    {
        return UTFCodeUtil::UTF8GetWordCount(utf8String);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFStringImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFStringImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

