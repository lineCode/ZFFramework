/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFOperation.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFOperationParam
ZFOBJECT_REGISTER(ZFOperationParam)

zfidentity ZFOperationParam::objectHash(void)
{
    return zfidentityCalcString(this->classData()->classNameFull());
}
ZFCompareResult ZFOperationParam::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->paramIsEqual(another))
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareUncomparable;
    }
}

zfbool ZFOperationParam::paramOnCheckPropertyIsEqual(ZF_IN ZFOperationParam *anotherOperationParam)
{
    return ZFClassUtil::allPropertyIsEqual(this, anotherOperationParam);
}
zfbool ZFOperationParam::paramOnCheckIsEqual(ZF_IN ZFOperationParam *anotherOperationParam)
{
    return zftrue;
}

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_1(ZFOperationParam, zfbool, paramIsEqual, ZFMP_IN(ZFOperationParam *, anotherOperationParam))

// ============================================================
// ZFOperationResult
ZFOBJECT_REGISTER(ZFOperationResult)

// ============================================================
// ZFOperationObserver
ZFOBJECT_REGISTER(ZFOperationObserver)

// ============================================================
// ZFOperationCache
ZFOBJECT_REGISTER(ZFOperationCache)

zfidentity ZFOperationCache::objectHash(void)
{
    return zfidentityHash(0
            , ZFObjectHash(this->operationParam())
            , ZFObjectHash(this->operationResult())
            , this->cacheTime()
            , this->cacheExpireTime()
        );
}
ZFCompareResult ZFOperationCache::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(zftrue
       && ZFObjectCompare(this->operationParam(), another->operationParam()) == ZFCompareTheSame
       && ZFObjectCompare(this->operationResult(), another->operationResult()) == ZFCompareTheSame
       && this->cacheTime() == another->cacheTime()
       && this->cacheExpireTime() == another->cacheExpireTime()
       )
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareUncomparable;
    }
}

ZFMETHOD_DEFINE_0(ZFOperationCache, zfbool, cacheIsExpired)
{
    return (ZFTime::currentTimeValue().sec - this->cacheTime() >= this->cacheExpireTime());
}

// ============================================================
// ZFOperationProgress
ZFOBJECT_REGISTER(ZFOperationProgress)

// ============================================================
// ZFOperationTaskData
ZFOBJECT_REGISTER(ZFOperationTaskData)

zfidentity ZFOperationTaskData::objectHash(void)
{
    return zfidentityHash(0
            , ZFObjectHash(this->operationOwner())
            , ZFObjectHash(this->operation())
            , this->operationId()
            , ZFObjectHash(this->taskCategory())
            , ZFObjectHash(this->operationParam())
            , ZFObjectHash(this->operationResult())
            , ZFObjectHash(this->operationObserver())
            , ZFObjectHash(this->operationCache())
            , ZFObjectHash(this->operationProgress())
            , this->cacheExpireTime()
            , this->cacheMatchAction()
            , this->taskDuplicateAction()
        );
}
ZFCompareResult ZFOperationTaskData::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(zftrue
       && ZFObjectCompare(this->operationOwner(), another->operationOwner()) == ZFCompareTheSame
       && ZFObjectCompare(this->operation(), another->operation()) == ZFCompareTheSame
       && this->operationId() == another->operationId()
       && ZFObjectCompare(this->taskCategory(), another->taskCategory()) == ZFCompareTheSame
       && ZFObjectCompare(this->operationParam(), another->operationParam()) == ZFCompareTheSame
       && ZFObjectCompare(this->operationResult(), another->operationResult()) == ZFCompareTheSame
       && ZFObjectCompare(this->operationObserver(), another->operationObserver()) == ZFCompareTheSame
       && ZFObjectCompare(this->operationCache(), another->operationCache()) == ZFCompareTheSame
       && ZFObjectCompare(this->operationProgress(), another->operationProgress()) == ZFCompareTheSame
       && this->cacheExpireTime() == another->cacheExpireTime()
       && this->cacheMatchAction() == another->cacheMatchAction()
       && this->taskDuplicateAction() == another->taskDuplicateAction()
        )
    {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareUncomparable;
    }
}

// ============================================================
ZFENUM_DEFINE(ZFOperationCacheMatchAction)
ZFENUM_DEFINE(ZFOperationTaskDuplicateAction)

ZF_NAMESPACE_GLOBAL_END

