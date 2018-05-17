/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIPageRequest.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFUIPageRequestPageCreateParam, ZFUIPageRequestPageCreateParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIPageRequestPageCreateParam, ZFUIPage * const &, page)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIPageRequestPageCreateParam, void, pageSet, ZFMP_IN(ZFUIPage * const &, page))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIPageRequestPageCreateParam, ZFObject * const &, pageCreateParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIPageRequestPageCreateParam, void, pageCreateParamSet, ZFMP_IN(ZFObject * const &, pageCreateParam))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFUIPageRequestPageCreateParam, zfbool const &, pageAutoResume)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFUIPageRequestPageCreateParam, void, pageAutoResumeSet, ZFMP_IN(zfbool const &, pageAutoResume))

// ============================================================
// request
ZFOBJECT_REGISTER(ZFUIPageRequest)
ZFOBJECT_REGISTER(ZFUIPageRequestPageCreate)
ZFOBJECT_REGISTER(ZFUIPageRequestPageResume)
ZFOBJECT_REGISTER(ZFUIPageRequestPageGroupResume)
ZFOBJECT_REGISTER(ZFUIPageRequestPageDestroy)
ZFOBJECT_REGISTER(ZFUIPageRequestCustom)

// ============================================================
ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_VAR(ZFUIPageRequestPageCreate, ZFUIPageRequestPageCreateParam, createParam)

ZF_NAMESPACE_GLOBAL_END

