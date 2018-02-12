/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFSerializableData.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableUtil.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_deque.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfbool ZFSerializableDataResolveCheckEnable = zftrue;

// ============================================================
// _ZFP_ZFSerializableDataPrivate
zfclassNotPOD _ZFP_ZFSerializableDataAttributeData
{
public:
    zfstlstringZ attrValue;
    zfbool resolved;
public:
    _ZFP_ZFSerializableDataAttributeData(void)
    : attrValue()
    , resolved(zffalse)
    {
    }
    _ZFP_ZFSerializableDataAttributeData(ZF_IN const zfchar *attrValue,
                                         ZF_IN_OPT zfbool resolved = zffalse)
    : attrValue(attrValue)
    , resolved(resolved)
    {
    }
};
typedef zfstlmap<zfstlstringZ, _ZFP_ZFSerializableDataAttributeData> _ZFP_ZFSerializableDataAttributeMapType;
typedef zfstlmap<zfstlstringZ, zfautoObject> _ZFP_ZFSerializableDataTagMapType;
zfclassNotPOD _ZFP_ZFSerializableDataPrivate
{
public:
    zfuint refCount;
    _ZFP_ZFSerializableDataPrivate *serializableDataParent;
    zfstlstringZ className;
    zfbool resolved;
    zfbool refInfoLoaded;
    ZFRefInfo *refInfo;
    ZFPathInfo *pathInfo;
    _ZFP_ZFSerializableDataAttributeMapType attributes;
    zfstldeque<ZFSerializableData> elements;
    _ZFP_ZFSerializableDataTagMapType serializableDataTagMap;

public:
    void removeAll(void)
    {
        this->className.clear();
        this->attributes.clear();
        if(!this->elements.empty())
        {
            for(zfstlsize i = this->elements.size() - 1; i != (zfstlsize)-1; --i)
            {
                this->elements[i].d->serializableDataParent = zfnull;
            }
            this->elements.clear();
        }

        zfdelete(this->refInfo);
        this->refInfo = zfnull;
        this->refInfoLoaded = zffalse;

        this->serializableDataTagMap.clear();
    }

public:
    _ZFP_ZFSerializableDataPrivate(void)
    : refCount(1)
    , serializableDataParent(zfnull)
    , className()
    , resolved(zffalse)
    , refInfoLoaded(zffalse)
    , refInfo(zfnull)
    , pathInfo(zfnull)
    , attributes()
    , elements()
    , serializableDataTagMap()
    {
    }
    ~_ZFP_ZFSerializableDataPrivate(void)
    {
        if(!this->elements.empty())
        {
            for(zfstlsize i = 0; i < this->elements.size(); ++i)
            {
                this->elements[i].d->serializableDataParent = zfnull;
            }
        }

        zfdelete(this->refInfo);
        zfdelete(this->pathInfo);
    }
};

// ============================================================
// ZFSerializableData
/** @cond ZFPrivateDoc */
ZFSerializableData::ZFSerializableData(ZF_IN _ZFP_ZFSerializableDataPrivate *d)
{
    zfCoreMutexLocker();
    this->d = d;
    ++(d->refCount);
}
/** @endcond */
ZFSerializableData::ZFSerializableData(void)
{
    zfCoreMutexLocker();
    d = zfnew(_ZFP_ZFSerializableDataPrivate);
}
ZFSerializableData::ZFSerializableData(ZF_IN const ZFSerializableData &ref)
{
    zfCoreMutexLocker();
    d = ref.d;
    ++(d->refCount);
}
ZFSerializableData &ZFSerializableData::operator =(ZF_IN const ZFSerializableData &ref)
{
    zfCoreMutexLocker();
    _ZFP_ZFSerializableDataPrivate *dTmp = d;
    d = ref.d;
    ++(ref.d->refCount);
    --(dTmp->refCount);
    if(dTmp->refCount == 0)
    {
        zfdelete(dTmp);
    }
    return *this;
}
zfbool ZFSerializableData::operator ==(ZF_IN const ZFSerializableData &ref) const
{
    return (d == ref.d);
}
ZFSerializableData::~ZFSerializableData(void)
{
    zfCoreMutexLocker();
    --(d->refCount);
    if(d->refCount == 0)
    {
        zfdelete(d);
    }
}

void ZFSerializableData::copyFrom(ZF_IN const ZFSerializableData &ref)
{
    if(d == ref.d)
    {
        return ;
    }

    d->className = ref.d->className;
    d->attributes = ref.d->attributes;

    d->refInfoLoaded = ref.d->refInfoLoaded;
    if(d->refInfo == zfnull)
    {
        if(ref.d->refInfo != zfnull)
        {
            d->refInfo = zfnew(ZFRefInfo, *(ref.d->refInfo));
        }
    }
    else
    {
        if(ref.d->refInfo == zfnull)
        {
            zfdelete(d->refInfo);
            d->refInfo = zfnull;
        }
        else
        {
            *(d->refInfo) = *(ref.d->refInfo);
        }
    }

    if(d->pathInfo == zfnull)
    {
        if(ref.d->pathInfo != zfnull)
        {
            d->pathInfo = zfnew(ZFPathInfo, *(ref.d->pathInfo));
        }
    }
    else
    {
        if(ref.d->pathInfo == zfnull)
        {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
        else
        {
            *(d->pathInfo) = *(ref.d->pathInfo);
        }
    }

    d->serializableDataTagMap = ref.d->serializableDataTagMap;

    this->elementRemoveAll();
    for(zfstlsize i = 0; i < ref.d->elements.size(); ++i)
    {
        this->elementAdd(ref.d->elements[i].copy());
    }
}

zfindex ZFSerializableData::objectRetainCount(void) const
{
    return d->refCount;
}

// ============================================================
// reference logic
void ZFSerializableData::refInfoSet(ZF_IN const ZFRefInfo *refInfo)
{
    if(refInfo != zfnull && !(refInfo->refType.isEmpty() && refInfo->refData.isEmpty()))
    {
        if(d->refInfo == zfnull)
        {
            d->refInfo = zfnew(ZFRefInfo, *refInfo);
        }
        else
        {
            *(d->refInfo) = *refInfo;
        }
    }
    else
    {
        if(d->refInfo != zfnull)
        {
            zfdelete(d->refInfo);
            d->refInfo = zfnull;
        }
    }
}
void ZFSerializableData::refInfoSet(ZF_IN const zfchar *refType, ZF_IN const zfchar *refData)
{
    if(!zfsIsEmpty(refType) || !zfsIsEmpty(refData))
    {
        if(d->refInfo == zfnull)
        {
            d->refInfo = zfnew(ZFRefInfo, refType, refData);
        }
        else
        {
            d->refInfo->refType = refType;
            d->refInfo->refData = refData;
        }
    }
    else
    {
        if(d->refInfo != zfnull)
        {
            zfdelete(d->refInfo);
            d->refInfo = zfnull;
        }
    }
}
const ZFRefInfo *ZFSerializableData::refInfo(void) const
{
    return d->refInfo;
}

void ZFSerializableData::refInfoRemove(void)
{
    zfdelete(d->refInfo);
    d->refInfo = zfnull;
    d->refInfoLoaded = zffalse;
}
void ZFSerializableData::refInfoRemoveRecursively(void)
{
    zfdelete(d->refInfo);
    d->refInfo = zfnull;
    d->refInfoLoaded = zffalse;
    for(zfstlsize i = 0; i < d->elements.size(); ++i)
    {
        d->elements[i].refInfoRemoveRecursively();
    }
}

zfbool ZFSerializableData::refInfoExist(void) const
{
    return (d->refInfo != zfnull);
}
zfbool ZFSerializableData::refInfoExistRecursively(void) const
{
    if(d->refInfo != zfnull)
    {
        return zftrue;
    }
    for(zfstlsize i = 0; i < d->elements.size(); ++i)
    {
        if(d->elements[i].refInfoExistRecursively())
        {
            return zftrue;
        }
    }
    return zffalse;
}

zfbool ZFSerializableData::_ZFP_ZFSerializableData_refInfoLoad(ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                               ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator itRefType = d->attributes.find(ZFSerializableKeyword_refType);
    _ZFP_ZFSerializableDataAttributeMapType::iterator itRefData = d->attributes.find(ZFSerializableKeyword_refData);
    if(itRefType != d->attributes.end() || itRefData != d->attributes.end())
    {
        if(itRefType == d->attributes.end())
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, *this,
                zfText("missing \"%s\""), ZFSerializableKeyword_refType);
            return zffalse;
        }
        if(itRefData == d->attributes.end())
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, *this,
                zfText("missing \"%s\""), ZFSerializableKeyword_refData);
            return zffalse;
        }

        this->refInfoSet(itRefType->second.attrValue.c_str(), itRefData->second.attrValue.c_str());
        d->attributes.erase(itRefType);
        d->attributes.erase(itRefData);
    }

    for(zfstlsize i = 0; i < d->elements.size(); ++i)
    {
        if(!d->elements[i]._ZFP_ZFSerializableData_refInfoLoad(outErrorHint, outErrorPos))
        {
            return zffalse;
        }
    }
    return zftrue;
}
zfbool ZFSerializableData::_ZFP_ZFSerializableData_refInfoApply(ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                                ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    zfbool recursiveRef = zffalse;
    if(d->refInfo == zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator itRefType = d->attributes.find(ZFSerializableKeyword_refType);
        _ZFP_ZFSerializableDataAttributeMapType::iterator itRefData = d->attributes.find(ZFSerializableKeyword_refData);
        if(itRefType != d->attributes.end() || itRefData != d->attributes.end())
        {
            if(itRefType == d->attributes.end())
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, *this,
                    zfText("missing \"%s\""), ZFSerializableKeyword_refType);
                return zffalse;
            }
            if(itRefData == d->attributes.end())
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, *this,
                    zfText("missing \"%s\""), ZFSerializableKeyword_refData);
                return zffalse;
            }

            this->refInfoSet(itRefType->second.attrValue.c_str(), itRefData->second.attrValue.c_str());
            d->attributes.erase(itRefType);
            d->attributes.erase(itRefData);
        }
    }
    if(d->refInfo != zfnull && !d->refInfoLoaded)
    {
        // load reference
        ZFSerializableData tmpData;
        _ZFP_ZFSerializableDataRefInfoLoadCallback callback = _ZFP_ZFSerializableDataRefTypeGet(d->refInfo->refType);
        if(callback == zfnull || !callback(tmpData, d->refInfo->refData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }

        // remove recursive reference info
        // only root node's reference info should be stored
        tmpData.refInfoRemoveRecursively();

        // copy all contents
        d->attributes.insert(tmpData.d->attributes.begin(), tmpData.d->attributes.end());

        for(zfstlsize i = 0; i < tmpData.d->elements.size(); ++i)
        {
            tmpData.d->elements[i].d->serializableDataParent = d;
        }
        tmpData.d->elements.insert(tmpData.d->elements.end(), d->elements.begin(), d->elements.end());
        d->elements.clear();
        tmpData.d->elements.swap(d->elements);

        tmpData.d->serializableDataTagMap.insert(d->serializableDataTagMap.begin(), d->serializableDataTagMap.end());
        tmpData.d->serializableDataTagMap.swap(d->serializableDataTagMap);

        if(recursiveRef)
        {
            this->refInfoRemove();
        }
        else
        {
            d->refInfoLoaded = zftrue;
        }
    }

    for(zfstlsize i = 0; i < d->elements.size(); ++i)
    {
        if(!d->elements[i]._ZFP_ZFSerializableData_refInfoApply(outErrorHint, outErrorPos))
        {
            return zffalse;
        }
    }
    return zftrue;
}
zfbool ZFSerializableData::refInfoLoad(ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                       ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    return this->_ZFP_ZFSerializableData_refInfoLoad(outErrorHint, outErrorPos)
        && this->_ZFP_ZFSerializableData_refInfoApply(outErrorHint, outErrorPos);
}

void ZFSerializableData::refInfoRestore(ZF_IN const ZFSerializableData &refNode)
{
    for(_ZFP_ZFSerializableDataAttributeMapType::iterator itRef = refNode.d->attributes.begin(); itRef != refNode.d->attributes.end(); ++itRef)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(itRef->first);
        if(it != d->attributes.end() && it->second.attrValue.compare(itRef->second.attrValue) == 0)
        {
            d->attributes.erase(it);
        }
    }
    for(zfstlsize i = 0, iRef = 0; i < d->elements.size() && iRef < refNode.d->elements.size(); ++i, ++iRef)
    {
        ZFSerializableData &child = d->elements[i];
        ZFSerializableData &childRef = refNode.d->elements[iRef];
        child.refInfoRestore(childRef);
        if(child.d->attributes.empty() && child.d->elements.empty())
        {
            this->elementRemoveAtIndex(i);
            --i;
        }
    }

    if(refNode.d->refInfo != zfnull)
    {
        this->attributeSet(ZFSerializableKeyword_refType, refNode.d->refInfo->refType);
        this->attributeSet(ZFSerializableKeyword_refType, refNode.d->refInfo->refData);
    }

    zfdelete(d->refInfo);
    d->refInfo = zfnull;
    d->refInfoLoaded = zffalse;
}

// ============================================================
// local path logic
const ZFPathInfo *ZFSerializableData::pathInfo(void) const
{
    return d->pathInfo;
}
void ZFSerializableData::pathInfoSet(ZF_IN const ZFPathInfo *pathInfo)
{
    if(pathInfo != zfnull && !(pathInfo->pathType.isEmpty() && pathInfo->pathData.isEmpty()))
    {
        if(d->pathInfo == zfnull)
        {
            d->pathInfo = zfnew(ZFPathInfo, *pathInfo);
        }
        else
        {
            *(d->pathInfo) = *pathInfo;
        }
    }
    else
    {
        if(d->pathInfo != zfnull)
        {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
    }
}
void ZFSerializableData::pathInfoSet(ZF_IN const zfchar *pathType, ZF_IN const zfchar *pathData)
{
    if(!zfsIsEmpty(pathType) || !zfsIsEmpty(pathData))
    {
        if(d->pathInfo == zfnull)
        {
            d->pathInfo = zfnew(ZFPathInfo, pathType, pathData);
        }
        else
        {
            d->pathInfo->pathType = pathType;
            d->pathInfo->pathData = pathData;
        }
    }
    else
    {
        if(d->pathInfo != zfnull)
        {
            zfdelete(d->pathInfo);
            d->pathInfo = zfnull;
        }
    }
}
const ZFPathInfo *ZFSerializableData::pathInfoCheck(void) const
{
    _ZFP_ZFSerializableDataPrivate *check = d;
    do
    {
        if(check->pathInfo != zfnull && !check->pathInfo->pathData.isEmpty())
        {
            return check->pathInfo;
        }
        check = check->serializableDataParent;
    } while(check != zfnull);
    return zfnull;
}

// ============================================================
// parent
zfbool ZFSerializableData::serializableDataParent(ZF_OUT ZFSerializableData &ret) const
{
    if(d->serializableDataParent != zfnull)
    {
        ret = ZFSerializableData(d->serializableDataParent);
        return zftrue;
    }
    else
    {
        return zffalse;
    }
}

// ============================================================
// class
void ZFSerializableData::itemClassSet(ZF_IN const zfchar *clsName)
{
    if(clsName == zfnull)
    {
        d->className.clear();
    }
    else
    {
        d->className = clsName;
    }
}
const zfchar *ZFSerializableData::itemClass(void) const
{
    return d->className.empty() ? zfnull : d->className.c_str();
}

// ============================================================
// other types
void ZFSerializableData::removeAll(void)
{
    d->removeAll();
}

// ============================================================
void ZFSerializableData::serializableDataTagSet(ZF_IN const zfchar *key,
                                                ZF_IN ZFObject *tag)
{
    if(key == zfnull)
    {
        return ;
    }

    _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
    _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
    if(it == m.end())
    {
        if(tag != zfnull)
        {
            m[key] = tag;
        }
    }
    else
    {
        if(tag == zfnull)
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            m.erase(it);
        }
        else
        {
            zfautoObject holder = it->second;
            ZFUNUSED(holder);
            it->second = tag;
        }
    }
}
ZFObject *ZFSerializableData::serializableDataTagGet(ZF_IN const zfchar *key) const
{
    if(key != zfnull)
    {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
        if(it != m.end())
        {
            return it->second.toObject();
        }
    }
    return zfnull;
}
void ZFSerializableData::serializableDataTagGetAllKeyValue(ZF_IN_OUT ZFCoreArray<const zfchar *> &allKey,
                                                           ZF_IN_OUT ZFCoreArray<ZFObject *> &allValue) const
{
    _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
    allKey.capacitySet(allKey.count() + m.size());
    allValue.capacitySet(allValue.count() + m.size());
    for(_ZFP_ZFSerializableDataTagMapType::iterator it = m.begin(); it != m.end(); ++it)
    {
        allKey.add(it->first.c_str());
        allValue.add(it->second.toObject());
    }
}
void ZFSerializableData::serializableDataTagRemove(ZF_IN const zfchar *key)
{
    if(key != zfnull)
    {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
        if(it != m.end())
        {
            m.erase(it);
        }
    }
}
zfautoObject ZFSerializableData::serializableDataTagRemoveAndGet(ZF_IN const zfchar *key)
{
    if(key != zfnull)
    {
        _ZFP_ZFSerializableDataTagMapType &m = d->serializableDataTagMap;
        _ZFP_ZFSerializableDataTagMapType::iterator it = m.find(key);
        if(it != m.end())
        {
            zfautoObject ret = it->second;
            m.erase(it);
            return ret;
        }
    }
    return zfautoObjectNull();
}
void ZFSerializableData::serializableDataTagRemoveAll(void)
{
    if(!d->serializableDataTagMap.empty())
    {
        _ZFP_ZFSerializableDataTagMapType tmp;
        tmp.swap(d->serializableDataTagMap);
    }
}

// ============================================================
// attribute
void ZFSerializableData::attributeSet(ZF_IN const zfchar *name,
                                      ZF_IN const zfchar *value)
{
    if(name != zfnull)
    {
        if(value != zfnull)
        {
            _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
            if(it != d->attributes.end())
            {
                it->second = _ZFP_ZFSerializableDataAttributeData(value);
            }
            else
            {
                d->attributes[name] = _ZFP_ZFSerializableDataAttributeData(value);
            }
        }
        else
        {
            d->attributes.erase(name);
        }
    }
}
const zfchar *ZFSerializableData::attributeForName(ZF_IN const zfchar *name) const
{
    if(name != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end())
        {
            return it->second.attrValue.c_str();
        }
    }
    return zfnull;
}

zfindex ZFSerializableData::attributeCount(void) const
{
    return (zfindex)d->attributes.size();
}
void ZFSerializableData::attributeRemove(ZF_IN const zfchar *name)
{
    if(name != zfnull)
    {
        d->attributes.erase(name);
    }
}
void ZFSerializableData::attributeRemoveAll(void)
{
    d->attributes.clear();
}

static void _ZFP_ZFSerializableData_iteratorDeleteCallback(void *data)
{
    zfdelete(ZFCastStatic(_ZFP_ZFSerializableDataAttributeMapType::iterator *, data));
}
static void *_ZFP_ZFSerializableData_iteratorCopyCallback(void *data)
{
    return zfnew(_ZFP_ZFSerializableDataAttributeMapType::iterator,
        *ZFCastStatic(_ZFP_ZFSerializableDataAttributeMapType::iterator *, data));
}
zfiterator ZFSerializableData::attributeIteratorForName(ZF_IN const zfchar *name) const
{
    return zfiterator(
        zfnew(_ZFP_ZFSerializableDataAttributeMapType::iterator, name ? d->attributes.find(name) : d->attributes.end()),
        _ZFP_ZFSerializableData_iteratorDeleteCallback,
        _ZFP_ZFSerializableData_iteratorCopyCallback);
}
zfiterator ZFSerializableData::attributeIterator(void) const
{
    return zfiterator(
        zfnew(_ZFP_ZFSerializableDataAttributeMapType::iterator, d->attributes.begin()),
        _ZFP_ZFSerializableData_iteratorDeleteCallback,
        _ZFP_ZFSerializableData_iteratorCopyCallback);
}
zfbool ZFSerializableData::attributeIteratorIsValid(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    return (data != zfnull && *data != d->attributes.end());
}
zfbool ZFSerializableData::attributeIteratorIsEqual(ZF_IN const zfiterator &it0,
                                                    ZF_IN const zfiterator &it1) const
{
    return zfiterator::iteratorIsEqual<_ZFP_ZFSerializableDataAttributeMapType::iterator *>(it0, it1);
}
void ZFSerializableData::attributeIteratorSet(ZF_IN_OUT zfiterator &it,
                                              ZF_IN const zfchar *value)
{
    if(value == zfnull)
    {
        this->attributeIteratorRemove(it);
        return ;
    }
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data)
    {
        (*data)->second = _ZFP_ZFSerializableDataAttributeData(value);
    }
}
void ZFSerializableData::attributeIteratorRemove(ZF_IN_OUT zfiterator &it)
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        d->attributes.erase((*data)++);
    }
}
const zfchar *ZFSerializableData::attributeIteratorGetKey(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        return (*data)->first.c_str();
    }
    return zfnull;
}
const zfchar *ZFSerializableData::attributeIteratorGet(ZF_IN const zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        return (*data)->second.attrValue.c_str();
    }
    return zfnull;
}
const zfchar *ZFSerializableData::attributeIteratorNextKey(ZF_IN_OUT zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        const zfchar *ret = (*data)->first.c_str();
        ++(*data);
        return ret;
    }
    return zfnull;
}
const zfchar *ZFSerializableData::attributeIteratorNext(ZF_IN_OUT zfiterator &it) const
{
    _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
    if(data != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeData &ret = (*data)->second;
        ++(*data);
        return ret.attrValue.c_str();
    }
    return zfnull;
}

zfbool ZFSerializableData::attributeIteratorResolved(ZF_IN const zfiterator &it) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
        if(data != zfnull)
        {
            return (*data)->second.resolved;
        }
    }
    return zffalse;
}
void ZFSerializableData::attributeIteratorResolveMark(ZF_IN const zfiterator &it) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
        if(data != zfnull)
        {
            (*data)->second.resolved = zftrue;
        }
    }
}
void ZFSerializableData::attributeIteratorResolveUnmark(ZF_IN const zfiterator &it) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator *data = it.data<_ZFP_ZFSerializableDataAttributeMapType::iterator *>();
        if(data != zfnull)
        {
            (*data)->second.resolved = zftrue;
        }
    }
}

// ============================================================
// element
void ZFSerializableData::elementAdd(ZF_IN const ZFSerializableData &element)
{
    zfCoreAssertWithMessage(d != element.d, zfTextA("adding self is not allowed"));
    zfCoreAssertWithMessage(d->serializableDataParent == zfnull, zfTextA("adding a data which already has parent"));
    d->elements.push_back(element);
    element.d->serializableDataParent = d;
}
void ZFSerializableData::elementAdd(ZF_IN const ZFSerializableData &element,
                                    ZF_IN zfindex atIndex)
{
    zfCoreAssertWithMessage(d != element.d, zfTextA("adding self is not allowed"));
    zfCoreAssertWithMessage(d->serializableDataParent == zfnull, zfTextA("adding a data which already has parent"));
    if(atIndex == zfindexMax())
    {
        atIndex = (zfindex)d->elements.size();
    }
    if(atIndex > (zfindex)d->elements.size())
    {
        zfCoreCriticalIndexOutOfRange(atIndex, (zfindex)(d->elements.size() + 1));
        return ;
    }

    d->elements.insert(d->elements.begin() + atIndex, element);
    element.d->serializableDataParent = d;
}

void ZFSerializableData::elementSetAtIndex(ZF_IN zfindex index,
                                           ZF_IN const ZFSerializableData &element)
{
    d->elements[(zfstlsize)index].d->serializableDataParent = zfnull;
    d->elements[(zfstlsize)index] = element;
    element.d->serializableDataParent = d;
}

zfindex ZFSerializableData::elementFindByName(ZF_IN const zfchar *name) const
{
    if(name != zfnull && *name != '\0')
    {
        for(zfstlsize i = 0; i < d->elements.size(); ++i)
        {
            if(zfscmpTheSame(d->elements[i].propertyName(), name))
            {
                return i;
            }
        }
    }
    return zfindexMax();
}
zfindex ZFSerializableData::elementFindByCategory(ZF_IN const zfchar *category) const
{
    if(category != zfnull && *category != '\0')
    {
        for(zfstlsize i = 0; i < d->elements.size(); ++i)
        {
            if(zfscmpTheSame(d->elements[i].category(), category))
            {
                return i;
            }
        }
    }
    return zfindexMax();
}

zfindex ZFSerializableData::elementCount(void) const
{
    return (zfindex)d->elements.size();
}
const ZFSerializableData &ZFSerializableData::elementAtIndex(ZF_IN zfindex index) const
{
    return d->elements[(zfstlsize)index];
}
ZFSerializableData &ZFSerializableData::elementAtIndex(ZF_IN zfindex index)
{
    return d->elements[(zfstlsize)index];
}

void ZFSerializableData::elementRemoveAtIndex(ZF_IN zfindex index)
{
    d->elements[(zfstlsize)index].d->serializableDataParent = zfnull;
    d->elements.erase(d->elements.begin() + (zfstlsize)index);
}
void ZFSerializableData::elementRemoveAll(void)
{
    if(!d->elements.empty())
    {
        for(zfstlsize i = 0; i < d->elements.size(); ++i)
        {
            d->elements[i].d->serializableDataParent = zfnull;
        }
        d->elements.clear();
    }
}

// ============================================================
// resolved state
zfbool ZFSerializableData::resolved(void) const
{
    return ZFSerializableDataResolveCheckEnable && d->resolved;
}
void ZFSerializableData::resolveMark(void) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        d->resolved = zftrue;
    }
}
void ZFSerializableData::resolveUnmark(void) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        d->resolved = zffalse;
    }
}

zfbool ZFSerializableData::resolvedAttribute(ZF_IN const zfchar *name) const
{
    if(ZFSerializableDataResolveCheckEnable && name != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end())
        {
            return it->second.resolved;
        }
    }
    return zffalse;
}
void ZFSerializableData::resolveAttributeMark(ZF_IN const zfchar *name) const
{
    if(ZFSerializableDataResolveCheckEnable && name != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end())
        {
            it->second.resolved = zftrue;
        }
    }
}
void ZFSerializableData::resolveAttributeUnmark(ZF_IN const zfchar *name) const
{
    if(ZFSerializableDataResolveCheckEnable && name != zfnull)
    {
        _ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.find(name);
        if(it != d->attributes.end())
        {
            it->second.resolved = zffalse;
        }
    }
}

zfbool ZFSerializableData::resolvedAll(ZF_OUT_OPT const ZFSerializableData **firstNotResolvedElement /* = zfnull */,
                                       ZF_OUT_OPT zfstring *firstNotResolvedAttribute /* = zfnull */) const
{
    if(!ZFSerializableDataResolveCheckEnable)
    {
        return zffalse;
    }
    if(!this->resolved())
    {
        if(firstNotResolvedElement != zfnull)
        {
            *firstNotResolvedElement = this;
        }
        return zffalse;
    }
    for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.begin(); it != d->attributes.end(); ++it)
    {
        if(!it->second.resolved)
        {
            if(firstNotResolvedElement != zfnull)
            {
                *firstNotResolvedElement = this;
            }
            if(firstNotResolvedAttribute != zfnull)
            {
                *firstNotResolvedAttribute += it->first.c_str();
            }
            return zffalse;
        }
    }
    for(zfindex i = 0; i < this->elementCount(); ++i)
    {
        if(!this->elementAtIndex(i).resolvedAll(firstNotResolvedElement, firstNotResolvedAttribute))
        {
            return zffalse;
        }
    }
    return zftrue;
}
void ZFSerializableData::resolveMarkAll(void) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        this->resolveMark();
        this->resolveAttributeMarkAll();
        for(zfindex i = 0; i < this->elementCount(); ++i)
        {
            this->elementAtIndex(i).resolveMarkAll();
        }
    }
}
void ZFSerializableData::resolveUnmarkAll(void) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        this->resolveUnmark();
        this->resolveAttributeUnmarkAll();
        for(zfindex i = 0; i < this->elementCount(); ++i)
        {
            this->elementAtIndex(i).resolveUnmarkAll();
        }
    }
}
void ZFSerializableData::resolveAttributeMarkAll(void) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.begin(); it != d->attributes.end(); ++it)
        {
            it->second.resolved = zftrue;
        }
    }
}
void ZFSerializableData::resolveAttributeUnmarkAll(void) const
{
    if(ZFSerializableDataResolveCheckEnable)
    {
        for(_ZFP_ZFSerializableDataAttributeMapType::iterator it = d->attributes.begin(); it != d->attributes.end(); ++it)
        {
            it->second.resolved = zftrue;
        }
    }
}

// ============================================================
// other functions
void ZFSerializableData::objectInfoT(ZF_IN_OUT zfstring &ret) const
{
    ret += '{';

    if(this->itemClass() == zfnull)
    {
        ret += ZFTOKEN_zfnull;
    }
    else
    {
        ret += this->itemClass();
    }

    if(this->refInfo() != zfnull)
    {
        ret += zfText("<");
        ret += this->refInfo()->refType;
        ret += zfText(":\"");
        ret += this->refInfo()->refData;
        ret += zfText("\">");
    }
    if(this->attributeCount() > 0)
    {
        ret += '(';
        zfbool first = zftrue;
        for(zfiterator it = this->attributeIterator(); this->attributeIteratorIsValid(it); this->attributeIteratorNext(it))
        {
            if(first)
            {
                first = zffalse;
            }
            else
            {
                ret += ' ';
            }
            ret += this->attributeIteratorGetKey(it);
            ret += zfText("=\"");
            ret += this->attributeIteratorGet(it);
            ret += zfText("\";");
        }
        ret += ')';
    }

    if(this->elementCount() > 0)
    {
        ret += zfText(" [\n");
        for(zfindex i = 0; i < this->elementCount(); ++i)
        {
            zfstring tmpInfo = this->elementAtIndex(i).objectInfo();
            ret += zfText("    ");
            for(zfindex i = 0; i < tmpInfo.length(); ++i)
            {
                ret += tmpInfo[i];
                if(tmpInfo[i] == '\n')
                {
                    ret += zfText("    ");
                }
            }
            ret += zfText("\n");
        }
        ret += ']';
    }

    ret += '}';
}

ZFCompareResult ZFSerializableData::objectCompare(ZF_IN const ZFSerializableData &another) const
{
    if(d == another.d)
    {
        return ZFCompareTheSame;
    }

    if(!zfscmpTheSame(this->itemClass(), another.itemClass())
        || this->attributeCount() != another.attributeCount()
        || this->elementCount() != another.elementCount())
    {
        return ZFCompareUncomparable;
    }

    for(zfiterator it = another.attributeIterator(); another.attributeIteratorIsValid(it); another.attributeIteratorNext(it))
    {
        const zfchar *value = this->attributeForName(another.attributeIteratorGetKey(it));
        if(value == zfnull || !zfscmpTheSame(another.attributeIteratorGet(it), value))
        {
            return ZFCompareUncomparable;
        }
    }
    for(zfindex i = 0; i < this->elementCount(); ++i)
    {
        if(this->elementAtIndex(i).objectCompare(another.elementAtIndex(i)) != ZFCompareTheSame)
        {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareTheSame;
}

zfbool ZFSerializableData::isEmpty(void) const
{
    return (
        d->className.empty()
        && d->attributes.empty()
        && d->elements.empty()
        && (d->refInfo == zfnull || (d->refInfo->refType.isEmpty() && d->refInfo->refData.isEmpty()))
        );
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, copyFrom, ZFMP_IN(const ZFSerializableData &, ref))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, ZFSerializableData, copy)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const ZFRefInfo *, refInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, refInfoSet, ZFMP_IN(const ZFRefInfo *, refInfo))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, refInfoSet, ZFMP_IN(const zfchar *, refType), ZFMP_IN(const zfchar *, refData))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, refInfoRemove)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, refInfoRemoveRecursively)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, refInfoExist)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, refInfoExistRecursively)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, zfbool, refInfoLoad, ZFMP_OUT_OPT(zfstring *, outErrorHint, zfnull), ZFMP_OUT_OPT(ZFSerializableData *, outErrorPos, zfnull))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, refInfoRestore, ZFMP_IN(const ZFSerializableData &, refNode))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const ZFPathInfo *, pathInfo)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, pathInfoSet, ZFMP_IN(const ZFPathInfo *, pathInfo))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, pathInfoSet, ZFMP_IN(const zfchar *, pathType), ZFMP_IN(const zfchar *, pathData))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const ZFPathInfo *, pathInfoCheck)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, serializableDataParent, ZFMP_OUT(ZFSerializableData &, ret))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, itemClassSet, ZFMP_IN(const zfchar *, clsName))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfchar *, itemClass)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, propertyNameSet, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfchar *, propertyName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, propertyValueSet, ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfchar *, propertyValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, categorySet, ZFMP_IN(const zfchar *, category))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, const zfchar *, category)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, editModeSet, ZFMP_IN(zfbool, editMode))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, editMode)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, removeAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, serializableDataTagSet, ZFMP_IN(const zfchar *, key), ZFMP_IN(ZFObject *, tag))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, ZFObject *, serializableDataTagGet, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, serializableDataTagGetAllKeyValue, ZFMP_IN_OUT(ZFCoreArray<const zfchar *> &, allKey), ZFMP_IN_OUT(ZFCoreArray<ZFObject *> &, allValue))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, serializableDataTagRemove, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfautoObject, serializableDataTagRemoveAndGet, ZFMP_IN(const zfchar *, key))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, serializableDataTagRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, attributeSet, ZFMP_IN(const zfchar *, name), ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attributeForName, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfindex, attributeCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attributeRemove, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, attributeRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfiterator, attributeIteratorForName, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfiterator, attributeIterator)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, attributeIteratorIsValid, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, zfbool, attributeIteratorIsEqual, ZFMP_IN(const zfiterator &, it0), ZFMP_IN(const zfiterator &, it1))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, attributeIteratorSet, ZFMP_IN_OUT(zfiterator &, it), ZFMP_IN(const zfchar *, value))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attributeIteratorRemove, ZFMP_IN_OUT(zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attributeIteratorGetKey, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attributeIteratorGet, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attributeIteratorNextKey, ZFMP_IN_OUT(zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const zfchar *, attributeIteratorNext, ZFMP_IN_OUT(zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, attributeIteratorResolved, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attributeIteratorResolveMark, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, attributeIteratorResolveUnmark, ZFMP_IN(const zfiterator &, it))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, elementAdd, ZFMP_IN(const ZFSerializableData &, element))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, elementAdd, ZFMP_IN(const ZFSerializableData &, element), ZFMP_IN(zfindex, atIndex))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_2(v_ZFSerializableData, void, elementSetAtIndex, ZFMP_IN(zfindex, index), ZFMP_IN(const ZFSerializableData &, element))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfindex, elementFindByName, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfindex, elementFindByCategory, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfindex, elementCount)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, const ZFSerializableData &, elementAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, elementRemoveAtIndex, ZFMP_IN(zfindex, index))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, elementRemoveAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, resolved)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveMark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveUnmark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, zfbool, resolvedAttribute, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, resolveAttributeMark, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_1(v_ZFSerializableData, void, resolveAttributeUnmark, ZFMP_IN(const zfchar *, name))
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, resolvedPropertyName)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolvePropertyNameMark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolvePropertyNameUnmark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, resolvedPropertyValue)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolvePropertyValueMark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolvePropertyValueUnmark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, resolvedCategory)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveCategoryMark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveCategoryUnmark)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveMarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveUnmarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveAttributeMarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, void, resolveAttributeUnmarkAll)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_FUNC_0(v_ZFSerializableData, zfbool, isEmpty)

ZF_NAMESPACE_GLOBAL_END
#endif

