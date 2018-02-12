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
 * @file ZFObjectCreator.h
 * @brief serailizable way to create a ZFObject
 */

#ifndef _ZFI_ZFObjectCreator_h_
#define _ZFI_ZFObjectCreator_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief function type to create an object type
 */
typedef zfautoObject (*ZFObjectCreator)(void);

/**
 * @brief create object from a descriptor, see #ZFOBJECT_CREATOR_DEFINE
 */
extern ZF_ENV_EXPORT zfautoObject ZFObjectCreate(ZF_IN const zfchar *type,
                                                 ZF_IN const zfchar *data);

/**
 * @brief usually for debug use only
 *
 * macro names are recommended to be ZFObjectCreatorType_XXX
 */
extern ZF_ENV_EXPORT void ZFObjectCreatorGetAllTypeT(ZF_OUT ZFCoreArray<const zfchar *> &ret);
/** @brief see #ZFObjectCreatorGetAllType */
inline ZFCoreArrayPOD<const zfchar *> ZFObjectCreatorGetAllType(void)
{
    ZFCoreArrayPOD<const zfchar *> ret;
    ZFObjectCreatorGetAllTypeT(ret);
    return ret;
}

typedef zfautoObject (*_ZFP_ZFObjectCreatorCallback)(ZF_IN const zfchar *data);
extern ZF_ENV_EXPORT void _ZFP_ZFObjectCreatorRegister(ZF_IN const zfchar *type,
                                                       ZF_IN _ZFP_ZFObjectCreatorCallback creatorCallback);
extern ZF_ENV_EXPORT void _ZFP_ZFObjectCreatorUnregister(ZF_IN const zfchar *type);

/**
 * @brief macro to define and register a creator for #ZFObjectCreate, for advance user only
 *
 * usage:
 * @code
 *   ZFOBJECT_CREATOR_DEFINE(registerSig, yourTypeName)
 *   {
 *       // decode from data and return a newly created object
 *       return yourObject;
 *   }
 * @endcode
 */
#define ZFOBJECT_CREATOR_DEFINE(registerSig, typeName) \
    static zfautoObject _ZFP_ZFObjectCreator_##registerSig(ZF_IN const zfchar *data); \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFObjectCreatorReg_##registerSig, ZFLevelZFFrameworkNormal) \
    { \
        _ZFP_ZFObjectCreatorRegister(typeName, _ZFP_ZFObjectCreator_##registerSig); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(ZFObjectCreatorReg_##registerSig) \
    { \
        _ZFP_ZFObjectCreatorUnregister(typeName); \
    } \
    ZF_GLOBAL_INITIALIZER_END(ZFObjectCreatorReg_##registerSig) \
    static zfautoObject _ZFP_ZFObjectCreator_##registerSig(ZF_IN const zfchar *data)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectCreator_h_

