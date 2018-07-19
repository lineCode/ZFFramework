/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFMethodFuncDeclare.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFMethodFuncDataHolder)
{
}
zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > > methodMap;
ZF_STATIC_INITIALIZER_END(ZFMethodFuncDataHolder)
#define _ZFP_ZFMethodFuncMethodMap ZF_STATIC_INITIALIZER_INSTANCE(ZFMethodFuncDataHolder)->methodMap

void _ZFP_ZFMethodFuncRegister(ZF_IN const ZFMethod *method)
{
    const zfchar *methodNamespace = method->methodNamespace() ? method->methodNamespace() : zfText("");
    _ZFP_ZFMethodFuncMethodMap
        [methodNamespace]
        [method->methodName()]
        .push_back(method);
}
void _ZFP_ZFMethodFuncUnregister(ZF_IN const ZFMethod *method)
{
    const zfchar *methodNamespace = method->methodNamespace() ? method->methodNamespace() : zfText("");
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > > &m = _ZFP_ZFMethodFuncMethodMap;
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > >::iterator itNS = m.find(methodNamespace);
    if(itNS == m.end())
    {
        return ;
    }
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = itNS->second.find(method->methodName());
    if(itName == itNS->second.end())
    {
        return ;
    }
    zfstlvector<const ZFMethod *> &l = itName->second;
    for(zfstlsize i = 0; i < l.size(); ++i)
    {
        if(l[i] == method)
        {
            l.erase(l.begin() + i);
            if(l.empty())
            {
                itNS->second.erase(itName);
                if(itNS->second.empty())
                {
                    m.erase(itNS);
                }
            }
        }
    }
}

// ============================================================
const ZFMethod *ZFMethodFuncGet(ZF_IN const zfchar *methodNamespace,
                                ZF_IN const zfchar *methodName)
{
    zfCoreMutexLocker();
    methodNamespace = ZFNamespaceSkipGlobal(methodNamespace);
    if(methodNamespace == zfnull)
    {
        methodNamespace = zfText("");
    }
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > > &m = _ZFP_ZFMethodFuncMethodMap;
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > >::iterator itNS = m.find(methodNamespace);
    if(itNS == m.end())
    {
        return zfnull;
    }
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = itNS->second.find(methodName);
    if(itName == itNS->second.end())
    {
        return zfnull;
    }
    else
    {
        return itName->second[0];
    }
}
const ZFMethod *ZFMethodFuncGet(ZF_IN const zfchar *methodNamespace,
                                ZF_IN const zfchar *methodName
                                , ZF_IN_OPT const zfchar *methodParamTypeId0
                                , ZF_IN_OPT const zfchar *methodParamTypeId1 /* = zfnull */
                                , ZF_IN_OPT const zfchar *methodParamTypeId2 /* = zfnull */
                                , ZF_IN_OPT const zfchar *methodParamTypeId3 /* = zfnull */
                                , ZF_IN_OPT const zfchar *methodParamTypeId4 /* = zfnull */
                                , ZF_IN_OPT const zfchar *methodParamTypeId5 /* = zfnull */
                                , ZF_IN_OPT const zfchar *methodParamTypeId6 /* = zfnull */
                                , ZF_IN_OPT const zfchar *methodParamTypeId7 /* = zfnull */
                                )
{
    zfCoreMutexLocker();
    methodNamespace = ZFNamespaceSkipGlobal(methodNamespace);
    if(methodNamespace == zfnull)
    {
        methodNamespace = zfText("");
    }
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > > &m = _ZFP_ZFMethodFuncMethodMap;
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > >::iterator itNS = m.find(methodNamespace);
    if(itNS == m.end())
    {
        return zfnull;
    }
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = itNS->second.find(methodName);
    if(itName == itNS->second.end())
    {
        return zfnull;
    }

    zfstlvector<const ZFMethod *> &l = itName->second;
    for(zfstlsize i = 0; i < l.size(); ++i)
    {
        const ZFMethod *m = l[i];
        if(m->methodParamTypeIdIsMatch(
                  methodParamTypeId0
                , methodParamTypeId1
                , methodParamTypeId2
                , methodParamTypeId3
                , methodParamTypeId4
                , methodParamTypeId5
                , methodParamTypeId6
                , methodParamTypeId7
            ))
        {
            return m;
        }
    }
    return zfnull;
}

void ZFMethodFuncGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret,
                         ZF_IN_OPT const ZFFilterForZFMethod *filter /* = zfnull */)
{
    zfCoreMutexLocker();

    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > > &m = _ZFP_ZFMethodFuncMethodMap;
    for(zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > >::iterator itNS = m.begin();
        itNS != m.end();
        ++itNS)
    {
        for(zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = itNS->second.begin();
            itName != itNS->second.end();
            ++itName)
        {
            if(filter == zfnull)
            {
                for(zfstlsize i = 0; i < itName->second.size(); ++i)
                {
                    ret.add(itName->second[i]);
                }
            }
            else
            {
                for(zfstlsize i = 0; i < itName->second.size(); ++i)
                {
                    if(filter->filterCheckActive(itName->second[i]))
                    {
                        ret.add(itName->second[i]);
                    }
                }
            }
        }
    }
}

void ZFMethodFuncGetAllT(ZF_IN_OUT ZFCoreArray<const ZFMethod *> &ret,
                         ZF_IN const zfchar *methodNamespace,
                         ZF_IN const zfchar *methodName,
                         ZF_IN_OPT const ZFFilterForZFMethod *filter /* = zfnull */)
{
    zfCoreMutexLocker();

    methodNamespace = ZFNamespaceSkipGlobal(methodNamespace);
    if(methodNamespace == zfnull)
    {
        methodNamespace = zfText("");
    }
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > > &m = _ZFP_ZFMethodFuncMethodMap;
    zfstlmap<zfstlstringZ, zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> > >::iterator itNS = m.find(methodNamespace);
    if(itNS == m.end())
    {
        return ;
    }
    zfstlmap<zfstlstringZ, zfstlvector<const ZFMethod *> >::iterator itName = itNS->second.find(methodName);
    if(itName == itNS->second.end())
    {
        return ;
    }
    if(filter == zfnull)
    {
        for(zfstlsize i = 0; i < itName->second.size(); ++i)
        {
            ret.add(itName->second[i]);
        }
    }
    else
    {
        for(zfstlsize i = 0; i < itName->second.size(); ++i)
        {
            if(filter->filterCheckActive(itName->second[i]))
            {
                ret.add(itName->second[i]);
            }
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(const ZFMethod *, ZFMethodFuncGet, ZFMP_IN(const zfchar *, methodNamespace), ZFMP_IN(const zfchar *, methodName))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_8(const ZFMethod *, ZFMethodFuncGet, ZFMP_IN(const zfchar *, methodNamespace), ZFMP_IN(const zfchar *, methodName)
    , ZFMP_IN(const zfchar *, param0)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId1, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId2, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId3, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId4, zfnull)
    , ZFMP_IN_OPT(const zfchar *, methodParamTypeId5, zfnull)
    /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId6, zfnull) */
    /* ZFMETHOD_MAX_PARAM , ZFMP_IN_OPT(const zfchar *, methodParamTypeId7, zfnull) */
    )
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_2(void, ZFMethodFuncGetAllT, ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret), ZFMP_IN_OPT(const ZFFilterForZFMethod *, filter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFCoreArrayPOD<const ZFMethod *>, ZFMethodFuncGetAll, ZFMP_IN_OPT(const ZFFilterForZFMethod *, filter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_4(void, ZFMethodFuncGetAllT, ZFMP_IN_OUT(ZFCoreArray<const ZFMethod *> &, ret), ZFMP_IN(const zfchar *, methodNamespace), ZFMP_IN(const zfchar *, methodName), ZFMP_IN_OPT(const ZFFilterForZFMethod *, filter, zfnull))
ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_3(ZFCoreArrayPOD<const ZFMethod *>, ZFMethodFuncGetAll, ZFMP_IN(const zfchar *, methodNamespace), ZFMP_IN(const zfchar *, methodName), ZFMP_IN_OPT(const ZFFilterForZFMethod *, filter, zfnull))

ZF_NAMESPACE_GLOBAL_END
#endif

