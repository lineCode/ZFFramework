/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFSerializable.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableDataStringConverter.h"
#include "ZFPropertyUtil.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// cache in class's meta data for performance
zfclassNotPOD _ZFP_ZFSerializable_PropertyTypeData
{
public:
    const ZFProperty *property;
    ZFSerializablePropertyType propertyType;
};
zfclass _ZFP_I_ZFSerializablePropertyTypeHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFSerializablePropertyTypeHolder, ZFObject)
public:
    ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> serializableProperty;
    ZFCoreMap serializablePropertyMap; // _ZFP_ZFSerializable_PropertyTypeData *

public:
    void addData(ZF_IN const ZFProperty *property,
                 ZF_IN ZFSerializablePropertyType propertyType)
    {
        _ZFP_ZFSerializable_PropertyTypeData *typeData = zfnew(_ZFP_ZFSerializable_PropertyTypeData);
        typeData->property = property;
        typeData->propertyType = propertyType;

        this->serializableProperty.add(typeData);
        this->serializablePropertyMap.set(property->propertyName(),
            ZFCorePointerForObject<_ZFP_ZFSerializable_PropertyTypeData *>(typeData));
    }
};

// ============================================================
// _ZFP_ZFSerializablePrivate
zfclassNotPOD _ZFP_ZFSerializablePrivate
{
public:
    zfstring editModeWrappedClassName;
    ZFCoreArray<ZFSerializableData> editModeWrappedElementDatas;
public:
    _ZFP_ZFSerializablePrivate(void)
    {
    }
    _ZFP_ZFSerializablePrivate(ZF_IN const _ZFP_ZFSerializablePrivate &ref)
    : editModeWrappedClassName(ref.editModeWrappedClassName)
    , editModeWrappedElementDatas()
    {
        this->_copyInternal(ref);
    }
    _ZFP_ZFSerializablePrivate &operator = (ZF_IN const _ZFP_ZFSerializablePrivate &ref)
    {
        this->editModeWrappedClassName = ref.editModeWrappedClassName;
        this->_copyInternal(ref);
        return *this;
    }
private:
    void _copyInternal(ZF_IN const _ZFP_ZFSerializablePrivate &ref)
    {
        this->editModeWrappedElementDatas.removeAll();
        this->editModeWrappedElementDatas.capacitySet(ref.editModeWrappedElementDatas.capacity());
        for(zfindex i = 0; i < ref.editModeWrappedElementDatas.count(); ++i)
        {
            this->editModeWrappedElementDatas.add(ref.editModeWrappedElementDatas[i].copy());
        }
    }
};

// ============================================================
// ZFSerializable
ZFSerializable::~ZFSerializable(void)
{
    zfdelete(d);
}

ZFCoreMap &ZFSerializable::editModeData(void)
{
    static ZFCoreMap m;
    return m;
}
zfbool &ZFSerializable::editMode(void)
{
    static zfbool v = zffalse;
    return v;
}
const zfchar *ZFSerializable::editModeWrappedClassName(void)
{
    if(d != zfnull && !d->editModeWrappedClassName.isEmpty())
    {
        return d->editModeWrappedClassName.cString();
    }
    else
    {
        return zfnull;
    }
}
void ZFSerializable::editModeWrappedClassNameSet(ZF_IN const zfchar *value)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFSerializablePrivate);
    }
    d->editModeWrappedClassName = value;
}
ZFCoreArray<ZFSerializableData> &ZFSerializable::editModeWrappedElementDatas(void)
{
    if(d == zfnull)
    {
        d = zfnew(_ZFP_ZFSerializablePrivate);
    }
    return d->editModeWrappedElementDatas;
}

zfbool ZFSerializable::serializable(void)
{
    return this->serializableOnCheck();
}
zfbool ZFSerializable::serializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                         ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                         ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    // style logic
    ZFStyleable *styleable = ZFCastZFObject(ZFStyleable *, this);
    {
        const zfchar *styleKey = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_styleKey);
        if(styleKey != zfnull)
        {
            if(styleable == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    zfText("styleKey exists but the object is not ZFStyleable: %s"),
                    this->toObject()->objectInfoOfInstance().cString());
                return zffalse;
            }
            if(!styleable->styleKeySet(styleKey))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                    zfText("unable to apply style from styleKey: %s"),
                    styleKey);
                return zffalse;
            }
        }
    }

    // editMode
    if(ZFSerializable::editMode())
    {
        ZFSerializable::EditModeData *editModeData = ZFSerializable::editModeData().get<ZFSerializable::EditModeData *>(this->editModeWrappedClassName());
        if(editModeData != zfnull)
        {
            for(zfindex i = 0; i < serializableData.elementCount(); ++i)
            {
                const ZFSerializableData &element = serializableData.elementAtIndex(i);
                if(element.editMode())
                {
                    this->editModeWrappedElementDatas().add(element.copy());
                    element.resolveMarkAll();
                }
            }
        }
    }

    // property
    {
        const ZFCoreMap &propertyMap = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializablePropertyMap;
        for(zfindex i = 0; i < serializableData.elementCount(); ++i)
        {
            const ZFSerializableData &element = serializableData.elementAtIndex(i);
            if(element.resolved() || element.category() != zfnull)
            {
                continue;
            }
            const zfchar *propertyName = ZFSerializableUtil::checkPropertyName(element);
            if(propertyName == zfnull)
            {
                continue;
            }
            _ZFP_ZFSerializable_PropertyTypeData *data = propertyMap.get<_ZFP_ZFSerializable_PropertyTypeData *>(propertyName);
            if(data == zfnull)
            {
                continue;
            }

            // serialize the property
            switch(data->propertyType)
            {
                case ZFSerializablePropertyTypeSerializableProperty:
                    {
                        const zfchar *styleKey = ZFSerializableUtil::checkAttribute(element, ZFSerializableKeyword_styleKey);
                        if(styleKey != zfnull)
                        {
                            if(styleable == zfnull)
                            {
                                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                                    zfText("styleKey exists in property %s but the object is not ZFStyleable: %s"),
                                    data->property->objectInfo().cString(),
                                    this->toObject()->objectInfoOfInstance().cString());
                                return zffalse;
                            }
                            if(styleable->styleKeySet(data->property, styleKey))
                            {
                                break;
                            }
                            else
                            {
                                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                                    zfText("unable to apply style for property %s from styleKey: %s"),
                                    data->property->objectInfo().cString(),
                                    styleKey);
                                return zffalse;
                            }
                        }
                    }
                    if(!this->serializableOnSerializePropertyFromData(
                        element,
                        data->property,
                        outErrorHint,
                        outErrorPos))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeEmbededProperty:
                    if(!this->serializableOnSerializeEmbededPropertyFromData(
                        element,
                        data->property,
                        outErrorHint,
                        outErrorPos))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeNotSerializable:
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zffalse;
            }
        }
    }

    // subclass
    if(!this->serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    // mark self as resolved
    serializableData.resolveMark();

    // check whether all resoved
    if(ZFSerializableDataResolveCheckEnable)
    {
        zfstring tmp;
        if(ZFSerializableUtil::printResolveStatus(serializableData, ZFOutputCallbackForString(tmp)))
        {
            #if ZF_ENV_DEBUG
                zfCoreCriticalMessageTrim(zfTextA("%s"), zfsCoreZ2A(tmp.cString()));
            #endif
        }
    }

    return zftrue;
}
zfbool ZFSerializable::serializeToData(ZF_OUT ZFSerializableData &serializableData,
                                       ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                       ZF_IN_OPT ZFSerializable *referencedOwnerOrNull /* = zfnull */)
{
    ZFSerializable *referencedObject = zfnull;
    ZFStyleable *styleable = ZFCastZFObject(ZFStyleable *, this);
    if(styleable != zfnull && styleable->styleKey() != zfnull)
    {
        referencedObject = ZFCastZFObject(ZFSerializable *, ZFStyleGet(styleable->styleKey()));
    }
    if(referencedObject == zfnull)
    {
        referencedObject = referencedOwnerOrNull;
    }

    if(referencedObject != zfnull
        && !referencedObject->classData()->classIsTypeOf(this->classData())
        && !this->classData()->classIsTypeOf(referencedObject->classData()))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("serialize with a reference style object whose type mismatch, self: %s, style: %s"),
            this->classData()->className(), referencedObject->classData()->className());
        return zffalse;
    }

    // property
    {
        const ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> &allProperty = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
        for(zfindex i = 0; i < allProperty.count(); ++i)
        {
            _ZFP_ZFSerializable_PropertyTypeData *data = allProperty[i];
            ZFSerializableData propertyData;

            switch(data->propertyType)
            {
                case ZFSerializablePropertyTypeSerializableProperty:
                    if(styleable != zfnull)
                    {
                        const zfchar *styleKey = styleable->styleKey(data->property);
                        if(styleKey != zfnull)
                        {
                            propertyData.itemClassSet(data->property->propertyTypeId());
                            propertyData.attributeSet(ZFSerializableKeyword_styleKey, styleKey);
                            break;
                        }
                    }
                    if(!this->serializableOnSerializePropertyToData(propertyData,
                                                                    data->property,
                                                                    referencedObject,
                                                                    outErrorHint))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeEmbededProperty:
                    if(!this->serializableOnSerializeEmbededPropertyToData(propertyData,
                                                                           data->property,
                                                                           referencedObject,
                                                                           outErrorHint))
                    {
                        return zffalse;
                    }
                    break;
                case ZFSerializablePropertyTypeNotSerializable:
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zffalse;
            }

            if(propertyData.itemClass() != zfnull)
            {
                propertyData.propertyNameSet(data->property->propertyName());
                serializableData.elementAdd(propertyData);
            }
        }
    }

    // subclass
    if(!this->serializableOnSerializeToData(serializableData, referencedObject, outErrorHint))
    {
        return zffalse;
    }

    if(ZFSerializable::editMode() && this->editModeWrappedClassName() != zfnull)
    {
        serializableData.itemClassSet(this->editModeWrappedClassName());
    }
    else
    {
        serializableData.itemClassSet(this->classData()->className());
    }

    if(ZFSerializable::editMode() && d != zfnull)
    {
        for(zfindex i = 0; i < d->editModeWrappedElementDatas.count(); ++i)
        {
            serializableData.elementAdd(d->editModeWrappedElementDatas.get(i));
        }
    }

    return zftrue;
}

_ZFP_I_ZFSerializablePropertyTypeHolder *ZFSerializable::_ZFP_ZFSerializable_getPropertyTypeHolder(void)
{
    zfCoreMutexLocker();
    _ZFP_I_ZFSerializablePropertyTypeHolder *holder = this->classData()->classTagGet<_ZFP_I_ZFSerializablePropertyTypeHolder *>(_ZFP_I_ZFSerializablePropertyTypeHolder::ClassData()->className());
    if(holder == zfnull)
    {
        zflockfree_zfblockedAlloc(_ZFP_I_ZFSerializablePropertyTypeHolder, holderTmp);
        ZFCoreArrayPOD<const ZFProperty *> allProperty;
        {
            // the order affects the serialization step's order,
            // sort parent's property first for human-readable
            ZFCoreArrayPOD<const ZFClass *> allClass;
            const ZFClass *tmpCls = this->classData();
            allClass.add(tmpCls);
            do
            {
                for(zfindex i = tmpCls->implementedInterfaceCount() - 1; i != zfindexMax(); --i)
                {
                    allClass.add(tmpCls->implementedInterfaceAtIndex(i));
                }
                if(tmpCls->parentClass() != zfnull)
                {
                    allClass.add(tmpCls->parentClass());
                }
                tmpCls = tmpCls->parentClass();
            } while(tmpCls != zfnull);
            for(zfindex i = allClass.count() - 1; i != zfindexMax(); --i)
            {
                tmpCls = allClass[i];
                for(zfindex iProperty = 0; iProperty < tmpCls->propertyCount(); ++iProperty)
                {
                    allProperty.add(tmpCls->propertyAtIndex(iProperty));
                }
            }
        }

        for(zfindex i = 0; i < allProperty.count(); ++i)
        {
            const ZFProperty *property = allProperty[i];
            switch(this->serializableOnCheckPropertyType(property))
            {
                case ZFSerializablePropertyTypeNotSerializable:
                    break;
                case ZFSerializablePropertyTypeSerializableProperty:
                    holderTmp->addData(property, ZFSerializablePropertyTypeSerializableProperty);
                    break;
                case ZFSerializablePropertyTypeEmbededProperty:
                    holderTmp->addData(property, ZFSerializablePropertyTypeEmbededProperty);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zfnull;
            }
        }

        holder = holderTmp;
        this->classData()->classTagSet(
            _ZFP_I_ZFSerializablePropertyTypeHolder::ClassData()->className(),
            holderTmp);
        this->classData()->classDataChangeAutoRemoveTagAdd(_ZFP_I_ZFSerializablePropertyTypeHolder::ClassData()->className());
    }
    return holder;
}
void ZFSerializable::serializableGetAllSerializablePropertyT(ZF_OUT ZFCoreArray<const ZFProperty *> &ret)
{
    const ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> &tmp = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->propertyType == ZFSerializablePropertyTypeSerializableProperty)
        {
            ret.add(tmp[i]->property);
        }
    }
}
void ZFSerializable::serializableGetAllSerializableEmbededPropertyT(ZF_OUT ZFCoreArray<const ZFProperty *> &ret)
{
    const ZFCoreArrayPOD<_ZFP_ZFSerializable_PropertyTypeData *> &tmp = this->_ZFP_ZFSerializable_getPropertyTypeHolder()->serializableProperty;
    for(zfindex i = 0; i < tmp.count(); ++i)
    {
        if(tmp[i]->propertyType == ZFSerializablePropertyTypeEmbededProperty)
        {
            ret.add(tmp[i]->property);
        }
    }
}

ZFSerializablePropertyType ZFSerializable::serializableOnCheckPropertyType(ZF_IN const ZFProperty *property)
{
    if(property->propertyIsRetainProperty())
    {
        if(property->setterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
        {
            if(property->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate
                || !property->propertyClassOfRetainProperty()->classIsTypeOf(ZFSerializable::ClassData()))
            {
                return ZFSerializablePropertyTypeNotSerializable;
            }
            else
            {
                return ZFSerializablePropertyTypeEmbededProperty;
            }
        }
        else
        {
            if(property->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
            {
                return ZFSerializablePropertyTypeNotSerializable;
            }
            else
            {
                return ZFSerializablePropertyTypeSerializableProperty;
            }
        }
    }
    else
    {
        if(!property->propertySerializable()
            || property->getterMethod()->methodPrivilegeType() == ZFMethodPrivilegeTypePrivate)
        {
            return ZFSerializablePropertyTypeNotSerializable;
        }
        else
        {
            return ZFSerializablePropertyTypeSerializableProperty;
        }
    }
}

zfbool ZFSerializable::serializableOnSerializePropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                               ZF_IN const ZFProperty *property,
                                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(property->propertyIsRetainProperty())
    {
        zfautoObject obj;
        if(!ZFObjectFromData(obj, propertyData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        if(obj != zfnull && !obj.toObject()->classData()->classIsTypeOf(property->propertyClassOfRetainProperty()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, propertyData,
                zfText("object %s not type of %s"),
                obj.toObject()->objectInfoOfInstance().cString(), property->propertyClassOfRetainProperty()->className());
            return zffalse;
        }
        property->callbackValueSet(property, this->toObject(), &obj);
        return zftrue;
    }

    ZFPropertyTypeSerializeFromCallback serializeFromCallback = ZFPropertyTypeGetSerializeFromCallback(propertyData.itemClass());
    if(serializeFromCallback == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, propertyData,
            zfText("type \"%s\" not registered while serializing \"%s\""),
            propertyData.itemClass(),
            this->classData()->className());
        return zffalse;
    }
    if(!serializeFromCallback(property, this->toObject(), propertyData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    return zftrue;
}
zfbool ZFSerializable::serializableOnSerializePropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                             ZF_IN const ZFProperty *property,
                                                             ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                             ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(referencedOwnerOrNull != zfnull
        && ZFPropertyCompare(property, this->toObject(), referencedOwnerOrNull->toObject()) == ZFCompareTheSame)
    {
        return zftrue;
    }
    if(property->callbackIsInitValue(property, this->toObject(), zfnull))
    {
        return zftrue;
    }

    if(property->propertyIsRetainProperty())
    {
        const zfautoObject *obj = (const zfautoObject *)property->callbackValueGet(property, this->toObject());
        if(obj == zfnull || !ZFObjectToData(propertyData, *obj, outErrorHint))
        {
            return zffalse;
        }
        return zftrue;
    }

    ZFPropertyTypeSerializeToCallback serializeToCallback = ZFPropertyTypeGetSerializeToCallback(property->propertyTypeId());
    if(serializeToCallback == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("type \"%s\" not registered while serializing \"%s\""),
            property->propertyTypeId(),
            this->classData()->className());
        return zffalse;
    }
    if(!serializeToCallback(property, this->toObject(), propertyData, outErrorHint))
    {
        return zffalse;
    }

    return zftrue;
}
zfbool ZFSerializable::serializableOnSerializeEmbededPropertyFromData(ZF_IN const ZFSerializableData &propertyData,
                                                                      ZF_IN const ZFProperty *property,
                                                                      ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                                      ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    const zfautoObject *objHolder = (const zfautoObject *)property->callbackValueGet(property, this->toObject());
    if(objHolder == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("unable to access property %s's value is null while serializing \"%s\""),
            property->propertyName(),
            this->classData()->className());
        return zffalse;
    }
    ZFObject *obj = *objHolder;
    if(obj == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("embeded property %s is null while serializing \"%s\""),
            property->propertyName(),
            this->classData()->className());
        return zffalse;
    }
    else if(!ZFObjectIsSerializable(obj))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("not serializable object %s while serializing \"%s\""),
            obj->objectInfoOfInstance().cString(),
            this->classData()->className());
        return zffalse;
    }
    else
    {
        const ZFClass *cls = ZFClass::classForName(propertyData.itemClass());
        if(cls == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint,
                zfText("no class named %s while serializing \"%s\"'s property %s"),
                propertyData.itemClass(),
                this->classData()->className(),
                property->propertyName());
            return zffalse;
        }
        else if(!cls->classIsTypeOf(property->propertyClassOfRetainProperty()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint,
                zfText("node %s is not type of %s while serializing \"%s\"'s property %s"),
                propertyData.itemClass(),
                property->propertyClassOfRetainProperty()->className(),
                this->classData()->className(),
                property->propertyName());
            return zffalse;
        }
    }
    return ZFCastZFObjectUnchecked(zfself *, obj)->serializeFromData(propertyData, outErrorHint, outErrorPos);
}
zfbool ZFSerializable::serializableOnSerializeEmbededPropertyToData(ZF_OUT ZFSerializableData &propertyData,
                                                                    ZF_IN const ZFProperty *property,
                                                                    ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                                    ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(referencedOwnerOrNull != zfnull
        && ZFPropertyCompare(property, this->toObject(), referencedOwnerOrNull->toObject()) == ZFCompareTheSame)
    {
        return zftrue;
    }
    zfautoObject initValue;
    if(property->callbackIsInitValue(property, this->toObject(), &initValue))
    {
        return zftrue;
    }

    const zfautoObject *obj = (const zfautoObject *)property->callbackValueGet(property, this->toObject());
    if(obj == zfnull || !ZFObjectIsSerializable(*obj))
    {
        return zftrue;
    }

    ZFSerializable *propertyRef = zfnull;
    if(referencedOwnerOrNull != zfnull)
    {
        const zfautoObject *t = (const zfautoObject *)property->callbackValueGet(property, referencedOwnerOrNull->toObject());
        if(t != zfnull)
        {
            propertyRef = t->to<ZFSerializable *>();
        }
    }
    if(propertyRef == zfnull)
    {
        propertyRef = initValue;
    }

    if(!obj->to<zfself *>()->serializeToData(propertyData, outErrorHint, propertyRef))
    {
        return zffalse;
    }

    if(propertyData.elementCount() == 0
        && propertyData.attributeCount() == 0)
    {
        propertyData.itemClassSet(zfnull);
    }
    return zftrue;
}

void ZFSerializable::serializableGetInfoT(ZF_IN_OUT zfstring &ret)
{
    ZFSerializableData serializableData;
    if(!this->serializable()
       || !this->serializeToData(serializableData))
    {
        ret += ZFTOKEN_ZFObjectInfoLeft;
        this->toObject()->objectInfoOfInstanceT(ret);
        ret += zfText(": not serializable");
        ret += ZFTOKEN_ZFObjectInfoRight;
    }
    else
    {
        serializableData.objectInfoT(ret);
    }
}

void ZFSerializable::serializableCopyInfoFrom(ZF_IN ZFSerializable *anotherSerializable)
{
    if(anotherSerializable == zfnull)
    {
        return ;
    }

    if(anotherSerializable->d == zfnull)
    {
        zfdelete(d);
        d = zfnull;
    }
    else
    {
        if(d == zfnull)
        {
            d = zfnew(_ZFP_ZFSerializablePrivate, *(anotherSerializable->d));
        }
        else
        {
            *d = *(anotherSerializable->d);
        }
    }
}

// ============================================================
zfbool ZFObjectIsSerializable(ZF_IN ZFObject *obj)
{
    if(obj == zfnull)
    {
        return zftrue;
    }
    else
    {
        ZFSerializable *tmp = ZFCastZFObject(ZFSerializable *, obj);
        return (tmp != zfnull && tmp->serializable());
    }
}

// ============================================================
zfbool ZFObjectFromString(ZF_OUT zfautoObject &result,
                          ZF_IN const zfchar *encodedData,
                          ZF_IN_OPT zfindex encodedDataLen /* = zfindexMax() */,
                          ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFSerializableData serializableData;
    if(!ZFSerializableDataFromString(serializableData, encodedData, encodedDataLen, outErrorHint))
    {
        return zffalse;
    }
    else
    {
        return ZFObjectFromData(result, serializableData, outErrorHint);
    }
}
zfautoObject ZFObjectFromString(ZF_IN const zfchar *encodedData,
                                ZF_IN_OPT zfindex encodedDataLen /* = zfindexMax() */,
                                ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    zfautoObject result;
    ZFObjectFromString(result, encodedData, encodedDataLen, outErrorHint);
    return result;
}

zfbool ZFObjectToString(ZF_OUT zfstring &encodedData,
                        ZF_IN ZFObject *obj,
                        ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFSerializableData serializableData;
    if(!ZFObjectToData(serializableData, obj, outErrorHint))
    {
        return zffalse;
    }
    else
    {
        return ZFSerializableDataToString(encodedData, serializableData, outErrorHint);
    }
}
zfstring ZFObjectToString(ZF_IN ZFObject *obj,
                          ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    zfstring ret;
    ZFObjectToString(ret, obj, outErrorHint);
    return ret;
}

// ============================================================
zfbool ZFObjectFromInput(ZF_OUT zfautoObject &result,
                         ZF_IN_OUT const ZFInputCallback &input,
                         ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFSerializableData serializableData;
    if(!ZFSerializableDataFromInput(serializableData, input, outErrorHint))
    {
        return zffalse;
    }
    else
    {
        return ZFObjectFromData(result, serializableData, outErrorHint);
    }
}
zfautoObject ZFObjectFromInput(ZF_IN_OUT const ZFInputCallback &input,
                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    zfautoObject ret;
    ZFObjectFromInput(ret, input, outErrorHint);
    return ret;
}
zfbool ZFObjectToOutput(ZF_IN_OUT const ZFOutputCallback &output,
                        ZF_IN ZFObject *obj,
                        ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    ZFSerializableData serializableData;
    if(!ZFObjectToData(serializableData, obj, outErrorHint))
    {
        return zffalse;
    }
    else
    {
        return ZFSerializableDataToOutput(output, serializableData, outErrorHint);
    }
}

// ============================================================
zfbool ZFObjectFromData(ZF_OUT zfautoObject &result,
                        ZF_IN const ZFSerializableData &serializableData,
                        ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                        ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    result = zfnull;

    const zfchar *serializableClass = ZFSerializableUtil::requireSerializableClass(ZFPropertyTypeId_none, serializableData, outErrorHint, outErrorPos);
    if(serializableClass == zfnull)
    {
        return zffalse;
    }
    if(zfscmpTheSame(serializableClass, ZFSerializableKeyword_null))
    {
        serializableData.resolveMark();
        return zftrue;
    }

    const ZFClass *cls = ZFClass::classForName(serializableClass);
    zfbool editModeWrapped = zffalse;
    if(cls == zfnull)
    {
        if(ZFSerializable::editMode())
        {
            ZFSerializable::EditModeData *editModeData = ZFSerializable::editModeData().get<ZFSerializable::EditModeData *>(serializableClass);
            if(editModeData != zfnull)
            {
                editModeWrapped = zftrue;
                cls = editModeData->wrappedClass;
            }
        }
        if(cls == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                zfText("no class named \"%s\""),
                serializableData.itemClass());
            return zffalse;
        }
    }
    zfautoObject obj;
    {
        const ZFMethod *overridedCreateMethod = cls->methodForNameIgnoreParent(ZFSerializableKeyword_serializableNewInstance);
        if(overridedCreateMethod != zfnull)
        {
            obj = overridedCreateMethod->executeStatic<zfautoObject>();
        }
        else
        {
            obj = cls->newInstance();
        }
    }
    ZFSerializable *tmp = ZFCastZFObject(ZFSerializable *, obj.toObject());
    if(tmp == zfnull || !tmp->serializable())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            zfText("object %s not serializable"),
            ZFObjectInfoOfInstance(obj.toObject()).cString());
        return zffalse;
    }
    if(editModeWrapped)
    {
        tmp->editModeWrappedClassNameSet(serializableClass);
    }
    if(!tmp->serializeFromData(serializableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    result = obj;
    return zftrue;
}
zfautoObject ZFObjectFromData(ZF_IN const ZFSerializableData &serializableData,
                              ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                              ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    zfautoObject result;
    ZFObjectFromData(result, serializableData, outErrorHint, outErrorPos);
    return result;
}
zfbool ZFObjectToData(ZF_OUT ZFSerializableData &serializableData,
                      ZF_IN ZFObject *obj,
                      ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                      ZF_IN_OPT ZFSerializable *referencedOwnerOrNull /* = zfnull */)
{
    if(obj == zfnull)
    {
        serializableData.itemClassSet(ZFSerializableKeyword_null);
        return zftrue;
    }
    ZFSerializable *tmp = ZFCastZFObject(ZFSerializable *, obj);
    if(tmp == zfnull || !tmp->serializable())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint,
            zfText("object %s not serializable"),
            obj->objectInfoOfInstance().cString());
        return zffalse;
    }
    return tmp->serializeToData(serializableData, outErrorHint, referencedOwnerOrNull);
}
ZFSerializableData ZFObjectToData(ZF_IN ZFObject *obj,
                                  ZF_OUT_OPT zfbool *outSuccess /* = zfnull */,
                                  ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                  ZF_IN_OPT ZFSerializable *referencedOwnerOrNull /* = zfnull */)
{
    ZFSerializableData serializableData;
    zfbool success = ZFObjectToData(serializableData, obj, outErrorHint, referencedOwnerOrNull);
    if(outSuccess != zfnull)
    {
        *outSuccess = success;
    }
    if(success)
    {
        return serializableData;
    }
    return ZFSerializableData();
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, zfbool, serializable)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFSerializable, zfbool, serializeFromData, ZFMP_IN(const ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_3(ZFSerializable, zfbool, serializeToData, ZFMP_OUT(ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_IN_OPT(ZFSerializable *, referencedOwnerOrNull, zfnull))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableGetAllSerializablePropertyT, ZFMP_OUT(ZFCoreArray<const ZFProperty *> &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, ZFCoreArrayPOD<const ZFProperty *>, serializableGetAllSerializableProperty)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableGetAllSerializableEmbededPropertyT, ZFMP_OUT(ZFCoreArray<const ZFProperty *> &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, ZFCoreArrayPOD<const ZFProperty *>, serializableGetAllSerializableEmbededProperty)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableGetInfoT, ZFMP_IN_OUT(zfstring &, ret))
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFSerializable, zfstring, serializableGetInfo)
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFSerializable, void, serializableCopyInfoFrom, ZFMP_IN(ZFSerializable *, anotherSerializable))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(zfbool, ZFObjectIsSerializable, ZFMP_IN(ZFObject *, obj))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFObjectFromString, ZFMP_OUT(zfautoObject &, result), ZFMP_IN(const zfchar *, encodedData), ZFMP_IN_OPT(zfindex, encodedDataLen, zfindexMax()), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfautoObject, ZFObjectFromString, ZFMP_IN(const zfchar *, encodedData), ZFMP_IN_OPT(zfindex, encodedDataLen, zfindexMax()), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectToString, ZFMP_OUT(zfstring &, encodedData), ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfstring, ZFObjectToString, ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectFromInput, ZFMP_OUT(zfautoObject &, result), ZFMP_IN_OUT(const ZFInputCallback &, input), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(zfautoObject, ZFObjectFromInput, ZFMP_IN_OUT(const ZFInputCallback &, input), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfbool, ZFObjectToOutput, ZFMP_IN_OUT(const ZFOutputCallback &, output), ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(zfautoObject, ZFObjectFromData, ZFMP_IN(const ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFObjectFromData, ZFMP_OUT(zfautoObject &, result), ZFMP_IN(const ZFSerializableData &, serializableData), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(zfbool, ZFObjectToData, ZFMP_OUT(ZFSerializableData &, serializableData), ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_IN_OPT(ZFSerializable *, referencedOwnerOrNull, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(ZFSerializableData, ZFObjectToData, ZFMP_IN(ZFObject *, obj), ZFMP_OUT_OPT(zfbool *, outSuccess, zfnull), ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_IN_OPT(ZFSerializable *, referencedOwnerOrNull, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif

