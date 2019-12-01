/**
 * @file ZFAni.h
 * @brief animation util based on time line
 */

#ifndef _ZFI_ZFAni_h_
#define _ZFI_ZFAni_h_

#include "ZFAnimationTimeLine.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief util to start a #ZFAnimationTimeLine based animation quickly
 *
 * usage:
 * @code
 *   ZFLISTENER_LOCAL(aniImpl, {
 *           zffloat progress = listenerData.param0<v_zffloat *>()->zfv;
 *           yourAniImpl(progress, listenerData.sender(), userData);
 *       })
 *   ZFAni(target, aniImpl, userData)->aniStart();
 * @endcode
 * aniImpl's param0 is #v_zffloat that holds progress
 */
ZFMETHOD_FUNC_DECLARE_3(zfautoObjectT<ZFAnimationTimeLine *>, ZFAni,
                        ZFMP_IN(ZFObject *, target),
                        ZFMP_IN(const ZFListener &, aniImpl),
                        ZFMP_IN_OPT(ZFObject *, userData, zfnull))

// ============================================================
/**
 * @brief util to start property animation based on #ZFAnimationTimeLine
 *
 * usage:
 * @code
 *   ZFAni(target, "yourProp", from, to)->aniStart();
 * @endcode
 * -  yourPropName can be:
 *   -  name of #ZFProperty
 *   -  name of #ZFMethod,
 *     but target must have proper setter and getter method
 *     similar to #ZFProperty
 * -  from and to value can be:
 *   -  proper #ZFObject value for retain property,
 *     and the property value should be #ZFProgressable
 *   -  proper #ZFTypeIdWrapper for assign property,
 *     and the type of the property must be registered by
 *     #ZFTYPEID_PROGRESS_DEFINE
 *   -  string that can be serialized by #ZFSerializable::serializeFromString
 */
ZFMETHOD_FUNC_DECLARE_4(zfautoObjectT<ZFAnimationTimeLine *>, ZFAni,
                        ZFMP_IN(ZFObject *, target),
                        ZFMP_IN(const zfchar *, name),
                        ZFMP_IN(const zfchar *, from),
                        ZFMP_IN(const zfchar *, to))
/** @brief see #ZFAni */
ZFMETHOD_FUNC_DECLARE_4(zfautoObjectT<ZFAnimationTimeLine *>, ZFAni,
                        ZFMP_IN(ZFObject *, target),
                        ZFMP_IN(const zfchar *, name),
                        ZFMP_IN(ZFObject *, from),
                        ZFMP_IN(ZFObject *, to))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFAni_h_

