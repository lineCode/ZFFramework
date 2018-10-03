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
 * @file ZFPropertySerializable.h
 * @brief serialization for ZFProperty
 */

#ifndef _ZFI_ZFPropertySerializable_h_
#define _ZFI_ZFPropertySerializable_h_

#include "ZFProperty.h"
#include "ZFSerializable.h"
#include "ZFTypeId.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFTYPEID_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFProperty owner="OwnerClassName" property="PropertyName" />
 * @endcode
 *
 * string data:
 * @code
 *   ClassName::propertyName
 * @endcode
 */
ZFTYPEID_DECLARE(ZFProperty, const ZFProperty *)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFProperty_owner "owner"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFProperty_property "property"

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertySerializable_h_

