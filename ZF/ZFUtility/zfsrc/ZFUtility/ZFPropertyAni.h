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
 * @file ZFPropertyAni.h
 * @brief update property by animation
 */

#ifndef _ZFI_ZFPropertyAni_h_
#define _ZFI_ZFPropertyAni_h_

#include "ZFTimeLineCurve.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief property animation settings for #ZFPropertyAniBegin
 */
zfclass ZF_ENV_EXPORT ZFPropertyAniSetting : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFPropertyAniSetting, ZFObject)
    ZFOBJECT_SINGLETON_DECLARE(zfself, instance)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when property animation stop,
     * would called even if no actual animation started
     */
    ZFOBSERVER_EVENT(PropertyAniOnStop)

public:
    /**
     * @brief ani duration in mili seconds, 250 by default
     * @note this property would automatically copy from #instance
     *   when #ZFPropertyAniBegin
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniDuration, 250)
    /**
     * @brief ani curve, null to use linear curve, null by default
     * @note this property would automatically copy from #instance
     *   when #ZFPropertyAniBegin
     */
    ZFPROPERTY_RETAIN(ZFTimeLineCurve *, aniCurve)
    /**
     * @brief ani duration in mili seconds, 35 by default
     * @note this property would automatically copy from #instance
     *   when #ZFPropertyAniBegin
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniInterval, 35)
    /**
     * @brief filter for object
     */
    ZFPROPERTY_ASSIGN(ZFFilterForZFObject, aniObjectFilter)
    /**
     * @brief filter for property
     */
    ZFPROPERTY_ASSIGN(ZFFilterForZFProperty, aniPropertyFilter)

public:
    zfoverride
    virtual zfbool objectIsPrivate(void) {return zftrue;}
    zfoverride
    virtual zfbool objectIsInternal(void) {return zftrue;}
};

/**
 * @brief start property animation
 *
 * with ZFFramework's powerful reflection mechanism,
 * we are able to perform a stateful property animation\n
 * typical usage:
 * @code
 *   // mark to start the animation
 *   ZFPropertyAniSetting *setting = ZFPropertyAniBegin();
 *   // you may change settings here
 *   setting->aniCurveSet(xxx);
 *
 *   // update your property freely
 *   myObj->myPropertySet(123);
 *
 *   // finally, commit the animation
 *   ZFPropertyAniEnd();
 *
 *   // or, you may use the util macro
 *   {
 *       ZFPropertyAniBlock(setting);
 *
 *       // update your property freely
 *       myObj->myPropertySet(123);
 *   } // animation would be commit after code block
 * @endcode
 * (note: the property's type must be registered by #ZFTYPEID_PROGRESS_DECLARE)\n
 * \n
 * the animation block can be safely embeded:
 * @code
 *   myObj->myPropertySet(0);
 *
 *   ZFPropertyAniBegin();
 *   myObj->myPropertySet(123);
 *
 *       ZFPropertyAniBegin();
 *       myObj->myPropertySet(456);
 *       ZFPropertyAniEnd();
 *
 *   myObj->myPropertySet(789);
 *   ZFPropertyAniEnd();
 * @endcode
 * the above code would result only `myObj->myPropertySet(789);` to be animated,
 * the previous would be ignored
 */
ZFMETHOD_FUNC_DECLARE_0(ZFPropertyAniSetting *, ZFPropertyAniBegin)
/**
 * @brief see #ZFPropertyAniBegin
 *
 * return true if any animation actually started
 */
ZFMETHOD_FUNC_DECLARE_0(zfbool, ZFPropertyAniEnd)

/**
 * @brief get current property ani setting,
 *   return null if not started by #ZFPropertyAniBegin
 */
ZFMETHOD_FUNC_DECLARE_0(ZFPropertyAniSetting *, ZFPropertyAniState)

/**
 * @brief see #ZFPropertyAniBegin
 */
#define ZFPropertyAniBlock(setting) \
    ZFPropertyAniSetting *setting = ZFPropertyAniBegin(); \
    ZFUNUSED(setting); \
    _ZFP_ZFPropertyAniBlock ZFUniqueName(PropAni_##setting)

zfclassNotPOD _ZFP_ZFPropertyAniBlock
{
public:
    ~_ZFP_ZFPropertyAniBlock(void)
    {
        ZFPropertyAniEnd();
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFPropertyAni_h_

