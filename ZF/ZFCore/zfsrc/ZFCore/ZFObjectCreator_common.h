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
 * @file ZFObjectCreator_common.h
 * @brief see #ZFOBJECT_CREATOR_DEFINE
 */

#ifndef _ZFI_ZFObjectCreator_common_h_
#define _ZFI_ZFObjectCreator_common_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief see #ZFOBJECT_CREATOR_DEFINE
 *
 * data is a string containing class name and method name,
 * which should be like this:\n
 *   "YourClass::YourMethod"\n
 * \n
 * YourClass must be a ZFObject or ZFInterface type (accessed by #ZFClass::classForName)\n
 * YourMethod must be a #ZFMethod with this proto type:\n
 *   static zfautoObject yourMethod(void);\n
 * no method id supported
 */
#define ZFObjectCreatorType_ZFMethod zfText("method")

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectCreator_common_h_

