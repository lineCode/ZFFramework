/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFObjectCore.h"
#include "ZFObjectImpl.h"

#include "ZFCore/ZFSTLWrapper/zfstl_map.h"
#include "ZFCore/ZFSTLWrapper/zfstl_deque.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFListenerData
void ZFListenerData::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += ZFTOKEN_ZFObjectInfoLeft;
    zfstringAppend(ret, zfText("ZFListenerData(%p)"), this);
    const zfchar *eventName = ZFIdMapGetName(this->eventId);
    if(eventName != zfnull)
    {
        ret += zfText(", event: ");
        ret += eventName;
    }
    if(this->sender != zfnull)
    {
        ret += zfText(", sender: ");
        ZFObjectInfoT(ret, this->sender);
    }
    if(this->param0 != zfnull)
    {
        ret += zfText(", param0: ");
        ZFObjectInfoT(ret, this->param0);
    }
    if(this->param1 != zfnull)
    {
        ret += zfText(", param1: ");
        ZFObjectInfoT(ret, this->param1);
    }
    ret += ZFTOKEN_ZFObjectInfoRight;
}

// ============================================================
// ZFObserverHolder types
zfclassLikePOD _ZFP_ZFObserverData
{
public:
    zfidentity taskId;
    ZFListener observer;
    ZFObject *userData; // no auto retain, manually retain or release
    ZFObject *owner;
    ZFLevel observerLevel;
    zfbool autoRemoveAfterActivate;

public:
    _ZFP_ZFObserverData(void)
    : taskId(zfidentityInvalid())
    , observer()
    , userData(zfnull)
    , owner(zfnull)
    , observerLevel(ZFLevelAppNormal)
    , autoRemoveAfterActivate(zffalse)
    {
    }
    _ZFP_ZFObserverData(ZF_IN zfidentity taskId,
                        ZF_IN const ZFListener &observer,
                        ZF_IN_OPT ZFObject *userData = zfnull,
                        ZF_IN_OPT ZFObject *owner = zfnull,
                        ZF_IN_OPT ZFLevel observerLevel = ZFLevelAppNormal,
                        ZF_IN_OPT zfbool autoRemoveAfterActivate = zffalse)
    : taskId(taskId)
    , observer(observer)
    , userData(userData)
    , owner(owner)
    , observerLevel(observerLevel)
    , autoRemoveAfterActivate(autoRemoveAfterActivate)
    {
    }
    _ZFP_ZFObserverData(ZF_IN const _ZFP_ZFObserverData &ref)
    : taskId(ref.taskId)
    , observer(ref.observer)
    , userData(ref.userData)
    , owner(ref.owner)
    , observerLevel(ref.observerLevel)
    , autoRemoveAfterActivate(ref.autoRemoveAfterActivate)
    {
    }
    _ZFP_ZFObserverData &operator = (ZF_IN const _ZFP_ZFObserverData &ref)
    {
        this->taskId = ref.taskId;
        this->observer = ref.observer;
        this->userData = ref.userData;
        this->owner = ref.owner;
        this->observerLevel = ref.observerLevel;
        this->autoRemoveAfterActivate = ref.autoRemoveAfterActivate;
        return *this;
    }
};
typedef zfstldeque<_ZFP_ZFObserverData> _ZFP_ZFObserverListType;
typedef zfstlmap<zfidentity, _ZFP_ZFObserverListType> _ZFP_ZFObserverMapType;

zfclassPOD _ZFP_ZFObserverHolderAttachState
{
public:
    zfuint *flag;
    zft_zfuint flagBit;
};
typedef zfstlmap<zfidentity, zfstldeque<_ZFP_ZFObserverHolderAttachState> > _ZFP_ZFObserverHolderAttachStateMapType;

zfclassNotPOD _ZFP_ZFObserverHolderPrivate
{
public:
    zfuint refCount;
    _ZFP_ZFObserverMapType observerMap;
    ZFIdentityGenerator taskIdGenerator;
    _ZFP_ZFObserverHolderAttachStateMapType attachMap;
public:
    _ZFP_ZFObserverHolderPrivate(void)
    : refCount(1)
    , observerMap()
    , taskIdGenerator()
    , attachMap()
    {
    }

public:
    void attachMapAttach(ZF_IN const zfidentity &eventId)
    {
        _ZFP_ZFObserverHolderAttachStateMapType::iterator it = this->attachMap.find(eventId);
        if(it != this->attachMap.end())
        {
            for(zfstlsize i = 0; i < it->second.size(); ++i)
            {
                _ZFP_ZFObserverHolderAttachState &state = it->second[i];
                ZFBitSet(*state.flag, state.flagBit);
            }
        }
    }
    void attachMapDetach(ZF_IN const zfidentity &eventId)
    {
        _ZFP_ZFObserverHolderAttachStateMapType::iterator it = this->attachMap.find(eventId);
        if(it != this->attachMap.end())
        {
            for(zfstlsize i = 0; i < it->second.size(); ++i)
            {
                _ZFP_ZFObserverHolderAttachState &state = it->second[i];
                ZFBitUnset(*state.flag, state.flagBit);
            }
        }
    }
    void observerNotifyPrepare(ZF_IN_OUT _ZFP_ZFObserverListType &toNotify,
                               ZF_IN_OUT zfstldeque<ZFObject *> &toRelease,
                               ZF_IN const zfidentity &eventId,
                               ZF_IN ZFObject *observerOwner)
    {
        if(this->observerMap.empty())
        {
            return ;
        }

        _ZFP_ZFObserverMapType::iterator it = this->observerMap.find(eventId);
        if(it != this->observerMap.end())
        {
            for(zfstlsize i = 0; i < it->second.size(); ++i)
            {
                _ZFP_ZFObserverData &observerData = it->second[i];
                toNotify.push_back(observerData);
                if(observerData.autoRemoveAfterActivate)
                {
                    this->taskIdGenerator.idRelease(observerData.taskId);
                    if(observerData.userData != zfnull)
                    {
                        toRelease.push_back(observerData.userData);
                    }
                    it->second.erase(it->second.begin() + i);
                    --i;
                }
            }
            if(it->second.empty())
            {
                this->observerMap.erase(it);
                this->attachMapDetach(eventId);
                if(observerOwner)
                {
                    observerOwner->observerOnRemove(eventId);
                }
            }
        }
    }
};

// ============================================================
// ZFObserverHolder
/** @cond ZFPrivateDoc */
ZFObserverHolder::ZFObserverHolder(void)
{
    zfCoreMutexLocker();
    d = zfpoolNew(_ZFP_ZFObserverHolderPrivate);
    _observerOwner = zfnull;
}
ZFObserverHolder::ZFObserverHolder(ZF_IN ZFObserverHolder const &ref)
{
    zfCoreMutexLocker();
    d = ref.d;
    ++(d->refCount);
    _observerOwner = ref._observerOwner;
}
ZFObserverHolder::~ZFObserverHolder(void)
{
    zfCoreMutexLocker();
    zfpoolDelete(d);
}
ZFObserverHolder &ZFObserverHolder::operator = (ZF_IN ZFObserverHolder const &ref)
{
    zfCoreMutexLocker();
    ++(ref.d->refCount);
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfpoolDelete(d);
    }
    d = ref.d;
    this->_observerOwner = ref._observerOwner;
    return *this;
}
zfbool ZFObserverHolder::operator == (ZF_IN ZFObserverHolder const &ref) const
{
    zfCoreMutexLocker();
    return (d == ref.d);
}
/** @endcond */

zfidentity ZFObserverHolder::observerAdd(ZF_IN const zfidentity &eventId,
                                         ZF_IN const ZFListener &observer,
                                         ZF_IN_OPT ZFObject *userData /* = zfnull */,
                                         ZF_IN_OPT ZFObject *owner /* = zfnull */,
                                         ZF_IN_OPT zfbool autoRemoveAfterActivate /* = zffalse */,
                                         ZF_IN_OPT ZFLevel observerLevel /* = ZFLevelAppNormal */) const
{
    zfCoreMutexLocker();

    if(eventId == zfidentityInvalid()
        || !observer.callbackIsValid())
    {
        return zfidentityInvalid();
    }

    zfidentity taskId = d->taskIdGenerator.idAcquire();
    _ZFP_ZFObserverListType &observerList = d->observerMap[eventId];
    zfstlsize index = observerList.size();
    while(index > 0 && observerList.at(index - 1).observerLevel > observerLevel)
    {
        --index;
    }
    observerList.insert(observerList.begin() + index,
        _ZFP_ZFObserverData(
              taskId
            , observer
            , zflockfree_zfRetain(userData)
            , owner
            , observerLevel
            , autoRemoveAfterActivate
        ));
    if(observerList.size() == 1)
    {
        d->attachMapAttach(eventId);
        if(this->observerOwner())
        {
            if(ZFBitTest(this->observerOwner()->objectInstanceState(), ZFObjectInstanceStateOnDealloc))
            {
                zfCoreCriticalMessageTrim(zfTextA("[ZFObject] you must not add observer while object is deallocating, class: %s, event: %s"),
                    zfsCoreZ2A(this->observerOwner()->classData()->className()),
                    ZFIdMapGetName(eventId));
                return zfidentityInvalid();
            }
            this->observerOwner()->observerOnAdd(eventId);
        }
    }
    return taskId;
}
void ZFObserverHolder::observerRemove(ZF_IN const zfidentity &eventId,
                                      ZF_IN const ZFListener &callback,
                                      ZF_IN_OPT ZFObject *userData /* = zfnull */,
                                      ZF_IN_OPT ZFComparer<ZFObject *>::Comparer userDataComparer /* = zfnull */) const
{
    zfCoreMutexLocker();

    _ZFP_ZFObserverMapType::iterator it = d->observerMap.find(eventId);
    if(it != d->observerMap.end())
    {
        for(zfstlsize iObserver = it->second.size() - 1; iObserver != (zfstlsize)(-1); --iObserver)
        {
            _ZFP_ZFObserverData &observerData = it->second[iObserver];
            if(observerData.observer.objectCompareByInstance(callback) == ZFCompareTheSame
                && (userDataComparer == zfnull || userDataComparer(userData, observerData.userData) == ZFCompareTheSame))
            {
                d->taskIdGenerator.idRelease(observerData.taskId);

                ZFObject *toRemove = observerData.userData;
                it->second.erase(it->second.begin() + iObserver);

                if(it->second.empty())
                {
                    d->observerMap.erase(it);
                    d->attachMapDetach(eventId);
                    if(this->observerOwner())
                    {
                        this->observerOwner()->observerOnRemove(eventId);
                    }
                }

                if(toRemove != zfnull)
                {
                    zflockfree_zfRelease(toRemove);
                }
                break;
            }
        }
    }
}
void ZFObserverHolder::observerRemoveByTaskId(ZF_IN zfidentity taskId) const
{
    zfCoreMutexLocker();
    if(taskId == zfidentityInvalid() || d->observerMap.empty())
    {
        return ;
    }

    zfbool removed = zffalse;
    ZFObject *removedUserData = zfnull;
    zfidentity removedLast = zfidentityInvalid();
    for(_ZFP_ZFObserverMapType::iterator it = d->observerMap.begin();
        it != d->observerMap.end() && !removed;)
    {
        for(zfstlsize iObserver = it->second.size() - 1; iObserver != (zfstlsize)-1; --iObserver)
        {
            if(it->second[iObserver].taskId == taskId)
            {
                d->taskIdGenerator.idRelease(taskId);
                removed = zftrue;
                removedUserData = it->second[iObserver].userData;
                it->second.erase(it->second.begin() + iObserver);
                if(it->second.empty())
                {
                    removedLast = it->first;
                    d->observerMap.erase(it++);
                }
                break;
            }
        }
    }

    if(removed)
    {
        if(removedLast != zfidentityInvalid())
        {
            d->attachMapDetach(removedLast);
            if(this->observerOwner())
            {
                this->observerOwner()->observerOnRemove(removedLast);
            }
        }
        zflockfree_zfRelease(removedUserData);
    }
}
void ZFObserverHolder::observerRemoveByOwner(ZF_IN ZFObject *owner) const
{
    zfCoreMutexLocker();
    if(owner == zfnull || d->observerMap.empty())
    {
        return ;
    }

    zfstldeque<ZFObject *> removedUserData;
    zfstldeque<zfidentity> removedLast;
    for(_ZFP_ZFObserverMapType::iterator it = d->observerMap.begin();
        it != d->observerMap.end();)
    {
        for(zfstlsize iObserver = it->second.size() - 1; iObserver != (zfstlsize)-1; --iObserver)
        {
            if(it->second[iObserver].owner == owner)
            {
                d->taskIdGenerator.idRelease(it->second[iObserver].taskId);
                removedUserData.push_back(it->second[iObserver].userData);
                it->second.erase(it->second.begin() + iObserver);
                --iObserver;
            }
        }
        if(it->second.empty())
        {
            removedLast.push_back(it->first);
            d->observerMap.erase(it++);
        }
        else
        {
            ++it;
        }
    }

    if(this->observerOwner())
    {
        for(zfstlsize i = removedLast.size() - 1; i != (zfstlsize)-1; --i)
        {
            d->attachMapDetach(removedLast[i]);
            this->observerOwner()->observerOnRemove(removedLast[i]);
        }
    }
    else
    {
        for(zfstlsize i = removedLast.size() - 1; i != (zfstlsize)-1; --i)
        {
            d->attachMapDetach(removedLast[i]);
        }
    }

    if(!removedUserData.empty())
    {
        for(zfstlsize i = removedUserData.size() - 1; i != (zfstlsize)-1; --i)
        {
            ZFObject *userDataTmp = removedUserData[i];
            zflockfree_zfRelease(userDataTmp);
        }
    }
}
void ZFObserverHolder::observerRemoveAll(ZF_IN const zfidentity &eventId) const
{
    zfCoreMutexLocker();
    if(d->observerMap.empty())
    {
        return ;
    }

    _ZFP_ZFObserverMapType::iterator it = d->observerMap.find(eventId);
    if(it != d->observerMap.end())
    {
        _ZFP_ZFObserverListType removed;
        it->second.swap(removed);
        d->observerMap.erase(it);
        d->attachMapDetach(eventId);
        if(this->observerOwner())
        {
            this->observerOwner()->observerOnRemove(eventId);
        }

        if(!removed.empty())
        {
            for(zfstlsize i = removed.size() - 1; i != (zfstlsize)-1; --i)
            {
                d->taskIdGenerator.idRelease(removed[i].taskId);
                zflockfree_zfRelease(removed[i].userData);
            }
        }
    }
}
void ZFObserverHolder::observerRemoveAll(void) const
{
    zfCoreMutexLocker();
    if(d->observerMap.empty())
    {
        return ;
    }

    _ZFP_ZFObserverMapType tmp;
    tmp.swap(d->observerMap);

    if(this->observerOwner())
    {
        for(_ZFP_ZFObserverMapType::iterator it = tmp.begin();
            it != tmp.end();
            ++it)
        {
            d->attachMapDetach(it->first);
            this->observerOwner()->observerOnRemove(it->first);
        }
    }
    else
    {
        for(_ZFP_ZFObserverMapType::iterator it = tmp.begin();
            it != tmp.end();
            ++it)
        {
            d->attachMapDetach(it->first);
        }
    }

    for(_ZFP_ZFObserverMapType::iterator it = tmp.begin();
        it != tmp.end();
        ++it)
    {
        for(zfstlsize iObserver = it->second.size() - 1; iObserver != (zfstlsize)-1; --iObserver)
        {
            d->taskIdGenerator.idRelease(it->second[iObserver].taskId);
            zflockfree_zfRelease(it->second[iObserver].userData);
        }
    }
}
zfbool ZFObserverHolder::observerHasAdd(void) const
{
    if(this->observerOwner() != zfnull)
    {
        return !d->observerMap.empty()
            || !ZFObjectGlobalEventObserver().d->observerMap.empty();
    }
    else
    {
        return !d->observerMap.empty();
    }
}
zfbool ZFObserverHolder::observerHasAdd(ZF_IN const zfidentity &eventId) const
{
    if(this->observerOwner() != zfnull)
    {
        _ZFP_ZFObserverMapType &g = ZFObjectGlobalEventObserver().d->observerMap;
        return (d->observerMap.find(eventId) != d->observerMap.end())
            || (g.find(eventId) != g.end());
    }
    else
    {
        return (d->observerMap.find(eventId) != d->observerMap.end());
    }
}

void ZFObserverHolder::observerNotifyWithCustomSender(ZF_IN ZFObject *customSender,
                                                      ZF_IN const zfidentity &eventId,
                                                      ZF_IN_OPT ZFObject *param0 /* = zfnull */,
                                                      ZF_IN_OPT ZFObject *param1 /* = zfnull */) const
{
    zfCoreMutexLock();
    if(eventId == zfidentityInvalid())
    {
        zfCoreMutexUnlock();
        return ;
    }

    _ZFP_ZFObserverListType toNotify;
    zfstldeque<ZFObject *> toRelease;

    d->observerNotifyPrepare(toNotify, toRelease, eventId, this->observerOwner());
    if(this->observerOwner() != zfnull)
    {
        this->observerOwner()->observerOnEvent(eventId, param0, param1);
        ZFObjectGlobalEventObserver().d->observerNotifyPrepare(toNotify, toRelease, eventId, this->observerOwner());
    }
    zfCoreMutexUnlock();

    if(toNotify.empty() && toRelease.empty())
    {
        return ;
    }

    if(!toNotify.empty())
    {
        ZFListenerData listenerData(eventId, customSender, param0, param1);
        for(zfstlsize i = 0; i < toNotify.size(); ++i)
        {
            const _ZFP_ZFObserverData &observerData = toNotify[i];
            observerData.observer.execute(listenerData, observerData.userData);
        }
    }

    zfCoreMutexLock();
    for(zfstlsize i = toRelease.size() - 1; i != (zfstlsize)-1; --i)
    {
        ZFObject *userDataTmp = toRelease[i];
        zflockfree_zfRelease(userDataTmp);
    }
    toNotify.clear(); // must be cleared within mutex to ensure thread safe for callback desctruction
    zfCoreMutexUnlock();
}

void ZFObserverHolder::observerHasAddStateAttach(ZF_IN const zfidentity &eventId,
                                                 ZF_IN_OUT zfuint *flag,
                                                 ZF_IN_OUT zfuint flagBit)
{
    _ZFP_ZFObserverHolderAttachState state;
    state.flag = flag;
    state.flagBit = flagBit;
    d->attachMap[eventId].push_back(state);
}
void ZFObserverHolder::observerHasAddStateDetach(ZF_IN const zfidentity &eventId,
                                                 ZF_IN_OUT zfuint *flag,
                                                 ZF_IN_OUT zfuint flagBit)
{
    _ZFP_ZFObserverHolderAttachStateMapType::iterator it = d->attachMap.find(eventId);
    if(it != d->attachMap.end())
    {
        for(zfstlsize i = 0; i < it->second.size(); ++i)
        {
            _ZFP_ZFObserverHolderAttachState &state = it->second[i];
            if(state.flag == flag && state.flagBit == flagBit)
            {
                it->second.erase(it->second.begin() + i);
                break;
            }
        }
    }
}

void ZFObserverHolder::objectInfoT(ZF_OUT zfstring &ret) const
{
    ret += zfText("<ZFObserverHolder");

    if(this->observerOwner() != zfnull)
    {
        ret += zfText(" ");
        this->observerOwner()->objectInfoT(ret);
    }

    if(d->observerMap.empty())
    {
        ret += zfText(">");
    }
    else
    {
        for(_ZFP_ZFObserverMapType::iterator it = d->observerMap.begin();
            it != d->observerMap.end();
            ++it)
        {
            ret += zfText("\n  ");
            ret += ZFIdMapGetName(it->first);
            ret += zfText(":");

            for(zfstlsize iObserver = 0; iObserver < it->second.size(); ++iObserver)
            {
                ret += zfText("\n    ");
                ret += it->second[iObserver].observer.objectInfo();
            }
        }

        ret += zfText("\n  >");
    }
}

void ZFObserverHolder::_ZFP_ZFObserverHolder_observerOwnerSet(ZF_IN ZFObject *obj) const
{
    /*
     * if observerOwner is not null,
     * means it belongs to a ZFObject, which would:
     * * also send all events to ZFObjectGlobalEventObserver
     * * notify events to owner ZFObject, such as observerOnEvent
     */
    const_cast<ZFObserverHolder *>(this)->_observerOwner = obj;
}

// ============================================================
ZFObserverHolder &_ZFP_ZFObjectGlobalEventObserverRef(void)
{
    static ZFObserverHolder d;
    return d;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFListenerData, zfidentity, eventId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFListenerData, ZFObject *, sender)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFListenerData, ZFObject *, param0)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_VAR(v_ZFListenerData, ZFObject *, param1)

ZFMETHOD_USER_REGISTER_2({return ZFListener(invokerObject->to<v_ZFCallback *>()->zfv).execute(listenerData, userData);}, v_ZFCallback, void, execute, ZFMP_IN(const ZFListenerData &, listenerData), ZFMP_IN_OPT(ZFObject *, userData, zfnull))

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverAddParam, zfidentity const &, eventId)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverAddParam, void, eventIdSet, ZFMP_IN(zfidentity const &, v))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverAddParam, ZFListener const &, observer)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverAddParam, void, observerSet, ZFMP_IN(ZFListener const &, v))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverAddParam, ZFObject * const &, userData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverAddParam, void, userDataSet, ZFMP_IN(ZFObject * const &, v))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverAddParam, ZFObject * const &, owner)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverAddParam, void, ownerSet, ZFMP_IN(ZFObject * const &, v))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverAddParam, zfbool const &, autoRemoveAfterActivate)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverAddParam, void, autoRemoveAfterActivateSet, ZFMP_IN(zfbool const &, v))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverAddParam, ZFLevel const &, observerLevel)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverAddParam, void, observerLevelSet, ZFMP_IN(ZFLevel const &, v))

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_6(v_ZFObserverHolder, zfidentity, observerAdd, ZFMP_IN(const zfidentity &, eventId), ZFMP_IN(const ZFListener &, observer), ZFMP_IN_OPT(ZFObject *, userData, zfnull), ZFMP_IN_OPT(ZFObject *, owner, zfnull), ZFMP_IN_OPT(zfbool, autoRemoveAfterActivate, zffalse), ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverHolder, zfidentity, observerAdd, ZFMP_IN(const ZFObserverAddParam &, param))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFObserverHolder, void, observerRemove, ZFMP_IN(const zfidentity &, eventId), ZFMP_IN(const ZFListener &, callback), ZFMP_IN_OPT(ZFObject *, userData, zfnull), ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, userDataComparer, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverHolder, void, observerRemoveByTaskId, ZFMP_IN(zfidentity, taskId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverHolder, void, observerRemoveByOwner, ZFMP_IN(ZFObject *, owner))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverHolder, void, observerRemoveAll, ZFMP_IN(const zfidentity &, eventId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverHolder, void, observerRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverHolder, zfbool, observerHasAdd)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFObserverHolder, zfbool, observerHasAdd, ZFMP_IN(const zfidentity &, eventId))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_3(v_ZFObserverHolder, void, observerNotify, ZFMP_IN(const zfidentity &, eventId), ZFMP_IN_OPT(ZFObject *, param0, zfnull), ZFMP_IN_OPT(ZFObject *, param1, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFObserverHolder, void, observerNotifyWithCustomSender, ZFMP_IN(ZFObject *, customSender), ZFMP_IN(const zfidentity &, eventId), ZFMP_IN_OPT(ZFObject *, param0, zfnull), ZFMP_IN_OPT(ZFObject *, param1, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFObserverHolder, ZFObject *, observerOwner)

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_0(ZFObserverHolder &, ZFObjectGlobalEventObserver)

ZF_NAMESPACE_GLOBAL_END
#endif

