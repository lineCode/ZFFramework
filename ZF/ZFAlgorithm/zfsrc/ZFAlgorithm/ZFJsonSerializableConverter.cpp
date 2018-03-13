/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFJsonSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

/*
 * <ZFString myAttr="myAttrValue" >
 *     <zfstring category="value" value="123" />
 *     <zfstring refType="123" refData="123" />
 * </ZFString>
 *
 * {
 *     "@ZFString" : [
 *         {
 *            "@zfstring" : [],
 *            "category" : "value",
 *            "value" : "123"
 *         },
 *         {
 *            "@zfstring" : [],
 *            "refType" : "123",
 *            "refData" : "123"
 *         }
 *     ],
 *     "myAttr" : "myAttrValue"
 * }
 */

#define _ZFP_ZFJsonSerializeKey_classPrefix '@'

ZFSERIALIZABLEDATA_REFERENCE_TYPE_DEFINE(json, ZFSerializableDataRefType_json)
{
    ZFJsonItem jsonObject = ZFJsonItemFromInput(ZFInputCallbackForPathInfoString(refData));
    if(jsonObject.jsonIsNull())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("failed to load json object from \"%s\""), refData);
        return zffalse;
    }
    return ZFSerializableDataFromJson(serializableData, jsonObject);
}

ZFOBJECT_CREATOR_DEFINE(json, ZFObjectCreatorType_json)
{
    ZFJsonItem jsonObject = ZFJsonItemFromInput(ZFInputCallbackForPathInfoString(data));
    if(jsonObject.jsonIsNull())
    {
        return zfnull;
    }
    ZFSerializableData serializableData;
    if(ZFSerializableDataFromJson(serializableData, jsonObject))
    {
        return ZFObjectFromData(serializableData);
    }
    return zfnull;
}

// ============================================================
static zfbool _ZFP_ZFSerializableDataFromJson(ZF_OUT ZFSerializableData &serializableData,
                                              ZF_IN const ZFJsonItem &jsonObject,
                                              ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                              ZF_OUT_OPT ZFJsonItem *outErrorPos = zfnull)
{
    if(jsonObject.jsonIsNull())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, zfText("null json object"));
        if(outErrorPos != zfnull)
        {
            *outErrorPos = jsonObject;
        }
        return zffalse;
    }

    ZFJsonItem elementJsonArray;
    ZFRefInfo refInfo;
    for(zfiterator jsonItemIt = jsonObject.jsonItemIterator(); jsonObject.jsonItemIteratorIsValid(jsonItemIt); jsonObject.jsonItemIteratorNext(jsonItemIt))
    {
        const zfchar *key = jsonObject.jsonItemIteratorGetKey(jsonItemIt);
        ZFJsonItem jsonItem = jsonObject.jsonItemIteratorGet(jsonItemIt);
        if(*key == _ZFP_ZFJsonSerializeKey_classPrefix)
        {
            serializableData.itemClassSet(key + 1);

            if(jsonItem.jsonType() != ZFJsonType::e_JsonArray)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    zfText("json item %s not type of %s"),
                    jsonItem.objectInfo().cString(),
                    ZFJsonType::EnumNameForValue(ZFJsonType::e_JsonArray));
                if(outErrorPos != zfnull)
                {
                    *outErrorPos = jsonItem;
                }
                return zffalse;
            }
            elementJsonArray = jsonItem;
        }
        else
        {
            if(jsonItem.jsonType() != ZFJsonType::e_JsonValue)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    zfText("json item %s not type of %s"),
                    jsonItem.objectInfo().cString(),
                    ZFJsonType::EnumNameForValue(ZFJsonType::e_JsonValue));
                if(outErrorPos != zfnull)
                {
                    *outErrorPos = jsonItem;
                }
                return zffalse;
            }
            const zfchar *value = jsonItem.jsonValue();

            if(zfscmpTheSame(key, ZFSerializableKeyword_refType))
            {
                refInfo.refType = value;
            }
            else if(zfscmpTheSame(key, ZFSerializableKeyword_refData))
            {
                refInfo.refData = value;
            }
            else
            {
                serializableData.attributeSet(zfstring(key).cString(), value);
            }
        }
    }
    serializableData.refInfoSet(&refInfo);

    if(serializableData.itemClass() == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, zfText("missing class node (which looks like \"@ClassName\")"));
        if(outErrorPos != zfnull)
        {
            *outErrorPos = jsonObject;
        }
        return zffalse;
    }

    if(!elementJsonArray.jsonIsNull())
    {
        for(zfindex i = 0; i < elementJsonArray.jsonObjectCount(); ++i)
        {
            ZFSerializableData childData;
            if(!_ZFP_ZFSerializableDataFromJson(childData, elementJsonArray.jsonObjectAtIndex(i), outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            serializableData.elementAdd(childData);
        }
    }

    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataFromJson,
                       ZFMP_OUT(ZFSerializableData &, serializableData),
                       ZFMP_IN(const ZFJsonItem &, jsonObject),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_OUT_OPT(ZFJsonItem *, outErrorPos, zfnull))
{
    return _ZFP_ZFSerializableDataFromJson(serializableData, jsonObject, outErrorHint, outErrorPos)
        && serializableData.refInfoLoad();
}
ZFMETHOD_FUNC_DEFINE_3(ZFSerializableData, ZFSerializableDataFromJson,
                       ZFMP_IN(const ZFJsonItem &, jsonObject),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_OUT_OPT(ZFJsonItem *, outErrorPos, zfnull))
{
    ZFSerializableData ret;
    if(ZFSerializableDataFromJson(ret, jsonObject, outErrorHint, outErrorPos))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataToJson,
                       ZFMP_OUT(ZFJsonItem &, jsonObject),
                       ZFMP_IN(const ZFSerializableData &, serializableData),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
{
    jsonObject = ZFSerializableDataToJson(serializableData, outErrorHint, outErrorPos);
    return !jsonObject.jsonIsNull();
}
ZFMETHOD_FUNC_DEFINE_3(ZFJsonItem, ZFSerializableDataToJson,
                       ZFMP_IN(const ZFSerializableData &, serializableData),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
{
    if(serializableData.itemClass() == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, zfText("missing serializable class"));
        return ZFJsonItem();
    }

    ZFJsonItem ret(ZFJsonType::e_JsonObject);

    if(serializableData.refInfo() != zfnull)
    {
        ret.jsonItemValueSet(ZFSerializableKeyword_refType, serializableData.refInfo()->refType);
        ret.jsonItemValueSet(ZFSerializableKeyword_refData, serializableData.refInfo()->refData);
    }

    for(zfiterator it = serializableData.attributeIterator();
        serializableData.attributeIteratorIsValid(it);
        serializableData.attributeIteratorNext(it))
    {
        ret.jsonItemValueSet(serializableData.attributeIteratorGetKey(it),
            serializableData.attributeIteratorGet(it));
    }

    ZFJsonItem elementJsonArray(ZFJsonType::e_JsonArray);
    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        ZFJsonItem child = ZFSerializableDataToJson(serializableData.elementAtIndex(i), outErrorHint, outErrorPos);
        if(child.jsonType() == ZFJsonType::e_JsonNull)
        {
            return ZFJsonItem();
        }
        elementJsonArray.jsonObjectAdd(child);
    }
    zfstring t;
    t += _ZFP_ZFJsonSerializeKey_classPrefix;
    t += serializableData.itemClass();
    ret.jsonItemSet(
        t.cString(),
        elementJsonArray);

    return ret;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFSerializableDataFromJson,
                       ZFMP_OUT(ZFSerializableData &, ret),
                       ZFMP_IN(const ZFInputCallback &, input),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    if(!input.callbackIsValid())
    {
        zfstringAppend(outErrorHint, zfText("invalid input callback"));
        return zffalse;
    }
    ZFJsonItem jsonElement = ZFJsonItemFromInput(input);
    if(jsonElement.jsonType() == ZFJsonType::e_JsonNull)
    {
        zfstringAppend(outErrorHint, zfText("unable to parse json from input"));
        return zffalse;
    }
    if(!ZFSerializableDataFromJson(ret, jsonElement, outErrorHint))
    {
        return zffalse;
    }

    ret.pathInfoSet(input.pathInfo());
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_2(ZFSerializableData, ZFSerializableDataFromJson,
                       ZFMP_IN(const ZFInputCallback &, input),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    ZFSerializableData ret;
    if(ZFSerializableDataFromJson(ret, input, outErrorHint))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataToJson,
                       ZFMP_IN(const ZFOutputCallback &, outputCallback),
                       ZFMP_IN(const ZFSerializableData &, serializableData),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_IN_OPT(const ZFJsonOutputFlags &, flags, ZFJsonOutputFlagsDefault()))
{
    ZFJsonItem jsonObject;
    if(!ZFSerializableDataToJson(jsonObject, serializableData, outErrorHint))
    {
        zfbool ret = ZFJsonItemToOutput(outputCallback, jsonObject, flags);
        outputCallback.execute(zfText("\n"));
        if(!ret)
        {
            zfstringAppend(outErrorHint, zfText("unable to convert json to string"));
        }
        return ret;
    }
    else
    {
        return zffalse;
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFObjectFromJson,
                       ZFMP_OUT(zfautoObject &, ret),
                       ZFMP_IN(const ZFInputCallback &, input),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    ZFSerializableData data;
    if(ZFSerializableDataFromJson(data, input, outErrorHint))
    {
        return ZFObjectFromData(ret, data, outErrorHint);
    }
    else
    {
        return zffalse;
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfautoObject, ZFObjectFromJson,
                       ZFMP_IN(const ZFInputCallback &, input),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    zfautoObject ret;
    ZFObjectFromJson(ret, input, outErrorHint);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFObjectToJson,
                       ZFMP_IN(const ZFOutputCallback &, outputCallback),
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_IN_OPT(const ZFJsonOutputFlags &, flags, ZFJsonOutputFlagsDefault()))
{
    ZFSerializableData serializableData;
    if(!ZFObjectToData(serializableData, obj, outErrorHint))
    {
        return zffalse;
    }
    else
    {
        return ZFSerializableDataToJson(outputCallback, serializableData, outErrorHint, flags);
    }
}

ZF_NAMESPACE_GLOBAL_END

