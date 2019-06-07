/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFPropertyDynamicRegister.h"
#include "ZFObjectImpl.h"
#include "ZFListenerDeclare.h"
#include "ZFPropertyCallbackDefaultImpl.h"

#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFPropertyDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFPropertyDynamicRegisterAutoRemove)
{
    zfstlmap<const ZFProperty *, zfbool> t;
    t.swap(m);
    for(zfstlmap<const ZFProperty *, zfbool>::iterator it = t.begin(); it != t.end(); ++it)
    {
        _ZFP_ZFPropertyUnregister(it->first);
    }
}
zfstlmap<const ZFProperty *, zfbool> m;
ZF_GLOBAL_INITIALIZER_END(ZFPropertyDynamicRegisterAutoRemove)

// ============================================================
zfclass _ZFP_I_PropDynRetainHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_PropDynRetainHolder, ZFObject)
public:
    zfautoObject zfv;

public:
    zfoverride
    ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj)
    {
        return ZFObjectCompare(this->zfv.toObject(), anotherObj);
    }
};
zfclass _ZFP_I_PropDynRegData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_PropDynRegData, ZFObject)

public:
    const ZFTypeIdBase *d; // null for retain property
    ZFPropertyDynamicRegisterInitValueCallback initValueCallback;
    /*
     * for assign property, store ZFTypeIdWrapper, ensured not null if accessed
     * for retain property, store _ZFP_I_PropDynRetainHolder, ensured not null if accessed
     */
    zfstring tagKey;
    zfstring valueStoreKey;
public:
    void objectAttach(ZF_IN ZFObject *obj)
    {
        if(this->_objAttached.find(obj) == this->_objAttached.end())
        {
            obj->observerAdd(ZFObject::EventObjectBeforeDealloc(), this->_objOnDeallocListener);
        }
        this->_objAttached[obj] = zftrue;
    }
    void objectDetach(void)
    {
        zfstlmap<ZFObject *, zfbool> t;
        t.swap(this->_objAttached);
        for(zfstlmap<ZFObject *, zfbool>::iterator it = t.begin(); it != t.end(); ++it)
        {
            it->first->observerRemove(ZFObject::EventObjectBeforeDealloc(), this->_objOnDeallocListener);
            it->first->tagRemove(this->tagKey);
            it->first->tagRemove(this->valueStoreKey);
        }
    }
    /*
     * if success, ret ensured:
     * * store ZFTypeIdWrapper for assign property
     * * store _ZFP_I_PropDynRetainHolder for retain property
     *   or typeIdWrapper return null with success state
     */
    zfbool initValue(ZF_OUT zfautoObject &ret,
                     ZF_IN const ZFProperty *property,
                     ZF_OUT_OPT zfstring *errorHint = zfnull)
    {
        if(property->propertyIsRetainProperty())
        {
            zfblockedAlloc(_ZFP_I_PropDynRetainHolder, holder);
            if(this->initValueCallback != zfnull)
            {
                holder->zfv = this->initValueCallback(property);
            }
            if(holder->zfv != zfnull && !holder->zfv.toObject()->classData()->classIsTypeOf(property->propertyClassOfRetainProperty()))
            {
                zfstringAppend(errorHint,
                    "invalid init value %s, desired: %s",
                    ZFObjectInfo(holder->zfv.toObject()).cString(),
                    property->propertyClassOfRetainProperty()->classNameFull());
                return zffalse;
            }
            ret = holder;
        }
        else
        {
            if(this->initValueCallback != zfnull)
            {
                ret = this->initValueCallback(property);
            }
            if(ret == zfnull)
            {
                this->d->typeIdWrapper(ret);
                if(ret == zfnull)
                {
                    zfblockedAlloc(_ZFP_I_PropDynRetainHolder, holder);
                    ret = holder;
                    return zftrue;
                }
            }
            ZFTypeIdWrapper *wrapper = ret;
            if(wrapper == zfnull)
            {
                zfstringAppend(errorHint,
                    "invalid init value %s, desired: %s",
                    ZFObjectInfo(ret.toObject()).cString(),
                    property->propertyTypeId());
                return zffalse;
            }
        }
        return zftrue;
    }
private:
    zfstlmap<ZFObject *, zfbool> _objAttached;
    ZFListener _objOnDeallocListener;
    ZFLISTENER_INLINE(_objOnDealloc)
    {
        this->_objAttached.erase(listenerData.sender);
    }
protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->_objOnDeallocListener = ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, _objOnDealloc));
    }
};
zfclass _ZFP_I_PropDynRegValueStore : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_PropDynRegValueStore, ZFObject)

public:
    zfstlmap<void *, zfautoObject> m;
};

// ============================================================
static zfbool _ZFP_PropDynReg_setterGI(ZFMETHOD_GENERIC_INVOKER_PARAMS)
{
    const ZFProperty *property = invokerMethod->methodOwnerProperty();
    _ZFP_I_PropDynRegData *d = ZFCastZFObject(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
    zfautoObject valueOld = invokerObject->tagGet(d->tagKey);

    zfautoObject value;
    zfautoObject &valueNew = paramList[0];
    do {
        if(property->propertyIsRetainProperty())
        {
            if(valueNew != zfnull && !valueNew.toObject()->classData()->classIsTypeOf(property->propertyClassOfRetainProperty()))
            {
                break;
            }
            zfblockedAlloc(_ZFP_I_PropDynRetainHolder, holder);
            holder->zfv = valueNew;
            value = holder;
        }
        else
        {
            if(d->d->typeIdWrapper(value) && value == zfnull)
            {
                zfblockedAlloc(_ZFP_I_PropDynRetainHolder, holder);
                holder->zfv = valueNew;
                value = holder;
            }
            else
            {
                ZFTypeIdWrapper *wrapper = ZFCastZFObject(ZFTypeIdWrapper *, valueNew);
                if(wrapper == zfnull
                    || !zfscmpTheSame(wrapper->wrappedValueTypeId(), property->propertyTypeId()))
                {
                    break;
                }
                value = valueNew;
            }
        }
    } while(zffalse);
    if(value == zfnull)
    {
        zfstringAppend(errorHint, "invalid value: %s, desired: %s",
            ZFObjectInfo(valueNew).cString(),
            property->propertyIsRetainProperty()
                ? property->propertyClassOfRetainProperty()->classNameFull()
                : property->propertyTypeId());
        return zffalse;
    }
    if(valueOld == zfnull)
    {
        d->objectAttach(invokerObject);
    }
    invokerObject->tagSet(d->tagKey, value);
    if(valueOld == zfnull)
    {
        invokerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
    }
    else
    {
        _ZFP_I_PropDynRetainHolder *holder = valueOld;
        if(holder != zfnull)
        {
            invokerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, &(holder->zfv));
        }
        else
        {
            invokerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property,
                    ZFCastZFObject(ZFTypeIdWrapper *, valueOld)->wrappedValue()
                );
        }
    }
    return zftrue;
}
static zfbool _ZFP_PropDynReg_getterGI(ZFMETHOD_GENERIC_INVOKER_PARAMS)
{
    // ZFTAG_LIMITATION: property's getter would return value reference,
    //   we would store the value to owner object's tag,
    //   see #ZFMethodDynamicRegister for the reason for this behavior
    const ZFProperty *property = invokerMethod->methodOwnerProperty();
    _ZFP_I_PropDynRegData *d = ZFCastZFObject(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
    ZFObject *wrapper = invokerObject->tagGet(d->tagKey);
    zfbool firstTime = (wrapper == zfnull);
    if(firstTime)
    {
        zfautoObject initValue;
        if(!d->initValue(initValue, property, errorHint))
        {
            return zffalse;
        }
        wrapper = initValue;
        invokerObject->tagSet(d->tagKey, initValue);
    }
    _ZFP_I_PropDynRetainHolder *wrapperTmp = ZFCastZFObject(_ZFP_I_PropDynRetainHolder *, wrapper);
    if(wrapperTmp != zfnull)
    {
        ret = wrapperTmp->zfv;
    }
    else
    {
        ret = wrapper;
    }
    if(firstTime)
    {
        d->objectAttach(invokerObject);
        invokerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
    }
    return zftrue;
}

static zfbool _ZFP_PropDynReg_callbackIsValueAccessed(ZF_IN const ZFProperty *property,
                                                      ZF_IN ZFObject *ownerObj)
{
    _ZFP_I_PropDynRegData *d = ZFCastZFObject(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
    return (ownerObj->tagGet(d->tagKey) != zfnull);
}
static zfbool _ZFP_PropDynReg_callbackIsInitValue(ZF_IN const ZFProperty *property,
                                                  ZF_IN ZFObject *ownerObj,
                                                  ZF_OUT_OPT void *outInitValue /* = zfnull */)
{
    _ZFP_I_PropDynRegData *d = ZFCastZFObject(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
    zfbool ret = zffalse;
    ZFObject *tag = ownerObj->tagGet(d->tagKey);
    if(tag == zfnull)
    {
        ret = zftrue;
        if(outInitValue != zfnull)
        {
            zfautoObject initValue;
            if(d->initValue(initValue, property))
            {
                _ZFP_I_PropDynRetainHolder *wrapperTmp = initValue;
                if(wrapperTmp != zfnull)
                {
                    *(zfautoObject *)outInitValue = wrapperTmp->zfv;
                }
                else
                {
                    initValue.to<ZFTypeIdWrapper *>()->wrappedValueGet(outInitValue);
                }
            }
        }
    }
    else
    {
        zfautoObject initValue;
        if(d->initValue(initValue, property))
        {
            _ZFP_I_PropDynRetainHolder *wrapperTmp = initValue;
            if(wrapperTmp != zfnull)
            {
                if(outInitValue != zfnull)
                {
                    *(zfautoObject *)outInitValue = wrapperTmp->zfv;
                }
            }
            else
            {
                if(outInitValue != zfnull)
                {
                    initValue.to<ZFTypeIdWrapper *>()->wrappedValueGet(outInitValue);
                }
            }
            ret = (tag->objectCompare(initValue) == ZFCompareTheSame);
        }
    }
    return ret;
}
static void _ZFP_PropDynReg_callbackValueReset(ZF_IN const ZFProperty *property,
                                               ZF_IN ZFObject *ownerObj)
{
    _ZFP_I_PropDynRegData *d = ZFCastZFObject(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
    ownerObj->tagRemove(d->tagKey);
}

// ============================================================
static zfbool _ZFP_ZFPropertyDynamicRegisterCustomImplCheck(ZF_IN const ZFPropertyDynamicRegisterParam &param,
                                                            ZF_OUT_OPT zfstring *errorHint = zfnull);
const ZFProperty *ZFPropertyDynamicRegister(ZF_IN const ZFPropertyDynamicRegisterParam &param,
                                            ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(param.propertyOwnerClass() == zfnull)
    {
        zfstringAppend(errorHint, "propertyOwnerClass not set");
        return zfnull;
    }
    if(param.propertyTypeId() == zfnull)
    {
        zfstringAppend(errorHint, "propertyTypeId not set");
        return zfnull;
    }
    if(param.propertyName() == zfnull)
    {
        zfstringAppend(errorHint, "propertyName not set");
        return zfnull;
    }
    if(param.propertyClassOfRetainProperty() == zfnull)
    {
        if(ZFClass::classForName(param.propertyTypeId()) != zfnull)
        {
            zfstringAppend(errorHint,
                "propertyTypeId %s is ZFObject type but propertyClassOfRetainProperty not set, weak property not supported",
                param.propertyTypeId());
            return zfnull;
        }
    }
    else
    {
        if(!zfscmpTheSame(param.propertyTypeId(), param.propertyClassOfRetainProperty()->classNameFull()))
        {
            zfstringAppend(errorHint,
                "propertyTypeId must be same as propertyClassOfRetainProperty for retain property");
            return zfnull;
        }
    }

    const ZFTypeIdBase *d = zfnull;
    if(param.propertyClassOfRetainProperty() == zfnull)
    {
        d = ZFTypeIdGet(param.propertyTypeId());
        if(d == zfnull)
        {
            zfstringAppend(errorHint,
                "propertyTypeId %s not registered",
                param.propertyTypeId());
            return zfnull;
        }
    }

    const ZFProperty *existProperty = param.propertyOwnerClass()->propertyForName(param.propertyName());
    if(existProperty != zfnull)
    {
        zfstringAppend(errorHint,
            "property with same name already exists: %s",
            existProperty->objectInfo().cString());
        return zfnull;
    }

    zfblockedAlloc(_ZFP_I_PropDynRegData, userDataWrapper);
    userDataWrapper->d = d;
    userDataWrapper->initValueCallback = param.propertyInitValueCallback();
    zfstringAppend(userDataWrapper->tagKey, "PropDyn_%s", param.propertyName());
    zfstringAppend(userDataWrapper->valueStoreKey, "PropDynV_%s", param.propertyName());
    const ZFProperty *property = zfnull;

    if(param.propertyCustomImplSetterMethod() != zfnull
        || param.propertyCustomImplGetterMethod() != zfnull)
    {
        if(!_ZFP_ZFPropertyDynamicRegisterCustomImplCheck(param, errorHint))
        {
            return zfnull;
        }
        property = _ZFP_ZFPropertyRegister(zffalse
            , zftrue
            , param.propertyDynamicRegisterUserData()
            , param.propertyOwnerClass()
            , param.propertyName()
            , param.propertyTypeName()
            , param.propertyTypeId()
            , param.propertyCustomImplSetterMethod()
            , param.propertyCustomImplGetterMethod()
            , param.propertyClassOfRetainProperty()
            , param.propertyCustomImplCallbackIsValueAccessed()
            , param.propertyCustomImplCallbackIsInitValue()
            , param.propertyCustomImplCallbackValueReset()
            , param.propertyCustomImplCallbackValueSet() ? param.propertyCustomImplCallbackValueSet() : _ZFP_propCbDValueSet_generic
            , param.propertyCustomImplCallbackValueGet() ? param.propertyCustomImplCallbackValueGet() : _ZFP_propCbDValueGet_generic
            , param.propertyCustomImplCallbackValueGetRelease() ? param.propertyCustomImplCallbackValueGetRelease() : _ZFP_propCbDValueGetRelease_generic
            , param.propertyCustomImplCallbackCompare() ? param.propertyCustomImplCallbackCompare() : _ZFP_propCbDCompare_generic
            , param.propertyCustomImplCallbackGetInfo() ? param.propertyCustomImplCallbackGetInfo() : _ZFP_propCbDGetInfo_generic
            , param.propertyCustomImplCallbackValueStore() ? param.propertyCustomImplCallbackValueStore() : _ZFP_propCbDValueStore_generic
            , param.propertyCustomImplCallbackValueRelease() ? param.propertyCustomImplCallbackValueRelease() : _ZFP_propCbDValueRelease_generic
            , _ZFP_propCbDSerializeFrom_generic
            , _ZFP_propCbDSerializeTo_generic
            , param.propertyCustomImplCallbackProgressUpdate() ? param.propertyCustomImplCallbackProgressUpdate() : _ZFP_propCbDProgressUpdate_generic
            , zfnull
            , zfnull
            );
        property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFProperty_propertyIsDynamicRegisterWithCustomImpl = zftrue;
    }
    else
    {
        zfstring errorHintTmp;
        const ZFMethod *setterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .methodDynamicRegisterUserDataSet(userDataWrapper)
                .methodOwnerClassSet(param.propertyOwnerClass())
                .methodGenericInvokerSet(_ZFP_PropDynReg_setterGI)
                .methodTypeSet(ZFMethodTypeVirtual)
                .methodPrivilegeTypeSet(param.propertySetterType())
                .methodNameSet(zfstringWithFormat("%sSet", param.propertyName()))
                .methodReturnTypeIdSet(ZFTypeId_void())
                .methodReturnTypeNameSet("void")
                .methodParamAdd(
                    param.propertyTypeId(),
                    zfstringWithFormat("%s const &", param.propertyTypeName()),
                    zfnull)
            , &errorHintTmp);
        if(setterMethod == zfnull)
        {
            zfstringAppend(errorHint,
                "failed to register setter method, reason: %s",
                errorHintTmp.cString());
            return zfnull;
        }
        const ZFMethod *getterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .methodDynamicRegisterUserDataSet(userDataWrapper)
                .methodOwnerClassSet(param.propertyOwnerClass())
                .methodGenericInvokerSet(_ZFP_PropDynReg_getterGI)
                .methodTypeSet(ZFMethodTypeVirtual)
                .methodPrivilegeTypeSet(param.propertyGetterType())
                .methodNameSet(param.propertyName())
                .methodReturnTypeIdSet(param.propertyTypeId())
                .methodReturnTypeNameSet(zfstringWithFormat("%s const &", param.propertyTypeName()))
            , &errorHintTmp);
        if(getterMethod == zfnull)
        {
            zfstringAppend(errorHint,
                "failed to register getter method, reason: %s",
                errorHintTmp.cString());
            ZFMethodDynamicUnregister(setterMethod);
            return zfnull;
        }

        property = _ZFP_ZFPropertyRegister(zffalse
            , zftrue
            , param.propertyDynamicRegisterUserData()
            , param.propertyOwnerClass()
            , param.propertyName()
            , param.propertyTypeName()
            , param.propertyTypeId()
            , setterMethod
            , getterMethod
            , param.propertyClassOfRetainProperty()
            , _ZFP_PropDynReg_callbackIsValueAccessed
            , _ZFP_PropDynReg_callbackIsInitValue
            , _ZFP_PropDynReg_callbackValueReset
            , _ZFP_propCbDValueSet_generic
            , _ZFP_propCbDValueGet_generic
            , _ZFP_propCbDValueGetRelease_generic
            , _ZFP_propCbDCompare_generic
            , _ZFP_propCbDGetInfo_generic
            , _ZFP_propCbDValueStore_generic
            , _ZFP_propCbDValueRelease_generic
            , _ZFP_propCbDSerializeFrom_generic
            , _ZFP_propCbDSerializeTo_generic
            , _ZFP_propCbDProgressUpdate_generic
            , zfnull
            , zfnull
            );
    }

    property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper = zfRetain(userDataWrapper);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyDynamicRegisterAutoRemove)->m[property] = zftrue;
    return property;
}
void ZFPropertyDynamicUnregister(ZF_IN const ZFProperty *property)
{
    if(property != zfnull)
    {
        zfCoreAssert(property->propertyIsDynamicRegister());
        zfCoreMutexLocker();
        zfblockedRelease(property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFPropertyDynamicRegisterAutoRemove)->m.erase(property);
        _ZFP_I_PropDynRegData *d = ZFCastZFObject(_ZFP_I_PropDynRegData *, property->_ZFP_ZFProperty_propertyDynamicRegisterUserDataWrapper);
        d->objectDetach();
        _ZFP_ZFPropertyUnregister(property);
    }
}

static zfbool _ZFP_ZFPropertyDynamicRegisterCustomImplCheck(ZF_IN const ZFPropertyDynamicRegisterParam &param,
                                                            ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(param.propertyCustomImplSetterMethod() == zfnull)
    {
        zfstringAppend(errorHint, "propertyCustomImplSetterMethod not set");
        return zffalse;
    }
    if(param.propertyCustomImplGetterMethod() == zfnull)
    {
        zfstringAppend(errorHint, "propertyCustomImplGetterMethod not set");
        return zffalse;
    }

    if(zfsncmp(param.propertyCustomImplSetterMethod()->methodName(), param.propertyName(), zfslen(param.propertyName())) != 0)
    {
        zfstringAppend(errorHint,
            "setter method name \"%s\" does not match property name \"%s\"",
            param.propertyCustomImplSetterMethod()->methodName(),
            param.propertyName());
        return zffalse;
    }
    if(param.propertyCustomImplSetterMethod()->methodParamCount() != 1
        || !zfscmpTheSame(param.propertyCustomImplSetterMethod()->methodReturnTypeId(), ZFTypeId_void())
        || !zfscmpTheSame(param.propertyCustomImplSetterMethod()->methodParamTypeIdAtIndex(0), param.propertyTypeId())
        )
    {
        zfstringAppend(errorHint,
            "setter method signature mismatch: %s, desired: void setter(%s const &)",
            param.propertyCustomImplSetterMethod()->objectInfo().cString(),
            param.propertyTypeId());
        return zffalse;
    }

    if(!zfscmpTheSame(param.propertyCustomImplGetterMethod()->methodName(), param.propertyName()))
    {
        zfstringAppend(errorHint,
            "getter method name \"%s\" does not match property name \"%s\"",
            param.propertyCustomImplGetterMethod()->methodName(),
            param.propertyName());
        return zffalse;
    }
    if(param.propertyCustomImplGetterMethod()->methodParamCount() != 0
        || !zfscmpTheSame(param.propertyCustomImplGetterMethod()->methodReturnTypeId(), param.propertyTypeId())
        )
    {
        zfstringAppend(errorHint,
            "getter method signature mismatch: %s, desired: %s const &getter(void)",
            param.propertyCustomImplGetterMethod()->objectInfo().cString(),
            param.propertyTypeId());
        return zffalse;
    }

    if(param.propertyCustomImplCallbackIsValueAccessed() == zfnull)
    {
        zfstringAppend(errorHint, "propertyCustomImplCallbackIsValueAccessed not set");
        return zffalse;
    }

    if(param.propertyCustomImplCallbackIsInitValue() == zfnull)
    {
        zfstringAppend(errorHint, "propertyCustomImplCallbackIsInitValue not set");
        return zffalse;
    }

    if(param.propertyCustomImplCallbackValueReset() == zfnull)
    {
        zfstringAppend(errorHint, "propertyCustomImplCallbackValueReset not set");
        return zffalse;
    }

    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

