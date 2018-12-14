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
 * @file ZFString.h
 * @brief string container as ZFObject
 */

#ifndef _ZFI_ZFString_h_
#define _ZFI_ZFString_h_

#include "ZFObject.h"
#include <wchar.h>
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief wchar_t wrapper */
#ifndef _ZFT_zfcharW
    ZFT_INT_WEAK(wchar_t, zfcharW);
#else
    ZFT_INT_WEAK(_ZFT_zfcharW, zfcharW);
#endif

/** @brief wstring wrapper */
#ifndef _ZFT_zfstringW
    ZFT_INT_WEAK(_zfstr<zfcharW>, zfstringW);
#else
    ZFT_INT_WEAK(_ZFT_zfstringW, zfstringW);
#endif

// ============================================================
/**
 * @brief supported string encoding
 *
 * we use internal string encoding for string processing,
 * and use native string type for string storage\n
 * \n
 * we have these encoding types:
 * -  zfchar: UTF8 encoded
 * -  zfcharW: usually used to adapt to Unicode version of Windows API,
 *   ensured no less than 2 bytes, UTF16 encoded
 *
 * we have these string types:
 * -  cosnt zfchar *: C-style strings, simple and efficient
 * -  zfstring: wrapper for std::string
 * -  void *: very low level type to native string type,
 *   such as NSString, QString
 * -  ZFString: string as a ZFObject to contain the low level void * types,
 *   has interfaces to convert between different string types
 *
 * \n
 * keep it in short, use zfchar (or zfstring) to process strings,
 * and use ZFString as ZFObject:
 * @code
 *   const zfchar *s = "my string";
 *   zfstring str = s;
 *
 *   // for Windows API, convert to zfcharW and use WCS versions:
 *   WindowsWAPI(ZFString::toUTF16(s, ZFStringEncoding::e_UTF8));
 * @endcode
 *
 * for more advanced string encoding operation,
 * you should consider third-party library
 */
ZFENUM_BEGIN(ZFStringEncoding)
    ZFENUM_VALUE(UTF8 = 1)
    ZFENUM_VALUE(UTF16LE = 2)
    ZFENUM_VALUE(UTF16BE = 3)
    ZFENUM_VALUE(UTF16 = e_UTF16LE)
ZFENUM_SEPARATOR_ALLOW_DUPLICATE_VALUE(ZFStringEncoding)
    ZFENUM_VALUE_REGISTER(UTF8)
    ZFENUM_VALUE_REGISTER_WITH_NAME(UTF16LE, "UTF16")
    ZFENUM_VALUE_REGISTER_WITH_NAME(UTF16BE, "UTF16BE")
    ZFENUM_VALUE_REGISTER_WITH_NAME(UTF16, "UTF16")
ZFENUM_END(ZFStringEncoding)

// ============================================================
/**
 * @brief storage type for a #ZFString,
 *   usually you should not concern this type
 */
ZFENUM_BEGIN(ZFStringStorageType)
    /**
     * @brief store to native string
     *
     * default storage type, need ZFString's implementation
     */
    ZFENUM_VALUE(NativeString)
    /**
     * @brief copy the string to internal storage
     *
     * have good performance and safety,
     * need no ZFString's implementation
     */
    ZFENUM_VALUE(Copy)
    /**
     * @brief reference to a char buffer (const zfchar *)
     *
     * have best performance,
     * but app must ensure the char buffer being alive
     * while using the #ZFString object,
     * use with extreme caution,
     * usually useful to create from static zfchar * strings for performance,
     * need no ZFString's implementation
     */
    ZFENUM_VALUE(CharBufferRef)
ZFENUM_SEPARATOR(ZFStringStorageType)
    ZFENUM_VALUE_REGISTER(NativeString)
    ZFENUM_VALUE_REGISTER(Copy)
    ZFENUM_VALUE_REGISTER(CharBufferRef)
ZFENUM_END(ZFStringStorageType)

// ============================================================
// ZFString
zfclassFwd _ZFP_ZFStringPrivate;
/**
 * @brief native string wrapper
 *
 * readonly string container\n
 * for advanced string operation,
 * you may want to:
 * -  get the string value, encoded as UTF8, then do your work
 * -  write native code, using nativeString
 *   (deprecated, use only for performance)
 *
 * \n
 * note, #ZFString is serializable,
 * but while serializing to a ZFString object,
 * storage type is ensured #ZFStringStorageType::EnumDefault\n
 * \n
 * serializable data:
 * @code
 *   <StringClass
 *       value="string content" // optional, empty by default
 *   />
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFString : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFString, ZFStyleableObject)

protected:
    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);

public:
    zfoverride
    virtual inline zfbool serializeFromString(ZF_IN const zfchar *src,
                                              ZF_IN_OPT zfindex srcLen = zfindexMax())
    {
        this->stringValueSet(srcLen == zfindexMax() || src[srcLen] == '\0' ? src : zfstring(src, srcLen).cString());
        return zftrue;
    }
    zfoverride
    virtual inline zfbool serializeToString(ZF_IN_OUT zfstring &ret)
    {
        ret += this->stringValue();
        return zftrue;
    }

protected:
    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
    {
        zfsuperI(ZFStyleable)::styleableOnCopyFrom(anotherStyleable);
        this->stringValueSet(ZFCastZFObjectUnchecked(zfself *, anotherStyleable)->stringValue());
    }

    // ============================================================
    // conversion
public:
    /**
     * @brief convert to deisred encoding, or empty string if failed
     *
     * result would be appended to tail without clear
     */
    static zfbool toUTF8(ZF_OUT zfstring &result,
                         ZF_IN const void *s,
                         ZF_IN ZFStringEncodingEnum srcEncoding);
    /** @brief see #toUTF8 */
    static zfstring toUTF8(ZF_IN const void *s,
                           ZF_IN ZFStringEncodingEnum srcEncoding,
                           ZF_OUT_OPT zfbool *success = zfnull)
    {
        zfstring ret;
        zfbool t = ZFString::toUTF8(ret, s, srcEncoding);
        if(success != zfnull) {*success = t;}
        return ret;
    }
    /** @brief see #toUTF8 */
    static zfbool toUTF16(ZF_OUT zfstringW &result,
                          ZF_IN const void *s,
                          ZF_IN ZFStringEncodingEnum srcEncoding);
    /** @brief see #toUTF8 */
    static zfstringW toUTF16(ZF_IN const void *s,
                             ZF_IN ZFStringEncodingEnum srcEncoding,
                             ZF_OUT_OPT zfbool *success = zfnull)
    {
        zfstringW ret;
        zfbool t = ZFString::toUTF16(ret, s, srcEncoding);
        if(success != zfnull) {*success = t;}
        return ret;
    }
    /** @brief see #toUTF8 */
    static zfbool toUTF16BE(ZF_OUT zfstringW &result,
                            ZF_IN const void *s,
                            ZF_IN ZFStringEncodingEnum srcEncoding);
    /** @brief see #toUTF8 */
    static zfstringW toUTF16BE(ZF_IN const void *s,
                               ZF_IN ZFStringEncodingEnum srcEncoding,
                               ZF_OUT_OPT zfbool *success = zfnull)
    {
        zfstringW ret;
        zfbool t = ZFString::toUTF16BE(ret, s, srcEncoding);
        if(success != zfnull) {*success = t;}
        return ret;
    }

public:
    /**
     * @brief get utf8 string's logical word count or -1 if invalid letter exist
     */
    static zfindex wordCountOfUTF8(ZF_IN const zfchar *utf8String);

    // ============================================================
    // instance method
protected:
    /**
     * @brief init from zfchar string
     */
    ZFOBJECT_ON_INIT_INLINE_2(ZFMP_IN(const zfchar *, s),
                              ZFMP_IN_OPT(ZFStringStorageTypeEnum, storageType, ZFStringStorageType::EnumDefault()))
    {
        this->objectOnInit();
        zfself::stringValueSet(s, storageType);
    }
    /**
     * @brief init from ZFString (no deep copy)
     */
    ZFOBJECT_ON_INIT_INLINE_1(ZFMP_IN(ZFString *, another))
    {
        this->objectOnInit();
        zfself::stringValueSet(another);
    }
    /**
     * @brief init from native string
     */
    ZFOBJECT_ON_INIT_INLINE_1(ZFMP_IN(void *, nativeString))
    {
        this->objectOnInit();
        zfself::nativeStringSet(nativeString);
    }

    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual inline void objectInfoT(ZF_IN_OUT zfstring &ret)
    {
        ret += "\"";
        ret += this->stringValue();
        ret += "\"";
    }
public:
    zfoverride
    virtual zfidentity objectHash(void);
    /**
     * @brief compare string value
     */
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

public:
    /**
     * @brief usually for debug or performance use only
     */
    virtual ZFStringStorageTypeEnum storageType(void);
    /**
     * @brief for reference storage type only,
     *   to ensure the internal referenced buffer is safe to release,
     *   see #ZFStringStorageType::e_CharBufferRef
     *
     * copy the buffer and change storage type if necessary
     */
    virtual void charBufferCheckCopy(void);

public:
    /**
     * @brief true if empty
     */
    ZFMETHOD_DECLARE_0(zfbool, isEmpty)
    /**
     * @brief get the length of the string
     *
     * note that the length is the size of the zfchar type,
     * may be or may not be the logical letter num,
     * the actual byte size can be calculated by
     * "sizeof(zfchar) * length()"
     * @see wordCount
     */
    ZFMETHOD_DECLARE_0(zfindex, length)
    /**
     * @brief get the logical word count instead of byte size
     */
    ZFMETHOD_DECLARE_0(zfindex, wordCount)
    /**
     * @brief get the string, ensured to be non-null
     *
     * result is cached so no need to worry about performance
     * @note the returned string is not ensured alive during owner ZFString's life-time,
     *   you should not store the returned pointer value
     */
    ZFMETHOD_DECLARE_0(const zfchar *, stringValue)
    /**
     * @brief get the nativeString, you must not change its content,
     *   ensured to be non-null, create if necessary
     */
    ZFMETHOD_DECLARE_0(void *, nativeString)

protected:
    /**
     * @brief clear and set by string
     *
     * store as #ZFStringStorageType::EnumDefault by default,
     * which need ZFString's implementation
     */
    virtual void stringValueSet(ZF_IN_OPT const zfchar *s = zfnull,
                                ZF_IN_OPT ZFStringStorageTypeEnum storageType = ZFStringStorageType::EnumDefault());
    /**
     * @brief clear and set by nativeString
     *
     * may or may not copy contents of the nativeString (depends on implementation),
     * so if you changed the contents of nativeString after set to another ZFString,
     * the behavior is undefined, use with caution
     */
    virtual void nativeStringSet(ZF_IN void *nativeString);
    /**
     * @brief set by ZFString, no deep copy
     */
    virtual void stringValueSet(ZF_IN ZFString *another);

private:
    _ZFP_ZFStringPrivate *d;
};

// ============================================================
// ZFStringEditable
/**
 * @brief editable version of ZFString
 */
zfclass ZF_ENV_EXPORT ZFStringEditable : zfextends ZFString
{
    ZFOBJECT_DECLARE(ZFStringEditable, ZFString)

public:
    ZFMETHOD_INLINE_2(void, stringValueSet,
                      ZFMP_IN_OPT(const zfchar *, s, zfnull),
                      ZFMP_IN_OPT(ZFStringStorageTypeEnum, storageType, ZFStringStorageType::EnumDefault()))
    {
        zfsuper::stringValueSet(s, storageType);
    }
    ZFMETHOD_INLINE_1(void, nativeStringSet,
                      ZFMP_IN(void *, nativeString))
    {
        zfsuper::nativeStringSet(nativeString);
    }
    ZFMETHOD_INLINE_1(void, stringValueSet,
                      ZFMP_IN(ZFString *, another))
    {
        zfsuper::stringValueSet(another);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFString_h_

