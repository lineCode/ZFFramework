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

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFPropertyDynamicRegisterParamPrivate
{
public:
    zfuint refCount;
    zfautoObject propertyDynamicRegisterUserData;
    const ZFClass *propertyOwnerClass;
    zfstring propertyTypeId;
    zfstring propertyTypeName;
    zfstring propertyName;
    const ZFClass *propertyClassOfRetainProperty;
    ZFPropertyDynamicRegisterInitValueCallback propertyInitValueCallback;
    ZFMethodPrivilegeType propertySetterType;
    ZFMethodPrivilegeType propertyGetterType;
public:
    _ZFP_ZFPropertyDynamicRegisterParamPrivate(void)
    : refCount(1)
    , propertyDynamicRegisterUserData(zfnull)
    , propertyOwnerClass()
    , propertyTypeId()
    , propertyTypeName()
    , propertyName()
    , propertyClassOfRetainProperty(zfnull)
    , propertyInitValueCallback(zfnull)
    , propertySetterType(ZFMethodPrivilegeTypePublic)
    , propertyGetterType(ZFMethodPrivilegeTypePublic)
    {
    }
};

// ============================================================
ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyDynamicRegisterUserDataSet(ZF_IN ZFObject *propertyDynamicRegisterUserData)
{
    d->propertyDynamicRegisterUserData = propertyDynamicRegisterUserData;
    return *this;
}
ZFObject *ZFPropertyDynamicRegisterParam::propertyDynamicRegisterUserData(void) const
{
    return d->propertyDynamicRegisterUserData;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyOwnerClassSet(ZF_IN const ZFClass *propertyOwnerClass)
{
    d->propertyOwnerClass = propertyOwnerClass;
    return *this;
}
const ZFClass *ZFPropertyDynamicRegisterParam::propertyOwnerClass(void) const
{
    return d->propertyOwnerClass;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyTypeIdSet(ZF_IN const zfchar *propertyTypeId)
{
    d->propertyTypeId = propertyTypeId;
    return *this;
}
const zfchar *ZFPropertyDynamicRegisterParam::propertyTypeId(void) const
{
    return (d->propertyTypeId.isEmpty() ? zfnull : d->propertyTypeId.cString());
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyTypeNameSet(ZF_IN const zfchar *propertyTypeName)
{
    d->propertyTypeName = propertyTypeName;
    return *this;
}
const zfchar *ZFPropertyDynamicRegisterParam::propertyTypeName(void) const
{
    return (d->propertyTypeName.isEmpty() ? d->propertyTypeId.cString() : d->propertyTypeName.cString());
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyNameSet(ZF_IN const zfchar *propertyName)
{
    d->propertyName = propertyName;
    return *this;
}
const zfchar *ZFPropertyDynamicRegisterParam::propertyName(void) const
{
    return (d->propertyName.isEmpty() ? zfnull : d->propertyName.cString());
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyClassOfRetainPropertySet(ZF_IN const ZFClass *propertyClassOfRetainProperty)
{
    d->propertyClassOfRetainProperty = propertyClassOfRetainProperty;
    return *this;
}
const ZFClass *ZFPropertyDynamicRegisterParam::propertyClassOfRetainProperty(void) const
{
    return d->propertyClassOfRetainProperty;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyInitValueCallbackSet(ZF_IN ZFPropertyDynamicRegisterInitValueCallback propertyInitValueCallback)
{
    d->propertyInitValueCallback = propertyInitValueCallback;
    return *this;
}
ZFPropertyDynamicRegisterInitValueCallback ZFPropertyDynamicRegisterParam::propertyInitValueCallback(void) const
{
    return d->propertyInitValueCallback;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertySetterTypeSet(ZF_IN ZFMethodPrivilegeType propertySetterType)
{
    d->propertySetterType = propertySetterType;
    return *this;
}
ZFMethodPrivilegeType ZFPropertyDynamicRegisterParam::propertySetterType(void) const
{
    return d->propertySetterType;
}

ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::propertyGetterTypeSet(ZF_IN ZFMethodPrivilegeType propertyGetterType)
{
    d->propertyGetterType = propertyGetterType;
    return *this;
}
ZFMethodPrivilegeType ZFPropertyDynamicRegisterParam::propertyGetterType(void) const
{
    return d->propertyGetterType;
}

/** @cond ZFPrivateDoc */
ZFPropertyDynamicRegisterParam::ZFPropertyDynamicRegisterParam(void)
: d(zfnew(_ZFP_ZFPropertyDynamicRegisterParamPrivate))
{
}
ZFPropertyDynamicRegisterParam::ZFPropertyDynamicRegisterParam(ZF_IN ZFPropertyDynamicRegisterParam const &ref)
: d(ref.d)
{
    ++(d->refCount);
}
ZFPropertyDynamicRegisterParam::~ZFPropertyDynamicRegisterParam(void)
{
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}
ZFPropertyDynamicRegisterParam &ZFPropertyDynamicRegisterParam::operator = (ZF_IN ZFPropertyDynamicRegisterParam const &ref)
{
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
    d = ref.d;
    return *this;
}
zfbool ZFPropertyDynamicRegisterParam::operator == (ZF_IN ZFPropertyDynamicRegisterParam const &ref) const
{
    return (d == ref.d);
}
/** @endcond */

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam &, propertyDynamicRegisterUserDataSet, ZFMP_IN(ZFObject *, propertyDynamicRegisterUserData))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, ZFObject *, propertyDynamicRegisterUserData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam &, propertyOwnerClassSet, ZFMP_IN(const ZFClass *, propertyOwnerClass))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const ZFClass *, propertyOwnerClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam &, propertyTypeIdSet, ZFMP_IN(const zfchar *, propertyTypeId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const zfchar *, propertyTypeId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam &, propertyTypeNameSet, ZFMP_IN(const zfchar *, propertyTypeName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const zfchar *, propertyTypeName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam &, propertyNameSet, ZFMP_IN(const zfchar *, propertyName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const zfchar *, propertyName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam &, propertyClassOfRetainPropertySet, ZFMP_IN(const ZFClass *, propertyClassOfRetainProperty))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, const ZFClass *, propertyClassOfRetainProperty)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam &, propertyInitValueCallbackSet, ZFMP_IN(ZFPropertyDynamicRegisterInitValueCallback, propertyInitValueCallback))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterInitValueCallback, propertyInitValueCallback)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam &, propertySetterTypeSet, ZFMP_IN(ZFMethodPrivilegeType, propertySetterType))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, ZFMethodPrivilegeType, propertySetterType)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFPropertyDynamicRegisterParam, ZFPropertyDynamicRegisterParam &, propertyGetterTypeSet, ZFMP_IN(ZFMethodPrivilegeType, propertyGetterType))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFPropertyDynamicRegisterParam, ZFMethodPrivilegeType, propertyGetterType)

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFProperty *, ZFPropertyDynamicRegister, ZFMP_IN(const ZFPropertyDynamicRegisterParam &, param), ZFMP_OUT_OPT(zfstring *, errorHint, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(void, ZFPropertyDynamicUnregister, ZFMP_IN(const ZFProperty *, property))

ZF_NAMESPACE_GLOBAL_END
#endif

