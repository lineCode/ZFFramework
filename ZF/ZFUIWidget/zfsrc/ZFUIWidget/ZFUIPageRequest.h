/**
 * @file ZFUIPageRequest.h
 * @brief abstract page manager
 */

#ifndef _ZFI_ZFUIPageRequest_h_
#define _ZFI_ZFUIPageRequest_h_

#include "ZFUIPage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief base class of request for #ZFUIPageManager,
 *   you may subclass this and achieve custom request
 */
zfclass ZF_ENV_EXPORT ZFUIPageRequest : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIPageRequest, ZFObject)

public:
    /**
     * @brief whether the request has been resolved
     */
    ZFPROPERTY_ASSIGN(zfbool, requestResolved)
};

/**
 * @brief param for #ZFUIPageManager::requestPageCreate
 */
zffinal zfclassLikePOD ZF_ENV_EXPORT ZFUIPageRequestPageCreateParam
{
    ZFCORE_PARAM_DECLARE_SELF(ZFUIPageRequestPageCreateParam)

public:
    /**
     * @brief page instance
     */
    ZFCORE_PARAM_RETAIN(ZFUIPage *, page)

    /**
     * @brief page create param which would be passed to #ZFUIPage::pageGroupId
     */
    ZFCORE_PARAM(zfstring, pageGroupId)

    /**
     * @brief page create param which would be passed to #ZFUIPage::pageCreateParam,
     *   auto retained, null by default
     */
    ZFCORE_PARAM_RETAIN(ZFObject *, pageCreateParam)

    /**
     * @brief whether resume page automatically after page created, true by default
     *
     * if true, #ZFUIPageManager::requestPageResume would be called automatically after page created
     */
    ZFCORE_PARAM_WITH_INIT(zfbool, pageAutoResume, zftrue)

public:
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFUIPageRequestPageCreateParam &ref) const
    {
        return (zftrue
                && this->page() == ref.page()
                && this->pageCreateParam() == ref.pageCreateParam()
                && this->pageAutoResume() == ref.pageAutoResume()
            );
    }
    zfbool operator != (ZF_IN const ZFUIPageRequestPageCreateParam &ref) const
    {
        return !this->operator == (ref);
    }
    /** @endcond */
};
ZFTYPEID_ACCESS_ONLY_DECLARE(ZFUIPageRequestPageCreateParam, ZFUIPageRequestPageCreateParam)

/**
 * @brief request for #ZFUIPageManager::requestPageCreate
 */
zfclass ZFUIPageRequestPageCreate : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(ZFUIPageRequestPageCreate, ZFUIPageRequest)

public:
    /** @brief page create param */
    ZFUIPageRequestPageCreateParam createParam;
};

/**
 * @brief request for #ZFUIPageManager::requestPageResume
 */
zfclass ZFUIPageRequestPageResume : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(ZFUIPageRequestPageResume, ZFUIPageRequest)

public:
    /** @brief the page to resume */
    ZFPROPERTY_RETAIN(ZFUIPage *, page)
};

/**
 * @brief request for #ZFUIPageManager::requestPageGroupResume
 */
zfclass ZFUIPageRequestPageGroupResume : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(ZFUIPageRequestPageGroupResume, ZFUIPageRequest)

public:
    /** @brief page group id to resume */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfstring, pageGroupId, ZFUIPageGroupIdInvalid)
};

/**
 * @brief request for #ZFUIPageManager::requestPageDestroy
 */
zfclass ZFUIPageRequestPageDestroy : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(ZFUIPageRequestPageDestroy, ZFUIPageRequest)

public:
    /** @brief the page to destroy */
    ZFPROPERTY_RETAIN(ZFUIPage *, page)
};

/**
 * @brief request for #ZFUIPageManager::requestPostCustom
 */
zfclass ZFUIPageRequestCustom : zfextends ZFUIPageRequest
{
    ZFOBJECT_DECLARE(ZFUIPageRequestCustom, ZFUIPageRequest)

public:
    /** @brief the listener to execute */
    ZFPROPERTY_ASSIGN(ZFListener, listener)
    /** @brief the param passed to the listener */
    ZFPROPERTY_RETAIN(ZFObject *, userData)
    /** @brief the param passed to the listener */
    ZFPROPERTY_RETAIN(ZFObject *, param0)
    /** @brief the param passed to the listener */
    ZFPROPERTY_RETAIN(ZFObject *, param1)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIPageRequest_h_

