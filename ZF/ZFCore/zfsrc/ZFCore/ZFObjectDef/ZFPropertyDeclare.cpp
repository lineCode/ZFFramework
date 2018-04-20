/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFPropertyDeclare.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const ZFProperty *ZFPropertyGet(ZF_IN const zfchar *clsName,
                                ZF_IN const zfchar *propertyName)
{
    const ZFClass *cls = ZFClass::classForName(clsName);
    if(cls != zfnull)
    {
        return cls->propertyForName(propertyName);
    }
    return zfnull;
}
const ZFProperty *ZFPropertyGet(ZF_IN const ZFClass *cls,
                                ZF_IN const zfchar *propertyName)
{
    if(cls != zfnull)
    {
        return cls->propertyForName(propertyName);
    }
    return zfnull;
}

// ============================================================
ZFCoreArrayPOD<_ZFP_PropLifeCycleData> &_ZFP_ZFPropertyLifeCycleDataRef(ZF_IN const zfchar *lifeCycleName,
                                                                        ZF_IN const ZFProperty *property)
{
    if(zffalse)
    {
    }
    else if(zfscmpTheSame(lifeCycleName, zfText("OnInit")))
    {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnInit;
    }
    else if(zfscmpTheSame(lifeCycleName, zfText("OnDealloc")))
    {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnDealloc;
    }
    else if(zfscmpTheSame(lifeCycleName, zfText("OnVerify")))
    {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnVerify;
    }
    else if(zfscmpTheSame(lifeCycleName, zfText("OnAttach")))
    {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnAttach;
    }
    else if(zfscmpTheSame(lifeCycleName, zfText("OnDetach")))
    {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnDetach;
    }
    else if(zfscmpTheSame(lifeCycleName, zfText("OnUpdate")))
    {
        return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnUpdate;
    }

    zfCoreCriticalShouldNotGoHere();
    return property->_ZFP_ZFProperty_removeConst()->_ZFP_ZFPropertyLifeCycle_OnInit;
}
void _ZFP_ZFPropertyLifeCycleRegister(ZF_IN const zfchar *lifeCycleName,
                                      ZF_IN const ZFProperty *property,
                                      ZF_IN const ZFClass *propertyOwnerClass,
                                      ZF_IN _ZFP_PropLifeCycleWrapper propertyLifeCycleWrapper)
{
    ZFCoreArrayPOD<_ZFP_PropLifeCycleData> &d = _ZFP_ZFPropertyLifeCycleDataRef(lifeCycleName, property);
    for(zfindex i = 0; i < d.count(); ++i)
    {
        if(d[i].propertyOwnerClass == propertyOwnerClass)
        {
            d[i].propertyLifeCycleWrapper = propertyLifeCycleWrapper;
            return ;
        }
    }
    _ZFP_PropLifeCycleData data;
    data.propertyOwnerClass = propertyOwnerClass;
    data.propertyLifeCycleWrapper = propertyLifeCycleWrapper;
    d.add(data);
}
void _ZFP_ZFPropertyLifeCycleUnregister(ZF_IN const zfchar *lifeCycleName,
                                        ZF_IN const ZFProperty *property,
                                        ZF_IN const ZFClass *propertyOwnerClass)
{
    ZFCoreArrayPOD<_ZFP_PropLifeCycleData> &d = _ZFP_ZFPropertyLifeCycleDataRef(lifeCycleName, property);
    for(zfindex i = 0; i < d.count(); ++i)
    {
        if(d[i].propertyOwnerClass == propertyOwnerClass)
        {
            d.remove(i);
            break;
        }
    }
}

static void _ZFP_ZFPropertyLifeCycleCallAction(ZF_IN ZFCoreArrayPOD<_ZFP_PropLifeCycleData> const &d,
                                               ZF_IN ZFObject *propertyOwnerObject,
                                               ZF_IN void *propertyValue,
                                               ZF_IN const void *propertyValueOld,
                                               ZF_IN_OPT zfbool subclassFirst = zffalse)
{
    if(subclassFirst)
    {
        for(const _ZFP_PropLifeCycleData *p = d.arrayBuf() + d.count() - 1, *pEnd = d.arrayBuf() - 1; p != pEnd; --p)
        {
            if(propertyOwnerObject->classData()->classIsTypeOf(p->propertyOwnerClass))
            {
                p->propertyLifeCycleWrapper(propertyOwnerObject, propertyValue, propertyValueOld);
            }
        }
    }
    else
    {
        for(const _ZFP_PropLifeCycleData *p = d.arrayBuf(), *pEnd = d.arrayBuf() + d.count(); p != pEnd; ++p)
        {
            if(propertyOwnerObject->classData()->classIsTypeOf(p->propertyOwnerClass))
            {
                p->propertyLifeCycleWrapper(propertyOwnerObject, propertyValue, propertyValueOld);
            }
        }
    }
}

void _ZFP_ZFPropertyLifeCycleCall_init_retain(ZF_IN const ZFProperty *property,
                                              ZF_IN ZFObject *propertyOwnerObject,
                                              ZF_IN ZFObject *value,
                                              ZF_IN zfbool needNotifyOwner,
                                              ZF_IN void (*rawValueStoreCallback)(ZF_IN void *rawValueStoreToken, ZF_IN ZFObject *value),
                                              ZF_IN void *rawValueStoreToken)
{
    zfautoObject valueHolder = value;
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnInit,
        propertyOwnerObject,
        &valueHolder,
        &valueHolder);
    rawValueStoreCallback(rawValueStoreToken, valueHolder);

    if(needNotifyOwner)
    {
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnAttach,
            propertyOwnerObject,
            &valueHolder,
            &valueHolder);
        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property, zftrue);
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnUpdate,
            propertyOwnerObject,
            &valueHolder,
            &valueHolder);
        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
    }
}
void _ZFP_ZFPropertyLifeCycleCall_init_assign(ZF_IN const ZFProperty *property,
                                              ZF_IN ZFObject *propertyOwnerObject,
                                              ZF_IN void *value,
                                              ZF_IN zfbool needNotifyOwner)
{
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnInit,
        propertyOwnerObject,
        value,
        value);

    if(needNotifyOwner)
    {
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnAttach,
            propertyOwnerObject,
            value,
            value);
        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property, zftrue);
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnUpdate,
            propertyOwnerObject,
            value,
            value);
        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
    }
}
void _ZFP_ZFPropertyLifeCycleCall_dealloc_retain(ZF_IN const ZFProperty *property,
                                                 ZF_IN ZFObject *propertyOwnerObject,
                                                 ZF_IN ZFObject *value)
{
    zfautoObject valueHolder = value;
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnDetach,
        propertyOwnerObject,
        &valueHolder,
        &valueHolder,
        zftrue);
    propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueDetach(property, zffalse);
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnDealloc,
        propertyOwnerObject,
        &valueHolder,
        &valueHolder,
        zftrue);
}
void _ZFP_ZFPropertyLifeCycleCall_dealloc_assign(ZF_IN const ZFProperty *property,
                                                 ZF_IN ZFObject *propertyOwnerObject,
                                                 ZF_IN void *value)
{
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnDetach,
        propertyOwnerObject,
        value,
        value,
        zftrue);
    propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueDetach(property, zffalse);
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnDealloc,
        propertyOwnerObject,
        value,
        value,
        zftrue);
}
void _ZFP_ZFPropertyLifeCycleCall_setter_retain(ZF_IN const ZFProperty *property,
                                                ZF_IN ZFObject *propertyOwnerObject,
                                                ZF_IN zfbool accessed,
                                                ZF_IN ZFObject *propertyValueOld,
                                                ZF_IN ZFObject *propertyValueNew,
                                                ZF_IN void (*rawValueStoreCallback)(ZF_IN void *rawValueStoreToken, ZF_IN ZFObject *value),
                                                ZF_IN void *rawValueStoreToken)
{
    zfautoObject valueOld = propertyValueOld;
    zfautoObject valueNew = propertyValueNew;
    if(accessed)
    {
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnDetach,
            propertyOwnerObject,
            &valueNew,
            &valueOld,
            zftrue);
        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueDetach(property, zffalse);

        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnDealloc,
            propertyOwnerObject,
            &valueNew,
            &valueOld,
            zftrue);
    }

    rawValueStoreCallback(rawValueStoreToken, valueNew);
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnVerify,
        propertyOwnerObject,
        &valueNew,
        &valueOld);
    rawValueStoreCallback(rawValueStoreToken, valueNew);

    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnAttach,
        propertyOwnerObject,
        &valueNew,
        &valueOld);
    propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property, zffalse);

    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnUpdate,
        propertyOwnerObject,
        &valueNew,
        &valueOld);
    propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, &propertyValueOld);
}
void _ZFP_ZFPropertyLifeCycleCall_setter_assign(ZF_IN const ZFProperty *property,
                                                ZF_IN ZFObject *propertyOwnerObject,
                                                ZF_IN zfbool accessed,
                                                ZF_IN void *propertyValueOld,
                                                ZF_IN const void *propertyValueNew,
                                                ZF_IN void *(*rawValueStoreCallback)(ZF_IN void *rawValueStoreToken, ZF_IN const void *value),
                                                ZF_IN void *rawValueStoreToken)
{
    if(accessed)
    {
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnDetach,
            propertyOwnerObject,
            propertyValueOld,
            propertyValueOld,
            zftrue);
        propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueDetach(property, zffalse);
        _ZFP_ZFPropertyLifeCycleCallAction(
            property->_ZFP_ZFPropertyLifeCycle_OnDealloc,
            propertyOwnerObject,
            propertyValueOld,
            propertyValueOld,
            zftrue);
    }
    void *valueRef = rawValueStoreCallback(rawValueStoreToken, propertyValueNew);
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnVerify,
        propertyOwnerObject,
        valueRef,
        propertyValueOld);
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnAttach,
        propertyOwnerObject,
        valueRef,
        propertyValueOld);
    propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueAttach(property, zffalse);
    _ZFP_ZFPropertyLifeCycleCallAction(
        property->_ZFP_ZFPropertyLifeCycle_OnUpdate,
        propertyOwnerObject,
        valueRef,
        propertyValueOld);
    propertyOwnerObject->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, propertyValueOld);
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFProperty *, ZFPropertyGet, ZFMP_IN(const zfchar *, clsName), ZFMP_IN(const zfchar *, propertyName))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFProperty *, ZFPropertyGet, ZFMP_IN(const ZFClass *, cls), ZFMP_IN(const zfchar *, propertyName))

ZF_NAMESPACE_GLOBAL_END
#endif

