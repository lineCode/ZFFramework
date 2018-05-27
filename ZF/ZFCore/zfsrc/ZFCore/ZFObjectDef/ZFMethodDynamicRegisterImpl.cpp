/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFMethodDynamicRegister.h"
#include "ZFObjectImpl.h"
#include "ZFMethodFuncDeclare.h"

#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFMethodDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFMethodDynamicRegisterAutoRemove)
{
    zfstlmap<const ZFMethod *, zfbool> t;
    t.swap(m);
    for(zfstlmap<const ZFMethod *, zfbool>::iterator it = t.begin(); it != t.end(); ++it)
    {
        _ZFP_ZFMethodUnregister(it->first);
    }
}
zfstlmap<const ZFMethod *, zfbool> m;
ZF_GLOBAL_INITIALIZER_END(ZFMethodDynamicRegisterAutoRemove)

// ============================================================
const ZFMethod *ZFMethodDynamicRegister(ZF_IN const ZFMethodDynamicRegisterParam &param,
                                        ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(param.methodOwnerClass() != zfnull && param.methodNamespace() != zfnull)
    {
        zfstringAppend(errorHint,
            zfText("methodOwnerClass(%s) and methodNamespace(%s) can not both set"),
            param.methodOwnerClass()->objectInfo().cString(),
            param.methodNamespace());
        return zfnull;
    }
    if(param.methodGenericInvoker() == zfnull)
    {
        zfstringAppend(errorHint, zfText("methodGenericInvoker not set"));
        return zfnull;
    }
    if(param.methodOwnerClass() == zfnull
        && (param.methodType() != ZFMethodTypeStatic || param.methodPrivilegeType() != ZFMethodPrivilegeTypePublic))
    {
        zfstringAppend(errorHint,
            zfText("function type method must specified as public static, but got %s %s"),
            ZFMethodTypeToString(param.methodType()).cString(),
            ZFMethodPrivilegeTypeToString(param.methodPrivilegeType()).cString());
        return zfnull;
    }
    if(param.methodName() == zfnull)
    {
        zfstringAppend(errorHint, zfText("methodName not set"));
        return zfnull;
    }
    if(param.methodReturnTypeId() == zfnull)
    {
        zfstringAppend(errorHint, zfText("methodReturnTypeId not set"));
        return zfnull;
    }
    for(zfindex i = 0; i < param.methodParamCount(); ++i)
    {
        if(param.methodParamDefaultValueCallbackAtIndex(i) != zfnull)
        {
            for( ; i < param.methodParamCount(); ++i)
            {
                if(param.methodParamDefaultValueCallbackAtIndex(i) == zfnull)
                {
                    zfstringAppend(errorHint,
                        zfText("param %zi has no default value by previous param has"),
                        i);
                }
            }
            break;
        }
    }
    const ZFMethod *existMethod = zfnull;
    if(param.methodOwnerClass() != zfnull)
    {
        existMethod = param.methodOwnerClass()->methodForNameIgnoreParent(
                param.methodName()
                , param.methodParamTypeIdAtIndex(0)
                , param.methodParamTypeIdAtIndex(1)
                , param.methodParamTypeIdAtIndex(2)
                , param.methodParamTypeIdAtIndex(3)
                , param.methodParamTypeIdAtIndex(4)
                , param.methodParamTypeIdAtIndex(5)
                , param.methodParamTypeIdAtIndex(6)
                , param.methodParamTypeIdAtIndex(7)
            );
    }
    else
    {
        existMethod = ZFMethodFuncGet(
                param.methodNamespace()
                , param.methodName()
                , param.methodParamTypeIdAtIndex(0)
                , param.methodParamTypeIdAtIndex(1)
                , param.methodParamTypeIdAtIndex(2)
                , param.methodParamTypeIdAtIndex(3)
                , param.methodParamTypeIdAtIndex(4)
                , param.methodParamTypeIdAtIndex(5)
                , param.methodParamTypeIdAtIndex(6)
                , param.methodParamTypeIdAtIndex(7)
            );
    }
    if(existMethod != zfnull)
    {
        zfstringAppend(errorHint,
            zfText("method with same sig already exists: %s"),
            existMethod->objectInfo().cString());
        return zfnull;
    }

    const zfchar *methodType = zfText("");
    switch(param.methodType())
    {
        case ZFMethodTypeNormal:
            methodType = _ZFP_ZFMethodTypeText(ZFMethodTypeNormal);
            break;
        case ZFMethodTypeStatic:
            methodType = _ZFP_ZFMethodTypeText(ZFMethodTypeStatic);
            break;
        case ZFMethodTypeVirtual:
            methodType = _ZFP_ZFMethodTypeText(ZFMethodTypeVirtual);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }

    zfCoreMutexLocker();
    const ZFMethod *method = _ZFP_ZFMethodRegister(zffalse
            , zftrue
            , param.methodDynamicRegisterUserData()
            , zfnull
            , param.methodGenericInvoker()
            , methodType
            , param.methodOwnerClass()
            , param.methodPrivilegeType()
            , param.methodNamespace() ? param.methodNamespace() : ZFMethodFuncNamespaceGlobal
            , zfText("MtdDynReg")
            , param.methodName()
            , param.methodReturnTypeId()
            , param.methodReturnTypeName()
            , param.methodParamTypeIdAtIndex(0), param.methodParamTypeNameAtIndex(0), param.methodParamDefaultValueCallbackAtIndex(0)
            , param.methodParamTypeIdAtIndex(1), param.methodParamTypeNameAtIndex(1), param.methodParamDefaultValueCallbackAtIndex(1)
            , param.methodParamTypeIdAtIndex(2), param.methodParamTypeNameAtIndex(2), param.methodParamDefaultValueCallbackAtIndex(2)
            , param.methodParamTypeIdAtIndex(3), param.methodParamTypeNameAtIndex(3), param.methodParamDefaultValueCallbackAtIndex(3)
            , param.methodParamTypeIdAtIndex(4), param.methodParamTypeNameAtIndex(4), param.methodParamDefaultValueCallbackAtIndex(4)
            , param.methodParamTypeIdAtIndex(5), param.methodParamTypeNameAtIndex(5), param.methodParamDefaultValueCallbackAtIndex(5)
            , param.methodParamTypeIdAtIndex(6), param.methodParamTypeNameAtIndex(6), param.methodParamDefaultValueCallbackAtIndex(6)
            , param.methodParamTypeIdAtIndex(7), param.methodParamTypeNameAtIndex(7), param.methodParamDefaultValueCallbackAtIndex(7)
            , zfnull
        );
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMethodDynamicRegisterAutoRemove)->m[method] = zftrue;
    return method;
}
void ZFMethodDynamicUnregister(ZF_IN const ZFMethod *method)
{
    if(method != zfnull)
    {
        zfCoreAssert(method->methodIsDynamicRegister());
        zfCoreMutexLocker();
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFMethodDynamicRegisterAutoRemove)->m.erase(method);
        _ZFP_ZFMethodUnregister(method);
    }
}

ZF_NAMESPACE_GLOBAL_END

