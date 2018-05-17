/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFOutputForFormat.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFOutputFormat)

// ============================================================
zfclass _ZFP_I_ZFOutputForFormatOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFOutputForFormatOwner, ZFObject)

public:
    ZFOutput output;
    ZFOutputFormat *format;
    ZFOutputFormat::OutputStep outputStep;
    zfindex writtenLen;

protected:
    _ZFP_I_ZFOutputForFormatOwner(void)
    : output()
    , format(zfnull)
    , outputStep(ZFOutputFormat::OutputStepBegin)
    , writtenLen(0)
    {
    }

protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        if(this->format != zfnull && this->output.callbackIsValid())
        {
            this->outputEnd();
        }
        zfsuper::objectOnDeallocPrepare();
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        if(this->format != zfnull)
        {
            zfRelease(this->format);
            this->format = zfnull;
        }
        zfsuper::objectOnDealloc();
    }

public:
    void outputBegin(void)
    {
        zfstring buf;
        this->format->_ZFP_format(buf, ZFOutputFormat::OutputStepBegin, zfText(""), 0, this->writtenLen);
        if(!buf.isEmpty())
        {
            this->writtenLen += this->output.execute(buf.cString(), buf.length() * sizeof(zfchar));
        }
    }
    void outputEnd(void)
    {
        zfstring buf;
        this->format->_ZFP_format(buf, ZFOutputFormat::OutputStepEnd, zfText(""), 0, this->writtenLen);
        if(!buf.isEmpty())
        {
            this->writtenLen += this->output.execute(buf.cString(), buf.length() * sizeof(zfchar));
        }
    }
    ZFMETHOD_INLINE_2(zfindex, onOutput,
                      ZFMP_IN(const void *, s),
                      ZFMP_IN(zfindex, count))
    {
        if(!this->output.callbackIsValid())
        {
            return 0;
        }

        if(count == zfindexMax())
        {
            count = zfslen((const zfchar *)s);
        }
        else
        {
            count /= sizeof(zfchar);
        }

        zfstring buf;
        this->format->_ZFP_format(
            buf,
            ZFOutputFormat::OutputStepAction,
            (const zfchar *)s,
            count,
            this->writtenLen);
        if(buf.isEmpty())
        {
            return count * sizeof(zfchar);
        }

        zfindex written = this->output.execute(buf.cString(), buf.length() * sizeof(zfchar));
        this->writtenLen += written;
        if(written == buf.length() * sizeof(zfchar))
        {
            return count * sizeof(zfchar);
        }
        else
        {
            return 0;
        }
    }
};

ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFOutputForFormatT,
                       ZFMP_OUT(ZFCallback &, ret),
                       ZFMP_IN(const ZFOutput &, output),
                       ZFMP_IN(ZFOutputFormat *, format))
{
    if(!output.callbackIsValid())
    {
        return zffalse;
    }
    if(format == zfnull)
    {
        ret = output;
        return zffalse;
    }
    zfbool needSerialize = (ret.callbackSerializeCustomType() == zfnull);

    _ZFP_I_ZFOutputForFormatOwner *outputOwner = zfAlloc(_ZFP_I_ZFOutputForFormatOwner);
    outputOwner->output = output;
    outputOwner->format = zfRetain(format);
    ret = ZFCallbackForMemberMethod(outputOwner, ZFMethodAccess(_ZFP_I_ZFOutputForFormatOwner, onOutput));
    ret.callbackOwnerObjectRetain();
    ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, output.callbackTagGet(ZFCallbackTagKeyword_ioOwner));
    zfRelease(outputOwner);

    if(needSerialize)
    {
        ZFSerializableData outputData;
        ZFSerializableData formatData;
        if(ZFCallbackToData(outputData, output) && ZFObjectToData(formatData, format->toObject()))
        {
            ZFSerializableData serializableData;
            outputData.categorySet(ZFSerializableKeyword_ZFOutputForFormat_output);
            serializableData.elementAdd(outputData);
            formatData.categorySet(ZFSerializableKeyword_ZFOutputForFormat_format);
            serializableData.elementAdd(formatData);
            ret.callbackSerializeCustomTypeSet(ZFCallbackSerializeCustomType_ZFOutputForFormat);
            ret.callbackSerializeCustomDataSet(serializableData);
        }
    }

    outputOwner->outputBegin();
    return zftrue;
}

ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForFormat,
                       ZFMP_IN(const ZFOutput &, output),
                       ZFMP_IN(ZFOutputFormat *, format))
{
    ZFOutput ret;
    ZFOutputForFormatT(ret, output, format);
    return ret;
}

ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFOutputForFormat, ZFCallbackSerializeCustomType_ZFOutputForFormat)
{
    const ZFSerializableData *outputData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFOutputForFormat_output, outErrorHint, outErrorPos);
    if(outputData == zfnull)
    {
        return zffalse;
    }
    ZFOutput output;
    if(!ZFCallbackFromData(output, *outputData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    const ZFSerializableData *formatData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFOutputForFormat_format, outErrorHint, outErrorPos);
    if(formatData == zfnull)
    {
        return zffalse;
    }
    zfautoObject formatHolder;
    if(!zfautoObjectFromData(formatHolder, *formatData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    ZFOutputFormat *format = formatHolder;
    if(format == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, *formatData,
            zfText("format object %s not type of %s"),
            ZFObjectInfo(formatHolder.toObject()).cString(),
            ZFOutputFormat::ClassData()->className());
        return zffalse;
    }


    ZFOutput retTmp;
    retTmp.callbackSerializeCustomDisable();
    if(!ZFOutputForFormatT(retTmp, output, format))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, *formatData,
            zfText("unable to create from output %s and format %s"),
            output.objectInfo().cString(),
            ZFObjectInfo(formatHolder.toObject()).cString());
        return zffalse;
    }
    ret = retTmp;

    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(ZFOutputFormat *, ZFOutputForFormatGetFormat,
                       ZFMP_IN(const ZFCallback &, callback))
{
    _ZFP_I_ZFOutputForFormatOwner *owner = ZFCastZFObject(_ZFP_I_ZFOutputForFormatOwner *, callback.callbackOwnerObject());
    if(owner == zfnull)
    {
        return zfnull;
    }
    else
    {
        return owner->format;
    }
}

ZFMETHOD_FUNC_DEFINE_1(ZFOutput, ZFOutputForFormatGetOutput,
                       ZFMP_IN(const ZFCallback &, callback))
{
    _ZFP_I_ZFOutputForFormatOwner *owner = ZFCastZFObject(_ZFP_I_ZFOutputForFormatOwner *, callback.callbackOwnerObject());
    if(owner == zfnull)
    {
        return ZFCallbackNull();
    }
    else
    {
        return owner->output;
    }
}

ZF_NAMESPACE_GLOBAL_END

