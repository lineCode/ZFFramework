/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFMethodGenericInvoker.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN
/* ZFMETHOD_MAX_PARAM */

// ============================================================
zfclass _ZFP_I_ZFMethodGenericInvokerDefaultParamType : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFMethodGenericInvokerDefaultParamType, ZFObject)
};
ZFObject *_ZFP_ZFMethodGenericInvokerDefaultParamRef = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMethodGenericInvokerDefaultParamInit, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFMethodGenericInvokerDefaultParamRef = zfAlloc(_ZFP_I_ZFMethodGenericInvokerDefaultParamType);
    _ZFP_ZFMethodGenericInvokerDefaultParamHolderRef = _ZFP_ZFMethodGenericInvokerDefaultParamRef;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMethodGenericInvokerDefaultParamInit)
{
    zfRelease(_ZFP_ZFMethodGenericInvokerDefaultParamRef);
    _ZFP_ZFMethodGenericInvokerDefaultParamRef = zfnull;

    _ZFP_ZFMethodGenericInvokerDefaultParamHolderRef = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFMethodGenericInvokerDefaultParamInit)

zfautoObject _ZFP_ZFMethodGenericInvokerDefaultParamHolderRef;

zfbool _ZFP_MtdGIParamCheck(ZF_OUT_OPT zfstring *errorHint,
                            ZF_IN zfbool accessAvailable,
                            ZF_IN const ZFMethod *invokerMethod,
                            ZF_IN zfindex paramIndex,
                            ZF_IN const zfchar *paramType,
                            ZF_IN ZFObject *param)
{
    if((param != ZFMethodGenericInvokerDefaultParam() && !accessAvailable)
        || (param == ZFMethodGenericInvokerDefaultParam() && paramIndex < invokerMethod->methodParamDefaultBeginIndex()))
    {
        zfstringAppend(errorHint,
                zfText("[ZFMethodGenericInvoker] unable to access param%zi as type (%s): %s"),
                paramIndex,
                paramType,
                ZFObjectInfo(param).cString()
            );
        return zffalse;
    }
    else
    {
        return zftrue;
    }
}
void _ZFP_MtdGIRetError(ZF_OUT_OPT zfstring *errorHint,
                        ZF_IN const zfchar *returnTypeId,
                        ZF_IN const zfchar *returnValueInfo)
{
    zfstringAppend(errorHint,
        zfText("[ZFMethodGenericInvoker] unable to convert return value as type %s: %s"),
        returnTypeId,
        returnValueInfo);
}

// ============================================================
zfbool _ZFP_ZFMethodGenericInvoke(ZF_IN const ZFMethod *invokerMethod
                                  , ZF_IN ZFObject *invokerObject
                                  , ZF_OUT_OPT zfstring *errorHint
                                  , ZF_OUT zfautoObject &ret
                                  , ZF_IN_OUT zfautoObject (&paramList)[ZFMETHOD_MAX_PARAM]
                                  )
{
    return invokerMethod->methodGenericInvoker()(
            invokerMethod
            , invokerObject
            , errorHint
            , ret
            , paramList
        );
}
void _ZFP_ZFMethodGenericInvokeError(ZF_IN const ZFMethod *method,
                                     ZF_IN ZFObject *obj,
                                     ZF_IN zfint pos,
                                     ZF_IN_OPT const zfchar *errorHint /* = zfnull */)
{
    if(pos >= 0)
    {
        zfCoreCriticalMessageTrim(
            zfTextA("[ZFMethodDynamicRegister] method %s unable to convert param %d"),
            zfsCoreZ2A(method->objectInfo().cString()),
            pos);
    }
    else if(pos == -1)
    {
        zfCoreCriticalMessageTrim(
            zfTextA("[ZFMethodDynamicRegister] method %s unable to perform generic invoker, reason: %s"),
            zfsCoreZ2A(method->objectInfo().cString()),
            errorHint);
    }
    else
    {
        zfCoreCriticalMessageTrim(
            zfTextA("[ZFMethodDynamicRegister] method %s unable to convert return value"),
            zfsCoreZ2A(method->objectInfo().cString()));
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_0({return ZFMethodGenericInvokerDefaultParam();}, ZFObject *, ZFMethodGenericInvokerDefaultParam)

ZF_NAMESPACE_GLOBAL_END
#endif

