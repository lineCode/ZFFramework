/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFTypeIdDynamicRegister.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTypeIdDynamic, ZFLevelZFFrameworkStatic)
{
}
zfstlmap<zfstlstringZ, ZFCorePointerForObject<ZFTypeIdBase *> > m;
ZF_GLOBAL_INITIALIZER_END(ZFTypeIdDynamic)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFTypeIdDynamicRegisterAutoRemove, ZFLevelZFFrameworkHigh)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFTypeIdDynamicRegisterAutoRemove)
{
    zfstlmap<zfstlstringZ, ZFCorePointerForObject<ZFTypeIdBase *> > &m = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTypeIdDynamic)->m;
    if(!m.empty())
    {
        zfstlmap<zfstlstringZ, ZFCorePointerForObject<ZFTypeIdBase *> > t;
        t.swap(m);
        for(zfstlmap<zfstlstringZ, ZFCorePointerForObject<ZFTypeIdBase *> >::iterator it = t.begin(); it != t.end(); ++it)
        {
            _ZFP_ZFTypeIdUnregister(it->first.c_str());
        }
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFTypeIdDynamicRegisterAutoRemove)

// ============================================================
static zfbool _ZFP_ZFTypeIdGI(ZFMETHOD_GENERIC_INVOKER_PARAMS)
{
    ret = zflineAlloc(v_zfstring, invokerMethod->methodName() + zfslen("ZFTypeId_"));
    return zftrue;
}
zfbool ZFTypeIdDynamicRegister(ZF_IN const zfchar *typeIdName,
                               ZF_IN const ZFCorePointerForObject<ZFTypeIdBase *> &typeIdData,
                               ZF_OUT_OPT zfstring *errorHint /* = zfnull */)
{
    if(zfsIsEmpty(typeIdName))
    {
        zfstringAppend(errorHint, "empty typeIdName");
        return zffalse;
    }
    if(typeIdData == zfnull)
    {
        zfstringAppend(errorHint, "null typeIdData");
        return zffalse;
    }
    ZF_GLOBAL_INITIALIZER_CLASS(ZFTypeIdDynamic) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTypeIdDynamic);
    if(d->m.find(typeIdName) != d->m.end())
    {
        zfstringAppend(errorHint, "type id %s already registered", typeIdName);
        return zffalse;
    }
    if(!ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
            .methodGenericInvokerSet(_ZFP_ZFTypeIdGI)
            .methodReturnTypeIdSet(ZFTypeId_zfstring())
            .methodNameSet(zfstringWithFormat("ZFTypeId_%s", typeIdName))
        , errorHint))
    {
        return zffalse;
    }
    d->m[typeIdName] = typeIdData;
    _ZFP_ZFTypeIdRegister(typeIdName, typeIdData);
    return zftrue;
}
void ZFTypeIdDynamicUnregister(ZF_IN const zfchar *typeIdName)
{
    if(!zfsIsEmpty(typeIdName))
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFTypeIdDynamic) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFTypeIdDynamic);
        zfstlmap<zfstlstringZ, ZFCorePointerForObject<ZFTypeIdBase *> >::iterator it = d->m.find(typeIdName);
        if(it != d->m.end())
        {
            ZFMethodDynamicUnregister(ZFMethodFuncGet(ZF_NAMESPACE_GLOBAL_NAME,
                zfstringWithFormat("ZFTypeId_%s", typeIdName)));
            _ZFP_ZFTypeIdUnregister(typeIdName);
            d->m.erase(it);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

