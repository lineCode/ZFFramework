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
#include "ZFMethodDeclare.h"
#include "ZFMethodFuncDeclare.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief wrapper for #ZFMETHOD_INLINE_2, see #ZFListener
 *
 * proto type:\n
 * virtual void listenerName(ZF_IN_OUT ZFListenerData &listenerData, ZFObject *userData);
 */
#define ZFLISTENER_INLINE(listenerName) \
    ZFMETHOD_INLINE_2(void, listenerName, \
                      ZFMP_IN_OUT(ZFListenerData &, listenerData), \
                      ZFMP_IN(ZFObject *, userData))
/** @brief see #ZFLISTENER_INLINE */
#define ZFLISTENER_DECLARE(listenerName) \
    ZFMETHOD_DECLARE_2(void, listenerName, \
                       ZFMP_IN_OUT(ZFListenerData &, listenerData), \
                       ZFMP_IN(ZFObject *, userData))
/** @brief see #ZFLISTENER_INLINE */
#define ZFLISTENER_DEFINE(OwnerClass, listenerName) \
    ZFMETHOD_DEFINE_2(OwnerClass, void, listenerName, \
                      ZFMP_IN_OUT(ZFListenerData &, listenerData), \
                      ZFMP_IN(ZFObject *, userData))

/**
 * @brief util macro to expand as\n
 *   void methodPlaceholder(ZF_IN_OUT ZFListenerData &listenerData, ZF_IN ZFObject *, userData)
 */
#define ZFLISTENER_PROTOTYPE_EXPAND(methodPlaceholder) \
    void methodPlaceholder(ZF_IN_OUT ZFListenerData &listenerData, ZF_IN ZFObject * userData)

/** @brief see #ZFLISTENER_INLINE */
#define ZFLISTENER_LOCAL_BEGIN(listenerName) \
    ZFCALLBACK_LOCAL_BEGIN_2(void, listenerName, ZFListenerData &, listenerData, ZFObject *, userData)
/** @brief see #ZFLISTENER_INLINE */
#define ZFLISTENER_LOCAL_END(listenerName) \
    ZFCALLBACK_LOCAL_END_WITH_TYPE(ZFListener, listenerName)
/** @brief see #ZFLISTENER_INLINE */
#define ZFLISTENER_LOCAL(listenerName, listenerContent) \
    ZFLISTENER_LOCAL_BEGIN(listenerName) \
    listenerContent \
    ZFLISTENER_LOCAL_END(listenerName)

// ============================================================
/**
 * @brief declare #ZFListener with #ZFLAMBDA_0
 */
#define ZFLISTENER_LAMBDA_0(name \
    , lambdaBody) \
    ZFLAMBDA_0(name \
        , lambdaBody, void \
        , ZFM_EXPAND(ZFMP_IN_OUT(ZFListenerData &, listenerData)) \
        , ZFM_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_1(name \
    , CaptureParam0, capture0 \
    , lambdaBody) \
    ZFLAMBDA_1(name \
        , CaptureParam0, capture0 \
        , lambdaBody, void \
        , ZFM_EXPAND(ZFMP_IN_OUT(ZFListenerData &, listenerData)) \
        , ZFM_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_2(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , lambdaBody) \
    ZFLAMBDA_2(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , lambdaBody, void \
        , ZFM_EXPAND(ZFMP_IN_OUT(ZFListenerData &, listenerData)) \
        , ZFM_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_3(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , lambdaBody) \
    ZFLAMBDA_3(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , lambdaBody, void \
        , ZFM_EXPAND(ZFMP_IN_OUT(ZFListenerData &, listenerData)) \
        , ZFM_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_4(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , lambdaBody) \
    ZFLAMBDA_4(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , lambdaBody, void \
        , ZFM_EXPAND(ZFMP_IN_OUT(ZFListenerData &, listenerData)) \
        , ZFM_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_5(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , lambdaBody) \
    ZFLAMBDA_5(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , lambdaBody, void \
        , ZFM_EXPAND(ZFMP_IN_OUT(ZFListenerData &, listenerData)) \
        , ZFM_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_6(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , lambdaBody) \
    ZFLAMBDA_6(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , CaptureParam5, capture5 \
        , lambdaBody, void \
        , ZFM_EXPAND(ZFMP_IN_OUT(ZFListenerData &, listenerData)) \
        , ZFM_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_7(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , lambdaBody) \
    ZFLAMBDA_7(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , CaptureParam5, capture5 \
        , CaptureParam6, capture6 \
        , lambdaBody, void \
        , ZFM_EXPAND(ZFMP_IN_OUT(ZFListenerData &, listenerData)) \
        , ZFM_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

/** @brief see #ZFLISTENER_LAMBDA_0 */
#define ZFLISTENER_LAMBDA_8(name \
    , CaptureParam0, capture0 \
    , CaptureParam1, capture1 \
    , CaptureParam2, capture2 \
    , CaptureParam3, capture3 \
    , CaptureParam4, capture4 \
    , CaptureParam5, capture5 \
    , CaptureParam6, capture6 \
    , CaptureParam7, capture7 \
    , lambdaBody) \
    ZFLAMBDA_8(name \
        , CaptureParam0, capture0 \
        , CaptureParam1, capture1 \
        , CaptureParam2, capture2 \
        , CaptureParam3, capture3 \
        , CaptureParam4, capture4 \
        , CaptureParam5, capture5 \
        , CaptureParam6, capture6 \
        , CaptureParam7, capture7 \
        , lambdaBody, void \
        , ZFM_EXPAND(ZFMP_IN_OUT(ZFListenerData &, listenerData)) \
        , ZFM_EXPAND(ZFMP_IN(ZFObject *, userData)) \
        )

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFListenerDeclare_h_

