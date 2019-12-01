#include "ZFUIPageRequest.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFUIPageRequestPageCreateParam, ZFUIPageRequestPageCreateParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIPageRequestPageCreateParam, ZFUIPage *, page)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIPageRequestPageCreateParam, ZFObject *, pageCreateParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFUIPageRequestPageCreateParam, zfbool, pageAutoResume)

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

