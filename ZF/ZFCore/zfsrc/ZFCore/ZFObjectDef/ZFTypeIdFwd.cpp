/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFTypeIdFwd.h"
#include "ZFObjectImpl.h"
#include "ZFListenerDeclare.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_vector.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_STATIC_INITIALIZER_INIT(ZFTypeIdHolder)
{
}
ZF_STATIC_INITIALIZER_DESTROY(ZFTypeIdHolder)
{
    zfCoreAssert(m.empty());
}
zfstlmap<zfstlstringZ, ZFTypeIdBase *> m;
ZF_STATIC_INITIALIZER_END(ZFTypeIdHolder)

void _ZFP_ZFTypeIdRegister(ZF_IN const zfchar *typeId,
                           ZF_IN ZFTypeIdBase *typeIdData)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, ZFTypeIdBase *> &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFTypeIdHolder)->m;
    zfCoreAssert(m.find(typeId) == m.end());
    m[typeId] = typeIdData;
}
ZFTypeIdBase *_ZFP_ZFTypeIdUnregister(ZF_IN const zfchar *typeId)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, ZFTypeIdBase *> &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFTypeIdHolder)->m;
    zfstlmap<zfstlstringZ, ZFTypeIdBase *>::iterator it = m.find(typeId);
    zfCoreAssert(it != m.end());
    ZFTypeIdBase *t = it->second;
    m.erase(it);
    return t;
}
const ZFTypeIdBase *ZFTypeIdGet(ZF_IN const zfchar *typeId)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, ZFTypeIdBase *> &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFTypeIdHolder)->m;
    zfstlmap<zfstlstringZ, ZFTypeIdBase *>::iterator it = m.find(typeId);
    if(it != m.end())
    {
        return it->second;
    }
    else
    {
        return zfnull;
    }
}
void ZFTypeIdGetAllT(ZF_OUT ZFCoreArray<const ZFTypeIdBase *> &ret)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, ZFTypeIdBase *> &m = ZF_STATIC_INITIALIZER_INSTANCE(ZFTypeIdHolder)->m;
    for(zfstlmap<zfstlstringZ, ZFTypeIdBase *>::iterator it = m.begin(); it != m.end(); ++it)
    {
        ret.add(it->second);
    }
}

// ============================================================
// special alias implicit convert
zfclass _ZFP_I_PropAliasHolder : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_PropAliasHolder, ZFObject)
public:
    ZFObject *obj;
    void *v;
    _ZFP_PropAliasDetachCallback detachCallback;
    ZFListener ownerOnDeallocListener;
protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        this->cleanup();
        zfsuper::objectOnDeallocPrepare();
    }
    ZFLISTENER_INLINE(ownerOnDealloc)
    {
        this->cleanup();
    }
private:
    void cleanup(void)
    {
        if(this->detachCallback != zfnull)
        {
            this->obj->observerRemove(ZFObject::EventObjectBeforeDealloc(), this->ownerOnDeallocListener);
            _ZFP_PropAliasDetachCallback detachCallbackTmp = this->detachCallback;
            this->detachCallback = zfnull;
            detachCallbackTmp(this->obj, this->v);
        }
    }
};

void _ZFP_PropAliasAttach(ZF_IN ZFObject *obj,
                          ZF_IN void *v,
                          ZF_IN const zfchar *typeName,
                          ZF_IN _ZFP_PropAliasDetachCallback detachCallback)
{
    zfstring key = "_ZFP_PropTypeAlias_";
    key += typeName;
    _ZFP_I_PropAliasHolder *d = obj->tagGet<_ZFP_I_PropAliasHolder *>(key);
    if(d == zfnull)
    {
        d = zfAlloc(_ZFP_I_PropAliasHolder);
        d->obj = obj;
        d->v = v;
        d->detachCallback = detachCallback;
        d->ownerOnDeallocListener = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_I_PropAliasHolder, ownerOnDealloc));
        obj->observerAdd(ZFObject::EventObjectBeforeDealloc(), d->ownerOnDeallocListener);
        obj->tagSet(key, d);
        zfRelease(d);
    }
    else
    {
        _ZFP_PropAliasDetachCallback detachCallbackOld = d->detachCallback;
        ZFObject *objOld = d->obj;
        void *vOld = d->v;
        d->obj = obj;
        d->v = v;
        d->detachCallback = detachCallback;
        detachCallbackOld(objOld, vOld);
    }
}
void _ZFP_PropAliasDetach(ZF_IN ZFObject *obj,
                          ZF_IN const zfchar *typeName)
{
    zfstring key = "_ZFP_PropTypeAlias_";
    key += typeName;
    obj->tagRemove(key);
}

void _ZFP_ZFTypeIdWrapperMarkConst(ZF_IN_OUT_OPT ZFObject *zfv)
{
    ZFTypeIdWrapper::markConst(zfv);
}

ZF_NAMESPACE_GLOBAL_END

