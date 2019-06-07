/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFStyleable.h"
#include "ZFObjectImpl.h"
#include "ZFClassUtil.h"
#include "ZFPropertyUtil.h"
#include "ZFListenerDeclare.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFStyleKeyHolder
{
public:
    zfchar *styleKey;
    zfstlmap<const ZFProperty *, zfstring> stylePropertyKeyMap;
public:
    static ZFLISTENER_PROTOTYPE_EXPAND(styleOnChange);
    static ZFLISTENER_PROTOTYPE_EXPAND(stylePropertyOnChange);
};

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStyleChangeDataHolder, ZFLevelZFFrameworkEssential)
{
    this->styleOnChangeListener = ZFCallbackForFunc(_ZFP_ZFStyleKeyHolder::styleOnChange);
}
public:
    ZFListener styleOnChangeListener;
ZF_GLOBAL_INITIALIZER_END(ZFStyleChangeDataHolder)

void _ZFP_ZFStyleKeyHolder::styleOnChange(ZF_IN_OUT ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    zfCoreMutexLocker();
    ZFStyleable *owner = userData->objectHolded();
    zfautoObject style = ZFStyleGet(owner->styleKey());
    if(style != zfnull)
    {
        zfCoreAssertWithMessageTrim(style->classData()->classIsTypeOf(owner->classData())
            || owner->classData()->classIsTypeOf(style->classData()),
            "[ZFStyle] applying style from incorrect type, to object: %s, from style: %s",
            owner->toObject()->objectInfoOfInstance().cString(),
            style->objectInfoOfInstance().cString());

        owner->styleableCopyFrom(style);
        if(!owner->styleKeyOnCheckValid())
        {
            owner->styleKeySet(zfnull);
        }
    }
    else
    {
        owner->styleKeySet(zfnull);
    }
}
zfbool ZFStyleable::styleKeySet(ZF_IN const zfchar *styleKey)
{
    zfCoreMutexLocker();
    if(styleKey == zfnull)
    {
        if(_ZFP_styleKey != zfnull)
        {
            zfsChange(_ZFP_styleKey->styleKey, styleKey);
            ZFObjectGlobalEventObserver().observerRemove(
                ZFGlobalEvent::EventZFStyleOnChange(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleChangeDataHolder)->styleOnChangeListener,
                this->toObject()->objectHolder());
        }
        return zftrue;
    }
    else
    {
        if(_ZFP_styleKey == zfnull)
        {
            _ZFP_styleKey = zfpoolNew(_ZFP_ZFStyleKeyHolder);
        }
        if(_ZFP_styleKey->styleKey == zfnull)
        {
            ZFObjectGlobalEventObserver().observerAdd(
                ZFGlobalEvent::EventZFStyleOnChange(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleChangeDataHolder)->styleOnChangeListener,
                this->toObject()->objectHolder());
        }
        zfsChange(_ZFP_styleKey->styleKey, styleKey);
        ZFListenerData listenerData;
        _ZFP_ZFStyleKeyHolder::styleOnChange(listenerData, this->toObject()->objectHolder());

        if(_ZFP_styleKey->styleKey == zfnull)
        {
            v_ZFProperty *param0 = zflockfree_zfAllocWithCache(v_ZFProperty);
            v_zfstring *param1 = zflockfree_zfAllocWithCache(v_zfstring);
            param1->zfv = styleKey;
            ZFObjectGlobalEventObserver().observerNotifyWithCustomSender(
                this->toObject(),
                ZFGlobalEvent::EventZFStyleOnInvalid(),
                param0,
                param1);
            zflockfree_zfRelease(param0);
            zflockfree_zfRelease(param1);
            return zffalse;
        }
        else
        {
            return zftrue;
        }
    }
}
const zfchar *ZFStyleable::styleKey(void)
{
    if(_ZFP_styleKey)
    {
        return _ZFP_styleKey->styleKey;
    }
    else
    {
        return zfnull;
    }
}

// ============================================================
static zfstlmap<zfstlstringZ, _ZFP_ZFStylePropertyCopyCallback> &_ZFP_ZFStylePropertyCustomCopyMapMap(void)
{
    static zfstlmap<zfstlstringZ, _ZFP_ZFStylePropertyCopyCallback> d;
    return d;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFStylePropertyChangeDataHolder, ZFLevelZFFrameworkEssential)
{
    this->stylePropertyOnChangeListener = ZFCallbackForFunc(_ZFP_ZFStyleKeyHolder::stylePropertyOnChange);
}
public:
    ZFListener stylePropertyOnChangeListener;
ZF_GLOBAL_INITIALIZER_END(ZFStylePropertyChangeDataHolder)

static zfbool _ZFP_ZFStylePropertyCopy(ZF_IN ZFObject *propertyOwner,
                                       ZF_IN const ZFProperty *property,
                                       ZF_IN const zfchar *styleKey)
{
    zfstlmap<zfstlstringZ, _ZFP_ZFStylePropertyCopyCallback> &m = _ZFP_ZFStylePropertyCustomCopyMapMap();
    zfstlmap<zfstlstringZ, _ZFP_ZFStylePropertyCopyCallback>::iterator it = m.find(property->propertyTypeId());
    if(it == m.end())
    {
        return zffalse;
    }
    zfautoObject styleValue = ZFStyleGet(styleKey);
    if(styleValue == zfnull)
    {
        return zffalse;
    }
    else
    {
        return it->second(propertyOwner, property, styleValue);
    }
}
void _ZFP_ZFStyleKeyHolder::stylePropertyOnChange(ZF_IN_OUT ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    zfCoreMutexLocker();
    ZFObject *ownerObj = userData->objectHolded();
    ZFStyleable *owner = ownerObj->to<ZFStyleable *>();
    zfstlmap<const ZFProperty *, zfstring> &m = owner->_ZFP_styleKey->stylePropertyKeyMap;
    for(zfstlmap<const ZFProperty *, zfstring>::iterator it = m.begin(); it != m.end(); )
    {
        if(_ZFP_ZFStylePropertyCopy(ownerObj, it->first, it->second))
        {
            ++it;
        }
        else
        {
            m.erase(it++);
        }
    }
    if(m.empty())
    {
        ZFObjectGlobalEventObserver().observerRemove(
            ZFGlobalEvent::EventZFStyleOnChange(),
            ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStylePropertyChangeDataHolder)->stylePropertyOnChangeListener,
            ownerObj->objectHolder());
    }
}
zfbool ZFStyleable::styleKeySet(ZF_IN const ZFProperty *property, ZF_IN const zfchar *styleKey)
{
    zfCoreMutexLocker();
    if(styleKey == zfnull)
    {
        if(_ZFP_styleKey != zfnull)
        {
            _ZFP_styleKey->stylePropertyKeyMap.erase(property);
            if(_ZFP_styleKey->stylePropertyKeyMap.empty())
            {
                ZFObjectGlobalEventObserver().observerRemove(
                    ZFGlobalEvent::EventZFStyleOnChange(),
                    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStylePropertyChangeDataHolder)->stylePropertyOnChangeListener,
                    this->toObject()->objectHolder());
            }
        }
        return zftrue;
    }
    else
    {
        if(_ZFP_styleKey == zfnull)
        {
            _ZFP_styleKey = zfpoolNew(_ZFP_ZFStyleKeyHolder);
        }
        zfbool oldEmpty = _ZFP_styleKey->stylePropertyKeyMap.empty();
        _ZFP_styleKey->stylePropertyKeyMap[property] = styleKey;
        if(oldEmpty)
        {
            ZFObjectGlobalEventObserver().observerAdd(
                ZFGlobalEvent::EventZFStyleOnChange(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStylePropertyChangeDataHolder)->stylePropertyOnChangeListener,
                this->toObject()->objectHolder());
        }
        if(_ZFP_ZFStylePropertyCopy(this->toObject(), property, styleKey))
        {
            return zftrue;
        }
        else
        {
            _ZFP_styleKey->stylePropertyKeyMap.erase(property);
            if(_ZFP_styleKey->stylePropertyKeyMap.empty())
            {
                ZFObjectGlobalEventObserver().observerRemove(
                    ZFGlobalEvent::EventZFStyleOnChange(),
                    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStylePropertyChangeDataHolder)->stylePropertyOnChangeListener,
                    this->toObject()->objectHolder());
            }
            v_ZFProperty *param0 = zflockfree_zfAllocWithCache(v_ZFProperty);
            param0->zfv = property;
            v_zfstring *param1 = zflockfree_zfAllocWithCache(v_zfstring);
            param1->zfv = styleKey;
            ZFObjectGlobalEventObserver().observerNotifyWithCustomSender(
                this->toObject(),
                ZFGlobalEvent::EventZFStyleOnInvalid(),
                param0,
                param1);
            zflockfree_zfRelease(param0);
            zflockfree_zfRelease(param1);
            return zffalse;
        }
    }
}
const zfchar *ZFStyleable::styleKey(ZF_IN const ZFProperty *property)
{
    if(property == zfnull)
    {
        return zfnull;
    }
    zfCoreMutexLocker();
    if(_ZFP_styleKey != zfnull)
    {
        zfstlmap<const ZFProperty *, zfstring>::iterator it = _ZFP_styleKey->stylePropertyKeyMap.find(property);
        if(it != _ZFP_styleKey->stylePropertyKeyMap.end())
        {
            return it->second;
        }
        else
        {
            return zfnull;
        }
    }
    else
    {
        return zfnull;
    }
}

void _ZFP_ZFStylePropertyCopyRegister(ZF_IN const zfchar *propertyTypeId,
                                      ZF_IN _ZFP_ZFStylePropertyCopyCallback callback)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, _ZFP_ZFStylePropertyCopyCallback> &m = _ZFP_ZFStylePropertyCustomCopyMapMap();
    zfstlmap<zfstlstringZ, _ZFP_ZFStylePropertyCopyCallback>::iterator it = m.find(propertyTypeId);
    zfCoreAssertWithMessageTrim(it == m.end(),
        "[ZFSTYLE_PROPERTY_COPY_DEFINE] %s already registered",
        propertyTypeId);
    m[propertyTypeId] = callback;
}
void _ZFP_ZFStylePropertyCopyUnregister(ZF_IN const zfchar *propertyTypeId)
{
    zfCoreMutexLocker();
    zfstlmap<zfstlstringZ, _ZFP_ZFStylePropertyCopyCallback> &m = _ZFP_ZFStylePropertyCustomCopyMapMap();
    zfstlmap<zfstlstringZ, _ZFP_ZFStylePropertyCopyCallback>::iterator it = m.find(propertyTypeId);
    if(it != m.end())
    {
        m.erase(it);
    }
}

ZFINTERFACE_ON_DEALLOC_DEFINE(ZFStyleable)
{
    if(_ZFP_styleKey)
    {
        if(_ZFP_styleKey->styleKey != zfnull)
        {
            ZFObjectGlobalEventObserver().observerRemove(
                ZFGlobalEvent::EventZFStyleOnChange(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStyleChangeDataHolder)->styleOnChangeListener,
                this->toObject()->objectHolder());
            zffree(_ZFP_styleKey->styleKey);
            _ZFP_styleKey->styleKey = zfnull;
        }
        if(!_ZFP_styleKey->stylePropertyKeyMap.empty())
        {
            ZFObjectGlobalEventObserver().observerRemove(
                ZFGlobalEvent::EventZFStyleOnChange(),
                ZF_GLOBAL_INITIALIZER_INSTANCE(ZFStylePropertyChangeDataHolder)->stylePropertyOnChangeListener,
                this->toObject()->objectHolder());
        }
        zfpoolDelete(_ZFP_styleKey);
    }
}

ZF_NAMESPACE_GLOBAL_END

