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
 * @file ZFSerializableDataRefType.h
 * @brief reference logic for #ZFSerializable
 */

#ifndef _ZFI_ZFSerializableDataRefType_h_
#define _ZFI_ZFSerializableDataRefType_h_

#include "ZFSerializableData.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief usually for debug use only
 *
 * macro names are recommended to be ZFSerializableDataRefType_XXX
 */
extern ZF_ENV_EXPORT void ZFSerializableDataRefTypeGetAllT(ZF_OUT ZFCoreArray<const zfchar *> &ret);
/** @brief see #ZFSerializableDataRefTypeGetAll */
inline ZFCoreArrayPOD<const zfchar *> ZFSerializableDataRefTypeGetAll(void)
{
    ZFCoreArrayPOD<const zfchar *> ret;
    ZFSerializableDataRefTypeGetAllT(ret);
    return ret;
}

/**
 * @brief advanced data reference logic for serializable
 *
 * used to reference another existing serializable data, typical example:
 * @code
 *   <OwnerType>
 *       <PropertyType refType="YourType" refData="YourType's reference data"
 *           yourExtraAttributeKey="yourExtraAttributeValue" >
 *           <YourExtraChildren />
 *       </PropertyType>
 *   </OwnerType>
 * @endcode
 * in this example, we declare a referenced node:
 * -  attribute "refType" shows the reference type,
 *   which was registered by #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 * -  attribute "refData" shows the data for the reference,
 *   may be file path or content data or others,
 *   which would be decoded by the method declared by #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 * -  you may add extra attributes or children to ref node,
 *   which would be appended to result decoded data
 *   (existing attributes would be replaced, and existing children would be appended normally)
 *
 * for example, assume we have two xml files:
 * @code
 *   // 1.xml
 *   <Holder>
 *       <MyProperty refType="myXmlParseLogic"
 *           refData="2.xml"
 *           myAttr1="myValue1" >
 *           <MyChild1 />
 *       </MyProperty>
 *   </Holder>
 *   // 2.xml
 *   <AnyType myAttr1="myValue1 replaced"
 *       myAttr2="myValue2" >
 *       <MyChild2 />
 *   </AnyType>
 * @endcode
 * then the decoded data would be like:
 * @code
 *   <Holder>
 *       <MyProperty myAttr1="myValue1 replaced"
 *           myAttr2="myValue2" >
 *           <MyChild1 />
 *           <MyChild2 />
 *       </MyProperty>
 *   </Holder>
 * @endcode
 * \n
 * to declare your own type:
 * @code
 *   // in cpp file only
 *   ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(registerSig, yourTypeName)
 *   {
 *       // proto type:
 *       //   zfbool resolveRef(
 *       //       ZF_OUT ZFSerializableData &serializableData,
 *       //       ZF_IN const zfchar *refData,
 *       //       ZF_OUT_OPT zfstring *outErrorHint = zfnull,
 *       //       ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
 *
 *       // you decode operation here
 *       // decode (const zfchar *)refData to (ZFSerializableData)serializableData
 *
 *       // return decode result
 *       return zftrue;
 *   }
 * @endcode
 *
 * REMARKS:\n
 * typical serialize from data steps:
 * -# load serializable data from source, e.g. file or buffer
 * -# resolve reference by #ZFSerializableData::refInfoLoad
 * -# serialize each item for owner serializable, and store necessary reference info
 *
 * typical serialize to data step:
 * -# check whether your item has previous stored reference info, serialize to data accorrding it
 * -# restore reference info by #ZFSerializableData::refInfoRestore
 * -# save serializable data to result, e.g. file or buffer
 *
 * usually, the #ZFSerializable and #ZFSerializableUtilSerializeAttributeFromData series util macro would do the work for you
 */
#define ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(registerSig, refTypeName) \
    static zfbool _ZFP_ZFSerializableDataRefInfoLoadCallback_##registerSig( \
        ZF_OUT ZFSerializableData &serializableData, \
        ZF_IN const zfchar *refData, \
        ZF_OUT_OPT zfstring *outErrorHint = zfnull, \
        ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(_ZFP_ZFSerializableDataRefTypeRegister_##registerSig, \
                                          ZFLevelZFFrameworkNormal) \
    { \
        _ZFP_ZFSerializableDataRefTypeRegister( \
            refTypeName, \
            _ZFP_ZFSerializableDataRefInfoLoadCallback_##registerSig); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(_ZFP_ZFSerializableDataRefTypeRegister_##registerSig) \
    { \
        _ZFP_ZFSerializableDataRefTypeUnregister(refTypeName); \
    } \
    ZF_GLOBAL_INITIALIZER_END(_ZFP_ZFSerializableDataRefTypeRegister_##registerSig) \
    static zfbool _ZFP_ZFSerializableDataRefInfoLoadCallback_##registerSig( \
        ZF_OUT ZFSerializableData &serializableData, \
        ZF_IN const zfchar *refData, \
        ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
        ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)

typedef zfbool (*_ZFP_ZFSerializableDataRefInfoLoadCallback)(ZF_OUT ZFSerializableData &serializableData,
                                                             ZF_IN const zfchar *data,
                                                             ZF_OUT_OPT zfstring *outErrorHint,
                                                             ZF_OUT_OPT ZFSerializableData *outErrorPos);
extern ZF_ENV_EXPORT void _ZFP_ZFSerializableDataRefTypeRegister(ZF_IN const zfchar *refType,
                                                                 ZF_IN _ZFP_ZFSerializableDataRefInfoLoadCallback callback);
extern ZF_ENV_EXPORT void _ZFP_ZFSerializableDataRefTypeUnregister(ZF_IN const zfchar *refType);
extern ZF_ENV_EXPORT _ZFP_ZFSerializableDataRefInfoLoadCallback _ZFP_ZFSerializableDataRefTypeGet(ZF_IN const zfchar *refType);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDataRefType_h_

