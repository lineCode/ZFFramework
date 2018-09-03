/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFIOCallback_output.h"
#include "ZFObjectImpl.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFOutputForString
zfclass _ZFP_I_ZFOutputForStringOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFOutputForStringOwner, ZFObject)

public:
    zfstring *pString;
    zfindex savedLength;
    zfindex curPos;
    ZFMETHOD_INLINE_2(zfindex, onOutput,
                      ZFMP_IN(const void *, s),
                      ZFMP_IN(zfindex, count))
    {
        if(count == zfindexMax())
        {
            count = zfslen((const zfchar *)s);
        }
        else
        {
            count /= sizeof(zfchar);
        }
        this->pString->replace(this->curPos, zfmMin(this->pString->length() - curPos, count), (const zfchar *)s, count);
        this->curPos += count;
        return count * sizeof(zfchar);
    }
    ZFMETHOD_INLINE_2(zfbool, ioSeek,
                      ZFMP_IN(zfindex, byteSize),
                      ZFMP_IN(ZFSeekPos, pos))
    {
        this->curPos = ZFIOCallbackCalcFSeek(this->savedLength, this->pString->length(), this->curPos, byteSize, pos);
        return zftrue;
    }
    ZFMETHOD_INLINE_0(zfindex, ioTell)
    {
        return ((this->pString->length() >= this->savedLength) ? (this->pString->length() - this->savedLength) : zfindexMax());
    }
    ZFMETHOD_INLINE_0(zfindex, ioSize)
    {
        return this->pString->length() - this->curPos;
    }
};
ZFOutput ZFOutputForString(ZF_IN zfstring &s)
{
    _ZFP_I_ZFOutputForStringOwner *owner = zfAlloc(_ZFP_I_ZFOutputForStringOwner);
    owner->pString = &s;
    owner->savedLength = s.length();
    owner->curPos = s.length();
    ZFOutput ret = ZFCallbackForMemberMethod(
        owner, ZFMethodAccess(_ZFP_I_ZFOutputForStringOwner, onOutput));
    ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, owner);
    zfRelease(owner);
    return ret;
}

// ============================================================
// ZFOutputForBuffer
zfclass _ZFP_I_ZFOutputForBufferOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFOutputForBufferOwner, ZFObject)

public:
    zfbool autoAppendNullToken;
    zfbyte *pStart;
    zfbyte *pEnd; // autoAppendNullToken ? (buf + maxCount - 1) : (buf + maxCount)
    zfbyte *p;

public:
    ZFMETHOD_INLINE_2(zfindex, onOutput,
                      ZFMP_IN(const void *, s),
                      ZFMP_IN(zfindex, count))
    {
        const zfbyte *pSrc = (const zfbyte *)s;
        if(count == zfindexMax())
        {
            while(*pSrc && p < pEnd)
            {
                *p = *pSrc;
                ++p;
                ++pSrc;
            }
            if(autoAppendNullToken)
            {
                *p = '\0';
            }
            return p - (const zfbyte *)s;
        }
        else
        {
            const zfbyte *pSrcEnd = (const zfbyte *)s + count;
            while(*pSrc && pSrc < pSrcEnd && p < pEnd)
            {
                *p = *pSrc;
                ++p;
                ++pSrc;
            }
            if(autoAppendNullToken)
            {
                *p = '\0';
            }
            return p - (const zfbyte *)s;
        }
    }
    ZFMETHOD_INLINE_2(zfbool, ioSeek,
                      ZFMP_IN(zfindex, byteSize),
                      ZFMP_IN(ZFSeekPos, pos))
    {
        p = pStart + ZFIOCallbackCalcFSeek(0, pEnd - pStart, p - pStart, byteSize, pos);
        return zftrue;
    }
    ZFMETHOD_INLINE_0(zfindex, ioTell)
    {
        return p - pStart;
    }
    ZFMETHOD_INLINE_0(zfindex, ioSize)
    {
        return pEnd - pStart;
    }
};
ZFOutput ZFOutputForBuffer(ZF_IN void *buf,
                           ZF_IN_OPT zfindex maxCount /* = zfindexMax() */,
                           ZF_IN_OPT zfbool autoAppendNullToken /* = zftrue */)
{
    if(buf == zfnull || maxCount == 0 || (maxCount == 1 && autoAppendNullToken))
    {
        return ZFCallbackNull();
    }
    _ZFP_I_ZFOutputForBufferOwner *owner = zfAlloc(_ZFP_I_ZFOutputForBufferOwner);
    owner->autoAppendNullToken = autoAppendNullToken;
    owner->pStart = (zfbyte *)buf;
    if(maxCount == zfindexMax())
    {
        owner->pEnd = zfnull;
        --(owner->pEnd);
    }
    else
    {
        owner->pEnd = owner->pStart + maxCount;
        if(autoAppendNullToken)
        {
            --(owner->pEnd);
        }
    }
    owner->p = owner->pStart;
    ZFOutput ret = ZFCallbackForMemberMethod(
        owner, ZFMethodAccess(_ZFP_I_ZFOutputForBufferOwner, onOutput));
    ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, owner);
    zfRelease(owner);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END

#if _ZFP_ZFOBJECT_METHOD_REG
#include "../ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZFMETHOD_USER_REGISTER_2({
        if(size == zfindexMax())
        {
            size = zfslen(src);
        }
        else
        {
            size /= sizeof(zfchar);
        }
        ZFOutput output = invokerObject->to<v_ZFCallback *>()->zfv;
        return output.execute(src, size) * sizeof(zfchar);
    }, v_ZFCallback, zfindex, output, ZFMP_IN(const zfchar *, src), ZFMP_IN_OPT(zfindex, size, zfindexMax()))

ZFMETHOD_FUNC_USER_REGISTER_FOR_FUNC_1(ZFOutput, ZFOutputForString, ZFMP_IN_OUT(zfstring &, s))

ZF_NAMESPACE_GLOBAL_END
#endif

