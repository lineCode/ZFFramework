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
 * @file ZFObjectRetain.h
 * @brief retain count logic for ZFFramework
 */

#ifndef _ZFI_ZFObjectRetain_h_
#define _ZFI_ZFObjectRetain_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfAlloc
/* ZFMETHOD_MAX_PARAM */
template<typename T_ZFObject, int valid>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_Obj_AllocCk;
template<typename T_ZFObject>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_Obj_AllocCk<T_ZFObject, 1>
{
public:
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj)
    {
        obj->objectOnInit();
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0)
    {
        obj->objectOnInit(p0);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1)
    {
        obj->objectOnInit(p0, p1);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1, typename P2>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1, P2 p2)
    {
        obj->objectOnInit(p0, p1, p2);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1, P2 p2, P3 p3)
    {
        obj->objectOnInit(p0, p1, p2, p3);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3, typename P4>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4)
    {
        obj->objectOnInit(p0, p1, p2, p3, p4);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
    {
        obj->objectOnInit(p0, p1, p2, p3, p4, p5);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
    {
        obj->objectOnInit(p0, p1, p2, p3, p4, p5, p6);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
    template<typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
    static inline T_ZFObject *CanAlloc(T_ZFObject *obj, P0 p0, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
    {
        obj->objectOnInit(p0, p1, p2, p3, p4, p5, p6, p7);
        obj->_ZFP_ZFObjectCheckOnInit();
        return obj;
    }
};
template<typename T_ZFObject>
zfclassNotPOD ZF_ENV_EXPORT _ZFP_Obj_AllocCk<T_ZFObject, 0>
{
};
#define _ZFP_zfAlloc(T_ZFObject, ...) \
    _ZFP_Obj_AllocCk<T_ZFObject, T_ZFObject::_ZFP_ZFObjectCanAlloc && T_ZFObject::_ZFP_ZFObjectCanAllocPublic>::CanAlloc( \
        _ZFP_ObjCastInternal(T_ZFObject *, T_ZFObject::_ZFP_Obj_ctor()) \
        , ##__VA_ARGS__)
/**
 * @brief alloc an object, see #ZFObject
 */
#define zfAlloc(T_ZFObject, ...) \
    (zfCoreMutexLockerHolder(), zflockfree_zfAlloc(T_ZFObject, ##__VA_ARGS__))
/** @brief no lock version of #zfAlloc, use with causion */
#define zflockfree_zfAlloc(T_ZFObject, ...) \
    _ZFP_zfAlloc(T_ZFObject, ##__VA_ARGS__)

// ============================================================
// zfRetain
inline void _ZFP_zfRetainAction(ZF_IN ZFObject *obj)
{
    if(obj)
    {
        obj->objectOnRetain();
    }
}
template<typename T_ZFObject>
inline T_ZFObject _ZFP_zfRetain(ZF_IN T_ZFObject obj)
{
    _ZFP_zfRetainAction(ZFCastZFObjectUnchecked(ZFObject *, obj));
    return obj;
}
/**
 * @brief retain an object, see #ZFObject
 */
#define zfRetain(obj) \
    (zfCoreMutexLockerHolder(), zflockfree_zfRetain(obj))
/** @brief no lock version of #zfRetain, use with causion */
#define zflockfree_zfRetain(obj) \
    _ZFP_zfRetain(obj)

// ============================================================
// zfRelease
inline void _ZFP_zfReleaseAction(ZF_IN ZFObject *obj)
{
    if(obj)
    {
        obj->objectOnRelease();
        if(obj->objectRetainCount() == 0)
        {
            ZFObject::_ZFP_ZFObjectDealloc(obj);
        }
    }
}
template<typename T_ZFObject>
inline void _ZFP_zfRelease(ZF_IN T_ZFObject obj)
{
    _ZFP_zfReleaseAction(ZFCastZFObjectUnchecked(ZFObject *, obj));
}
/**
 * @brief release an object, see #ZFObject
 */
#define zfRelease(obj) \
    (zfCoreMutexLockerHolder(), zflockfree_zfRelease(obj))
/** @brief no lock version of #zfRelease, use with causion */
#define zflockfree_zfRelease(obj) \
    _ZFP_zfRelease(obj)

// ============================================================
/**
 * @brief util to release property's old value, retain new value, then set to property
 *
 * this macro is similar to the retain property in Object-C\n
 * typical usage:
 * @code
 *   ZFObject *property = ...;
 *   ZFObject *newProperty = ...;
 *
 *   // OK, release property, retain newProperty, then set to property
 *   zfRetainChange(property, newProperty);
 *
 *   // OK, use return value of a function as new value
 *   // but keep it in mind, that the new value will be retained
 *   zfRetainChange(property, funcThatReturnZFObject());
 *
 *   // OK, same as release old property and set it to zfnull
 *   zfRetainChange(property, zfnull);
 *
 *   // error, new value must be ZFObject
 *   // zfRetainChange(property, 123);
 *
 *   // error, property must be a variable contains a (ZFObject *)
 *   // zfRetainChange(zfnull, newProperty);
 *   // zfRetainChange(funcThatReturnZFObject(), newProperty);
 * @endcode
 * @see zfRetain, zfRelease, ZFPROPERTY_RETAIN
 */
#define zfRetainChange(property, propertyValue) \
    do \
    { \
        zfCoreMutexLock(); \
        ZFAny _ZFP_zfRetainChangeTmpValue = property; \
        zflockfree_zfRetain(property = propertyValue); \
        zflockfree_zfRelease(_ZFP_zfRetainChangeTmpValue.toObject()); \
        zfCoreMutexUnlock(); \
    } while(zffalse)
/** @brief no lock version of #zfRetainChange, use with causion */
#define zflockfree_zfRetainChange(property, propertyValue) \
    do \
    { \
        ZFAny _ZFP_zfRetainChangeTmpValue = property; \
        zflockfree_zfRetain(property = propertyValue); \
        zflockfree_zfRelease(_ZFP_zfRetainChangeTmpValue.toObject()); \
    } while(zffalse)
/** @see zfRetainChange */
#define zfRetainChange(property, propertyValue) \
    do \
    { \
        zfCoreMutexLock(); \
        ZFAny _ZFP_zfRetainChangeTmpValue = property; \
        zflockfree_zfRetain(property = propertyValue); \
        zflockfree_zfRelease(_ZFP_zfRetainChangeTmpValue.toObject()); \
        zfCoreMutexUnlock(); \
    } while(zffalse)
/** @brief no lock version of #zfRetainChange, use with causion */
#define zflockfree_zfRetainChange(property, propertyValue) \
    do \
    { \
        ZFAny _ZFP_zfRetainChangeTmpValue = property; \
        zflockfree_zfRetain(property = propertyValue); \
        zflockfree_zfRelease(_ZFP_zfRetainChangeTmpValue.toObject()); \
    } while(zffalse)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFObjectRetain_h_

