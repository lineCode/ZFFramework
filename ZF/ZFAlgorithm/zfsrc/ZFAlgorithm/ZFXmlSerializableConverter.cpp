/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFXmlSerializableConverter.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static zfbool _ZFP_ZFSerializableDataFromXml(ZF_OUT ZFSerializableData &serializableData,
                                             ZF_IN const ZFXmlItem &xmlElement,
                                             ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                             ZF_OUT_OPT ZFXmlItem *outErrorPos = zfnull)
{
    if(xmlElement.xmlIsNull())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, zfText("null xml element"));
        if(outErrorPos != zfnull)
        {
            *outErrorPos = xmlElement;
        }
        return zffalse;
    }
    if(xmlElement.xmlType() != ZFXmlType::e_XmlElement)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, zfText("param not type of xml element"));
        if(outErrorPos != zfnull)
        {
            *outErrorPos = xmlElement;
        }
        return zffalse;
    }

    if(xmlElement.xmlName() == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, zfText("missing xml node name"));
        if(outErrorPos != zfnull)
        {
            *outErrorPos = xmlElement;
        }
        return zffalse;
    }
    serializableData.itemClassSet(xmlElement.xmlName());

    ZFXmlItem attribute = xmlElement.xmlAttributeFirst();
    while(!attribute.xmlIsNull())
    {
        if(attribute.xmlName() == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, zfText("missing xml attribute name"));
            if(outErrorPos != zfnull)
            {
                *outErrorPos = attribute;
            }
            return zffalse;
        }
        serializableData.attributeSet(attribute.xmlName(), attribute.xmlValue());

        attribute = attribute.xmlAttributeNext();
    }

    ZFXmlItem element = xmlElement.xmlChildElementFirst();
    while(!element.xmlIsNull())
    {
        ZFSerializableData childData;
        if(!_ZFP_ZFSerializableDataFromXml(childData, element, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        serializableData.elementAdd(childData);
        element = element.xmlSiblingElementNext();
    }

    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataFromXml,
                       ZFMP_OUT(ZFSerializableData &, serializableData),
                       ZFMP_IN(const ZFXmlItem &, xmlElement),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_OUT_OPT(ZFXmlItem *, outErrorPos, zfnull))
{
    return _ZFP_ZFSerializableDataFromXml(serializableData, xmlElement, outErrorHint, outErrorPos);
}
ZFMETHOD_FUNC_DEFINE_3(ZFSerializableData, ZFSerializableDataFromXml,
                       ZFMP_IN(const ZFXmlItem &, xmlElement),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_OUT_OPT(ZFXmlItem *, outErrorPos, zfnull))
{
    ZFSerializableData ret;
    if(ZFSerializableDataFromXml(ret, xmlElement, outErrorHint, outErrorPos))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataToXml,
                       ZFMP_OUT(ZFXmlItem &, xmlElement),
                       ZFMP_IN(const ZFSerializableData &, serializableData),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
{
    xmlElement = ZFSerializableDataToXml(serializableData, outErrorHint, outErrorPos);
    return (xmlElement.xmlType() != ZFXmlType::e_XmlNull);
}
ZFMETHOD_FUNC_DEFINE_3(ZFXmlItem, ZFSerializableDataToXml,
                       ZFMP_IN(const ZFSerializableData &, serializableData),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
{
    if(serializableData.itemClass() == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData, zfText("missing serializable class"));
        return ZFXmlItem();
    }

    ZFXmlItem ret(ZFXmlType::e_XmlElement);
    ret.xmlNameSet(serializableData.itemClass());

    for(zfiterator it = serializableData.attributeIterator();
        serializableData.attributeIteratorIsValid(it);
        serializableData.attributeIteratorNext(it))
    {
        ret.xmlAttributeAdd(serializableData.attributeIteratorGetKey(it),
            serializableData.attributeIteratorGet(it));
    }

    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        ZFXmlItem child = ZFSerializableDataToXml(serializableData.elementAtIndex(i), outErrorHint, outErrorPos);
        if(child.xmlType() == ZFXmlType::e_XmlNull)
        {
            return ZFXmlItem();
        }
        ret.xmlChildAdd(child);
    }

    return ret;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFSerializableDataFromXml,
                       ZFMP_OUT(ZFSerializableData &, ret),
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    if(!input.callbackIsValid())
    {
        zfstringAppend(outErrorHint, zfText("invalid input callback"));
        return zffalse;
    }
    ZFXmlItem xmlElement = ZFXmlParseFirstElement(input);
    if(xmlElement.xmlType() == ZFXmlType::e_XmlNull)
    {
        zfstringAppend(outErrorHint, zfText("unable to parse xml from input"));
        return zffalse;
    }
    if(!ZFSerializableDataFromXml(ret, xmlElement, outErrorHint))
    {
        return zffalse;
    }

    ret.pathInfoSet(input.pathInfo());
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_2(ZFSerializableData, ZFSerializableDataFromXml,
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    ZFSerializableData ret;
    if(ZFSerializableDataFromXml(ret, input, outErrorHint))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFSerializableDataToXml,
                       ZFMP_IN(const ZFOutput &, outputCallback),
                       ZFMP_IN(const ZFSerializableData &, serializableData),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_IN_OPT(const ZFXmlOutputFlags &, flags, ZFXmlOutputFlagsDefault()))
{
    ZFXmlItem xmlElement;
    if(ZFSerializableDataToXml(xmlElement, serializableData, outErrorHint))
    {
        xmlElement.xmlAttributeSortRecursively();
        zfbool ret = ZFXmlItemToOutput(outputCallback, xmlElement, flags);
        outputCallback.execute(zfText("\n"));
        if(!ret)
        {
            zfstringAppend(outErrorHint, zfText("unable to convert xml to string"));
        }
        return ret;
    }
    else
    {
        return zffalse;
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFObjectFromXml,
                       ZFMP_OUT(zfautoObject &, ret),
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    ZFSerializableData data;
    if(ZFSerializableDataFromXml(data, input, outErrorHint))
    {
        return ZFObjectFromData(ret, data, outErrorHint);
    }
    else
    {
        return zffalse;
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfautoObject, ZFObjectFromXml,
                       ZFMP_IN(const ZFInput &, input),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
{
    zfautoObject ret;
    ZFObjectFromXml(ret, input, outErrorHint);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFObjectToXml,
                       ZFMP_IN(const ZFOutput &, outputCallback),
                       ZFMP_IN(ZFObject *, obj),
                       ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull),
                       ZFMP_IN_OPT(const ZFXmlOutputFlags &, flags, ZFXmlOutputFlagsDefault()))
{
    ZFSerializableData serializableData;
    if(!ZFObjectToData(serializableData, obj, outErrorHint))
    {
        return zffalse;
    }
    else
    {
        return ZFSerializableDataToXml(outputCallback, serializableData, outErrorHint, flags);
    }
}

ZF_NAMESPACE_GLOBAL_END

