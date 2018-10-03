/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
/**
 * @file ZFPropertyUserRegister.h
 * @brief user registered ZFProperty
 */

#ifndef _ZFI_ZFPropertyUserRegister_h_
#define _ZFI_ZFPropertyUserRegister_h_

#include "ZFMethodUserRegister.h"
#include "ZFPropertyCallbackDefaultImpl.h"
#include "ZFObjectSmartPointer.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass ZF_ENV_EXPORT _ZFP_I_PropURDIVH : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_PropURDIVH, ZFObject)
public:
    typedef void (*DeleteCallback)(ZF_IN void *v);
    void *v;
    DeleteCallback deleteCallback;
    zfautoObject retainValueHolder;
public:
    static zfautoObject create(ZF_IN void *v,
                               ZF_IN DeleteCallback deleteCallback,
                               ZF_IN_OPT ZFObject *retainValue = zfnull);
protected:
    zfoverride
    virtual void objectOnDealloc(void)
    {
        if(this->v)
        {
            this->deleteCallback(this->v);
        }
        zfsuper::objectOnDealloc();
    }
};

// ============================================================
/** @brief default impl for #ZFPropertyUserRegisterRetain */
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT ZFPropertyUserRegisterDefaultImplRetain
{
private:
    static void _deleteCallback(ZF_IN void *p)
    {
        T_Type *t = (T_Type *)p;
        zfRelease(*t);
        zfdelete(t);
    }
public:
    /** @brief default impl for #ZFPropertyUserRegisterRetain */
    static void setterInvoker(ZF_IN const ZFMethod *method, ZF_IN ZFObject *ownerObj, ZF_IN T_Type const &v)
    {
        zfCoreMutexLocker();
        zfstring key = "_ZFP_PropURDIRetain_";
        key += method->methodName();
        key.remove(key.length() - 3 /* zfslen("Set") */);
        const ZFProperty *property = method->methodOwnerClass()->propertyForName(
            key.cString() + zfslen("_ZFP_PropURDIRetain_"));

        _ZFP_I_PropURDIVH *holder = ownerObj->tagGet<_ZFP_I_PropURDIVH *>(key);
        if(holder == zfnull)
        {
            holder = _ZFP_valueHolderAccess(property, ownerObj);
        }
        zfautoObject oldValue = holder->retainValueHolder;
        ownerObj->tagSet(
                key,
                _ZFP_I_PropURDIVH::create(
                    zfnew(T_Type, zfRetain(v)),
                    _deleteCallback,
                    v ? v->toObject() : zfnull
                )
            );
        ownerObj->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, &oldValue);
    }
    /** @brief default impl for #ZFPropertyUserRegisterRetain */
    static T_Type const &getterInvoker(ZF_IN const ZFMethod *method, ZF_IN ZFObject *ownerObj)
    {
        _ZFP_I_PropURDIVH *holder = _ZFP_valueHolderAccess(
            method->methodOwnerClass()->propertyForName(method->methodName()),
            ownerObj);
        return *(T_Type *)holder->v;
    }
    /** @brief default impl for #ZFPropertyUserRegisterRetain */
    static zfbool callbackIsValueAccessed(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj)
    {
        return (ownerObj->tagGet(
                zfsConnectLineFree("_ZFP_PropURDIRetain_", property->propertyName())
            ) != zfnull);
    }
    /** @brief default impl for #ZFPropertyUserRegisterRetain */
    static zfbool callbackIsInitValue(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj, ZF_OUT_OPT void *outInitValue)
    {
        zfCoreMutexLocker();
        if(!callbackIsValueAccessed(property, ownerObj))
        {
            return zftrue;
        }
        zfautoObject tmp;
        if(property->callbackUserRegisterInitValueSetup)
        {
            property->callbackUserRegisterInitValueSetup(property, (void *)&tmp);
        }
        if(outInitValue != zfnull)
        {
            *(zfautoObject *)outInitValue = tmp;
        }
        T_Type cur = getterInvoker(property->getterMethod(), ownerObj);
        T_Type initValueTmp = tmp.to<T_Type>();
        if(cur == initValueTmp)
        {
            return zftrue;
        }
        else if(cur == zfnull || initValueTmp == zfnull)
        {
            return zffalse;
        }
        else
        {
            return (cur->toObject()->objectCompare(initValueTmp->toObject()) == ZFCompareTheSame);
        }
    }
    /** @brief default impl for #ZFPropertyUserRegisterRetain */
    static void callbackValueReset(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj)
    {
        zfCoreMutexLocker();
        zfstring key = "_ZFP_PropURDIAssign_";
        key += property->propertyName();
        ownerObj->tagRemove(key);
    }
    /** @brief default impl for #ZFPropertyUserRegisterRetain */
    static const void *callbackValueGet(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj, ZF_IN_OUT void *&valueToken)
    {
        _ZFP_I_PropURDIVH *holder = _ZFP_valueHolderAccess(property, ownerObj);
        return &(holder->retainValueHolder);
    }
    /** @brief default impl for #ZFPropertyUserRegisterRetain */
    static void callbackValueGetRelease(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj, ZF_IN void *valueToken, ZF_IN const void *value)
    {
    }
private:
    static _ZFP_I_PropURDIVH *_ZFP_valueHolderAccess(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj)
    {
        zfCoreMutexLocker();
        zfstring key = "_ZFP_PropURDIAssign_";
        key += property->propertyName();
        _ZFP_I_PropURDIVH *holder = ownerObj->tagGet<_ZFP_I_PropURDIVH *>(key);
        if(holder == zfnull)
        {
            zfautoObject tmp;
            if(property->callbackUserRegisterInitValueSetup)
            {
                property->callbackUserRegisterInitValueSetup(property, (void *)&tmp);
            }

            zfautoObject holderTmp = _ZFP_I_PropURDIVH::create(
                    zfnew(T_Type, zfRetain(tmp.to<T_Type>())),
                    _deleteCallback,
                    tmp
                );
            ownerObj->tagSet(key, holderTmp);
            holder = holderTmp;
            ownerObj->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
        }
        return holder;
    }
};

// ============================================================
/** @brief default impl for #ZFPropertyUserRegisterAssign */
template<typename T_Type>
zfclassNotPOD ZF_ENV_EXPORT ZFPropertyUserRegisterDefaultImplAssign
{
private:
    static void _deleteCallback(ZF_IN void *p)
    {
        T_Type *t = (T_Type *)p;
        zfdelete(t);
    }
public:
    /** @brief default impl for #ZFPropertyUserRegisterAssign */
    static void setterInvoker(ZF_IN const ZFMethod *method, ZF_IN ZFObject *ownerObj, ZF_IN T_Type const &v)
    {
        zfCoreMutexLocker();
        zfstring key = "_ZFP_PropURDIAssign_";
        key += method->methodName();
        key.remove(key.length() - 3 /* zfslen("Set") */);
        const ZFProperty *property = method->methodOwnerClass()->propertyForName(
            key.cString() + zfslen("_ZFP_PropURDIRetain_"));

        _ZFP_I_PropURDIVH *holder = ownerObj->tagGet<_ZFP_I_PropURDIVH *>(key);
        if(holder == zfnull)
        {
            holder = _ZFP_valueHolderAccess(property, ownerObj);
        }
        T_Type oldValue = *(T_Type *)holder->v;
        ownerObj->tagSet(
                key,
                _ZFP_I_PropURDIVH::create(
                    zfnew(T_Type, v),
                    _deleteCallback
                )
            );
        ownerObj->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, &oldValue);
    }
    /** @brief default impl for #ZFPropertyUserRegisterAssign */
    static T_Type const &getterInvoker(ZF_IN const ZFMethod *method, ZF_IN ZFObject *ownerObj)
    {
        _ZFP_I_PropURDIVH *holder = _ZFP_valueHolderAccess(
            method->methodOwnerClass()->propertyForName(method->methodName()),
            ownerObj);
        return *(T_Type *)holder->v;
    }
    /** @brief default impl for #ZFPropertyUserRegisterAssign */
    static zfbool callbackIsValueAccessed(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj)
    {
        return (ownerObj->tagGet(
                zfsConnectLineFree("_ZFP_PropURDIAssign_", property->propertyName())
            ) != zfnull);
    }
    /** @brief default impl for #ZFPropertyUserRegisterAssign */
    static zfbool callbackIsInitValue(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj, ZF_OUT_OPT void *outInitValue)
    {
        zfCoreMutexLocker();
        if(!callbackIsValueAccessed(property, ownerObj))
        {
            return zftrue;
        }
        T_Type tmp = T_Type();
        if(property->callbackUserRegisterInitValueSetup)
        {
            property->callbackUserRegisterInitValueSetup(property, (void *)&tmp);
        }
        if(outInitValue != zfnull)
        {
            *(T_Type *)outInitValue = tmp;
        }
        return (getterInvoker(property->getterMethod(), ownerObj) == tmp);
    }
    /** @brief default impl for #ZFPropertyUserRegisterRetain */
    static void callbackValueReset(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj)
    {
        zfCoreMutexLocker();
        zfstring key = "_ZFP_PropURDIAssign_";
        key += property->propertyName();
        ownerObj->tagRemove(key);
    }
    /** @brief default impl for #ZFPropertyUserRegisterRetain */
    static const void *callbackValueGet(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj, ZF_IN_OUT void *&valueToken)
    {
        _ZFP_I_PropURDIVH *holder = _ZFP_valueHolderAccess(property, ownerObj);
        return holder->v;
    }
    /** @brief default impl for #ZFPropertyUserRegisterRetain */
    static void callbackValueGetRelease(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj, ZF_IN void *valueToken, ZF_IN const void *value)
    {
    }
private:
    static _ZFP_I_PropURDIVH *_ZFP_valueHolderAccess(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj)
    {
        zfCoreMutexLocker();
        zfstring key = "_ZFP_PropURDIAssign_";
        key += property->propertyName();
        _ZFP_I_PropURDIVH *holder = ownerObj->tagGet<_ZFP_I_PropURDIVH *>(key);
        if(holder == zfnull)
        {
            T_Type tmp = T_Type();
            if(property->callbackUserRegisterInitValueSetup)
            {
                property->callbackUserRegisterInitValueSetup(property, (void *)&tmp);
            }

            zfautoObject holderTmp = _ZFP_I_PropURDIVH::create(
                    zfnew(T_Type, tmp),
                    _deleteCallback
                );
            ownerObj->tagSet(key, holderTmp);
            holder = holderTmp;
            ownerObj->_ZFP_ZFObject_objectPropertyValueOnUpdate(property, zfnull);
        }
        return holder;
    }
};

// ============================================================
#define _ZFP_ZFPropertyUserRegister_PropInit_Retain(registerSig, Type, InitValueOrEmpty) \
    zfclassNotPOD _ZFP_PropURInit_##registerSig \
    { \
    public: \
        static void propertyInit(ZF_IN const ZFProperty *property, ZF_IN_OUT void *p) \
        { \
            typedef Type T_Type; \
            T_Type *valueTmp = zfnull; \
            zflockfree_zfRetain(*(valueTmp = zfpoolNew(T_Type, InitValueOrEmpty))); \
            *(zfautoObject *)p = *valueTmp; \
            zfpoolDelete(valueTmp); \
        } \
    };
#define _ZFP_ZFPropertyUserRegister_PropInit_Assign(registerSig, Type, InitValueOrEmpty) \
    zfclassNotPOD _ZFP_PropURInit_##registerSig \
    { \
    public: \
        static void propertyInit(ZF_IN const ZFProperty *property, ZF_IN_OUT void *p) \
        { \
            typedef Type T_Type; \
            T_Type *valueTmp = zfpoolNew(T_Type, InitValueOrEmpty); \
            *(T_Type *)p = *valueTmp; \
            zfpoolDelete(valueTmp); \
        } \
    };

#define _ZFP_ZFPropertyUserRegister_ParamExpand_Retain( \
        registerSig, \
        ownerClass, \
        Type, propertyNameString, \
        ZFTypeId_noneOrType, \
        setterMethod, getterMethod, \
        propertyClassOfRetainProperty \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
    ) \
    ( \
        zftrue \
        , zffalse \
        , zfnull \
        , ownerClass \
        , propertyNameString \
        , ZFM_TOSTRING(Type) \
        , ZFTypeId_noneOrType \
        , setterMethod \
        , getterMethod \
        , propertyClassOfRetainProperty \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , _ZFP_propCbDValueSet<zfautoObject, Type> \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
        , _ZFP_propCbDCompare<zfautoObject> \
        , _ZFP_propCbDGetInfo<zfautoObject> \
        , _ZFP_propCbDValueStore<zfautoObject> \
        , _ZFP_propCbDValueRelease<zfautoObject> \
        , _ZFP_propCbDSerializeFrom_get<Type>() \
        , _ZFP_propCbDSerializeTo_get<Type>() \
        , _ZFP_propCbDProgressUpdate<zfautoObject, Type> \
        , _ZFP_PropURInit_##registerSig::propertyInit \
        , zfnull \
    );
#define _ZFP_ZFPropertyUserRegister_ParamExpand_Assign( \
        registerSig, \
        ownerClass, \
        Type, propertyNameString, \
        ZFTypeId_noneOrType, \
        setterMethod, \
        getterMethod, \
        propertyClassOfRetainProperty \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
    ) \
    ( \
        zftrue \
        , zffalse \
        , zfnull \
        , ownerClass \
        , propertyNameString \
        , ZFM_TOSTRING(Type) \
        , ZFTypeId_noneOrType \
        , setterMethod \
        , getterMethod \
        , propertyClassOfRetainProperty \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , _ZFP_propCbDValueSet<Type, Type> \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
        , _ZFP_propCbDCompare<Type> \
        , _ZFP_propCbDGetInfo<Type> \
        , _ZFP_propCbDValueStore<Type> \
        , _ZFP_propCbDValueRelease<Type> \
        , _ZFP_propCbDSerializeFrom_get<Type>() \
        , _ZFP_propCbDSerializeTo_get<Type>() \
        , _ZFP_propCbDProgressUpdate<Type, Type> \
        , _ZFP_PropURInit_##registerSig::propertyInit \
        , zfnull \
    )

// ============================================================
#define _ZFP_ZFPropertyUserRegister(resultProperty, ownerClass, \
        Type, propertyNameString, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType, \
        RetainOrAssign, \
        ZFTypeId_noneOrType, \
        propertyClassOfRetainProperty \
        , Func_ZFPropertySetterInvoker \
        , Func_ZFPropertyGetterInvoker \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
    ) \
    const ZFProperty *resultProperty = zfnull; \
    { \
        zfCoreMutexLocker(); \
        const ZFClass *_ownerClass = ownerClass; \
        zfstring _propertyName(propertyNameString); \
        \
        ZFMethodUserRegisterDetail_1(setterMethod, Func_ZFPropertySetterInvoker, _ownerClass, \
            SetterAccessType, ZFMethodTypeVirtual, \
            void, zfstringWithFormat("%sSet", _propertyName.cString()), \
            ZFMP_IN(Type const &, value)); \
        ZFMethodUserRegisterDetail_0(getterMethod, Func_ZFPropertyGetterInvoker, _ownerClass, \
            GetterAccessType, ZFMethodTypeVirtual, \
            Type const &, _propertyName); \
        _ZFP_ZFPropertyUserRegister_PropInit_##RetainOrAssign(_, Type, InitValueOrEmpty) \
        resultProperty = _ZFP_ZFPropertyRegister _ZFP_ZFPropertyUserRegister_ParamExpand_##RetainOrAssign( \
                _, _ownerClass, \
                Type, _propertyName, \
                ZFTypeId_noneOrType, \
                setterMethod, getterMethod, \
                propertyClassOfRetainProperty \
                , Func_ZFPropertyCallbackIsValueAccessed \
                , Func_ZFPropertyCallbackIsInitValue \
                , Func_ZFPropertyCallbackValueReset \
                , Func_ZFPropertyCallbackValueGet \
                , Func_ZFPropertyCallbackValueGetRelease \
            ); \
    } \
    ZFUNUSED(resultProperty)

#define _ZFP_ZFPROPERTY_USER_REGISTER(ownerClassSig, \
        Type, propertyNameSig, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType, \
        RetainOrAssign, \
        ZFTypeId_noneOrType, \
        propertyClassOfRetainProperty \
        , Func_ZFPropertySetterInvoker \
        , Func_ZFPropertyGetterInvoker \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
    ) \
    zfclassNotPOD _ZFP_PropURMH_##ownerClassSig##_##propertyNameSig \
    { \
    public: \
        static const ZFMethod *S(void) \
        { \
            ZFMethodUserRegisterDetail_1(setterMethod, Func_ZFPropertySetterInvoker, ownerClassSig::ClassData(), \
                SetterAccessType, ZFMethodTypeVirtual, \
                void, ZFM_TOSTRING(propertyNameSig) "Set", \
                ZFMP_IN(Type const &, value)); \
            return setterMethod; \
        } \
        static const ZFMethod *G(void) \
        { \
            ZFMethodUserRegisterDetail_0(getterMethod, Func_ZFPropertyGetterInvoker, ownerClassSig::ClassData(), \
                GetterAccessType, ZFMethodTypeVirtual, \
                Type const &, ZFM_TOSTRING(propertyNameSig)); \
            return getterMethod; \
        } \
    }; \
    _ZFP_ZFPropertyUserRegister_PropInit_##RetainOrAssign(ownerClassSig##_##propertyNameSig, Type, InitValueOrEmpty) \
    static _ZFP_ZFPropertyRegisterHolder _ZFP_PropURH_##ownerClassSig##_##propertyNameSig \
        _ZFP_ZFPropertyUserRegister_ParamExpand_##RetainOrAssign( \
            ownerClassSig##_##propertyNameSig, ownerClassSig::ClassData(), \
            Type, ZFM_TOSTRING(propertyNameSig), \
            ZFTypeId_noneOrType, \
            _ZFP_PropURMH_##ownerClassSig##_##propertyNameSig::S(), \
            _ZFP_PropURMH_##ownerClassSig##_##propertyNameSig::G(), \
            propertyClassOfRetainProperty \
            , Func_ZFPropertyCallbackIsValueAccessed \
            , Func_ZFPropertyCallbackIsInitValue \
            , Func_ZFPropertyCallbackValueReset \
            , Func_ZFPropertyCallbackValueGet \
            , Func_ZFPropertyCallbackValueGetRelease \
        );

// ============================================================
/**
 * @brief register a custom property to existing class, for advanced reflection use only
 *
 * example:
 * @code
 *   // declare each callback for ZFProperty
 *   static void myCallbackSetterInvoker(ZF_IN const ZFProperty *, ZF_IN ZFObject *ownerObj, ZF_IN MyPropType const &v) {...}
 *   static MyPropType const &myCallbackGetterInvoker(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj) {...}
 *   static zfbool myCallbackIsValueAccessed(ZF_IN const ZFProperty *, ZF_IN ZFObject *ownerObj) {...}
 *   static zfbool myCallbackIsInitValue(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj, ZF_OUT_OPT void *outInitValue) {...}
 *   static void myCallbackValueReset(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj) {...}
 *   static const void *myCallbackValueGet(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj) {...}
 *   static void myCallbackValueGetRelease(ZF_IN const ZFProperty *property, ZF_IN ZFObject *ownerObj, ZF_IN const void *value) {...}
 *
 *   // register it, using ZF_GLOBAL_INITIALIZER_INIT is recommended
 *   ZF_GLOBAL_INITIALIZER_INIT(MyPropertyRegister)
 *   {
 *       ZFPropertyUserRegisterRetainDetail(resultProperty, MyClassToAttachTheProperty::ClassData(),
 *           Type, "propertyName", ZFPropertyNoInitValue,
 *           public, public
 *           , myCallbackSetterInvoker
 *           , myCallbackGetterInvoker
 *           , myCallbackIsValueAccessed
 *           , myCallbackIsInitValue
 *           , myCallbackValueReset
 *           , myCallbackValueGet
 *           , myCallbackValueGetRelease
 *           );
 *       _property = resultProperty;
 *       zfLogTrimT() << resultProperty;
 *   }
 *   ZF_GLOBAL_INITIALIZER_DESTROY(MyPropertyRegister)
 *   {
 *       ZFPropertyUserUnregister(_property);
 *   }
 *   const ZFProperty *_property;
 *   ZF_GLOBAL_INITIALIZER_END(MyPropertyRegister)
 *
 *   // or, you may use this macro for short, at cpp files only
 *   ZFPROPERTY_USER_REGISTER_RETAIN_DETAIL(MyClassToAttachTheProperty, MyClassToAttachTheProperty::ClassData(),
 *       Type, "propertyName", ZFPropertyNoInitValue,
 *       public, public,
 *       , myCallbackSetterInvoker
 *       , myCallbackGetterInvoker
 *       , myCallbackIsValueAccessed
 *       , myCallbackIsInitValue
 *       , myCallbackValueReset
 *       , myCallbackValueGet
 *       , myCallbackValueGetRelease
 *       )
 *
 *   // or, if you simply want to register a raw value,
 *   // you may use ZFPropertyUserRegisterDefaultImplRetain or ZFPropertyUserRegisterDefaultImplAssign as default impl
 *   ZFPROPERTY_USER_REGISTER_RETAIN_DETAIL(MyClassToAttachTheProperty, MyClassToAttachTheProperty::ClassData(),
 *       Type, "propertyName", ZFPropertyNoInitValue,
 *       public, public
 *       ZFPropertyUserRegisterDefaultImplRetain<Type>::setterInvoker,
 *       ZFPropertyUserRegisterDefaultImplRetain<Type>::getterInvoker,
 *       ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackIsValueAccessed,
 *       ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackIsInitValue,
 *       ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackValueReset
 *       ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackValueGet
 *       ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackValueGetRelease
 *       )
 * @endcode
 *
 * note:
 * -  if there is already a property exist with same property name,
 *   register would fail
 * -  the registered property must be unregister manually at proper time,
 *   use #ZF_GLOBAL_INITIALIZER_INIT is recommeded
 * -  once registered, the property can be reflected by #ZFClass::propertyForName,
 *   but there's no way to access the property by object directly,
 *   it can only be accessed by #ZFProperty's method
 */
#define ZFPropertyUserRegisterRetain(resultProperty, ownerClass, \
        Type, propertyNameString, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType \
    ) \
    ZFPropertyUserRegisterRetainDetail(resultProperty, ownerClass, \
        Type, propertyNameString, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::setterInvoker \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::getterInvoker \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackIsValueAccessed \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackIsInitValue \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackValueReset \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackValueGet \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackValueGetRelease \
    )
/** @brief see #ZFPropertyUserRegisterRetain */
#define ZFPropertyUserRegisterRetainDetail(resultProperty, ownerClass, \
        Type, propertyNameString, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType \
        , Func_ZFPropertySetterInvoker \
        , Func_ZFPropertyGetterInvoker \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
    ) \
    _ZFP_ZFPropertyUserRegister(resultProperty, ownerClass, \
        Type, propertyNameString, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType, \
        Retain, \
        zftTraits<Type>::TrType::ClassData()->classNameFull(), \
        zftTraits<Type>::TrType::ClassData() \
        , Func_ZFPropertySetterInvoker \
        , Func_ZFPropertyGetterInvoker \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
    )
/** @brief see #ZFPropertyUserRegisterRetain */
#define ZFPROPERTY_USER_REGISTER_RETAIN(ownerClassSig, \
        Type, propertyNameSig, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType \
    ) \
    ZFPROPERTY_USER_REGISTER_RETAIN_DETAIL(ownerClassSig, \
        Type, propertyNameSig, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::setterInvoker \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::getterInvoker \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackIsValueAccessed \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackIsInitValue \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackValueReset \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackValueGet \
        , ZFPropertyUserRegisterDefaultImplRetain<Type>::callbackValueGetRelease \
    )
/** @brief see #ZFPropertyUserRegisterRetain */
#define ZFPROPERTY_USER_REGISTER_RETAIN_DETAIL(ownerClassSig, \
        Type, propertyNameSig, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType \
        , Func_ZFPropertySetterInvoker \
        , Func_ZFPropertyGetterInvoker \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
    ) \
    _ZFP_ZFPROPERTY_USER_REGISTER(ownerClassSig, \
        Type, propertyNameSig, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType, \
        Retain, \
        zftTraits<Type>::TrType::ClassData()->classNameFull(), \
        zftTraits<Type>::TrType::ClassData() \
        , Func_ZFPropertySetterInvoker \
        , Func_ZFPropertyGetterInvoker \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
    )

// ============================================================
/** @brief see #ZFPropertyUserRegisterRetain */
#define ZFPropertyUserRegisterAssign(resultProperty, ownerClass, \
        Type, propertyNameString, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType \
    ) \
    ZFPropertyUserRegisterAssignDetail(resultProperty, ownerClass, \
        Type, propertyNameString, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::setterInvoker \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::getterInvoker \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::callbackIsValueAccessed \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::callbackIsInitValue \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::callbackValueReset \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::callbackValueGet \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::callbackValueGetRelease \
    )
/** @brief see #ZFPropertyUserRegisterRetain */
#define ZFPropertyUserRegisterAssignDetail(resultProperty, ownerClass, \
        Type, propertyNameString, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType \
        , Func_ZFPropertySetterInvoker \
        , Func_ZFPropertyGetterInvoker \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
    ) \
    _ZFP_ZFPropertyUserRegister(resultProperty, ownerClass, \
        Type, propertyNameString, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType, \
        Assign, \
        ZFTypeId<zftTraits<Type>::TrNoRef>::TypeId(), \
        zfnull \
        , Func_ZFPropertySetterInvoker \
        , Func_ZFPropertyGetterInvoker \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
    )
/** @brief see #ZFPropertyUserRegisterAssign */
#define ZFPROPERTY_USER_REGISTER_ASSIGN(ownerClassSig, \
        Type, propertyNameSig, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType \
    ) \
    ZFPROPERTY_USER_REGISTER_ASSIGN_DETAIL(ownerClassSig, \
        Type, propertyNameSig, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::setterInvoker \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::getterInvoker \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::callbackIsValueAccessed \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::callbackIsInitValue \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::callbackValueReset \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::callbackValueGet \
        , ZFPropertyUserRegisterDefaultImplAssign<Type>::callbackValueGetRelease \
    )
/** @brief see #ZFPropertyUserRegisterAssign */
#define ZFPROPERTY_USER_REGISTER_ASSIGN_DETAIL(ownerClassSig, \
        Type, propertyNameSig, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType \
        , Func_ZFPropertySetterInvoker \
        , Func_ZFPropertyGetterInvoker \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
    ) \
    _ZFP_ZFPROPERTY_USER_REGISTER(ownerClassSig, \
        Type, propertyNameSig, InitValueOrEmpty, \
        SetterAccessType, GetterAccessType, \
        Assign, \
        ZFTypeId<zftTraits<Type>::TrNoRef>::TypeId(), \
        zfnull \
        , Func_ZFPropertySetterInvoker \
        , Func_ZFPropertyGetterInvoker \
        , Func_ZFPropertyCallbackIsValueAccessed \
        , Func_ZFPropertyCallbackIsInitValue \
        , Func_ZFPropertyCallbackValueReset \
        , Func_ZFPropertyCallbackValueGet \
        , Func_ZFPropertyCallbackValueGetRelease \
    )

// ============================================================
/** @brief see #ZFPropertyUserRegisterRetain */
extern ZF_ENV_EXPORT void ZFPropertyUserUnregister(ZF_IN const ZFProperty *zfproperty);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyUserRegister_h_

