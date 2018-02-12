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
 * @file ZFLangUtil_ZFSerializableHelper.h
 * @brief language util for localization
 */

#ifndef _ZFI_ZFLangUtil_ZFSerializableHelper_h_
#define _ZFI_ZFLangUtil_ZFSerializableHelper_h_

#include "ZFLang.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 *
 * format:
 * @code
 *   <zfstring refType="lang_zfstring" refData="langKey used by zfLang" />
 * @endcode
 */
#define ZFSerializableDataRefType_lang_zfstring zfText("lang_zfstring")

/**
 * @brief see #ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE
 *
 * format:
 * @code
 *   <ZFString refType="lang_ZFString" refData="langKey used by zfLang" />
 * @endcode
 */
#define ZFSerializableDataRefType_lang_ZFString zfText("lang_ZFString")

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFLangUtil_ZFSerializableHelper_h_

