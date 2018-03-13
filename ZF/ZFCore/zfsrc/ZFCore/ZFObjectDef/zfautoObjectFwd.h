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
 * @file zfautoObjectFwd.h
 * @brief smart pointer for ZFObject
 */

#ifndef _ZFI_zfautoObjectFwd_h_
#define _ZFI_zfautoObjectFwd_h_

#include "ZFAny.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// zfautoObject
zfclassNotPOD ZF_ENV_EXPORT _ZFP_zfautoObjectPrivate
{
public:
    zfuint refCount;
    ZFObject *obj;
public:
    _ZFP_zfautoObjectPrivate(void) : refCount(1), obj(zfnull) {}
};

/**
 * @brief a ZFObject holder which would release content object automatically when destroyed
 *
 * useful to hold a ZFObject as temp return value:
 * @code
 *   zfautoObject yourFunc(void)
 *   {
 *       // alloc an object for return
 *       zfblockedAlloc(ZFObject, obj);
 *
 *       // use zfautoObject to wrap the returned object
 *       return zfautoObject(obj);
 *   }
 *
 *   {
 *       zfautoObject value = yourFunc();
 *       ZFObject *obj = value;
 *   } // content obj would be released when zfautoObject destroyed
 * @endcode
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT zfautoObject
{
    /** @cond ZFPrivateDoc */
public:
    zfautoObject(void) : d(zfnull) {}
    zfautoObject(ZF_IN zfautoObject const &ref);
    zfautoObject(ZF_IN zft_zfnull obj) : d(zfnull) {(void)obj;}
    template<typename T_ZFObject>
    zfautoObject(ZF_IN T_ZFObject *obj);
    template<typename T_ZFObject>
    zfautoObject(ZF_IN T_ZFObject const &obj);
    ~zfautoObject(void);

public:
    zfautoObject &operator = (ZF_IN zfautoObject const &ref);
    zfautoObject &operator = (ZF_IN zft_zfnull obj);
    template<typename T_ZFObject>
    zfautoObject &operator = (ZF_IN T_ZFObject *obj);
    template<typename T_ZFObject>
    zfautoObject &operator = (ZF_IN T_ZFObject const &obj);

public:
    zfbool operator == (ZF_IN zft_zfnull obj) const
    {
        (void)obj;
        return (this->toObject() == zfnull);
    }
    zfbool operator != (ZF_IN zft_zfnull obj) const
    {
        (void)obj;
        return (this->toObject() != zfnull);
    }
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject *obj) const
    {
        return (this->toObject() == (obj ? obj->toObject() : zfnull));
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject *obj) const
    {
        return (this->toObject() != (obj ? obj->toObject() : zfnull));
    }
    template<typename T_ZFObject>
    zfbool operator == (ZF_IN T_ZFObject const &obj) const
    {
        return (this->toObject() == obj.toObject());
    }
    template<typename T_ZFObject>
    zfbool operator != (ZF_IN T_ZFObject const &obj) const
    {
        return (this->toObject() != obj.toObject());
    }
    /** @endcond */

public:
    /**
     * @brief get current retain count
     */
    zfindex objectRetainCount(void) const
    {
        return (zfindex)(d ? d->refCount : 0);
    }

public:
    ZFObject *operator -> (void) const
    {
        return this->toObject();
    }
    template<typename T_ZFObject>
    operator T_ZFObject * (void) const
    {
        return ZFCastZFObject(T_ZFObject *, this->toObject());
    }
    /**
     * @brief get the holded object
     */
    inline ZFObject *toObject(void) const
    {
        return (d ? d->obj : zfnull);
    }
    /**
     * @brief get the holded object
     */
    template<typename T_ZFObject>
    inline T_ZFObject to(void) const
    {
        return ZFCastZFObjectUnchecked(T_ZFObject, this->toObject());
    }

private:
    _ZFP_zfautoObjectPrivate *d;
};

extern ZF_ENV_EXPORT const zfautoObject _ZFP_zfautoObjectNull;
/**
 * @brief global null zfautoObject
 */
#define zfautoObjectNull() _ZFP_zfautoObjectNull

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_zfautoObjectFwd_h_

