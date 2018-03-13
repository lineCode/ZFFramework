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
 * @file ZFRefInfo.h
 * @brief reference related info storage for impl
 */

#ifndef _ZFI_ZFRefInfo_h_
#define _ZFI_ZFRefInfo_h_

#include "ZFCoreTypeDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief reference related info storage for impl,
 *   actual meaning of the info depends on impl
 *
 * to use, recommend to define a macro named "ZFReferenceType_xxx" for each of your reference type
 */
zfclassNotPOD ZF_ENV_EXPORT ZFRefInfo
{
public:
    zfstring refType; /**< @brief reference type, text, xml, base64, etc */
    zfstring refData; /**< @brief reference value, content, file path, etc */

public:
    /** @cond ZFPrivateDoc */
    ZFRefInfo(void) : refType(), refData() {}
    ZFRefInfo(ZF_IN const ZFRefInfo &refInfo) : refType(refInfo.refType), refData(refInfo.refData) {}
    ZFRefInfo(ZF_IN const zfchar *refType, ZF_IN const zfchar *refData) : refType(refType), refData(refData) {}
    ZFRefInfo &operator = (ZF_IN const ZFRefInfo &refInfo) {this->refType = refInfo.refType; this->refData = refInfo.refData; return *this;}
    zfbool operator == (ZF_IN const ZFRefInfo &refInfo) const {return (this->refType == refInfo.refType && this->refData == refInfo.refData);}
    zfbool operator != (ZF_IN const ZFRefInfo &refInfo) const {return !this->operator == (refInfo);}
    /** @endcond */
};

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFRefInfo_h_

