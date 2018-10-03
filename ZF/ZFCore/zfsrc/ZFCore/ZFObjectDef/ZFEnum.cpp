/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFEnum.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFEnum
ZFOBJECT_REGISTER(ZFEnum)

zfbool ZFEnum::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    const zfchar *valueString = ZFSerializableUtil::checkPropertyValue(serializableData);
    if(valueString != zfnull)
    {
        zfuint enumValue = ZFEnumInvalid();
        if(!zfscmpTheSame(valueString, ZFEnumNameInvalid()))
        {
            if(this->enumIsFlags())
            {
                zfflags t = zfflagsZero();
                if(zfflagsFromString(t, this->classData(), valueString))
                {
                    enumValue = (zfuint)t;
                }
            }
            else
            {
                enumValue = this->enumValueForName(valueString);
            }
            if(enumValue == ZFEnumInvalid())
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    "invalid value %s for enum %s", valueString, this->classData()->classNameFull());
                return zffalse;
            }
        }
        this->enumValueSet(enumValue);
    }
    else
    {
        this->enumValueSet(ZFEnumInvalid());
    }
    return zftrue;
}
zfbool ZFEnum::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                             ZF_IN ZFSerializable *referencedOwnerOrNull,
                                             ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    if((ref == zfnull && this->enumValue() != ZFEnumInvalid())
        || (ref != zfnull && this->enumValue() != ref->enumValue()))
    {
        if(this->enumIsFlags())
        {
            zfstring s;
            if(!zfflagsToString(s, this->classData(), (zfflags)this->enumValue()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    "unable convert enum value to string: %s",
                    this->objectInfo().cString());
                return zffalse;
            }
            serializableData.propertyValueSet(s);
        }
        else
        {
            serializableData.propertyValueSet(this->enumName());
        }
    }
    return zftrue;
}

void ZFEnum::objectOnInit(ZF_IN zfuint value)
{
    this->objectOnInit();
    zfself::enumValueSet(value);
}

void ZFEnum::objectOnInit(ZF_IN ZFEnum *another)
{
    this->objectOnInit();
    if(another != zfnull && another->classData()->classIsTypeOf(this->classData()))
    {
        zfself::enumValueSet(another->enumValue());
    }
}

void ZFEnum::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    if(this->classData()->classNamespace() != zfnull)
    {
        ret += this->classData()->classNamespace();
        ret += ZFNamespaceSeparator();
    }
    if(this->enumIsFlags()
        && zfsncmp(this->classData()->className(), ZFTypeIdWrapperPrefixName, ZFTypeIdWrapperPrefixNameLen) == 0)
    {
        ret += this->classData()->className() + ZFTypeIdWrapperPrefixNameLen;
    }
    else
    {
        ret += this->classData()->className();
    }
    ret += "::";
    if(this->enumValue() == ZFEnumInvalid())
    {
        ret += ZFEnumNameInvalid();
    }
    else
    {
        if(this->enumIsFlags())
        {
            zfflagsToString(ret, this->classData(), (zfflags)this->enumValue());
        }
        else
        {
            ret += this->enumName();
        }
    }
}

ZFCompareResult ZFEnum::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj)
    {
        return ZFCompareTheSame;
    }
    if(anotherObj == zfnull)
    {
        return ZFCompareUncomparable;
    }
    if(!anotherObj->classData()->classIsTypeOf(this->classData())
        && !this->classData()->classIsTypeOf(anotherObj->classData()))
    {
        return ZFCompareUncomparable;
    }

    zfself *another = ZFCastZFObjectUnchecked(zfself *, anotherObj);
    if(this->enumValue() == ZFEnumInvalid()
        || another->enumValue() == ZFEnumInvalid())
    {
        return ZFCompareUncomparable;
    }
    zfint n = this->enumValue() - another->enumValue();
    if(n < 0)
    {
        return ZFCompareSmaller;
    }
    else if(n == 0)
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareGreater;
    }
}

void *ZFEnum::wrappedValue(void)
{
    return &(this->_ZFP_ZFEnum_value); /* EnumReinterpretCast */
}
void ZFEnum::wrappedValueSet(ZF_IN const void *v)
{
    this->_ZFP_ZFEnum_value = *(const zfuint *)v; /* EnumReinterpretCast */
}
void ZFEnum::wrappedValueGet(ZF_IN void *v)
{
    *(zfuint *)v = this->_ZFP_ZFEnum_value; /* EnumReinterpretCast */
}
void ZFEnum::wrappedValueReset(void)
{
    this->_ZFP_ZFEnum_value = ZFEnumInvalid();
}
zfbool ZFEnum::wrappedValueIsInit(void)
{
    return (this->_ZFP_ZFEnum_value == ZFEnumInvalid());
}
ZFCompareResult ZFEnum::wrappedValueCompare(ZF_IN const void *v0,
                                            ZF_IN const void *v1)
{
    return ZFComparerDefault(*(const zfuint *)v0, *(const zfuint *)v1); /* EnumReinterpretCast */
}
void ZFEnum::wrappedValueGetInfo(ZF_IN_OUT zfstring &ret,
                                 ZF_IN const void *v)
{
    /* EnumReinterpretCast */
    zfflagsToString(ret, this->classData(), (zfflags)*(const zfuint *)v);
}
zfbool ZFEnum::wrappedValueProgressUpdate(ZF_IN const void *from,
                                          ZF_IN const void *to,
                                          ZF_IN zffloat progress)
{
    return zffalse;
}
zfbool ZFEnum::wrappedValueFromData(ZF_IN const ZFSerializableData &serializableData,
                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                    ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    return this->serializeFromData(serializableData, outErrorHint, outErrorPos);
}
zfbool ZFEnum::wrappedValueToData(ZF_OUT ZFSerializableData &serializableData,
                                  ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    return this->serializeToData(serializableData, outErrorHint);
}
zfbool ZFEnum::wrappedValueFromString(ZF_IN const zfchar *src,
                                      ZF_IN_OPT zfindex srcLen /* = zfindexMax() */)
{
    if(zfsncmp(ZFEnumNameInvalid(), src, srcLen == zfindexMax() ? zfslen(src) : srcLen) == 0)
    {
        this->enumValueSet(ZFEnumInvalid());
        return zftrue;
    }

    if(this->enumIsFlags())
    {
        zfflags t = zfflagsZero();
        if(zfflagsFromString(t, this->classData(), src, srcLen))
        {
            this->enumValueSet((zfuint)t);
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }
    else
    {
        zfuint v = this->enumValueForName((srcLen == zfindexMax()) ? src : zfstring(src, srcLen).cString());
        if(v == ZFEnumInvalid())
        {
            return zffalse;
        }
        else
        {
            this->enumValueSet(v);
            return zftrue;
        }
    }
}
zfbool ZFEnum::wrappedValueToString(ZF_IN_OUT zfstring &s)
{
    if(this->enumIsFlags())
    {
        return zfflagsToString(s, this->classData(), (zfflags)this->enumValue());
    }
    else
    {
        s += this->enumName();
        return zftrue;
    }
}

// ============================================================
// zfflags conversion
zfclass _ZFP_I_ZFEnum_stringConverterDataHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFEnum_stringConverterDataHolder, ZFObject)

public:
    zfindex enumCount;
    zfflags *flagList;
    const zfchar **nameList;

public:
    static _ZFP_I_ZFEnum_stringConverterDataHolder *setup(ZF_IN const ZFClass *enumClass)
    {
        _ZFP_I_ZFEnum_stringConverterDataHolder *ret = enumClass->classTagGet<_ZFP_I_ZFEnum_stringConverterDataHolder *>(_ZFP_I_ZFEnum_stringConverterDataHolder::ClassData()->classNameFull());
        if(ret == zfnull)
        {
            const ZFMethod *enumCountMethod = enumClass->methodForName("EnumCount");
            const ZFMethod *enumValueAtIndexMethod = enumClass->methodForName("EnumValueAtIndex");
            const ZFMethod *enumNameAtIndexMethod = enumClass->methodForName("EnumNameAtIndex");
            zfCoreAssert(enumCountMethod != zfnull && enumValueAtIndexMethod != zfnull && enumNameAtIndexMethod != zfnull);

            ret = zfAlloc(_ZFP_I_ZFEnum_stringConverterDataHolder);
            enumClass->classTagSet(_ZFP_I_ZFEnum_stringConverterDataHolder::ClassData()->classNameFull(), ret);
            zfRelease(ret);

            ret->enumCount = enumCountMethod->execute<zfindex>(zfnull);
            ret->flagList = (zfflags *)zfmalloc(sizeof(zfflags) * ret->enumCount);
            ret->nameList = (const zfchar **)zfmalloc(sizeof(const zfchar *) * ret->enumCount);
            for(zfindex i = 0; i < ret->enumCount; ++i)
            {
                ret->flagList[i] = enumValueAtIndexMethod->execute<zfuint, zfindex>(zfnull, i);
                ret->nameList[i] = enumNameAtIndexMethod->execute<const zfchar *, zfindex>(zfnull, i);
            }
        }
        return ret;
    }

protected:
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zffree(this->flagList);
        this->flagList = zfnull;
        zffree(this->nameList);
        this->nameList = zfnull;
        zfsuper::objectOnDealloc();
    }
};
zfbool zfflagsToString(ZF_IN_OUT zfstring &ret,
                       ZF_IN const ZFClass *enumClass,
                       ZF_IN zfflags const &value,
                       ZF_IN_OPT zfbool includeNotConverted /* = zftrue */,
                       ZF_IN_OPT zfbool exclusiveMode /* = zffalse */,
                       ZF_OUT_OPT zfflags *notConverted /* = zfnull */,
                       ZF_IN_OPT zfchar separatorToken /* = '|' */)
{
    zfCoreMutexLocker();
    if(enumClass != zfnull && enumClass->classIsTypeOf(ZFEnum::ClassData()))
    {
        _ZFP_I_ZFEnum_stringConverterDataHolder *d = _ZFP_I_ZFEnum_stringConverterDataHolder::setup(enumClass);
        return zfflagsToString(ret,
            d->flagList, d->nameList, d->enumCount,
            value,
            includeNotConverted, exclusiveMode, notConverted, separatorToken);
    }
    else
    {
        return zfflagsToString(ret,
            zfnull, zfnull, 0,
            value,
            includeNotConverted, exclusiveMode, notConverted, separatorToken);
    }
}
zfbool zfflagsFromString(ZF_OUT zfflags &ret,
                         ZF_IN const ZFClass *enumClass,
                         ZF_IN const zfchar *src,
                         ZF_IN_OPT zfindex srcLen /* = zfindexMax() */,
                         ZF_IN_OPT zfchar separatorToken /* = '|' */,
                         ZF_OUT_OPT const zfchar **outErrorPos /* = zfnull */)
{
    zfCoreMutexLocker();
    if(enumClass != zfnull && enumClass->classIsTypeOf(ZFEnum::ClassData()))
    {
        _ZFP_I_ZFEnum_stringConverterDataHolder *d = _ZFP_I_ZFEnum_stringConverterDataHolder::setup(enumClass);
        return zfflagsFromString(
            ret,
            d->flagList, d->nameList, d->enumCount,
            src, srcLen, separatorToken,
            outErrorPos);
    }
    else
    {
        return zfflagsFromString(
            ret,
            zfnull, zfnull, 0,
            src, srcLen, separatorToken,
            outErrorPos);
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_DETAIL_1(ZFEnum_objectOnInit_zfflags, ZFEnum::_ZFP_ZFEnum_objectOnInit_zfflags, ZFEnum::ClassData(),
    protected, ZFMethodTypeVirtual,
    void, "objectOnInit"
    , ZFMP_IN(zfflags, value)
    )
ZFMETHOD_USER_REGISTER_DETAIL_1(ZFEnum_objectOnInit_zfuint, ZFEnum::_ZFP_ZFEnum_objectOnInit_zfuint, ZFEnum::ClassData(),
    protected, ZFMethodTypeVirtual,
    void, "objectOnInit"
    , ZFMP_IN(zfuint, value)
    )
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFEnum, zfindex, enumCount)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFEnum, zfindex, enumIndexForValue, ZFMP_IN(zfuint, value))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFEnum, zfuint, enumValueAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFEnum, const zfchar *, enumNameAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFEnum, zfbool, enumContainValue, ZFMP_IN(zfuint, value))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFEnum, zfuint, enumValueForName, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFEnum, const zfchar *, enumNameForValue, ZFMP_IN(zfuint, value))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFEnum, zfbool, enumIsFlags)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFEnum, zfuint, enumDefault)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFEnum, zfuint, enumValue)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFEnum, const zfchar *, enumName)
static void _ZFP_ZFEnum_enumValueSet_methodInvoker(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfuint value)
{
    invokerObject->to<ZFEnum *>()->_ZFP_ZFEnum_value = value;
}
ZFMETHOD_USER_REGISTER_DETAIL_1(ZFEnum_enumValueSet, _ZFP_ZFEnum_enumValueSet_methodInvoker, ZFEnum::ClassData(), protected, ZFMethodTypeVirtual, void, "enumValueSet", ZFMP_IN(zfuint, value))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_7(zfbool, zfflagsToString, ZFMP_IN_OUT(zfstring &, ret), ZFMP_IN(const ZFClass *, enumClass), ZFMP_IN(zfflags const &, value), ZFMP_IN_OPT(zfbool, includeNotConverted, zftrue), ZFMP_IN_OPT(zfbool, exclusiveMode, zffalse), ZFMP_OUT_OPT(zfflags *, notConverted, zfnull), ZFMP_IN_OPT(zfchar, separatorToken, '|'))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_6(zfstring, zfflagsToString, ZFMP_IN(const ZFClass *, enumClass), ZFMP_IN(zfflags const &, value), ZFMP_IN_OPT(zfbool, includeNotConverted, zftrue), ZFMP_IN_OPT(zfbool, exclusiveMode, zffalse), ZFMP_OUT_OPT(zfflags *, notConverted, zfnull), ZFMP_IN_OPT(zfchar, separatorToken, '|'))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_5(zfbool, zfflagsFromString, ZFMP_OUT(zfflags &, ret), ZFMP_IN(const ZFClass *, enumClass), ZFMP_IN(const zfchar *, src), ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()), ZFMP_IN_OPT(zfchar, separatorToken, '|'))

ZF_NAMESPACE_GLOBAL_END
#endif

