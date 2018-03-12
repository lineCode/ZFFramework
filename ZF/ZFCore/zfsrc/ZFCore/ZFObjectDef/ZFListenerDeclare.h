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
 * @file ZFListenerDeclare.h
 * @brief util macros to declare #ZFListener as #ZFCallback
 */

#ifndef _ZFI_ZFListenerDeclare_h_
#define _ZFI_ZFListenerDeclare_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief wrapper for #ZFMETHOD_INLINE_2, see #ZFListener
 *
 * proto type:\n
 * virtual void listenerName(ZF_IN const ZFListenerData &listenerData, ZFObject *userData);
 */
#define ZFLISTENER_INLINE(listenerName) \
    ZFMETHOD_INLINE_2(void, listenerName, \
                      ZFMP_IN(const ZFListenerData &, listenerData), \
                      ZFMP_IN(ZFObject *, userData))
/** @brief see #ZFLISTENER_INLINE */
#define ZFLISTENER_DECLARE(listenerName) \
    ZFMETHOD_DECLARE_2(void, listenerName, \
                       ZFMP_IN(const ZFListenerData &, listenerData), \
                       ZFMP_IN(ZFObject *, userData))
/** @brief see #ZFLISTENER_INLINE */
#define ZFLISTENER_DEFINE(OwnerClass, listenerName) \
    ZFMETHOD_DEFINE_2(OwnerClass, void, listenerName, \
                      ZFMP_IN(const ZFListenerData &, listenerData), \
                      ZFMP_IN(ZFObject *, userData))

/**
 * @brief util macro to expand as\n
 *   void methodPlaceholder(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *, userData)
 */
#define ZFLISTENER_PROTOTYPE_EXPAND(methodPlaceholder) \
    void methodPlaceholder(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject * userData)

/** @brief see #ZFLISTENER_INLINE */
#define ZFLISTENER_LOCAL_BEGIN(listenerName) \
    ZFCALLBACK_LOCAL_BEGIN_2(void, listenerName, const ZFListenerData &, listenerData, ZFObject *, userData)
/** @brief see #ZFLISTENER_INLINE */
#define ZFLISTENER_LOCAL_END(listenerName) \
    ZFCALLBACK_LOCAL_END_WITH_TYPE(ZFListener, listenerName)
/** @brief see #ZFLISTENER_INLINE */
#define ZFLISTENER_LOCAL(listenerName, listenerContent) \
    ZFLISTENER_LOCAL_BEGIN(listenerName) \
    listenerContent \
    ZFLISTENER_LOCAL_END(listenerName)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFListenerDeclare_h_

