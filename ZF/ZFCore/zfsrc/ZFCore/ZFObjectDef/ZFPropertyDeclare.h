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
 * @file ZFPropertyDeclare.h
 * @brief macros to declare ZFProperty
 */

#ifndef _ZFI_ZFPropertyDeclare_h_
#define _ZFI_ZFPropertyDeclare_h_

#include "ZFProperty.h"
#include "zfautoObjectFwd.h"
#include "ZFMethodDeclare.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFClass;
// ============================================================
/**
 * @brief access the property directly, compile error if not declared
 */
#define ZFPropertyAccess(OwnerClass, Name) ((const ZFProperty *)OwnerClass::_ZFP_Prop_##Name())

/**
 * @brief get property by name, or null if not registered
 */
extern ZF_ENV_EXPORT const ZFProperty *ZFPropertyGet(ZF_IN const zfchar *clsName,
                                                     ZF_IN const zfchar *propertyName);
/**
 * @brief get property by name, or null if not registered
 */
extern ZF_ENV_EXPORT const ZFProperty *ZFPropertyGet(ZF_IN const ZFClass *cls,
                                                     ZF_IN const zfchar *propertyName);

// ============================================================
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPropertyNoInitValue

// ============================================================
/**
 * @brief declare a retain property
 *
 * -  ZFPROPERTY_XXX is similar to \@property in Object-C,
 *   which would automatically generate setter and getter for you\n
 *   here's a list of what they'll generate
 *   (assume property's type is Type and property's name is Name):
 *   -  ZFPROPERTY_RETAIN / ZFPROPERTY_ASSIGN:
 *     -  public:\n
 *       virtual void NameSet(Type const &propertyValue); // (reflectable)\n
 *       virtual Type const &Name(void); // (reflectable)
 *     -  protected:\n
 *       inline void NameSetInternal(Type const &propertyValue);\n
 *       inline Type const &NameInternal(void);
 *   -  for a retain property, Type must be ZFObject or ZFObject's subclass,
 *     and value is set by #zfRetainChange
 *   -  for a assign property, Type could be any type except for const type
 * -  ZFProperty support those type only:
 *   -  base type, such as zfcharA, int, const zfcharA *
 *   -  ZFObject *
 *   -  classes that have constructor with no argument and have operator = support
 * -  ZFProperty is also reflectable,
 *   you may reflect the ZFProperty itself by #ZFClass::propertyForName,
 *   or reflect the setter and getter as ZFMethod,
 *   for how to reflect, see #ZFMethod\n
 *   also, similar to ZFMethod, if you want the static binding for ZFProperty,
 *   use ZFPROPERTY_REGISTER,
 *   however, since a property is valid only if a instance has created,
 *   it's not quite necessary to register it
 * -  you may customize the accessibility for getter and setter,
 *   or add default value for setter,
 *   by using ZFPROPERTY_XXX_DETAIL:\n
 *   ZFPROPERTY_RETAIN_DETAIL(
 *       Type, Name, InitValueOrEmpty,
 *       SetterAccessType,
 *       GetterAccessType)\n
 *   the InitValueOrEmpty is the init value for the property,
 *     could be your_value or ZFPropertyNoInitValue,
 *     if not set, the built-in value would be set,
 *     e.g. zffalse for zfbool, empty string for zfstring
 *     (for retain property, the init value must be auto released object,
 *     use #zfautoObject or #zflineAlloc or #zflineRelease is recommended)\n
 *   the SetterAccessType/GetterAccessType could be
 *     public/protected/private\n
 *   for example:
 *   @code
 *     zfclass OwnerClass : zfextends Base
 *     {
 *         ZFOBJECT_DECLARE(OwnerClass, Base)
 *
 *         ZFPROPERTY_RETAIN_DETAIL(
 *             zfstring, ZFPropertyTypeId_zfstring(), StringProperty, \
 *             ZFPropertyNoInitValue,
 *             public,
 *             public)
 *     };
 *   @endcode
 *   @note for a retain property, it'll be released automatically when it's owner is dealloced
 * -  to override a property in subclass, you may use ZFPROPERTY_OVERRIDE_ON_XXX
 *   @warning you must not declare two property with same name in child and base class
 *   @note overrided property won't be included in it's ZFClass,
 *     it's only a method override, no new ZFMethod or ZFProperty would be declared in child class
 * -  to add Doxygen docs, you should:
 *   @code
 *    / **
 *      * @brief without fn defines, doc for getters only (ie. Name)
 *      *
 *      * setters' docs would be auto generated,
 *      * which is referencing to getters
 *      * /
 *     ZFPROPERTY_RETAIN(Type, Name)
 *   @endcode
 *   or:
 *   @code
 *    / **
 *      * @fn NameSet
 *      * @brief doc for NameSet
 *      * @fn Name
 *      * @brief doc for Name
 *      * /
 *     ZFPROPERTY_RETAIN(Type, Name)
 *   @endcode
 */
#define ZFPROPERTY_RETAIN(Type, Name) \
    ZFPROPERTY_RETAIN_DETAIL( \
        Type, Name, ZFPropertyNoInitValue, \
        public, public)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_RETAIN_WITH_INIT(Type, Name, InitValueOrEmpty) \
    ZFPROPERTY_RETAIN_DETAIL( \
        Type, Name, InitValueOrEmpty, \
        public, public)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_RETAIN_READONLY(Type, Name, InitValueOrEmpty) \
    ZFPROPERTY_RETAIN_DETAIL( \
        Type, Name, InitValueOrEmpty, \
        private, public)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_RETAIN_DETAIL( \
    Type, Name, InitValueOrEmpty, \
    SetterAccessType, GetterAccessType) \
        _ZFP_ZFPROPERTY_GETTER(GetterAccessType, Type, Name) \
        /** @brief see @ref Name */ \
        _ZFP_ZFPROPERTY_SETTER_RETAIN(SetterAccessType, Type, Name) \
        _ZFP_ZFPROPERTY_DECLARE_RETAIN(Type, ZFPropertyTypeId_ZFObject, Name, \
                                       InitValueOrEmpty) \
    public:

// ============================================================
/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN(Type, Name) \
    ZFPROPERTY_ASSIGN_DETAIL( \
        Type, Name, ZFPropertyNoInitValue, \
        public, public)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN_WITH_INIT(Type, Name, InitValueOrEmpty) \
    ZFPROPERTY_ASSIGN_DETAIL( \
        Type, Name, InitValueOrEmpty, \
        public, public)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN_READONLY(Type, Name, InitValueOrEmpty) \
    ZFPROPERTY_ASSIGN_DETAIL( \
        Type, Name, InitValueOrEmpty, \
        private, public)

/** @brief see #ZFPROPERTY_RETAIN */
#define ZFPROPERTY_ASSIGN_DETAIL( \
    Type, Name, InitValueOrEmpty, \
    SetterAccessType, GetterAccessType) \
        _ZFP_ZFPROPERTY_GETTER(GetterAccessType, Type, Name) \
        /** @brief see @ref Name */ \
        _ZFP_ZFPROPERTY_SETTER_ASSIGN(SetterAccessType, Type, Name) \
        _ZFP_ZFPROPERTY_DECLARE_ASSIGN(Type, ZFPropertyTypeIdData<zftTraits<Type>::TrNoRef>::PropertyTypeId(), Name, \
                                       InitValueOrEmpty) \
    public:

// ============================================================
/**
 * @brief see #ZFPROPERTY_RETAIN
 *
 * statically register a property\n
 * you can find the property in its owner class only if it's registered,
 * which can be achieved by any of these:
 * -  any instance of owner class has ever been created,
 *   by default, #ZFClass would do the registration work for you automatically
 *   if the owner class is not an abstract class
 * -  ZFPROPERTY_REGISTER is declared
 */
#define ZFPROPERTY_REGISTER(OwnerClass, Type, Name) \
    ZF_STATIC_REGISTER_INIT(PropR_##OwnerClass##_##Name) \
    { \
        (void)OwnerClass::_ZFP_Prop_##Name(); \
    } \
    ZF_STATIC_REGISTER_END(PropR_##OwnerClass##_##Name) \
    ZFMETHOD_REGISTER(OwnerClass, _ZFP_ZFPROPERTY_SETTER_NAME(Type, Name)) \
    ZFMETHOD_REGISTER(OwnerClass, _ZFP_ZFPROPERTY_GETTER_NAME(Type, Name))

// ============================================================
/**
 * @brief override life cycle callback for the property
 *
 * proto type:
 * @code
 *   // for retain property
 *   void propertyOnInit(
 *       ZF_IN_OUT zfautoObject [const] &propertyValue,
 *       ZF_IN zfautoObject const &propertyValueOld);
 *
 *   // for assign property
 *   void propertyOnInit(
 *       ZF_IN_OUT YourPropertyType [const] &propertyValue,
 *       ZF_IN YourPropertyType const &propertyValueOld);
 * @endcode
 *
 * we have these property life cycle events:
 * -  propertyOnInit: called when property first accessed\n
 *   you may change the propertyValue during this step
 *   to modify the property's init value
 * -  propertyOnDealloc: called when owner object deallocated
 *   (after #ZFObject::objectOnDeallocPrepare and before #ZFObject::objectOnDealloc)\n
 *   you may do additional cleanup steps here
 * -  propertyOnVerify: called when setter called\n
 *   you may verify the property value, and modify propertyValue to correct one
 * -  propertyOnAttach: called when property first accessed or setter called\n
 *   you may do additional attach steps here
 * -  propertyOnDetach: called when property setter called or owner object deallocated\n
 *   you may do additional detach steps here
 * -  propertyOnUpdate: called after property first accessed or setter called\n
 *   you may apply your impl accorrding to the property's new value
 *
 *
 * after override, for detach events (detach/dealloc),
 * subclass's one would be called before parent's one,
 * for others (init/verify/attach/update),
 * parent's one would be called first\n
 * \n
 * \n
 * here's a simple hint for the property's life cycle:
 * -  property first time accessed:
 *   -# store raw init value
 *   -# propertyOnInit
 *   -# propertyOnAttach
 *   -# propertyOnUpdate
 * -  property's setter called:
 *   -# propertyOnDetach
 *   -# propertyOnDealloc
 *   -# store raw value
 *   -# propertyOnVerify
 *   -# propertyOnAttach
 *   -# #ZFObject::EventObjectPropertyValueOnUpdate
 *   -# propertyOnUpdate
 * -  property's owner object deallocated:
 *   -# propertyOnDetach
 *   -# propertyOnDealloc
 *
 * @note if you override OnInit event from subclass of the property's owner class,
 *   the property's getter would be called during owner object's allocation
 *   to suit most logic case,
 *   which may cause unnecessary property allocation,
 *   to skip the step, use #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE_NO_AUTO_INIT instead
 */
#define ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(Type, Name) \
    _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, OnInit, ZFM_EMPTY, ZFM_EXPAND( \
            zfself::ClassData()->_ZFP_ZFClass_propertyAutoInitRegister(zfself::_ZFP_Prop_##Name()); \
            zfself::ClassData()->_ZFP_ZFClass_propertyInitStepRegister(zfself::_ZFP_Prop_##Name()); \
        ))
/** @brief see #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE */
#define ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE_NO_AUTO_INIT(Type, Name) \
    _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, OnInit, ZFM_EMPTY, ZFM_EXPAND( \
            zfself::ClassData()->_ZFP_ZFClass_propertyInitStepRegister(zfself::_ZFP_Prop_##Name()); \
        ))
/** @brief see #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE */
#define ZFPROPERTY_OVERRIDE_ON_INIT_DEFINE(OwnerClass, Type, Name) \
    _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DEFINE(OwnerClass, Type, Name, OnInit, ZFM_EMPTY)

/** @brief see #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE */
#define ZFPROPERTY_OVERRIDE_ON_DEALLOC_DECLARE(Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, OnDealloc, ZFM_EXPAND, ZFM_EMPTY())
/** @brief see #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE */
#define ZFPROPERTY_OVERRIDE_ON_DEALLOC_DEFINE(OwnerClass, Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DEFINE(OwnerClass, Type, Name, OnDealloc, ZFM_EXPAND)

/** @brief see #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE */
#define ZFPROPERTY_OVERRIDE_ON_VERIFY_DECLARE(Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, OnVerify, ZFM_EMPTY, ZFM_EMPTY())
/** @brief see #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE */
#define ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(OwnerClass, Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DEFINE(OwnerClass, Type, Name, OnVerify, ZFM_EMPTY)

/** @brief see #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE */
#define ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, OnAttach, ZFM_EXPAND, ZFM_EMPTY())
/** @brief see #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE */
#define ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(OwnerClass, Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DEFINE(OwnerClass, Type, Name, OnAttach, ZFM_EXPAND)

/** @brief see #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE */
#define ZFPROPERTY_OVERRIDE_ON_DETACH_DECLARE(Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, OnDetach, ZFM_EXPAND, ZFM_EMPTY())
/** @brief see #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE */
#define ZFPROPERTY_OVERRIDE_ON_DETACH_DEFINE(OwnerClass, Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DEFINE(OwnerClass, Type, Name, OnDetach, ZFM_EXPAND)

/** @brief see #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE */
#define ZFPROPERTY_OVERRIDE_ON_UPDATE_DECLARE(Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, OnUpdate, ZFM_EXPAND, ZFM_EMPTY())
/** @brief see #ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE */
#define ZFPROPERTY_OVERRIDE_ON_UPDATE_DEFINE(OwnerClass, Type, Name) \
     _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DEFINE(OwnerClass, Type, Name, OnUpdate, ZFM_EXPAND)

// ============================================================
#define _ZFP_ZFPROPERTY_DECLARE_REGISTER_RETAIN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                                propertyClassOfRetainProperty) \
    public: \
        static ZFProperty *_ZFP_Prop_##Name(void) \
        { \
            static _ZFP_ZFPropertyRegisterHolder _propertyInfoHolder(zffalse \
                    , zfself::ClassData() \
                    , ZFM_TOSTRING(Name) \
                    , ZFM_TOSTRING(Type) \
                    , ZFPropertyTypeId_noneOrType \
                    , ZFMethodAccess(zfself, _ZFP_ZFPROPERTY_SETTER_NAME(Type, Name)) \
                    , ZFMethodAccess(zfself, _ZFP_ZFPROPERTY_GETTER_NAME(Type, Name)) \
                    , propertyClassOfRetainProperty \
                    , &zfself::_ZFP_propCbAccessed_##Name \
                    , &zfself::_ZFP_propCbIsInit_##Name \
                    , &ZFPropertyCallbackCompareDefault<Type> \
                    , &ZFPropertyCallbackCopyDefault<Type> \
                    , &ZFPropertyCallbackRetainSetDefault<Type> \
                    , &ZFPropertyCallbackRetainGetDefault<Type> \
                    , zfnull \
                    , zfnull \
                    , &ZFPropertyCallbackGetInfoDefault<Type> \
                    , zfnull \
                    , zfself::_ZFP_propCbDel_##Name \
                ); \
            return _propertyInfoHolder.propertyInfo; \
        }
#define _ZFP_ZFPROPERTY_DECLARE_REGISTER_ASSIGN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                                propertyClassOfRetainProperty) \
    public: \
        static ZFProperty *_ZFP_Prop_##Name(void) \
        { \
            static _ZFP_ZFPropertyRegisterHolder _propertyInfoHolder(zffalse \
                    , zfself::ClassData() \
                    , ZFM_TOSTRING(Name) \
                    , ZFM_TOSTRING(Type) \
                    , ZFPropertyTypeId_noneOrType \
                    , ZFMethodAccess(zfself, _ZFP_ZFPROPERTY_SETTER_NAME(Type, Name)) \
                    , ZFMethodAccess(zfself, _ZFP_ZFPROPERTY_GETTER_NAME(Type, Name)) \
                    , propertyClassOfRetainProperty \
                    , zfself::_ZFP_propCbAccessed_##Name \
                    , zfself::_ZFP_propCbIsInit_##Name \
                    , &ZFPropertyCallbackCompareDefault<Type> \
                    , &ZFPropertyCallbackCopyDefault<Type> \
                    , zfnull \
                    , zfnull \
                    , &ZFPropertyCallbackAssignSetDefault<Type> \
                    , &ZFPropertyCallbackAssignGetDefault<Type> \
                    , &ZFPropertyCallbackGetInfoDefault<Type> \
                    , zfnull \
                    , zfself::_ZFP_propCbDel_##Name \
                ); \
            return _propertyInfoHolder.propertyInfo; \
        }

#define _ZFP_ZFPROPERTY_VALUE_DECLARE_RETAIN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                             InitValueOrEmpty) \
    public: \
        typedef Type _ZFP_PropVT_##Name; \
        typedef zfautoObject _ZFP_PropHT_##Name; \
    private: \
        zffinal zfclassNotPOD _ZFP_PropV_##Name \
        { \
        public: \
            _ZFP_PropV_##Name(void) \
            : value(zfnull) \
            { \
                zfself::_ZFP_Prop_##Name(); \
            } \
            ~_ZFP_PropV_##Name(void) \
            { \
                if(this->value != zfnull) \
                { \
                    zflockfree_zfRelease(*(this->value)); \
                    zfpoolDelete(this->value); \
                } \
            } \
        public: \
            zfself::_ZFP_PropVT_##Name &propertyInit(ZF_IN ZFObject *owner, \
                                                     ZF_IN_OPT zfbool needNotifyOwner = zftrue) \
            { \
                if(!(this->value)) \
                { \
                    zflockfree_zfRetain(*(this->value = zfpoolNew(zfself::_ZFP_PropVT_##Name, InitValueOrEmpty))); \
                    _ZFP_ZFPropertyLifeCycleCall_init_retain( \
                        zfself::_ZFP_Prop_##Name(), \
                        owner, \
                        ZFObjectToObject(*(this->value)), \
                        needNotifyOwner, \
                        rawValueStoreCallback, \
                        this); \
                } \
                return (*(this->value)); \
            } \
            inline zfbool propertyAccessed(void) const \
            { \
                return (this->value != zfnull); \
            } \
            void propertyDealloc(ZF_IN ZFObject *owner) \
            { \
                _ZFP_ZFPropertyLifeCycleCall_dealloc_retain( \
                    zfself::_ZFP_Prop_##Name(), \
                    owner, \
                    ZFObjectToObject(*(this->value))); \
                zfpoolDelete(this->value); \
                this->value = zfnull; \
            } \
            static void rawValueStoreCallback(ZF_IN void *rawValueStoreToken, ZF_IN ZFObject *value) \
            { \
                *(((_ZFP_PropV_##Name *)rawValueStoreToken)->value) = ZFCastZFObjectUnchecked(zfself::_ZFP_PropVT_##Name, value); \
            } \
        public: \
            zfself::_ZFP_PropVT_##Name *value; \
        }; \
        zfself::_ZFP_PropV_##Name Name##_PropV; \
    private: \
        static zfbool _ZFP_propCbIsInit_##Name(ZF_IN const ZFProperty *property, \
                                               ZF_IN ZFObject *ownerObj, \
                                               ZF_OUT_OPT void *outInitValue) \
        { \
            zfCoreMutexLocker(); \
            zfself *t = ZFCastZFObjectUnchecked(zfself *, ownerObj); \
            if(t->Name##_PropV.propertyAccessed()) \
            { \
                zfself::_ZFP_PropV_##Name _holder; \
                if(outInitValue != zfnull) \
                { \
                    *(zfself::_ZFP_PropHT_##Name *)outInitValue = \
                        ZFCastZFObjectUnchecked(ZFObject *, _holder.propertyInit(ownerObj, zffalse)); \
                } \
                return (ZFComparerDefault( \
                        t->_ZFP_ZFPROPERTY_GETTER_NAME(Type, Name)(), _holder.propertyInit(ownerObj, zffalse)) \
                    == ZFCompareTheSame); \
            } \
            else \
            { \
                return zftrue; \
            } \
        } \
    public:
#define _ZFP_ZFPROPERTY_VALUE_DECLARE_ASSIGN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                             InitValueOrEmpty) \
    public: \
        typedef Type _ZFP_PropVT_##Name; \
        typedef Type _ZFP_PropHT_##Name; \
    private: \
        zffinal zfclassNotPOD _ZFP_PropV_##Name \
        { \
        public: \
            _ZFP_PropV_##Name(void) \
            : value(zfnull) \
            { \
                zfself::_ZFP_Prop_##Name(); \
            } \
            ~_ZFP_PropV_##Name(void) \
            { \
                zfpoolDelete(this->value); \
            } \
        public: \
            zfself::_ZFP_PropVT_##Name &propertyInit(ZF_IN ZFObject *owner, \
                                                     ZF_IN_OPT zfbool needNotifyOwner = zftrue) \
            { \
                if(!(this->value)) \
                { \
                    this->value = zfpoolNew(zfself::_ZFP_PropVT_##Name, InitValueOrEmpty); \
                    _ZFP_ZFPropertyLifeCycleCall_init_assign( \
                        zfself::_ZFP_Prop_##Name(), \
                        owner, \
                        this->value, \
                        needNotifyOwner); \
                } \
                return (*(this->value)); \
            } \
            inline zfbool propertyAccessed(void) const \
            { \
                return (this->value != zfnull); \
            } \
            void propertyDealloc(ZF_IN ZFObject *owner) \
            { \
                _ZFP_ZFPropertyLifeCycleCall_dealloc_assign( \
                    zfself::_ZFP_Prop_##Name(), \
                    owner, \
                    this->value); \
                zfpoolDelete(this->value); \
                this->value = zfnull; \
            } \
            static void *rawValueStoreCallback(ZF_IN void *rawValueStoreToken, ZF_IN const void *value) \
            { \
                return &(*(((_ZFP_PropV_##Name *)rawValueStoreToken)->value) = *(const zfself::_ZFP_PropVT_##Name *)value); \
            } \
        private: \
            zfself::_ZFP_PropVT_##Name *value; \
        }; \
        zfself::_ZFP_PropV_##Name Name##_PropV; \
    private: \
        static zfbool _ZFP_propCbIsInit_##Name(ZF_IN const ZFProperty *property, \
                                               ZF_IN ZFObject *ownerObj, \
                                               ZF_OUT_OPT void *outInitValue) \
        { \
            zfCoreMutexLocker(); \
            zfself *t = ZFCastZFObjectUnchecked(zfself *, ownerObj); \
            if(t->Name##_PropV.propertyAccessed()) \
            { \
                zfself::_ZFP_PropV_##Name _holder; \
                if(outInitValue != zfnull) \
                { \
                    *(zfself::_ZFP_PropHT_##Name *)outInitValue = _holder.propertyInit(ownerObj, zffalse); \
                } \
                return (ZFComparerDefault( \
                        t->_ZFP_ZFPROPERTY_GETTER_NAME(Type, Name)(), _holder.propertyInit(ownerObj, zffalse)) \
                    == ZFCompareTheSame); \
            } \
            else \
            { \
                return zftrue; \
            } \
        } \
    public:

// ============================================================
#define _ZFP_ZFPROPERTY_DECLARE_CALLBACK(Type, Name) \
    private: \
        static zfbool _ZFP_propCbAccessed_##Name(ZF_IN const ZFProperty *property, \
                                                 ZF_IN ZFObject *ownerObj) \
        { \
            return ZFCastZFObjectUnchecked(zfself *, ownerObj)->Name##_PropV.propertyAccessed(); \
        } \
    private: \
        static void _ZFP_propCbDel_##Name(ZF_IN ZFObject *owner, ZF_IN const ZFProperty *property) \
        { \
            zfCoreMutexLocker(); \
            ZFCastZFObjectUnchecked(zfself *, owner)->Name##_PropV.propertyDealloc(owner); \
        } \
    public:

// ============================================================
#define _ZFP_ZFPROPERTY_DECLARE_RETAIN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                       InitValueOrEmpty) \
    _ZFP_ZFPROPERTY_DECLARE_REGISTER_RETAIN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                            zftTraits<Type>::TrType::ClassData()) \
    _ZFP_ZFPROPERTY_VALUE_DECLARE_RETAIN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                         InitValueOrEmpty) \
    _ZFP_ZFPROPERTY_DECLARE_CALLBACK(Type, Name)
#define _ZFP_ZFPROPERTY_DECLARE_ASSIGN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                       InitValueOrEmpty) \
    _ZFP_ZFPROPERTY_DECLARE_REGISTER_ASSIGN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                            zfnull) \
    _ZFP_ZFPROPERTY_VALUE_DECLARE_ASSIGN(Type, ZFPropertyTypeId_noneOrType, Name, \
                                         InitValueOrEmpty) \
    _ZFP_ZFPROPERTY_DECLARE_CALLBACK(Type, Name)

// ============================================================
#define _ZFP_ZFPROPERTY_SETTER_NAME(Type, Name) Name##Set
#define _ZFP_ZFPROPERTY_GETTER_NAME(Type, Name) Name

// ============================================================
#define _ZFP_ZFPROPERTY_SETTER_RETAIN(AccessType, Type, Name) \
    AccessType: \
        _ZFP_ZFMETHOD_INLINE( \
            notAutoRegister, \
            AccessType, ZFMethodIsVirtual, \
            void, _ZFP_ZFPROPERTY_SETTER_NAME(Type, Name), ZF_CALLER_LINE \
            , ZFMP_IN(Type const &, propertyValue) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            ) \
        { \
            zfCoreMutexLock(); \
            _ZFP_ZFPropertyLifeCycleCall_setter_retain( \
                zfself::_ZFP_Prop_##Name(), \
                this->toObject(), \
                Name##_PropV.propertyAccessed(), \
                ZFObjectToObject(Name##_PropV.propertyInit(this->toObject(), zffalse)), \
                ZFObjectToObject(propertyValue), \
                _ZFP_PropV_##Name::rawValueStoreCallback, \
                &Name##_PropV); \
            zfCoreMutexUnlock(); \
        } \
    public:
#define _ZFP_ZFPROPERTY_SETTER_ASSIGN(AccessType, Type, Name) \
    AccessType: \
        _ZFP_ZFMETHOD_INLINE( \
            notAutoRegister, \
            AccessType, ZFMethodIsVirtual, \
            void, _ZFP_ZFPROPERTY_SETTER_NAME(Type, Name), ZF_CALLER_LINE \
            , ZFMP_IN(Type const &, propertyValue) \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            ) \
        { \
            zfCoreMutexLock(); \
            Type valueOld = Name##_PropV.propertyInit(this->toObject(), zffalse); \
            _ZFP_ZFPropertyLifeCycleCall_setter_assign( \
                zfself::_ZFP_Prop_##Name(), \
                this->toObject(), \
                Name##_PropV.propertyAccessed(), \
                &valueOld, \
                &propertyValue, \
                _ZFP_PropV_##Name::rawValueStoreCallback, \
                &Name##_PropV); \
            zfCoreMutexUnlock(); \
        } \
    public:
#define _ZFP_ZFPROPERTY_GETTER(AccessType, Type, Name) \
    AccessType: \
        _ZFP_ZFMETHOD_INLINE( \
            notAutoRegister, \
            AccessType, ZFMethodIsVirtual, \
            Type const &, _ZFP_ZFPROPERTY_GETTER_NAME(Type, Name), ZF_CALLER_LINE \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            , _ZFP_ZFMP_DUMMY() \
            ) \
        { \
            zfCoreMutexLocker(); \
            return Name##_PropV.propertyInit(this->toObject()); \
        } \
    public:

// ============================================================
#define _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DECLARE(Type, Name, lifeCycleName, constFix, extraRegStep) \
    private: \
        zfclassNotPOD _ZFP_propLReg_##lifeCycleName##_##Name \
        { \
        public: \
            _ZFP_propLReg_##lifeCycleName##_##Name(void) \
            { \
                static zfbool needInit = zftrue; \
                if(needInit) \
                { \
                    needInit = zffalse; \
                    _ZFP_ZFPropertyLifeCycleRegister( \
                        ZFM_TOSTRING(lifeCycleName), \
                        zfself::_ZFP_Prop_##Name(), \
                        zfself::ClassData(), \
                        zfself::_ZFP_propLI_##lifeCycleName##_##Name); \
                    extraRegStep \
                } \
            } \
        }; \
        _ZFP_propLReg_##lifeCycleName##_##Name _ZFP_propLReg_##lifeCycleName##_##Name; \
        static void _ZFP_propLI_##lifeCycleName##_##Name( \
            ZF_IN ZFObject *propertyOwnerObject, \
            ZF_IN void *propertyValue, \
            ZF_IN const void *propertyValueOld) \
        { \
            ZFCastZFObjectUnchecked(zfself *, propertyOwnerObject)->zfself::_ZFP_propL_##lifeCycleName##_##Name( \
                *(constFix(const) zfself::_ZFP_PropHT_##Name *)propertyValue, \
                *(zfself::_ZFP_PropHT_##Name *)propertyValueOld); \
        } \
    public: \
        zffinal void _ZFP_propL_##lifeCycleName##_##Name( \
            ZF_IN zfself::_ZFP_PropHT_##Name constFix(const) &propertyValue, \
            ZF_IN zfself::_ZFP_PropHT_##Name const &propertyValueOld)
#define _ZFP_ZFPROPERTY_LIFE_CYCLE_OVERRIDE_DEFINE(OwnerClass, Type, Name, lifeCycleName, constFix) \
    void OwnerClass::_ZFP_propL_##lifeCycleName##_##Name( \
        ZF_IN zfself::_ZFP_PropHT_##Name constFix(const) &propertyValue, \
        ZF_IN zfself::_ZFP_PropHT_##Name const &propertyValueOld)

extern ZF_ENV_EXPORT void _ZFP_ZFPropertyLifeCycleRegister(ZF_IN const zfchar *lifeCycleName,
                                                           ZF_IN const ZFProperty *property,
                                                           ZF_IN const ZFClass *propertyOwnerClass,
                                                           ZF_IN _ZFP_PropLifeCycleWrapper propertyLifeCycleWrapper);
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyLifeCycleUnregister(ZF_IN const zfchar *lifeCycleName,
                                                             ZF_IN const ZFProperty *property,
                                                             ZF_IN const ZFClass *propertyOwnerClass);
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyLifeCycleCall_init_retain(ZF_IN const ZFProperty *property,
                                                                   ZF_IN ZFObject *propertyOwnerObject,
                                                                   ZF_IN ZFObject *value,
                                                                   ZF_IN zfbool needNotifyOwner,
                                                                   ZF_IN void (*rawValueStoreCallback)(ZF_IN void *rawValueStoreToken, ZF_IN ZFObject *value),
                                                                   ZF_IN void *rawValueStoreToken);
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyLifeCycleCall_init_assign(ZF_IN const ZFProperty *property,
                                                                   ZF_IN ZFObject *propertyOwnerObject,
                                                                   ZF_IN void *value,
                                                                   ZF_IN zfbool needNotifyOwner);
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyLifeCycleCall_dealloc_retain(ZF_IN const ZFProperty *property,
                                                                      ZF_IN ZFObject *propertyOwnerObject,
                                                                      ZF_IN ZFObject *value);
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyLifeCycleCall_dealloc_assign(ZF_IN const ZFProperty *property,
                                                                      ZF_IN ZFObject *propertyOwnerObject,
                                                                      ZF_IN void *value);
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyLifeCycleCall_setter_retain(ZF_IN const ZFProperty *property,
                                                                     ZF_IN ZFObject *propertyOwnerObject,
                                                                     ZF_IN zfbool accessed,
                                                                     ZF_IN ZFObject *propertyValueOld,
                                                                     ZF_IN ZFObject *propertyValueNew,
                                                                     ZF_IN void (*rawValueStoreCallback)(ZF_IN void *rawValueStoreToken, ZF_IN ZFObject *value),
                                                                     ZF_IN void *rawValueStoreToken);
extern ZF_ENV_EXPORT void _ZFP_ZFPropertyLifeCycleCall_setter_assign(ZF_IN const ZFProperty *property,
                                                                     ZF_IN ZFObject *propertyOwnerObject,
                                                                     ZF_IN zfbool accessed,
                                                                     ZF_IN void *propertyValueOld,
                                                                     ZF_IN const void *propertyValueNew,
                                                                     ZF_IN void *(*rawValueStoreCallback)(ZF_IN void *rawValueStoreToken, ZF_IN const void *value),
                                                                     ZF_IN void *rawValueStoreToken);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyDeclare_h_

