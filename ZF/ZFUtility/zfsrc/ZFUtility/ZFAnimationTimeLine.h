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
 * @file ZFAnimationTimeLine.h
 * @brief abstract animation based on time line
 */

#ifndef _ZFI_ZFAnimationTimeLine_h_
#define _ZFI_ZFAnimationTimeLine_h_

#include "ZFAnimation.h"
#include "ZFTimeLineCurve.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassFwd _ZFP_ZFAnimationTimeLinePrivate;
/**
 * @brief abstract animation based on time line
 *
 * serializable data:
 * @code
 *   <ZFAnimationTimeLine />
 * @endcode
 */
zfabstract ZF_ENV_EXPORT ZFAnimationTimeLine : zfextends ZFAnimation
{
    ZFOBJECT_DECLARE_ABSTRACT(ZFAnimationTimeLine, ZFAnimation)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * param0 is a float #ZFValue containing current time line progress
     */
    ZFOBSERVER_EVENT(AniTimeLineOnUpdate)

    // ============================================================
    // object
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    zfoverride
    virtual zfidentity objectHash(void);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

    // ============================================================
    // property
public:
    /**
     * @brief curve for time line, null to use linear time line, null by default
     */
    ZFPROPERTY_RETAIN(ZFTimeLineCurve *, aniTimeLineCurve)
    /**
     * @brief interval to update time line, in miliseconds, 30 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniTimeLineInterval, 30)
    /**
     * @brief whether time line callback should be called in main thread, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, aniTimeLineNotifyInMainThread, zftrue)

    // ============================================================
    // start stop
protected:
    zfoverride
    virtual void aniImplStart(void);
    zfoverride
    virtual void aniImplStop(void);

protected:
    /**
     * @brief called to do the actual update
     *
     * note, progress is based on #aniTimeLineCurve,
     * which typically has value in range [0, 1] as base value,
     * but may exceeds the range for bounce curve
     */
    virtual void aniTimeLineOnUpdate(ZF_IN zffloat progress) zfpurevirtual;

private:
    _ZFP_ZFAnimationTimeLinePrivate *d;
    friend zfclassFwd _ZFP_ZFAnimationTimeLinePrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAnimationTimeLine_h_

