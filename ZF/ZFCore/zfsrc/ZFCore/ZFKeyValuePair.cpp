/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFKeyValuePair.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFEXPORT_VAR_READONLY_DEFINE(ZFKeyValuePair, ZFKeyValuePairZero, ZFKeyValuePairMake(zfnull, zfnull))

// ============================================================
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFKeyValuePairHolder, ZFKeyValuePairHolder, {
        const ZFSerializableData *data = zfnull;
        data = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFKeyValuePair_key);
        if(data != zfnull)
        {
            if(!ZFObjectFromData(v.key, *data, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
        }
        else
        {
            v.key = zfnull;
        }
        data = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFKeyValuePair_value);
        if(data != zfnull)
        {
            if(!ZFObjectFromData(v.value, *data, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
        }
        else
        {
            v.value = zfnull;
        }
        return zftrue;
    }, {
        if(v.key != zfnull)
        {
            ZFSerializableData data;
            if(!ZFObjectToData(data, v.key, outErrorHint))
            {
                return zffalse;
            }
            data.categorySet(ZFSerializableKeyword_ZFKeyValuePair_key);
            serializableData.elementAdd(data);
        }
        if(v.value != zfnull)
        {
            ZFSerializableData data;
            if(!ZFObjectToData(data, v.value, outErrorHint))
            {
                return zffalse;
            }
            data.categorySet(ZFSerializableKeyword_ZFKeyValuePair_value);
            serializableData.elementAdd(data);
        }
        return zftrue;
    })

ZFTYPEID_ALIAS_DEFINE(ZFKeyValuePairHolder, ZFKeyValuePairHolder, ZFKeyValuePair, ZFKeyValuePair)

ZF_NAMESPACE_GLOBAL_END

