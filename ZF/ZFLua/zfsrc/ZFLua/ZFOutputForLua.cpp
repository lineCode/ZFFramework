/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFOutputForLua.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_I_ZFOutputForLuaOwner : zfextends ZFObject
{
public:
    ZFOBJECT_DECLARE(_ZFP_I_ZFOutputForLuaOwner, ZFObject)

public:
    ZFListener luaCallback;
    zfautoObject userData;

private:
    v_zfstring *_srcCache;
    v_zfindex *_countCache;

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->_srcCache = zfAlloc(v_zfstring);
        this->_countCache = zfAlloc(v_zfindex);
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfRelease(this->_srcCache);
        zfRelease(this->_countCache);
        zfsuper::objectOnDealloc();
    }

protected:
    ZFMETHOD_DECLARE_2(zfindex, onOutput,
                       ZFMP_IN(const void *, src),
                       ZFMP_IN_OPT(zfindex, count, zfindexMax()))
};
ZFMETHOD_DEFINE_2(_ZFP_I_ZFOutputForLuaOwner, zfindex, onOutput,
                  ZFMP_IN(const void *, src),
                  ZFMP_IN_OPT(zfindex, count, zfindexMax()))
{
    if(this->luaCallback.callbackIsValid())
    {
        this->_srcCache->zfv.assign((const zfchar *)src, count / sizeof(zfchar));
        this->_countCache->zfv = count;
        this->luaCallback.execute(ZFListenerData(
                zfidentityInvalid(),
                zfnull,
                this->_srcCache,
                this->_countCache
            ), this->userData);
        return this->_countCache->zfv;
    }
    else
    {
        return 0;
    }
}

ZFMETHOD_FUNC_DEFINE_2(ZFOutput, ZFOutputForLua,
                       ZFMP_IN(const ZFListener &, luaCallback),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull))
{
    if(!luaCallback.callbackIsValid())
    {
        return ZFCallbackNull();
    }
    zfblockedAlloc(_ZFP_I_ZFOutputForLuaOwner, owner);
    owner->luaCallback = luaCallback;
    owner->userData = userData;
    ZFOutput ret = ZFCallbackForMemberMethod(owner, ZFMethodAccess(_ZFP_I_ZFOutputForLuaOwner, onOutput));
    ret.callbackOwnerObjectRetain();
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

