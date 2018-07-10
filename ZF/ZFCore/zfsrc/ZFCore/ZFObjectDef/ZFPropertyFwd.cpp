/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFPropertyFwd.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

void ZFPropertyCallbackIsValueAccessedChange(ZF_IN const ZFProperty *property,
                                             ZF_IN ZFPropertyCallbackIsValueAccessed callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackIsValueAccessed = callback;
}
void ZFPropertyCallbackIsInitValueChange(ZF_IN const ZFProperty *property,
                                         ZF_IN ZFPropertyCallbackIsInitValue callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackIsInitValue = callback;
}
void ZFPropertyCallbackValueResetChange(ZF_IN const ZFProperty *property,
                                        ZF_IN ZFPropertyCallbackValueReset callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackValueReset = callback;
}
void ZFPropertyCallbackValueSetChange(ZF_IN const ZFProperty *property,
                                      ZF_IN ZFPropertyCallbackValueSet callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackValueSet = callback;
}
void ZFPropertyCallbackValueGetChange(ZF_IN const ZFProperty *property,
                                      ZF_IN ZFPropertyCallbackValueGet callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackValueGet = callback;
}
void ZFPropertyCallbackValueGetReleaseChange(ZF_IN const ZFProperty *property,
                                             ZF_IN ZFPropertyCallbackValueGetRelease callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackValueGetRelease = callback;
}
ZFPropertyCallbackValueGetHolder::ZFPropertyCallbackValueGetHolder(ZF_IN const ZFProperty *property,
                                                                   ZF_IN ZFObject *ownerObj)
: _property(property)
, _ownerObj(ownerObj)
, _valueToken(zfnull)
, _value(zfnull)
{
    _value = property->callbackValueGet(property, ownerObj, _valueToken);
}
ZFPropertyCallbackValueGetHolder::~ZFPropertyCallbackValueGetHolder(void)
{
    _property->callbackValueGetRelease(_property, _ownerObj, _valueToken, _value);
}
void ZFPropertyCallbackCompareChange(ZF_IN const ZFProperty *property,
                                     ZF_IN ZFPropertyCallbackCompare callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackCompare = callback;
}
void ZFPropertyCallbackGetInfoChange(ZF_IN const ZFProperty *property,
                                     ZF_IN ZFPropertyCallbackGetInfo callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackGetInfo = callback;
}
void ZFPropertyCallbackValueStoreChange(ZF_IN const ZFProperty *property,
                                        ZF_IN ZFPropertyCallbackValueStore callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackValueStore = callback;
}
void ZFPropertyCallbackValueReleaseChange(ZF_IN const ZFProperty *property,
                                          ZF_IN ZFPropertyCallbackValueRelease callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackValueRelease = callback;
}
void ZFPropertyCallbackSerializeFromChange(ZF_IN const ZFProperty *property,
                                           ZF_IN ZFPropertyCallbackSerializeFrom callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackSerializeFrom = callback;
}
void ZFPropertyCallbackSerializeToChange(ZF_IN const ZFProperty *property,
                                         ZF_IN ZFPropertyCallbackSerializeTo callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackSerializeTo = callback;
}
void ZFPropertyCallbackProgressUpdateChange(ZF_IN const ZFProperty *property,
                                            ZF_IN ZFPropertyCallbackProgressUpdate callback)
{
    zfCoreAssert(property != zfnull && callback != zfnull);
    property->_ZFP_ZFProperty_removeConst()->callbackProgressUpdate = callback;
}

ZF_NAMESPACE_GLOBAL_END

