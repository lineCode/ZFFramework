/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFPropertyCallbackDefaultImpl.h"
#include "ZFObjectImpl.h"

#include "ZFSerializableUtil.h"

#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
void _ZFP_propCbDValueSet_generic(ZF_IN const ZFProperty *property,
                                  ZF_IN ZFObject *dstObj,
                                  ZF_IN const void *value)
{
    zfautoObject valueNew;
    if(property->propertyIsRetainProperty())
    {
        valueNew = *(const zfautoObject *)value;
    }
    else
    {
        zfCoreAssert(property->propertyTypeIdData() != zfnull);
        property->propertyTypeIdData()->typeIdWrapper(valueNew);
        if(valueNew == zfnull)
        {
            valueNew = *(const zfautoObject *)value;
        }
        else
        {
            ZFTypeIdWrapper *wrapper = valueNew;
            if(wrapper != zfnull)
            {
                wrapper->wrappedValueSet(value);
            }
        }
    }
    property->setterMethod()->methodGenericInvoke(dstObj, valueNew);
}

// ============================================================
const void *_ZFP_propCbDValueGet_generic(ZF_IN const ZFProperty *property,
                                         ZF_IN ZFObject *ownerObj,
                                         ZF_IN_OUT void *&valueToken)
{
    zfautoObject value = property->getterMethod()->methodGenericInvoke(ownerObj);
    valueToken = zfnew(zfautoObject, value);
    if(property->propertyIsRetainProperty()
        || zfscmpTheSame(property->propertyTypeId(), ZFTypeId_zfautoObject()))
    {
        return valueToken;
    }
    ZFTypeIdWrapper *wrapper = value;
    if(wrapper != zfnull)
    {
        return wrapper->wrappedValue();
    }
    else
    {
        return valueToken;
    }
}
void _ZFP_propCbDValueGetRelease_generic(ZF_IN const ZFProperty *property,
                                         ZF_IN ZFObject *ownerObj,
                                         ZF_IN void *valueToken,
                                         ZF_IN const void *value)
{
    zfdelete((zfautoObject *)valueToken);
}

// ============================================================
ZFCompareResult _ZFP_propCbDCompare_generic(ZF_IN const ZFProperty *property,
                                            ZF_IN ZFObject *ownerObj,
                                            ZF_IN const void *v0,
                                            ZF_IN const void *v1)
{
    if(property->propertyIsRetainProperty())
    {
        return ZFObjectCompare(
            ((const zfautoObject *)v0)->toObject(),
            ((const zfautoObject *)v1)->toObject());
    }
    if(property->propertyTypeIdData() == zfnull)
    {
        return ZFCompareUncomparable;
    }

    zfautoObject holder;
    if(property->propertyTypeIdData()->typeIdWrapper(holder) && holder == zfnull)
    {
        return ZFObjectCompare(
            ((const zfautoObject *)v0)->toObject(),
            ((const zfautoObject *)v1)->toObject());
    }
    ZFTypeIdWrapper *wrapper = holder;
    if(wrapper != zfnull)
    {
        return wrapper->wrappedValueCompare(v0, v1);
    }
    else
    {
        return ZFCompareUncomparable;
    }
}

// ============================================================
void _ZFP_propCbDGetInfo_generic(ZF_IN const ZFProperty *property,
                                 ZF_IN ZFObject *ownerObj,
                                 ZF_IN const void *value,
                                 ZF_IN_OUT zfstring &ret)
{
    if(property->propertyIsRetainProperty())
    {
        ZFObjectInfoT(ret, ((const zfautoObject *)value)->toObject());
        return ;
    }
    if(property->propertyTypeIdData() == zfnull)
    {
        ret += ZFTOKEN_ZFCoreInfoGetterNotAvailable;
        return ;
    }
    zfautoObject holder;
    if(property->propertyTypeIdData()->typeIdWrapper(holder) && holder == zfnull)
    {
        ZFObjectInfoT(ret, ((const zfautoObject *)value)->toObject());
        return ;
    }
    ZFTypeIdWrapper *wrapper = holder;
    if(wrapper != zfnull)
    {
        wrapper->wrappedValueGetInfo(ret, value);
    }
    else
    {
        ret += ZFTOKEN_ZFCoreInfoGetterNotAvailable;
    }
}

// ============================================================
zfclass _ZFP_I_ZFPropertyValueStoreHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFPropertyValueStoreHolder, ZFObject)

public:
    zfstlmap<const ZFProperty *, zfstlmap<void *, ZFCorePointerForObject<ZFCorePointerBase *> > > d;
};
void _ZFP_ZFPropertyValueStoreImpl(ZF_IN const ZFProperty *property,
                                   ZF_IN ZFObject *ownerObj,
                                   ZF_IN void *valueStored,
                                   ZF_IN ZFCorePointerBase *valueHolder)
{
    zfCoreMutexLocker();
    _ZFP_I_ZFPropertyValueStoreHolder *d = ownerObj->tagGet<_ZFP_I_ZFPropertyValueStoreHolder *>(
            _ZFP_I_ZFPropertyValueStoreHolder::ClassData()->classNameFull());
    if(d == zfnull)
    {
        d = zflockfree_zfAlloc(_ZFP_I_ZFPropertyValueStoreHolder);
        ownerObj->tagSet(_ZFP_I_ZFPropertyValueStoreHolder::ClassData()->classNameFull(), d);
        zflockfree_zfRelease(d);
    }
    d->d[property][valueStored] = ZFCorePointerForObject<ZFCorePointerBase *>(valueHolder);
}
void _ZFP_ZFPropertyValueReleaseImpl(ZF_IN const ZFProperty *property,
                                     ZF_IN ZFObject *ownerObj,
                                     ZF_IN void *valueStored)
{
    zfCoreMutexLocker();
    _ZFP_I_ZFPropertyValueStoreHolder *d = ownerObj->tagGet<_ZFP_I_ZFPropertyValueStoreHolder *>(
        _ZFP_I_ZFPropertyValueStoreHolder::ClassData()->classNameFull());
    if(d == zfnull)
    {
        return ;
    }
    zfstlmap<const ZFProperty *, zfstlmap<void *, ZFCorePointerForObject<ZFCorePointerBase *> > >::iterator it =
        d->d.find(property);
    if(it == d->d.end())
    {
        return ;
    }
    it->second.erase(valueStored);
    if(it->second.empty())
    {
        d->d.erase(it);
    }
}

void *_ZFP_propCbDValueStore_generic(ZF_IN const ZFProperty *property,
                                     ZF_IN ZFObject *ownerObj,
                                     ZF_IN_OPT const void *value /* = zfnull */)
{
    zfautoObject wrapperHolder;
    if(property->propertyIsRetainProperty()
        || (property->propertyTypeIdData()->typeIdWrapper(wrapperHolder) && wrapperHolder == zfnull))
    {
        zfautoObject *valueStored = zfnew(zfautoObject);
        if(value != zfnull)
        {
            *valueStored = *(const zfautoObject *)value;
        }
        _ZFP_ZFPropertyValueStoreImpl(property, ownerObj, valueStored, zfnew(ZFCorePointerForObject<zfautoObject *>, valueStored));
        return valueStored;
    }

    ZFTypeIdWrapper *wrapper = wrapperHolder;
    wrapper->wrappedValueSet(value);
    void *valueStored = wrapper->wrappedValue();
    _ZFP_ZFPropertyValueStoreImpl(property, ownerObj, valueStored, zfnew(ZFCorePointerForZFObject<ZFObject *>, wrapperHolder));
    return valueStored;
}
void _ZFP_propCbDValueRelease_generic(ZF_IN const ZFProperty *property,
                                      ZF_IN ZFObject *ownerObj,
                                      ZF_IN void *valueStored)
{
    _ZFP_ZFPropertyValueReleaseImpl(property, ownerObj, valueStored);
}

// ============================================================
zfbool _ZFP_propCbDSerializeFrom_generic(ZF_IN const ZFProperty *propertyInfo,
                                         ZF_IN ZFObject *ownerObject,
                                         ZF_IN const ZFSerializableData &serializableData,
                                         ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                         ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    zfautoObject zfv;
    if(!_ZFP_propCbDSerializeFrom_impl(zfv, propertyInfo, serializableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    propertyInfo->callbackValueSet(propertyInfo, ownerObject, zfv.to<ZFTypeIdWrapper *>()->wrappedValue());
    return zftrue;
}
zfbool _ZFP_propCbDSerializeFrom_impl(ZF_OUT zfautoObject &zfv,
                                      ZF_IN const ZFProperty *propertyInfo,
                                      ZF_IN const ZFSerializableData &serializableData,
                                      ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                      ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    const ZFTypeIdBase *t = ZFTypeIdGet(propertyInfo->propertyTypeId());
    if(t == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "no such type id: %s",
            propertyInfo->propertyTypeId());
        return zffalse;
    }
    if(t->typeIdWrapper(zfv))
    {
        if(zfv == zfnull)
        {
            return ZFObjectFromData(zfv, serializableData, outErrorHint, outErrorPos);
        }
        ZFTypeIdWrapper *w = zfv;
        if(w != zfnull)
        {
            return w->wrappedValueFromData(serializableData, outErrorHint, outErrorPos);
        }
    }
    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
        "unable to access type id wrapper: %s",
        propertyInfo->propertyTypeId());
    return zffalse;
}
void _ZFP_propCbDSerializeFrom_errorOccurred(ZF_IN const ZFSerializableData &serializableData,
                                             ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                             ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
        "unable to access type id wrapper value");
}
zfbool _ZFP_propCbDSerializeTo_generic(ZF_IN const ZFProperty *propertyInfo,
                                       ZF_IN ZFObject *ownerObject,
                                       ZF_OUT ZFSerializableData &serializableData,
                                       ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    const ZFTypeIdBase *t = ZFTypeIdGet(propertyInfo->propertyTypeId());
    if(t == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "no such type id: %s",
            propertyInfo->propertyTypeId());
        return zffalse;
    }
    zfautoObject zfv;
    if(t->typeIdWrapper(zfv))
    {
        ZFPropertyCallbackValueGetHolder _valueGetHolder(propertyInfo, ownerObject);
        const void *v = _valueGetHolder.value();
        if(zfv == zfnull)
        {
            if(propertyInfo->propertyIsRetainProperty()
                || zfscmpTheSame(ZFTypeId_zfautoObject(), propertyInfo->propertyTypeId()))
            {
                return ZFObjectToData(serializableData, *(const zfautoObject *)v, outErrorHint);
            }
        }
        else
        {
            ZFTypeIdWrapper *w = zfv;
            if(w != zfnull)
            {
                w->wrappedValueSet(v);
                return w->wrappedValueToData(serializableData, outErrorHint);
            }
        }
    }
    ZFSerializableUtil::errorOccurred(outErrorHint,
        "unable to access type id wrapper: %s",
        propertyInfo->propertyTypeId());
    return zffalse;
}
zfbool _ZFP_propCbDSerializeTo_impl(ZF_IN const ZFProperty *propertyInfo,
                                    ZF_IN ZFObject *zfv,
                                    ZF_OUT ZFSerializableData &serializableData,
                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    const ZFTypeIdBase *t = ZFTypeIdGet(propertyInfo->propertyTypeId());
    if(t == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "no such type id: %s",
            propertyInfo->propertyTypeId());
        return zffalse;
    }
    ZFTypeIdWrapper *w = ZFCastZFObject(ZFTypeIdWrapper *, zfv);
    if(w == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            "unable to access type id wrapper: %s",
            propertyInfo->propertyTypeId());
        return zffalse;
    }
    return w->wrappedValueToData(serializableData, outErrorHint);
}
void _ZFP_propCbDSerializeTo_errorOccurred(ZF_IN const ZFSerializableData &serializableData,
                                           ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFSerializableUtil::errorOccurred(outErrorHint,
        "unable to access type id wrapper value");
}

// ============================================================
zfbool _ZFP_propCbDProgressUpdate_generic(ZF_IN const ZFProperty *property,
                                          ZF_IN ZFObject *ownerObj,
                                          ZF_IN_OPT const void *from /* = zfnull */,
                                          ZF_IN_OPT const void *to /* = zfnull */,
                                          ZF_IN_OPT zffloat progress /* = 1 */)
{
    if(property->propertyIsRetainProperty())
    {
        // dynamic registerd retain property does not support progress update
        // since lack of template type
        // but you can use ZFPropertyCallbackProgressUpdateChange to supply custom progress update logic
        // typically there's no need for retain property's progress update, though
        return zffalse;
    }

    zfautoObject valueOld = property->getterMethod()->methodGenericInvoke(ownerObj);

    ZFTypeIdWrapper *wrapper = valueOld;
    if(wrapper == zfnull)
    {
        return zffalse;
    }
    zfautoObject valueNew = wrapper->copy();
    if(!valueNew->to<ZFTypeIdWrapper *>()->wrappedValueProgressUpdate(from, to, progress))
    {
        return zffalse;
    }
    if(from == zfnull)
    {
        return zftrue;
    }
    property->setterMethod()->methodGenericInvoke(ownerObj, valueNew);
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

