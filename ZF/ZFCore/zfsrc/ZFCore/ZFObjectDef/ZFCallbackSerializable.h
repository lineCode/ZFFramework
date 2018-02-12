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
 * @file ZFCallbackSerializable.h
 * @brief serialization for ZFCallback
 */

#ifndef _ZFI_ZFCallbackSerializable_h_
#define _ZFI_ZFCallbackSerializable_h_

#include "ZFIOCallback.h"
#include "ZFSerializable.h"
#include "ZFPropertyType.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   // for #ZFCallbackNull
 *   <null />
 *
 *   // for #ZFCallbackForMethod
 *   <ZFCallback>
 *       <ZFMethod category="method" ... />
 *   </ZFCallback>
 *
 *   // for #ZFCallbackForMemberMethod
 *   // not supported
 *
 *   // for #ZFCallbackForRawFunction
 *   // not supported
 * @endcode
 * \n
 * ADVANCED:\n
 * you may also supply your own serialize logic by registering
 * with #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE,
 * serializable data:
 * @code
 *   <ZFCallback callbackType="yourType" >
 *       <something category="callbackData" ... >
 *           // custom datas
 *       </something>
 *   </ZFCallback>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFCallback, ZFCallback)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCallback_method zfText("method")

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCallback_callbackType zfText("callbackType")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFCallback_callbackData zfText("callbackData")

// ============================================================
ZFPROPERTY_TYPE_ALIAS_DECLARE(ZFCallback, ZFCallback, ZFListener, ZFListener)
ZFPROPERTY_TYPE_ALIAS_DECLARE(ZFCallback, ZFCallback, ZFIOCallback, ZFIOCallback)
ZFPROPERTY_TYPE_ALIAS_DECLARE(ZFCallback, ZFCallback, ZFOutputCallback, ZFOutputCallback)
ZFPROPERTY_TYPE_ALIAS_DECLARE(ZFCallback, ZFCallback, ZFInputCallback, ZFInputCallback)

// ============================================================
// custom serialize logic
typedef zfbool (*_ZFP_ZFCallbackSerializeCustomCallback)(ZF_IN_OUT ZFCallback &ret,
                                                         ZF_IN const ZFSerializableData &serializableData,
                                                         ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                         ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */);
extern ZF_ENV_EXPORT void _ZFP_ZFCallbackSerializeCustomTypeRegister(ZF_IN const zfchar *customType,
                                                                     ZF_IN _ZFP_ZFCallbackSerializeCustomCallback serializeCallback);
extern ZF_ENV_EXPORT void _ZFP_ZFCallbackSerializeCustomTypeUnregister(ZF_IN const zfchar *customType);
extern ZF_ENV_EXPORT _ZFP_ZFCallbackSerializeCustomCallback _ZFP_ZFCallbackSerializeCustomTypeGet(ZF_IN const zfchar *customType);

/**
 * @brief see #ZFPropertyTypeId_ZFCallback
 *
 * usage:
 * @code
 *   // in cpp file
 *   ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(registerSig, yourTypeName)
 *   {
 *       // serialize callback from data
 *       // proto type:
 *       //   zfbool action(ZF_IN_OUT ZFCallback &ret,
 *       //                 ZF_IN const ZFSerializableData &serializableData,
 *       //                 ZF_OUT_OPT zfstring *outErrorHint,
 *       //                 ZF_OUT_OPT ZFSerializableData *outErrorPos);
 *       return zftrue;
 *   }
 * @endcode
 */
#define ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(registerSig, type) \
    static zfbool _ZFP_ZFCallbackSerializeCustom_##registerSig(ZF_IN_OUT ZFCallback &ret, \
                                                               ZF_IN const ZFSerializableData &serializableData, \
                                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
                                                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFCallbackSerializeCustomTypeReg_##registerSig, ZFLevelZFFrameworkNormal) \
    { \
        _ZFP_ZFCallbackSerializeCustomTypeRegister(type, _ZFP_ZFCallbackSerializeCustom_##registerSig); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFCallbackSerializeCustomTypeReg_##registerSig) \
    { \
        _ZFP_ZFCallbackSerializeCustomTypeUnregister(type); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFCallbackSerializeCustomTypeReg_##registerSig) \
    static zfbool _ZFP_ZFCallbackSerializeCustom_##registerSig(ZF_IN_OUT ZFCallback &ret, \
                                                               ZF_IN const ZFSerializableData &serializableData, \
                                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */, \
                                                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFCallbackSerializable_h_

