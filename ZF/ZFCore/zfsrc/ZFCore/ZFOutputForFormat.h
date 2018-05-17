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
 * @file ZFOutputForFormat.h
 * @brief output callback util
 */

#ifndef _ZFI_ZFOutputForFormat_h_
#define _ZFI_ZFOutputForFormat_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief see #ZFOutputForFormat */
zfinterface ZFOutputFormat : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFOutputFormat, ZFInterface)

public:
    /** @brief see #ZFOutputForFormat */
    typedef enum
    {
        /**
         * @brief before anything would be outputed,
         *   ensured called only once for each output
         */
        OutputStepBegin,
        /**
         * @brief called each time before any output would be outputed
         */
        OutputStepAction,
        /**
         * @brief called only once just before finish/destroy the output
         */
        OutputStepEnd,
    } OutputStep;

protected:
    /**
     * @brief see #ZFOutputForFormat
     *
     * during output, it's ensured:
     * -  src is valid
     * -  srcLen is the src's length
     *
     * writtenLen is the formated length already written
     * to the original output
     */
    virtual void format(ZF_IN_OUT zfstring &ret,
                        ZF_IN zfself::OutputStep outputStep,
                        ZF_IN const zfchar *src,
                        ZF_IN zfindex srcLen,
                        ZF_IN zfindex writtenLen)
    {
    }
public:
    inline void _ZFP_format(ZF_IN_OUT zfstring &ret,
                            ZF_IN zfself::OutputStep outputStep,
                            ZF_IN const zfchar *src,
                            ZF_IN zfindex srcLen,
                            ZF_IN zfindex writtenLen)
    {
        this->format(ret, outputStep, src, srcLen, writtenLen);
    }
};

// ============================================================
/** @brief see #ZFOutputForFormat */
ZFMETHOD_FUNC_DECLARE_3(zfbool, ZFOutputForFormatT,
                        ZFMP_OUT(ZFCallback &, ret),
                        ZFMP_IN(const ZFOutput &, output),
                        ZFMP_IN(ZFOutputFormat *, format))

/**
 * @brief create an output callback with specified format rule
 *
 * each time before anything would be outputed to the original output,
 * it would be processed by the format object to determin the final output\n
 * \n
 * note:
 * -  the original output and the format object would be retained by the result output
 *   until it's destroyed
 * -  the result output is serializable if both the
 *   original output and the format object are serializable
 * -  serialization may cause performance issue,
 *   you may use #ZFOutputForFormatT and ##ZFCallbackSerializeCustomTypeDisable
 *   to disable the default serialization step,
 *   see #ZFCallbackSerializeCustomType_ZFInputForPathInfo for more info
 * -  if output success, the original output size would be returned,
 *   instead of the formated size
 */
ZFMETHOD_FUNC_DECLARE_2(ZFOutput, ZFOutputForFormat,
                        ZFMP_IN(const ZFOutput &, output),
                        ZFMP_IN(ZFOutputFormat *, format))

/**
 * @brief see #ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE
 *
 * serializable data:
 * @code
 *   <node>
 *       <ZFCallback category="output" ... /> // the output
 *       <YourOutputFormat category="format" ... /> // the format object
 *   </node>
 * @endcode
 */
#define ZFCallbackSerializeCustomType_ZFOutputForFormat zfText("ZFOutputForFormat")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFOutputForFormat_output zfText("output")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFOutputForFormat_format zfText("format")

// ============================================================
/**
 * @brief try access the output format passed to #ZFOutputForFormat,
 *   return null if not available
 */
ZFMETHOD_FUNC_DECLARE_1(ZFOutputFormat *, ZFOutputForFormatGetFormat,
                        ZFMP_IN(const ZFCallback &, callback))

/**
 * @brief try access the output passed to #ZFOutputForFormat,
 *   return null callback if not available
 */
ZFMETHOD_FUNC_DECLARE_1(ZFOutput, ZFOutputForFormatGetOutput,
                        ZFMP_IN(const ZFCallback &, callback))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFOutputForFormat_h_

