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
 * @file ZFSerializableDataRefType_common.h
 * @brief common type for #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 */

#ifndef _ZFI_ZFSerializableDataRefType_common_h_
#define _ZFI_ZFSerializableDataRefType_common_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 *
 * format:
 * @code
 *   <Node refType="ZFObjectCreator" refData="data, see below" />
 * @endcode
 *
 * refData's rule:
 * -  "[MyObjectCreatorType|MyObjectCreatorData]"\n
 *   use #ZFObjectCreate to create a ZFObject,
 *   then serialize the object to data and copy it to the result node
 * -  "[MyObjectCreatorType|MyObjectCreatorData]::myPropertyName1::myPropertyName2"
 *   use #ZFObjectCreate to create a ZFObject,
 *   then access the object's property value,
 *   serialize the value to data and copy it to the result node
 *
 * \n
 * typical useage:
 * @code
 *   <Node refType="ZFObjectCreator" refData="[method|MyStyle::DefaultStyleReflect]::myProperty"
 * @endcode
 * this would access MyStyle's default style (by #ZFObjectCreatorType_ZFMethod),
 * then copy the "myProperty" value to the result node
 */
#define ZFSerializableDataRefType_ZFObjectCreator zfText("ZFObjectCreator")

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSerializableDataRefType_common_h_

