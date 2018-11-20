/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFEnumDeclare.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFEnumDataHolder)
{
}
ZFCoreMap enumDataMap;
ZF_STATIC_INITIALIZER_END(ZFEnumDataHolder)
#define _ZFP_ZFEnumDataMap (ZF_STATIC_INITIALIZER_INSTANCE(ZFEnumDataHolder)->enumDataMap)

// ============================================================
zfclassNotPOD _ZFP_ZFEnumDataPrivate
{
public:
    typedef zfstlmap<zfuint, ZFCoreArrayPOD<zfchar *> > ValueMapType;
    ValueMapType valueMap;
    typedef zfstlmap<const zfchar *, zfuint, zfcharConst_zfstlComparer> NameMapType;
    NameMapType nameMap;
    ZFCoreArrayPOD<zfuint> vl; // ensured no duplicated value
    ZFCoreArrayPOD<zfchar *> nl; // for duplicated value, only first stored
public:
    ~_ZFP_ZFEnumDataPrivate(void)
    {
        this->nameMap.clear();
        for(ValueMapType::iterator it = this->valueMap.begin(); it != this->valueMap.end(); ++it)
        {
            for(zfindex j = 0; j < it->second.count(); ++j)
            {
                zffree(it->second[j]);
            }
        }
    }
};
_ZFP_ZFEnumData::_ZFP_ZFEnumData(void)
: needInitFlag(zftrue)
, ownerClass(zfnull)
, enumDefault(0)
, enumIsFlags(zffalse)
, d(zfnew(_ZFP_ZFEnumDataPrivate))
{
}
_ZFP_ZFEnumData::~_ZFP_ZFEnumData(void)
{
    zfdelete(d);
    d = zfnull;
}
void _ZFP_ZFEnumData::add(ZF_IN zfbool isEnableDuplicateValue,
                          ZF_IN zfuint value,
                          ZF_IN const zfchar *name)
{
    zfCoreAssert(value != ZFEnumInvalid());
    zfCoreAssert(!zfsIsEmpty(name));
    _ZFP_ZFEnumDataPrivate::ValueMapType::iterator it = d->valueMap.find(value);
    if(it != d->valueMap.end())
    {
        zfCoreAssertWithMessageTrim(isEnableDuplicateValue,
            "[ZFEnum] duplicate value %s (new: %s, old: %s) when define %s",
            zfsFromInt(value).cString(),
            name,
            it->second[0],
            this->ownerClass->classNameFull());
        it->second.add(zfsCopy(name));
    }
    else
    {
        zfchar *nameTmp = zfsCopy(name);
        d->valueMap[value].add(nameTmp);
        d->vl.add(value);
        d->nl.add(nameTmp);
    }
    if(d->nameMap.find(name) == d->nameMap.end())
    {
        d->nameMap[name] = value;
    }
}
zfindex _ZFP_ZFEnumData::enumCount(void) const
{
    return d->vl.count();
}
zfindex _ZFP_ZFEnumData::enumIndexForValue(ZF_IN zfuint value) const
{
    return d->vl.find(value);
}
zfuint _ZFP_ZFEnumData::enumValueAtIndex(ZF_IN zfindex index) const
{
    if(index >= d->vl.count())
    {
        return ZFEnumInvalid();
    }
    else
    {
        return d->vl[index];
    }
}
const zfchar *_ZFP_ZFEnumData::enumNameAtIndex(ZF_IN zfindex index) const
{
    if(index >= d->nl.count())
    {
        return zfnull;
    }
    else
    {
        return d->nl[index];
    }
}
zfbool _ZFP_ZFEnumData::enumContainValue(ZF_IN zfuint value) const
{
    return (d->valueMap.find(value) != d->valueMap.end());
}
zfuint _ZFP_ZFEnumData::enumValueForName(ZF_IN const zfchar *name) const
{
    if(!zfsIsEmpty(name))
    {
        _ZFP_ZFEnumDataPrivate::NameMapType::iterator it = d->nameMap.find(name);
        if(it != d->nameMap.end())
        {
            return it->second;
        }
    }
    return ZFEnumInvalid();
}
const zfchar *_ZFP_ZFEnumData::enumNameForValue(ZF_IN zfuint value) const
{
    _ZFP_ZFEnumDataPrivate::ValueMapType::const_iterator it = d->valueMap.find(value);
    if(it != d->valueMap.end())
    {
        return it->second[0];
    }
    else
    {
        return ZFEnumNameInvalid();
    }
}

_ZFP_ZFEnumData *_ZFP_ZFEnumDataAccess(ZF_IN const ZFClass *ownerClass)
{
    zfCoreMutexLocker();
    _ZFP_ZFEnumData *d = _ZFP_ZFEnumDataMap.get<_ZFP_ZFEnumData *>(ownerClass->classNameFull());
    if(d != zfnull)
    {
        return d;
    }
    d = zfnew(_ZFP_ZFEnumData);
    _ZFP_ZFEnumDataMap.set(ownerClass->classNameFull(), ZFCorePointerForObject<_ZFP_ZFEnumData *>(d));
    d->ownerClass = ownerClass;
    return d;
}
void _ZFP_ZFEnumDataCleanup(ZF_IN const ZFClass *ownerClass)
{
    _ZFP_ZFEnumDataMap.remove(ownerClass->classNameFull());
}

// ============================================================
static zfbool _ZFP_ZFEnumIvk_EnumIsFlags(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject)
{return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumIsFlags;}

static zfuint _ZFP_ZFEnumIvk_EnumDefault(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject)
{return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumDefault;}

static zfindex _ZFP_ZFEnumIvk_EnumCount(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject)
{return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumCount();}

static zfindex _ZFP_ZFEnumIvk_EnumIndexForValue(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfuint value)
{return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumIndexForValue(value);}

static zfuint _ZFP_ZFEnumIvk_EnumValueAtIndex(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfindex index)
{return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumValueAtIndex(index);}

static const zfchar *_ZFP_ZFEnumIvk_EnumNameAtIndex(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfindex index)
{return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumNameAtIndex(index);}

static zfbool _ZFP_ZFEnumIvk_EnumContainValue(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfuint value)
{return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumContainValue(value);}

static zfuint _ZFP_ZFEnumIvk_EnumValueForName(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN const zfchar *name)
{return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumValueForName(name);}

static const zfchar *_ZFP_ZFEnumIvk_EnumNameForValue(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfuint value)
{return _ZFP_ZFEnumDataFind(invokerMethod->methodOwnerClass())->enumNameForValue(value);}

static void _ZFP_ZFEnumIvk_enumValueSet(ZF_IN const ZFMethod *invokerMethod, ZF_IN ZFObject *invokerObject, ZF_IN zfuint value)
{return invokerObject->to<ZFEnum *>()->_ZFP_enumValueSet(value);}

const _ZFP_ZFEnumData *_ZFP_ZFEnumDataFind(ZF_IN const ZFClass *enumClass)
{
    return _ZFP_ZFEnumDataMap.get<_ZFP_ZFEnumData *>(enumClass->classNameFull());
}
void _ZFP_ZFEnumMethodReg(ZF_IN_OUT ZFCoreArrayPOD<const ZFMethod *> &ret,
                          ZF_IN const _ZFP_ZFEnumData *d)
{
    {
        ZFMethodUserRegisterDetail_0(resultMethod, &_ZFP_ZFEnumIvk_EnumIsFlags, d->ownerClass,
            public, ZFMethodTypeStatic,
            zfbool, "EnumIsFlags");
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_0(resultMethod, &_ZFP_ZFEnumIvk_EnumDefault, d->ownerClass,
            public, ZFMethodTypeStatic,
            zfuint, "EnumDefault");
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_0(resultMethod, &_ZFP_ZFEnumIvk_EnumCount, d->ownerClass,
            public, ZFMethodTypeStatic,
            zfindex, "EnumCount");
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, &_ZFP_ZFEnumIvk_EnumIndexForValue, d->ownerClass,
            public, ZFMethodTypeStatic,
            zfindex, "EnumIndexForValue",
            ZFMP_IN(zfuint, value));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, &_ZFP_ZFEnumIvk_EnumValueAtIndex, d->ownerClass,
            public, ZFMethodTypeStatic,
            zfuint, "EnumValueAtIndex",
            ZFMP_IN(zfindex, index));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, &_ZFP_ZFEnumIvk_EnumNameAtIndex, d->ownerClass,
            public, ZFMethodTypeStatic,
            const zfchar *, "EnumNameAtIndex",
            ZFMP_IN(zfindex, index));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, &_ZFP_ZFEnumIvk_EnumContainValue, d->ownerClass,
            public, ZFMethodTypeStatic,
            zfbool, "EnumContainValue",
            ZFMP_IN(zfuint, value));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, &_ZFP_ZFEnumIvk_EnumValueForName, d->ownerClass,
            public, ZFMethodTypeStatic,
            zfuint, "EnumValueForName",
            ZFMP_IN(const zfchar *, name));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, &_ZFP_ZFEnumIvk_EnumNameForValue, d->ownerClass,
            public, ZFMethodTypeStatic,
            const zfchar *, "EnumNameForValue",
            ZFMP_IN(zfuint, value));
        ret.add(resultMethod);
    }
    {
        ZFMethodUserRegisterDetail_1(resultMethod, &_ZFP_ZFEnumIvk_enumValueSet, d->ownerClass,
            public, ZFMethodTypeVirtual,
            void, "enumValueSet",
            ZFMP_IN(zfuint, value));
        ret.add(resultMethod);
    }
}

ZF_NAMESPACE_GLOBAL_END

