/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFLangUtil_ZFSerializableHelper.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(lang_zfstring, ZFSerializableDataRefType_lang_zfstring)
{
    serializableData.propertyValueSet(zfLang(refData));
    return zftrue;
}

ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(lang_ZFString, ZFSerializableDataRefType_lang_ZFString)
{
    serializableData.propertyValueSet(zfLang(refData));
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

