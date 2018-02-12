/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFPropertyTypeFwd.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFPropertyTypeIdDataHolder)
{
}
ZF_STATIC_INITIALIZER_DESTROY(ZFPropertyTypeIdDataHolder)
{
    zfCoreAssert(m.empty());
}
zfstlmap<zfstlstringZ, ZFPropertyTypeIdDataBase *> m;
ZF_STATIC_INITIALIZER_END(ZFPropertyTypeIdDataHolder)

void _ZFP_ZFPropertyTypeIdDataRegister(ZF_IN const zfchar *propertyTypeId,
                                       ZF_IN ZFPropertyTypeIdDataBase *propertyTypeIdData)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, ZFPropertyTypeIdDataBase *> &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFPropertyTypeIdDataHolder)->m;
    zfCoreAssert(m.find(propertyTypeId) == m.end());
    m[propertyTypeId] = propertyTypeIdData;
}
void _ZFP_ZFPropertyTypeIdDataUnregister(ZF_IN const zfchar *propertyTypeId)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, ZFPropertyTypeIdDataBase *> &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFPropertyTypeIdDataHolder)->m;
    zfstlmap<zfstlstringZ, ZFPropertyTypeIdDataBase *>::iterator it = m.find(propertyTypeId);
    zfCoreAssert(it != m.end());
    ZFPropertyTypeIdDataBase *t = it->second;
    m.erase(it);
    zfdelete(t);
}
const ZFPropertyTypeIdDataBase *ZFPropertyTypeIdDataGet(ZF_IN const zfchar *propertyTypeId)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, ZFPropertyTypeIdDataBase *> &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFPropertyTypeIdDataHolder)->m;
    zfstlmap<zfstlstringZ, ZFPropertyTypeIdDataBase *>::iterator it = m.find(propertyTypeId);
    if(it != m.end())
    {
        return it->second;
    }
    else
    {
        return zfnull;
    }
}
void ZFPropertyTypeIdDataGetAllT(ZF_OUT ZFCoreArray<const ZFPropertyTypeIdDataBase *> &ret)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, ZFPropertyTypeIdDataBase *> &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFPropertyTypeIdDataHolder)->m;
    for(zfstlmap<zfstlstringZ, ZFPropertyTypeIdDataBase *>::iterator it = m.begin(); it != m.end(); ++it)
    {
        ret.add(it->second);
    }
}

// ============================================================
// special alias implicit convert
zfclassPOD _ZFP_PropAliasData
{
public:
    void *v;
    _ZFP_PropAliasConverter converter;
};
zfclass _ZFP_I_PropAliasHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_PropAliasHolder, ZFObject)
public:
    ZFObject *obj;
    zfstlvector<_ZFP_PropAliasData> l;

protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        for(zfstlsize i = 0; i < this->l.size(); ++i)
        {
            _ZFP_PropAliasData &d = this->l[i];
            d.converter(this->obj, d.v);
        }
        zfsuper::objectOnDeallocPrepare();
    }
};

void _ZFP_PropAliasAttach(ZF_IN ZFObject *obj,
                          ZF_IN void *v,
                          ZF_IN _ZFP_PropAliasConverter converter)
{
    _ZFP_I_PropAliasHolder *d = obj->tagGet<_ZFP_I_PropAliasHolder *>(_ZFP_PropAliasKey);
    if(d == zfnull)
    {
        d = zfAlloc(_ZFP_I_PropAliasHolder);
        d->obj = obj;
        obj->tagSet(_ZFP_PropAliasKey, d);
        zfRelease(d);
    }
    _ZFP_PropAliasData data;
    data.v = v;
    data.converter = converter;
    d->l.push_back(data);
}

ZF_NAMESPACE_GLOBAL_END

