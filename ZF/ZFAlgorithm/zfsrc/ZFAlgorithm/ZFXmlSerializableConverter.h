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
 * @file ZFXmlSerializableConverter.h
 * @brief serializable convertion between xml and serializalbe
 */

#ifndef _ZFI_ZFXmlSerializableConverter_h_
#define _ZFI_ZFXmlSerializableConverter_h_

#include "ZFXml.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// xml serializable conversion

/**
 * @brief see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 *
 * format:
 * @code
 *   <Node refType="xml" refData="pathInfo, see ZFPathInfo" />
 * @endcode
 */
#define ZFSerializableDataRefType_xml zfText("xml")

/**
 * @brief see #ZFOBJECT_CREATOR_DEFINE
 *
 * data:
 * @code
 *   "pathInfo, see ZFPathInfo"
 * @endcode
 * data is a pathInfo to a xml file,
 * which should contain a serializable object
 */
#define ZFObjectCreatorType_xml zfText("xml")

// ============================================================
/**
 * @brief parse xml formated data to serializable
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFSerializableDataFromXml,
                        ZFMP_OUT(ZFSerializableData &, serializableData),
                        ZFMP_IN(const ZFXmlItem &, xmlElement),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                        ZFMP_OUT_OPT(ZFXmlItem *, outErrorPos, zfnull))
/**
 * @brief see #ZFSerializableDataFromXml
 */
ZFMETHOD_FUNC_DECLARE_3(ZFSerializableData, ZFSerializableDataFromXml,
                        ZFMP_IN(const ZFXmlItem &, xmlElement),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                        ZFMP_OUT_OPT(ZFXmlItem *, outErrorPos, zfnull))
/**
 * @brief print serializable to xml formated data
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFSerializableDataToXml,
                        ZFMP_OUT(ZFXmlItem &, xmlElement),
                        ZFMP_IN(const ZFSerializableData &, serializableData),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                        ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
/**
 * @brief see #ZFSerializableDataToXml
 */
ZFMETHOD_FUNC_DECLARE_3(ZFXmlItem, ZFSerializableDataToXml,
                        ZFMP_IN(const ZFSerializableData &, serializableData),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                        ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))

// ============================================================
/**
 * @brief util method to parse serializable data from input with xml format
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFSerializableDataFromXml,
                        ZFMP_OUT(ZFSerializableData &, ret),
                        ZFMP_IN(const ZFInputCallback &, input),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
/** @brief see #ZFSerializableDataFromXml */
ZFMETHOD_FUNC_DECLARE_2(ZFSerializableData, ZFSerializableDataFromXml,
                        ZFMP_IN(const ZFInputCallback &, input),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
/**
 * @brief util method to convert serializable object to xml format
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFSerializableDataToXml,
                        ZFMP_IN(const ZFOutputCallback &, outputCallback),
                        ZFMP_IN(const ZFSerializableData &, serializableData),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                        ZFMP_IN_OPT(const ZFXmlOutputFlags &, flags, ZFXmlOutputFlagsDefault()))

// ============================================================
/**
 * @brief util method to parse object from input with xml format
 */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFObjectFromXml,
                        ZFMP_OUT(zfautoObject &, ret),
                        ZFMP_IN(const ZFInputCallback &, input),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
/** @brief see #ZFObjectFromXml */
ZFMETHOD_FUNC_DECLARE_2(zfautoObject, ZFObjectFromXml,
                        ZFMP_IN(const ZFInputCallback &, input),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
/**
 * @brief util method to convert serializable object to xml format
 */
ZFMETHOD_FUNC_DECLARE_4(zfbool, ZFObjectToXml,
                        ZFMP_IN(const ZFOutputCallback &, outputCallback),
                        ZFMP_IN(ZFObject *, obj),
                        ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                        ZFMP_IN_OPT(const ZFXmlOutputFlags &, flags, ZFXmlOutputFlagsDefault()))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFXmlSerializableConverter_h_

