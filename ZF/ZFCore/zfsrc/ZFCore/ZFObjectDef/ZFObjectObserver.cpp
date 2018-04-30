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
    _ZFP_ZFObserverData *pPrev;
    _ZFP_ZFObserverData *pNext;

public:
    _ZFP_ZFObserverData(void)
    : taskId(zfidentityInvalid())
    , observer()
    , userData(zfnull)
    , owner(zfnull)
    , observerLevel(ZFLevelAppNormal)
    , autoRemoveAfterActivate(zffalse)
    , pPrev(zfnull)
    , pNext(zfnull)
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
    , pPrev(zfnull)
    , pNext(zfnull)
    {
    }
    ~_ZFP_ZFObserverData(void)
    {
        zflockfree_zfRelease(this->userData);
    }
private:
    _ZFP_ZFObserverData(ZF_IN const _ZFP_ZFObserverData &ref);
    _ZFP_ZFObserverData &operator = (ZF_IN const _ZFP_ZFObserverData &ref);
};

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
    zfstlmap<zfidentity, _ZFP_ZFObserverData *> observerMap; // <eventId, p>
    zfstlmap<zfidentity, _ZFP_ZFObserverData *> observerTaskIdMap; // <taskId, p>
    zfstlmap<ZFObject *, zfstlmap<_ZFP_ZFObserverData *, zfidentity> > observerOwnerMap; // <owner, <p, eventId> >
    ZFIdentityGenerator taskIdGenerator;
    _ZFP_ZFObserverHolderAttachStateMapType attachMap;
public:
    _ZFP_ZFObserverHolderPrivate(void)
    : refCount(1)
    , observerMap()
    , observerTaskIdMap()
    , observerOwnerMap()
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
    void observerNotifyPrepare(ZF_IN_OUT zfstldeque<_ZFP_ZFObserverData *> &toNotify,
                               ZF_IN_OUT _ZFP_ZFObserverData *&toDelete,
                               ZF_IN const zfidentity &eventId,
                               ZF_IN ZFObject *observerOwner)
    {
        zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = this->observerMap.find(eventId);
        if(it != this->observerMap.end())
        {
            _ZFP_ZFObserverData *p = it->second;
            do
            {
                toNotify.push_back(p);
                if(p->autoRemoveAfterActivate)
                {
                    this->observerDetach(it, p);

                    if(toDelete == zfnull)
                    {
                        toDelete = p;
                        toDelete->pPrev = toDelete->pNext = zfnull;
                    }
                    else
                    {
                        p->pPrev = zfnull;
                        p->pNext = toDelete;
                        toDelete->pPrev = p;
                        toDelete = p;
                    }
                }
                p = p->pNext;
            } while(p != zfnull);

            if(it->second == zfnull)
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
    void observerDetach(ZF_IN_OUT _ZFP_ZFObserverData *p)
    {
        this->taskIdGenerator.idRelease(p->taskId);
        this->observerTaskIdMap.erase(p->taskId);
        if(p->owner != zfnull)
        {
            zfstlmap<ZFObject *, zfstlmap<_ZFP_ZFObserverData *, zfidentity> >::iterator itTmp = this->observerOwnerMap.find(p->owner);
            itTmp->second.erase(p);
            if(itTmp->second.empty())
            {
                this->observerOwnerMap.erase(itTmp);
            }
        }
    }
    void observerDetach(ZF_IN_OUT zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator &it,
                        ZF_IN_OUT _ZFP_ZFObserverData *p)
    {
        this->observerDetach(p);

        if(it->second == p)
        {
            it->second = p->pNext;
        }

        if(p->pPrev != zfnull)
        {
            p->pPrev->pNext = p->pNext;
        }
        if(p->pNext != zfnull)
        {
            p->pNext->pPrev = p->pPrev;
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
    if(this->observerOwner() && ZFBitTest(this->observerOwner()->objectInstanceState(), ZFObjectInstanceStateOnDealloc))
    {
        zfCoreCriticalMessageTrim(zfTextA("[ZFObject] you must not add observer while object is deallocating, class: %s, event: %s"),
            zfsCoreZ2A(this->observerOwner()->classData()->className()),
            ZFIdMapGetName(eventId));
        return zfidentityInvalid();
    }

    zfidentity taskId = d->taskIdGenerator.idAcquire();
    _ZFP_ZFObserverData *t = zfpoolNew(_ZFP_ZFObserverData
            , taskId
            , observer
            , zflockfree_zfRetain(userData)
            , owner
            , observerLevel
            , autoRemoveAfterActivate
        );
    d->observerTaskIdMap[taskId] = t;
    if(owner != zfnull)
    {
        d->observerOwnerMap[owner][t] = zftrue;
    }

    _ZFP_ZFObserverData *&head = d->observerMap[eventId];
    _ZFP_ZFObserverData *p = head;
    if(p != zfnull)
    {
        while(p->pNext != zfnull && p->pNext->observerLevel <= observerLevel)
        {
            p = p->pNext;
        }
    }
    if(p == zfnull)
    {
        head = t;
        t->pNext = zfnull;
        t->pNext = zfnull;

        d->attachMapAttach(eventId);
        if(this->observerOwner())
        {
            this->observerOwner()->observerOnAdd(eventId);
        }
    }
    else
    {
        t->pNext = p->pNext;
        t->pPrev = p;
        if(p->pNext != zfnull)
        {
            p->pNext->pPrev = t;
        }
        p->pNext = t;
    }

    return taskId;
}
void ZFObserverHolder::observerRemove(ZF_IN const zfidentity &eventId,
                                      ZF_IN const ZFListener &callback,
                                      ZF_IN_OPT ZFObject *userData /* = zfnull */,
                                      ZF_IN_OPT ZFComparer<ZFObject *>::Comparer userDataComparer /* = ZFComparerCheckEqual */) const
{
    zfCoreMutexLocker();

    zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.find(eventId);
    if(it != d->observerMap.end())
    {
        _ZFP_ZFObserverData *p = it->second;
        do
        {
            if(p->observer.objectCompareByInstance(callback) == ZFCompareTheSame
                && (userData == zfnull || userDataComparer(userData, p->userData) == ZFCompareTheSame))
            {
                d->observerDetach(it, p);

                if(it->second == zfnull)
                {
                    d->observerMap.erase(it);
                    d->attachMapDetach(eventId);
                    if(this->observerOwner())
                    {
                        this->observerOwner()->observerOnRemove(eventId);
                    }
                }
                zfpoolDelete(p);
                break;
            }
            p = p->pNext;
        } while(p != zfnull);
    }
}
void ZFObserverHolder::observerRemoveByTaskId(ZF_IN zfidentity taskId) const
{
    zfCoreMutexLocker();
    zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator itTaskId = d->observerTaskIdMap.find(taskId);
    if(itTaskId == d->observerTaskIdMap.end())
    {
        return ;
    }

    _ZFP_ZFObserverData *p = itTaskId->second;
    zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.find(p->taskId);
    d->observerDetach(it, p);
    if(it->second == zfnull)
    {
        d->attachMapDetach(p->taskId);
        if(this->observerOwner())
        {
            this->observerOwner()->observerOnRemove(p->taskId);
        }
    }
    zfpoolDelete(p);
}
void ZFObserverHolder::observerRemoveByOwner(ZF_IN ZFObject *owner) const
{
    zfCoreMutexLocker();
    zfstlmap<ZFObject *, zfstlmap<_ZFP_ZFObserverData *, zfidentity> >::iterator itOwner = d->observerOwnerMap.find(owner);
    if(itOwner == d->observerOwnerMap.end())
    {
        return ;
    }

    zfstlmap<_ZFP_ZFObserverData *, zfidentity> toRemove = itOwner->second;
    for(zfstlmap<_ZFP_ZFObserverData *, zfidentity>::iterator itToRemove = toRemove.begin(); itToRemove != toRemove.end(); ++itToRemove)
    {
        zfidentity eventId = itToRemove->second;
        _ZFP_ZFObserverData *p = itToRemove->first;
        zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.find(eventId);
        if(it != d->observerMap.end())
        {
            d->observerDetach(it, p);
            if(it->second == zfnull)
            {
                if(this->observerOwner())
                {
                    d->attachMapDetach(eventId);
                    this->observerOwner()->observerOnRemove(eventId);
                }
            }
        }
        zfpoolDelete(p);
    }
}
void ZFObserverHolder::observerRemoveAll(ZF_IN const zfidentity &eventId) const
{
    zfCoreMutexLocker();
    zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.find(eventId);
    if(it == d->observerMap.end())
    {
        return ;
    }
    zfstldeque<_ZFP_ZFObserverData *> toDelete;
    while(it->second != zfnull)
    {
        toDelete.push_back(it->second);
        d->observerDetach(it, it->second);
    }
    d->observerMap.erase(it);

    d->attachMapDetach(eventId);
    if(this->observerOwner())
    {
        this->observerOwner()->observerOnRemove(eventId);
    }

    for(zfstlsize i = toDelete.size() - 1; i != (zfstlsize)-1; --i)
    {
        zfpoolDelete(toDelete[i]);
    }
}
void ZFObserverHolder::observerRemoveAll(void) const
{
    zfCoreMutexLocker();
    if(d->observerMap.empty())
    {
        return ;
    }

    zfstlmap<zfidentity, _ZFP_ZFObserverData *> tmp;
    tmp.swap(d->observerMap);
    d->observerMap.clear();
    d->observerTaskIdMap.clear();
    d->observerOwnerMap.clear();

    if(this->observerOwner())
    {
        for(zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = tmp.begin();
            it != tmp.end();
            ++it)
        {
            d->attachMapDetach(it->first);
            this->observerOwner()->observerOnRemove(it->first);
        }
    }
    else
    {
        for(zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = tmp.begin();
            it != tmp.end();
            ++it)
        {
            d->attachMapDetach(it->first);
        }
    }

    for(zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = tmp.begin();
        it != tmp.end();
        ++it)
    {
        _ZFP_ZFObserverData *p = it->second;
        do
        {
            _ZFP_ZFObserverData *t = p;
            p = p->pNext;
            zfpoolDelete(t);
        } while(p != zfnull);
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
        zfstlmap<zfidentity, _ZFP_ZFObserverData *> &g = ZFObjectGlobalEventObserver().d->observerMap;
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

    zfstldeque<_ZFP_ZFObserverData *> toNotify;
    _ZFP_ZFObserverData *toDelete = zfnull;

    d->observerNotifyPrepare(toNotify, toDelete, eventId, this->observerOwner());
    if(this->observerOwner() != zfnull)
    {
        this->observerOwner()->observerOnEvent(eventId, param0, param1);
        ZFObjectGlobalEventObserver().d->observerNotifyPrepare(toNotify, toDelete, eventId, this->observerOwner());
    }
    zfCoreMutexUnlock();

    if(!toNotify.empty())
    {
        ZFListenerData listenerData(eventId, customSender, param0, param1);
        for(zfstlsize i = 0; i < toNotify.size(); ++i)
        {
            const _ZFP_ZFObserverData &observerData = *(toNotify[i]);
            observerData.observer.execute(listenerData, observerData.userData);
        }
    }

    if(toDelete != zfnull)
    {
        zfCoreMutexLock();
        do
        {
            _ZFP_ZFObserverData *t = toDelete;
            toDelete = toDelete->pNext;
            zfpoolDelete(t);
        } while(toDelete != zfnull);
        zfCoreMutexUnlock();
    }
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
    zfCoreMutexLocker();
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
        for(zfstlmap<zfidentity, _ZFP_ZFObserverData *>::iterator it = d->observerMap.begin();
            it != d->observerMap.end();
            ++it)
        {
            ret += zfText("\n  ");
            ret += ZFIdMapGetName(it->first);
            ret += zfText(":");

            _ZFP_ZFObserverData *p = it->second;
            do
            {
                ret += zfText("\n    ");
                ret += p->observer.objectInfo();
                p = p->pNext;
            } while(p != zfnull);
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
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFObserverHolder, void, observerRemove, ZFMP_IN(const zfidentity &, eventId), ZFMP_IN(const ZFListener &, callback))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_4(v_ZFObserverHolder, void, observerRemove, ZFMP_IN(const zfidentity &, eventId), ZFMP_IN(const ZFListener &, callback), ZFMP_IN(ZFObject *, userData), ZFMP_IN_OPT(ZFComparer<ZFObject *>::Comparer, userDataComparer, ZFComparerCheckEqual))
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

