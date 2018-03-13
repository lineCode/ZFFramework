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
 * @file ZFIOCallback_input.h
 * @brief common callback defines
 */

#ifndef _ZFI_ZFIOCallback_input_h_
#define _ZFI_ZFIOCallback_input_h_

#include "ZFIOCallback_fwd.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFInputCallback
/**
 * @brief general input callback
 *
 * proto type:
 * -  zfindex input(void *, zfindex);
 *
 * params:
 * -  (void *) buffer to write to
 * -  (zfindex) max byte size to read (excluding '\0')
 *
 * return:
 * -  (zfindex) if buffer is null,
 *   return max byte size in the input or zfindexMax if not available\n
 *   if buffer is not null, return byte size actually read even if error (excluding '\0')
 *
 * ADVANCED:\n
 * for implementations, see #ZFCallbackTagKeyword_ioOwner if need support seek
 */
ZFCALLBACK_DECLARE_BEGIN(ZFInputCallback, ZFIOCallback)
public:
    /** @brief see #ZFInputCallback */
    inline zfindex execute(ZF_OUT void *buf,
                           ZF_IN zfindex count) const
    {
        return ZFCallback::executeExact<zfindex, void *, zfindex>(buf, count);
    }
    /** @brief see #ZFInputCallback */
    inline zfindex operator () (ZF_OUT void *buf,
                                ZF_IN zfindex count) const
    {
        return ZFCallback::executeExact<zfindex, void *, zfindex>(buf, count);
    }
_ZFP_ZFCALLBACK_DECLARE_END_NO_ALIAS(ZFInputCallback, ZFIOCallback)

// ============================================================
/**
 * @brief read single logic char to buf, return count of sizoef(zfchar) if success, or 0 if fail
 *
 * since UTF-8 has variable logic char size, use this method to read a single logic char\n
 * note: result may have more than one char for invalid encoding,
 * and buf size must at least 9 byte size
 */
extern ZF_ENV_EXPORT zfindex ZFInputCallbackReadChar(ZF_OUT zfchar *buf, ZF_IN_OUT const ZFInputCallback &input);
/**
 * @brief see #ZFInputCallbackReadChar
 */
extern ZF_ENV_EXPORT zfindex ZFInputCallbackReadChar(ZF_IN_OUT zfstring &buf, ZF_IN_OUT const ZFInputCallback &input);
/**
 * @brief util method to read all contents from input to string
 */
extern ZF_ENV_EXPORT void ZFInputCallbackReadToString(ZF_IN_OUT zfstring &ret, ZF_IN_OUT const ZFInputCallback &input);
/**
 * @brief util method to read all contents from input to a buffer
 *
 * returned buffer is ensured null-terminated,
 * and buffer's size shows the content's size excluding tail '\0'\n
 * return null buffer if input invalid or input error
 */
extern ZF_ENV_EXPORT ZFBuffer ZFInputCallbackReadToBuffer(ZF_IN_OUT const ZFInputCallback &input);

/**
 * @brief util method to skip chars in set
 *
 * return false if all chars are skipped before end,
 * other wise, buf stores the first logic char (see #ZFInputCallbackReadChar)
 */
extern ZF_ENV_EXPORT zfbool ZFInputCallbackSkipChars(ZF_OUT zfchar *buf,
                                                     ZF_IN_OUT const ZFInputCallback &input,
                                                     ZF_IN_OPT const zfchar *charSet = zfText(" \t\r\n"));

/**
 * @brief read until any char in charSet shows up, or reached maxCount,
 *   return count read so far
 *
 * the first char matched charSet would be read and discarded,
 * and you may check it by firstCharMatchedCharSet,
 * if reached end or maxCount before matched charSet,
 * 0 would be returned to firstCharMatchedCharSet
 */
extern ZF_ENV_EXPORT zfindex ZFInputCallbackReadUntil(ZF_IN_OUT zfstring &ret,
                                                      ZF_IN_OUT const ZFInputCallback &input,
                                                      ZF_IN_OPT const zfchar *charSet = zfText(" \t\r\n"),
                                                      ZF_IN_OPT zfindex maxCount = zfindexMax(),
                                                      ZF_OUT_OPT zfchar *firstCharMatchedCharSet = zfnull);

/**
 * @brief util method to check whether the input match the tokens
 *
 * return token's index if match or zfindexMax() if no match or error,
 * header white spaces would be skipped automatically\n
 * if no match, this method would try to restore the callback state by ioSeek to original position
 */
extern ZF_ENV_EXPORT zfindex ZFInputCallbackCheckMatch(ZF_IN const zfchar **tokens,
                                                       ZF_IN zfindex tokenCount,
                                                       ZF_IN_OUT const ZFInputCallback &input);

// ============================================================
// common input callbacks
/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <something category="input" ... />
 *       <zfindex category="start" ... /> // optional, 0 by default
 *       <zfindex category="count" ... /> // optional, zfindexMax() by default
 *       <zfbool category="autoRestorePos" ... /> // optional, zftrue by default
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFInputCallbackForInputInRange zfText("ZFInputCallbackForInputInRange")

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputCallbackForInputInRange_input zfText("input")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputCallbackForInputInRange_start zfText("start")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputCallbackForInputInRange_count zfText("count")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFInputCallbackForInputInRange_autoRestorePos zfText("autoRestorePos")
/**
 * @brief create a intput callback from another input callback, specified in range
 *
 * params:
 * -  (const ZFInputCallback &) input callback to use
 * -  (zfindex) src's start index
 * -  (zfindex) src's count or zfindexMax() to use whole
 * -  (zfbool) whether to restore src input callback's position after result callback deleted
 *
 * seeking the result input callback would ensure in range [start, start + count]\n
 * src must support seek, otherwise a null callback would be returned
 */
extern ZF_ENV_EXPORT ZFInputCallback ZFInputCallbackForInputInRange(ZF_IN const ZFInputCallback &inputCallback,
                                                                    ZF_IN_OPT zfindex start = 0,
                                                                    ZF_IN_OPT zfindex count = zfindexMax(),
                                                                    ZF_IN_OPT zfbool autoRestorePos = zftrue);

/**
 * @brief create a intput callback input from a const void *,
 *   you must ensure the buffer is alive during the callback's life time
 *
 * params:
 * -  (const void *) src to use
 * -  (zfindex) src's count or zfindexMax() to calculate automatically (treated as const zfchar *),
 *   zfindexMax() by default
 */
extern ZF_ENV_EXPORT ZFInputCallback ZFInputCallbackForBuffer(ZF_IN const void *src,
                                                              ZF_IN_OPT zfindex count = zfindexMax());
/**
 * @brief see #ZFInputCallbackForBuffer,
 *   copy the contents and auto free it
 */
extern ZF_ENV_EXPORT ZFInputCallback ZFInputCallbackForBufferCopy(ZF_IN const void *src,
                                                                  ZF_IN_OPT zfindex count = zfindexMax());

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFIOCallback_input_h_

