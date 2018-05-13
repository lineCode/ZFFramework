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
 * @file ZFStyleable.h
 * @brief styleable
 */

#ifndef _ZFI_ZFStyleable_h_
#define _ZFI_ZFStyleable_h_

#include "ZFSerializable.h"
#include "ZFCopyable.h"
#include "ZFObjectObserver.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFStyleable
zfclassFwd _ZFP_I_ZFStyleable_PropertyTypeHolder;
zfclassFwd _ZFP_ZFStyleKeyHolder;
/**
 * @brief styleable element that can apply style from another object
 *
 * a styleable typically is a serializable,
 * while serializable process data from and to #ZFSerializableData,
 * a styleable process data from and to other styleable object\n
 * \n
 * to use styleable, simply implements from #ZFStyleable,
 * if all of your properties are declared as #ZFProperty,
 * then every thing's done,
 * and style can be copied by #ZFStyleable::styleableCopyFrom\n
 * for a list of copy method, please refer to #ZFStyleable::styleableOnCheckPropertyType
 */
zfinterface ZF_ENV_EXPORT ZFStyleable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE_WITH_CUSTOM_CTOR(ZFStyleable, ZFInterface)

public:
    /**
     * @brief return default style of this instance
     *
     * note this method use reflection to find the default style
     * (see #ZFSTYLE_DEFAULT_DECLARE),
     * cache it first if necessary
     */
    virtual ZFStyleable *defaultStyle(void);

public:
    /**
     * @brief copy style from another styleable, see #ZFStyleable
     *
     * usually you should not override this method,
     * override #styleableOnCopyFrom instead
     */
    zffinal void styleableCopyFrom(ZF_IN ZFStyleable *anotherStyleable);

public:
    /**
     * @brief true if this object is #defaultStyle
     */
    virtual zfbool styleableIsDefaultStyle(void)
    {
        return zffalse;
    }

protected:
    /** @brief see #ZFStyleable::styleableOnCheckPropertyType */
    typedef enum {
        PropertyTypeNotStyleable, /**< @brief see #ZFStyleable::styleableOnCheckPropertyType */
        PropertyTypeNormal, /**< @brief see #ZFStyleable::styleableOnCheckPropertyType */
        PropertyTypeStyleable, /**< @brief see #ZFStyleable::styleableOnCheckPropertyType */
        PropertyTypeCopyable, /**< @brief see #ZFStyleable::styleableOnCheckPropertyType */
    } PropertyType;
    /**
     * @brief check property type
     *
     * property type shows how styleable would act while copying from another styleable:
     * -  PropertyTypeNotStyleable:
     *   shows the property should not be copied
     * -  PropertyTypeNormal:
     *   shows the property should be copied normally,
     *   i.e. use getter and setter to copy value
     * -  PropertyTypeStyleable:
     *   shows the property itself is styleable,
     *   we would access its value and copy style by #styleableCopyFrom\n
     *   if the property is both #ZFStyleable and #ZFCopyable,
     *   it would be treated as #ZFStyleable
     * -  PropertyTypeCopyable:
     *   shows the property itself is copyable,
     *   we would access its value and copy style by #ZFCopyable::copyFrom
     *
     * \n
     * by default, a property is treated as not copyable if matches any of conditions below:
     * -  getter or setter method is private
     *
     * a property is treated as styleable/copyable property if matches all of conditions below:
     * -  is retain property and the type is ZFStyleable/ZFCopyable
     * -  getter method is not private and setter method is private
     *
     * all other property are treated as normal property
     */
    virtual ZFStyleable::PropertyType styleableOnCheckPropertyType(ZF_IN const ZFProperty *property);
    /**
     * @brief copy property with styleable logic
     *
     * @note anotherStyleable not ensured to be same type as self,
     *   while the property is ensured to be same and safe to copy
     */
    virtual void styleableOnCopyPropertyFrom(ZF_IN ZFStyleable *anotherStyleable,
                                             ZF_IN const ZFProperty *property,
                                             ZF_IN ZFStyleable::PropertyType propertyType);
    /**
     * @brief for subclass to achieve custom style copy step,
     *   called by #styleableCopyFrom, see #ZFStyleable
     */
    virtual inline void styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
    {
    }

private:
    zffinal _ZFP_I_ZFStyleable_PropertyTypeHolder *_ZFP_ZFStyleable_getPropertyTypeHolder(void);

public:
    /** @brief see #ZFStyleSet */
    zffinal zfbool styleKeySet(ZF_IN const zfchar *styleKey);
    /** @brief see #ZFStyleSet */
    zffinal const zfchar *styleKey(void);
    /** @brief see #ZFStyleSet */
    zffinal zfbool styleKeySet(ZF_IN const ZFProperty *property, ZF_IN const zfchar *styleKey);
    /** @brief see #ZFStyleSet */
    zffinal const zfchar *styleKey(ZF_IN const ZFProperty *property);
    /** @brief see #ZFStyleSet */
    zffinal zfbool styleKeySet(ZF_IN const zfchar *propertyName, ZF_IN const zfchar *styleKey)
    {return this->styleKeySet(this->classData()->propertyForName(propertyName), styleKey);}
    /** @brief see #ZFStyleSet */
    zffinal const zfchar *styleKey(ZF_IN const zfchar *propertyName)
    {return this->styleKey(this->classData()->propertyForName(propertyName));}
protected:
    /**
     * @brief called when #styleKeySet or associated style changed
     *
     * if the style is not valid,
     * the styleKey would be reset to null
     */
    virtual inline zfbool styleKeyOnCheckValid(void)
    {
        return zftrue;
    }
private:
    _ZFP_ZFStyleKeyHolder *_ZFP_styleKey;
    friend zfclassFwd _ZFP_ZFStyleKeyHolder;
protected:
    /** @cond ZFPrivateDoc */
    ZFStyleable(void) : _ZFP_styleKey(zfnull) {}
    /** @endcond */
    ZFINTERFACE_ON_DEALLOC_DECLARE();
};

// ============================================================
// ZFStyleableObject
/**
 * @brief common styleable object
 *
 * implements #ZFStyleable, #ZFSerializable, #ZFCopyable\n
 * every style, serialize and copy logic has been done by reflect,
 * if all of your properties are declared as #ZFProperty
 */
zfclass ZF_ENV_EXPORT ZFStyleableObject : zfextends ZFObject, zfimplements ZFStyleable, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFStyleableObject, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFStyleable, ZFSerializable, ZFCopyable)

protected:
    zfoverride
    virtual void copyableOnCopyFrom(ZF_IN ZFObject *anotherObj)
    {
        zfsuperI(ZFCopyable)::copyableOnCopyFrom(anotherObj);
        this->styleableCopyFrom(ZFCastZFObjectUnchecked(zfself *, anotherObj));
    }
};

// ============================================================
// DefaultStyle
#define _ZFP_ZFStyleableDefault_level ZFLevelZFFrameworkHigh
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFStyleableDefaultPointerHolder
{
public:
    void *d;
    _ZFP_ZFStyleableDefaultPointerHolder(void) : d(zfnull) {}
};

extern ZF_ENV_EXPORT _ZFP_ZFStyleableDefaultPointerHolder *_ZFP_ZFStyleableDefaultRefAccess(ZF_IN const zfchar *name);
typedef void (*_ZFP_ZFStyleableDefaultDeleteCallback)(ZF_IN void *instance);
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFStyleableDefaultDeleteCallbackHolder
{
public:
    _ZFP_ZFStyleableDefaultDeleteCallbackHolder(ZF_IN _ZFP_ZFStyleableDefaultDeleteCallback deleteCallback,
                                                ZF_IN void *instance)
    : deleteCallback(deleteCallback)
    , instance(instance)
    {
    }
    ~_ZFP_ZFStyleableDefaultDeleteCallbackHolder(void)
    {
        if(this->deleteCallback && this->instance)
        {
            this->deleteCallback(this->instance);
        }
    }
private:
    _ZFP_ZFStyleableDefaultDeleteCallback deleteCallback;
    void *instance;
};

#define _ZFP_ZFSTYLE_DEFAULT_DECLARE(YourStyle) \
    public: \
        zfclass _ZFP_ZFStyleableDefault_##YourStyle; \
    public: \
        /** \n default style for @ref YourStyle */ \
        static YourStyle *DefaultStyle(void); \
        /** @brief default style for @ref YourStyle (reflectable) */ \
        static zfautoObject DefaultStyleReflect(void); \
    private: \
        static void _ZFP_ZFStyleablEnumDefaultStyleSet(ZF_IN YourStyle *newInstance); \
        static ZFCorePointerBase *&_ZFP_ZFStyleableDefaultCleaner(void); \
        static void _ZFP_ZFStyleableDefaultOnDelete(ZF_IN void *instance); \
    public:
#define _ZFP_ZFSTYLE_DEFAULT_DEFINE(YourStyle) \
    zfclass YourStyle::_ZFP_ZFStyleableDefault_##YourStyle : zfextends YourStyle \
    { \
        ZFOBJECT_DECLARE(_ZFP_ZFStyleableDefault_##YourStyle, YourStyle) \
    public: \
        zfoverride \
        virtual zfbool styleableIsDefaultStyle(void) \
        { \
            return zftrue; \
        } \
    }; \
    YourStyle *YourStyle::DefaultStyle(void) \
    { \
        zfCoreMutexLocker(); \
        static _ZFP_ZFStyleableDefaultPointerHolder *holder = _ZFP_ZFStyleableDefaultRefAccess(ZFM_TOSTRING(YourStyle)); \
        if(holder->d == zfnull) \
        { \
            if(ZFFrameworkStateCheck(_ZFP_ZFStyleableDefault_level) == ZFFrameworkStateNotAvailable) \
            { \
                return zfnull; \
            } \
            zfautoObject obj = _ZFP_ZFStyleableDefault_##YourStyle::ClassData()->newInstance(); \
            if(obj != zfnull) \
            { \
                zfself::_ZFP_ZFStyleablEnumDefaultStyleSet(obj.to<YourStyle *>()); \
            } \
        } \
        return ZFCastStatic(YourStyle *, holder->d); \
    } \
    zfautoObject YourStyle::DefaultStyleReflect(void) \
    { \
        return ZFCastZFObjectUnchecked(ZFObject *, zfself::DefaultStyle()); \
    } \
    ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_STATIC_0(YourStyle, zfautoObject, DefaultStyleReflect) \
    void YourStyle::_ZFP_ZFStyleablEnumDefaultStyleSet(ZF_IN YourStyle *newInstance) \
    { \
        if(ZFFrameworkStateCheck(_ZFP_ZFStyleableDefault_level) == ZFFrameworkStateNotAvailable) \
        { \
            return ; \
        } \
        _ZFP_ZFStyleableDefaultPointerHolder *holder = _ZFP_ZFStyleableDefaultRefAccess(ZFM_TOSTRING(YourStyle)); \
        if(holder->d == newInstance) \
        { \
            return ; \
        } \
        ZFCorePointerBase *&cleanerRef = _ZFP_ZFStyleableDefaultCleaner(); \
        ZFCorePointerBase *cleanerOld = cleanerRef; \
        cleanerRef = zfnull; \
        if(newInstance != zfnull) \
        { \
            holder->d = zfRetain(newInstance); \
            cleanerRef = ZFObjectGlobalInstanceAdd(ZFCorePointerForObject<_ZFP_ZFStyleableDefaultDeleteCallbackHolder *>( \
                zfnew(_ZFP_ZFStyleableDefaultDeleteCallbackHolder, YourStyle::_ZFP_ZFStyleableDefaultOnDelete, holder->d)), \
                _ZFP_ZFStyleableDefault_level); \
        } \
        if(cleanerOld != zfnull) \
        { \
            ZFObjectGlobalInstanceRemove(cleanerOld, _ZFP_ZFStyleableDefault_level); \
        } \
    } \
    ZFCorePointerBase *&YourStyle::_ZFP_ZFStyleableDefaultCleaner(void) \
    { \
        static ZFCorePointerBase *_cleaner = zfnull; \
        return _cleaner; \
    } \
    void YourStyle::_ZFP_ZFStyleableDefaultOnDelete(ZF_IN void *instance) \
    { \
        _ZFP_ZFStyleableDefaultPointerHolder *holder = _ZFP_ZFStyleableDefaultRefAccess(ZFM_TOSTRING(YourStyle)); \
        if(holder->d == instance) \
        { \
            holder->d = zfnull; \
        } \
        YourStyle *tmp = ZFCastStatic(YourStyle *, instance); \
        zfRelease(tmp); \
    }

/**
 * @brief used to declare a default style, see #ZFStyleable
 *
 * usage:
 * @code
 *   // in h file
 *   zfclass YourStyleableObject : zfextends ParentStyleable
 *   {
 *       ZFOBJECT_DECLARE(YourStyleableObject, ParentStyleable)
 *       ZFSTYLE_DEFAULT_DECLARE(YourStyleableObject)
 *   };
 *
 *   // in cpp file
 *   ZFSTYLE_DEFAULT_DEFINE(YourStyleableObject)
 * @endcode
 * the macros above declare these types for you:
 * -  default style access method:
 *   static YourObjectStyle *DefaultStyle(void);
 *   virtual ZFStyleable *defaultStyle(void);
 * -  default style access method as ZFMethod (reflectable):
 *   static zfautoObject DefaultStyleReflect(void);
 *
 * \n
 * @note a style holder object would be created automatically when access the default style,
 *   and would be deleted automatically during #ZFFrameworkCleanup with level #ZFLevelZFFrameworkHigh,
 *   so it would result null if you access DefaultStyle before initialized or after it's deallocated
 * @note the default style holder object should only be used to hold the styles,
 *   typically you should not use it directly
 * @note it is useful to combine styleable logic and serializable logic,
 *   see #ZFStyleSet for more info
 * @note for performance, the default style won't be copied to its instance by default,
 *   you may use #ZFClass::instanceObserverAdd to observe styleable object's
 *   instance alloc event, and apply your default styles\n
 *   or use #ZFStyleDefaultApplyAutoCopy or #ZFSTYLE_DEFAULT_AUTO_COPY to make the auto copy
 */
#define ZFSTYLE_DEFAULT_DECLARE(YourStyle) \
    _ZFP_ZFSTYLE_DEFAULT_DECLARE(YourStyle)
/** @brief see #ZFSTYLE_DEFAULT_DECLARE */
#define ZFSTYLE_DEFAULT_DEFINE(YourStyle) \
    _ZFP_ZFSTYLE_DEFAULT_DEFINE(YourStyle)

/**
 * @brief util method to setup automatically copy style from
 *   #ZFStyleable::defaultStyle if its property value changed
 *
 * for performance, the default style won't be copied to each instance by default,
 * by calling this method, when #ZFStyleable::defaultStyle's property value changed,
 * it would be copied to the registered instance
 */
extern ZF_ENV_EXPORT void ZFStyleDefaultApplyAutoCopy(ZF_IN ZFStyleable *style);

/**
 * @brief util macro to setup automatically copy style from
 *   #ZFStyleable::defaultStyle
 *
 * use #ZFINTERFACE_ON_INIT_DECLARE to copy styles during init,
 * and use #ZFStyleDefaultApplyAutoCopy to ensure default style value changes would be applied
 */
#define ZFSTYLE_DEFAULT_AUTO_COPY() \
    ZFINTERFACE_ON_INIT_DECLARE() \
    { \
        if(!this->styleableIsDefaultStyle()) \
        { \
            ZFStyleable *defaultStyle = this->defaultStyle(); \
            if(defaultStyle != zfnull) \
            { \
                this->styleableCopyFrom(defaultStyle); \
                ZFStyleDefaultApplyAutoCopy(this); \
            } \
        } \
    }

// ============================================================
// style holder
/**
 * @brief used to store style holder
 *
 * typical style logic are implemented by:
 * 1. use #ZFStyleable::styleKeySet to attach object to observe style change
 * 1. use #ZFStyleSet or #ZFStyleLoad to modify styles
 * 1. during style change event,
 *   all the styles would be copied automatically
 *
 * example:
 * @code
 *   // register
 *   MyStyleObject *obj = xxx;
 *   obj->styleKeySet(zfText("MyStyle/MyStyleObject"));
 *
 *   // change style
 *   ZFStyleChangeBegin();
 *       ZFStyleSet(zfText("MyStyle/MyStyleObject"), xxx);
 *       ZFStyleChangeBegin(); // can be embeded, but must be paired
 *           ZFStyleSet(zfText("xxx"), xxx);
 *       ZFStyleChangeEnd();
 *   ZFStyleChangeEnd();
 *
 *   // use ZFStyleLoad is recommended
 *   ZFStyleLoad(xxx);
 * @endcode
 * \n
 * \n
 * the style logic can also be used during serialization,
 * when serializable contains key #ZFSerializableKeyword_styleKey,
 * the style would be copied during serialization, example:
 * @code
 *   <MyStyleObject styleKey="MyStyle/MyStyleObject" />
 * @endcode
 * during serialization, the styleKey would be stored as #ZFStyleable::styleKey,
 * causing the referenced style copied to the target object\n
 * \n
 * by default, all #ZFStyleable supports style logic,
 * for non-ZFStyleable properties,
 * you may supply #ZFSTYLE_PROPERTY_COPY_DEFINE to register your own copy action,
 * example:
 * @code
 *   // register
 *   ZFSTYLE_PROPERTY_COPY_DEFINE(ZFPropertyTypeId_YourPropertyTypeId, {
 *           // proto type:
 *           //   zfbool copyFrom(ZF_IN ZFObject *propertyOwner,
 *           //                   ZF_IN const ZFProperty *property,
 *           //                   ZF_IN ZFStyleable *styleValue);
 *           ... // do your copy action
 *           return zftrue;
 *       })
 *
 *   // attach style
 *   myObject->styleKeySet(ZFPropertyAccess(MyObject, myStyleProperty), zfText("myStyleKey"));
 * @endcode
 * \n
 * \n
 * note, by default, all styles would be cleared during #ZFFrameworkCleanup as level #ZFLevelZFFrameworkNormal
 */
extern ZF_ENV_EXPORT void ZFStyleSet(ZF_IN const zfchar *styleKey, ZF_IN ZFStyleable *styleValue);
/**
 * @brief see #ZFStyleSet
 */
extern ZF_ENV_EXPORT ZFStyleable *ZFStyleGet(ZF_IN const zfchar *styleKey);
/**
 * @brief get all styles, for debug use only, see #ZFStyleSet
 */
extern ZF_ENV_EXPORT void ZFStyleGetAll(ZF_IN_OUT ZFCoreArrayPOD<const zfchar *> &styleKey,
                                        ZF_IN_OUT ZFCoreArrayPOD<ZFStyleable *> &styleValue);
/**
 * @brief remove all styles, see #ZFStyleSet
 */
extern ZF_ENV_EXPORT void ZFStyleRemoveAll(void);

/**
 * @brief see #ZFStyleSet
 */
extern ZF_ENV_EXPORT void ZFStyleChangeBegin();
/**
 * @brief see #ZFStyleSet
 */
extern ZF_ENV_EXPORT void ZFStyleChangeEnd();

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFStyleChangeEnd
 */
ZFOBSERVER_EVENT_GLOBAL(ZFStyleOnChange)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when setting an invalid style value\n
 * sender is the styleable object that changing the styleKey,
 * param0 is a #ZFPointerHolder to #ZFProperty if chaning property
 * or null if changing the styleable object itself,
 * param1 is a #ZFPointerHolder to (const zfchar *)
 * that holds the new styleKey\n
 * \n
 * by default, we would assert fail if invalid style occurred,
 * you may close it by #ZFStyleInvalidCheckDisable
 */
ZFOBSERVER_EVENT_GLOBAL(ZFStyleOnInvalid)
ZF_NAMESPACE_END(ZFGlobalEvent)

/** @brief see #ZFGlobalEvent::EventZFStyleOnInvalid */
extern ZF_ENV_EXPORT void ZFStyleInvalidCheckDisable(void);

// ============================================================
/**
 * @brief see #ZFStyleSet
 */
#define ZFSTYLE_PROPERTY_COPY_DEFINE(propertyTypeIdSig, stylePropertyCopyAction) \
    ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(StylePropCp_##propertyTypeIdSig, ZFLevelZFFrameworkNormal) \
    { \
        _ZFP_ZFStylePropertyCopyRegister(ZFPropertyTypeId_##propertyTypeIdSig(), zfself::action); \
    } \
    ZF_GLOBAL_INITIALIZER_DESTROY(StylePropCp_##propertyTypeIdSig) \
    { \
        _ZFP_ZFStylePropertyCopyUnregister(ZFPropertyTypeId_##propertyTypeIdSig()); \
    } \
    static zfbool action(ZF_IN ZFObject *propertyOwner, \
                         ZF_IN const ZFProperty *property, \
                         ZF_IN ZFStyleable *styleValue) \
    { \
        stylePropertyCopyAction \
    } \
    ZF_GLOBAL_INITIALIZER_END(StylePropCp_##propertyTypeIdSig)
typedef zfbool (*_ZFP_ZFStylePropertyCopyCallback)(ZF_IN ZFObject *propertyOwner,
                                                   ZF_IN const ZFProperty *property,
                                                   ZF_IN ZFStyleable *styleValue);
extern ZF_ENV_EXPORT void _ZFP_ZFStylePropertyCopyRegister(ZF_IN const zfchar *propertyTypeId,
                                                           ZF_IN _ZFP_ZFStylePropertyCopyCallback callback);
extern ZF_ENV_EXPORT void _ZFP_ZFStylePropertyCopyUnregister(ZF_IN const zfchar *propertyTypeId);

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFStyleable_h_

