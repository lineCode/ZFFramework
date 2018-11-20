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
 * @file ZFFilterForZFProperty.h
 * @brief filter for ZFProperty types
 */

#ifndef _ZFI_ZFFilterForZFProperty_h_
#define _ZFI_ZFFilterForZFProperty_h_

#include "ZFFilterForZFClass.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFProperty;
// ============================================================
zfclassPOD ZF_ENV_EXPORT _ZFP_ZFFilterForZFPropertySetting
{
public:
    zfbool ignorePublicSetter;
    zfbool ignoreProtectedSetter;
    zfbool ignorePrivateSetter;
    zfbool ignorePublicGetter;
    zfbool ignoreProtectedGetter;
    zfbool ignorePrivateGetter;
};
/**
 * @brief util class to filter ZFProperty
 */
zfclassLikePOD ZF_ENV_EXPORT ZFFilterForZFProperty : zfextendsLikePOD ZFFilterBasic<const ZFProperty *>
{
    ZFFILTER_DECLARE(ZFFilterForZFProperty, ZFFilterBasic<const ZFProperty *>)

public:
    /**
     * @brief see #ZFFilterBase
     */
    ZFFilterForZFProperty(void)
    : zfsuper()
    , classFilter()
    , d()
    {
        _ZFP_ZFFilterForZFPropertySetting *setting = zfnew(_ZFP_ZFFilterForZFPropertySetting);
        setting->ignorePublicSetter = zffalse;
        setting->ignoreProtectedSetter = zffalse;
        setting->ignorePrivateSetter = zffalse;
        setting->ignorePublicGetter = zffalse;
        setting->ignoreProtectedGetter = zffalse;
        setting->ignorePrivateGetter = zffalse;
        d = setting;
    }
    /**
     * @brief see #ZFFilterBase
     */
    ZFFilterForZFProperty(ZF_IN const ZFFilterForZFProperty &ref)
    : zfsuper()
    , classFilter(ref.classFilter)
    , d(ref.d)
    {
    }

    /** @cond ZFPrivateDoc */
    virtual ZFFilterForZFProperty &operator = (ZF_IN const ZFFilterForZFProperty &ref)
    {
        zfsuper::operator = (ref);
        this->classFilter = ref.classFilter;
        this->d = ref.d;
        return *this;
    }
    virtual void copyFrom(ZF_IN ZFFilterBase<const ZFProperty *, const ZFProperty *> const &ref)
    {
        zfsuper::copyFrom(ref);
        this->classFilter.copyFrom(((ZFFilterForZFProperty const &)ref).classFilter);
        _ZFP_ZFFilterForZFPropertySetting *setting = zfnew(_ZFP_ZFFilterForZFPropertySetting);
        zfmemcpy(setting, ((ZFFilterForZFProperty const &)ref).d, sizeof(_ZFP_ZFFilterForZFPropertySetting));
        d = setting;
    }
    /** @endcond */

public:
    /**
     * @brief filter the method's owner class
     */
    ZFFilterForZFClass classFilter;

public:
    /**
     * @brief ignore public setter, false by default
     */
    virtual void ignorePublicSetterSet(ZF_IN zfbool value)
    {
        d->ignorePublicSetter = value;
    }
    /**
     * @brief see #ignorePublicSetterSet
     */
    virtual zfbool ignorePublicSetter(void) const
    {
        return d->ignorePublicSetter;
    }

    /**
     * @brief ignore protected setter, false by default
     */
    virtual void ignoreProtectedSetterSet(ZF_IN zfbool value)
    {
        d->ignoreProtectedSetter = value;
    }
    /**
     * @brief see #ignoreProtectedSetterSet
     */
    virtual zfbool ignoreProtectedSetter(void) const
    {
        return d->ignoreProtectedSetter;
    }

    /**
     * @brief ignore private setter, false by default
     */
    virtual void ignorePrivateSetterSet(ZF_IN zfbool value)
    {
        d->ignorePrivateSetter = value;
    }
    /**
     * @brief see #ignorePrivateSetterSet
     */
    virtual zfbool ignorePrivateSetter(void) const
    {
        return d->ignorePrivateSetter;
    }

    /**
     * @brief ignore public getter, false by default
     */
    virtual void ignorePublicGetterSet(ZF_IN zfbool value)
    {
        d->ignorePublicGetter = value;
    }
    /**
     * @brief see #ignorePublicGetterSet
     */
    virtual zfbool ignorePublicGetter(void) const
    {
        return d->ignorePublicGetter;
    }

    /**
     * @brief ignore protected getter, false by default
     */
    virtual void ignoreProtectedGetterSet(ZF_IN zfbool value)
    {
        d->ignoreProtectedGetter = value;
    }
    /**
     * @brief see #ignoreProtectedGetterSet
     */
    virtual zfbool ignoreProtectedGetter(void) const
    {
        return d->ignoreProtectedGetter;
    }

    /**
     * @brief ignore private getter, false by default
     */
    virtual void ignorePrivateGetterSet(ZF_IN zfbool value)
    {
        d->ignorePrivateGetter = value;
    }
    /**
     * @brief see #ignorePrivateGetterSet
     */
    virtual zfbool ignorePrivateGetter(void) const
    {
        return d->ignorePrivateGetter;
    }

public:
    zfoverride
    virtual void objectInfoT(ZF_IN_OUT zfstring &ret) const;

protected:
    /** @cond ZFPrivateDoc */ // ZFTAG_DOXYGEN_BUG: can't auto detect template specialization
    zfoverride
    virtual zfbool filterOnCheckValid(ZF_IN const ZFProperty * const &e) const
    {
        return (e != zfnull);
    }
    zfoverride
    virtual zfbool filterOnCheckActive(ZF_IN const ZFProperty * const &e) const;
    /** @endcond */

private:
    ZFCorePointerForPOD<_ZFP_ZFFilterForZFPropertySetting *> d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFFilterForZFProperty_h_

