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
 * @file ZFLogLevel.h
 * @brief log level define
 */

#ifndef _ZFI_ZFLogLevel_h_
#define _ZFI_ZFLogLevel_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief log level
 *
 * similar to Android's Log.v() to Log.e()\n
 * app's global log level can be set or get by #ZFLogLevelSet/#ZFLogLevelGet,
 * you may achieve your own log level specified behavior\n
 * it's ensured lower level has smaller value
 */
ZFENUM_BEGIN(ZFLogLevel)
    ZFENUM_VALUE(Verbose)
    ZFENUM_VALUE(Debug)
    ZFENUM_VALUE(Info)
    ZFENUM_VALUE(Warning)
    ZFENUM_VALUE(Error)
    ZFENUM_VALUE(Assert)
ZFENUM_SEPARATOR(ZFLogLevel)
    ZFENUM_VALUE_REGISTER(Verbose)
    ZFENUM_VALUE_REGISTER(Debug)
    ZFENUM_VALUE_REGISTER(Info)
    ZFENUM_VALUE_REGISTER(Warning)
    ZFENUM_VALUE_REGISTER(Error)
    ZFENUM_VALUE_REGISTER(Assert)
ZFENUM_END_WITH_DEFAULT(ZFLogLevel, ZFLogLevel::e_Warning)

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFLogLevelSet called
 */
ZFOBSERVER_EVENT_GLOBAL(LogLevelOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

/** @brief see #ZFLogLevel, #ZFLogLevel::e_Warning by default */
extern ZF_ENV_EXPORT void ZFLogLevelSet(ZF_IN ZFLogLevelEnum level);
/** @brief see #ZFLogLevel */
extern ZF_ENV_EXPORT ZFLogLevelEnum ZFLogLevelGet(void);

/**
 * @brief true if specified level is active
 *   (i.e. #ZFLogLevelGet not greater than the specified level)
 */
inline zfbool ZFLogLevelIsActive(ZF_IN ZFLogLevelEnum level)
{
    return (ZFLogLevelGet() <= level);
}

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFLogLevel_h_

