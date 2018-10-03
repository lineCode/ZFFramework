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
 * @file ZFCoreTypeDef_OtherType.h
 * @brief types for ZFFramework
 */

#ifndef _ZFI_ZFCoreTypeDef_OtherType_h_
#define _ZFI_ZFCoreTypeDef_OtherType_h_

#include "ZFCoreTypeDef_StringType.h"
#include "ZFCoreTypeDef_ClassType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/** @brief string tokens */
#define ZFTOKEN_zfnull "null"

/** @brief string tokens */
#define ZFTOKEN_zfbool_zftrue "true"
/** @brief string tokens */
#define ZFTOKEN_zfbool_zffalse "false"

// ============================================================
/**
 * @brief compare result of two ZFObjects
 * @see ZFObject::compare
 */
typedef enum
{
    ZFCompareUncomparable = -2, /**< can't be compared */
    ZFCompareSmaller = -1, /**< left < right */
    ZFCompareTheSame = 0, /**< left == right */
    ZFCompareGreater = 1, /**< left > right */
} ZFCompareResult;
/** @brief string tokens */
#define ZFTOKEN_ZFCompareUncomparable "CompareUncomparable"
/** @brief string tokens */
#define ZFTOKEN_ZFCompareSmaller "CompareSmaller"
/** @brief string tokens */
#define ZFTOKEN_ZFCompareTheSame "CompareTheSame"
/** @brief string tokens */
#define ZFTOKEN_ZFCompareGreater "CompareGreater"

// ============================================================
/**
 * @brief seek position similar to SEEK_SET of FILE operation
 */
typedef enum
{
    ZFSeekPosBegin,
    ZFSeekPosCur,
    ZFSeekPosCurReversely,
    ZFSeekPosEnd,
} ZFSeekPos;
/** @brief string tokens */
#define ZFTOKEN_ZFSeekPosBegin "SeekBegin"
/** @brief string tokens */
#define ZFTOKEN_ZFSeekPosCur "SeekCur"
/** @brief string tokens */
#define ZFTOKEN_ZFSeekPosCurReversely "SeekCurReversely"
/** @brief string tokens */
#define ZFTOKEN_ZFSeekPosEnd "SeekEnd"

// ============================================================
/**
 * @brief POD data to hold index range
 */
zfclassPOD ZF_ENV_EXPORT ZFIndexRange
{
public:
    zfindex start; /**< @brief start */
    zfindex count; /**< @brief count */
};
extern ZF_ENV_EXPORT const ZFIndexRange _ZFP_ZFIndexRangeZero;
extern ZF_ENV_EXPORT const ZFIndexRange _ZFP_ZFIndexRangeMax;
/**
 * @brief zero index range
 */
#define ZFIndexRangeZero() _ZFP_ZFIndexRangeZero
/**
 * @brief max index range (0, zfindexMax())
 */
#define ZFIndexRangeMax() _ZFP_ZFIndexRangeMax
/**
 * @brief make a ZFIndexRange
 */
inline ZFIndexRange ZFIndexRangeMake(ZF_IN zfindex start,
                                     ZF_IN zfindex count)
{
    ZFIndexRange ret = {start, count};
    return ret;
}
/**
 * @brief compare
 */
inline ZFCompareResult ZFIndexRangeIsEqual(ZF_IN const ZFIndexRange &e0,
                                           ZF_IN const ZFIndexRange &e1)
{
    return ((e0.start == e1.start && e0.count == e1.count) ? ZFCompareTheSame : ZFCompareUncomparable);
}
/**
 * @brief true if contain index in range
 */
inline zfbool ZFIndexRangeContain(ZF_IN const ZFIndexRange &range,
                                  ZF_IN zfindex index)
{
    return (range.count > 0 && index >= range.start && index < range.start + range.count);
}
/** @cond ZFPrivateDoc */
inline zfbool operator == (ZF_IN const ZFIndexRange &v0,
                           ZF_IN const ZFIndexRange &v1)
{
    return (v0.start == v1.start && v0.count == v1.count);
}
inline zfbool operator != (ZF_IN const ZFIndexRange &v0,
                           ZF_IN const ZFIndexRange &v1)
{
    return (v0.start != v1.start || v0.count != v1.count);
}
/** @endcond */

// ============================================================
/**
 * @brief comparar for template types
 *
 * for common types (that support operation "==", "<", ">", such as pointers, built-in types)
 * you may use #ZFComparerDefault:
 * @code
 *   ZFComparerDefault<YourType>
 * @endcode
 * for custom types you may declare your own comparar:
 * @code
 *   ZFCompareResult YourCompareFunction(ZF_IN YourType const &e0, ZF_IN YourType const &e1)
 *   {
 *       // do your compare
 *   }
 * @endcode
 * and pass the comparar as:
 * @code
 *   YourCompareFunction
 * @endcode
 */
template<typename T_Comparable, typename T_Comparable2 = T_Comparable>
zfclassNotPOD ZFComparer
{
public:
    /**
     * @brief see #ZFComparer
     */
    typedef ZFCompareResult (*Comparer)(ZF_IN T_Comparable const &e0, ZF_IN T_Comparable2 const &e1);
};

// ============================================================
/**
 * @brief converter template typedef
 *
 * example:
 * @code
 *   static const zfint *MyConverter0(zfint const &v)
 *   {
 *       return &v;
 *   }
 *   static zfstring MyConverter1(const zfchar * const &v)
 *   {
 *       return zfstring(v);
 *   }
 *
 *   ZFTypeConverter<zfint, const zfint *>::TypeConverter converter0 = MyConverter0;
 *   zfint src0 = ...;
 *   const zfint *dst0 = ...;
 *   dst0 = converter0(src0);
 *
 *   ZFTypeConverter<const zfchar *, zfstring>::TypeConverter converter1 = MyConverter1;
 *   const zfchar *src1 = ...;
 *   zfstring dst0 = ...;
 *   dst1 = converter1(src1);
 * @endcode
 */
template<typename T_Src, typename T_Dst>
zfclassNotPOD ZFTypeConverter
{
public:
    /**
     * @brief see #ZFTypeConverter
     */
    typedef T_Dst (*TypeConverter)(ZF_IN T_Src const &);
};

/**
 * @brief dummy type converter that return the src itself
 */
template<typename T_Type>
T_Type ZFTypeConverterDummy(ZF_IN T_Type const &src)
{
    return src;
}

// ============================================================
/**
 * @brief base type for info getter
 */
template<typename T_Element>
zfclassNotPOD ZF_ENV_EXPORT ZFCoreInfoGetter
{
public:
    /** @brief see #ZFCoreInfoGetter */
    typedef void (*InfoGetter)(ZF_IN_OUT zfstring &ret, ZF_IN T_Element const &v);
};

/**
 * @brief string if the content info not available
 */
#define ZFTOKEN_ZFCoreInfoGetterNotAvailable "N/A"

// ============================================================
/**
 * @brief dummy token holder for generic type
 */
typedef void *ZFToken;
/**
 * @brief invalid #ZFToken
 */
#define ZFTokenInvalid() zfnull

// ============================================================
/**
 * @brief dummy function address type
 */
typedef void (*ZFFuncAddrType)(void);

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreTypeDef_OtherType_h_

