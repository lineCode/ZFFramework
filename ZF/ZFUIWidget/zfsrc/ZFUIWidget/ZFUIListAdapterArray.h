/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
/**
 * @file ZFUIListAdapterArray.h
 * @brief basic list adapter which hold all list cells directly (with no recycle logic)
 */

#ifndef _ZFI_ZFUIListAdapterArray_h_
#define _ZFI_ZFUIListAdapterArray_h_

#include "ZFUIListAdapter.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief basic list adapter which hold all list cells directly (with no recycle logic)
 */
zfclass ZF_ENV_EXPORT ZFUIListAdapterArray : zfextends ZFObject, zfimplements ZFUIListAdapter
{
    ZFOBJECT_DECLARE(ZFUIListAdapterArray, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFUIListAdapter)

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        d = zfAlloc(ZFArrayEditable);
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfRelease(d);
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        this->cellRemoveAll();
        zfsuper::objectOnDeallocPrepare();
    }

public:
    /**
     * @brief directly access internal cell array,
     *   you may modify it directly, with caution
     */
    ZFMETHOD_INLINE_0(ZFArrayEditable *, cellArray)
    {
        return d;
    }
    /**
     * @brief add cell
     */
    ZFMETHOD_INLINE_1(void, cellAdd,
                      ZFMP_IN(ZFUIListCell *, cell))
    {
        d->add(cell);
    }
    /**
     * @brief add cell at index
     */
    ZFMETHOD_INLINE_2(void, cellAdd,
                      ZFMP_IN(zfindex, index),
                      ZFMP_IN(ZFUIListCell *, cell))
    {
        d->add(index, cell);
    }
    /**
     * @brief remove cell
     */
    ZFMETHOD_INLINE_2(void, cellRemove,
                      ZFMP_IN(zfindex, index),
                      ZFMP_IN_OPT(zfindex, count, 1))
    {
        d->remove(index, count);
    }
    /**
     * @brief remove cell
     */
    ZFMETHOD_INLINE_1(void, cellRemoveElement,
                      ZFMP_IN(ZFUIListCell *, cell))
    {
        d->removeElement(cell);
    }
    /**
     * @brief remove all cell
     */
    ZFMETHOD_INLINE_0(void, cellRemoveAll)
    {
        d->removeAll();
    }

public:
    ZFMETHOD_INLINE_0(zfindex, cellCount)
    {
        return d->count();
    }
    ZFMETHOD_INLINE_1(zfautoObject, cellAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        return d->get(index);
    }

protected:
    zfoverride
    virtual zfautoObject cellCacheOnAccess(ZF_IN zfindex index)
    {
        return d->get(index);
    }

private:
    ZFArrayEditable *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListAdapterArray_h_

