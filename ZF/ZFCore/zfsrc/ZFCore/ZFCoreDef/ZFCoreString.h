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
 * @file ZFCoreString.h
 * @brief string impl to reduce dependency of std::string
 */

#ifndef _ZFI_ZFCoreString_h_
#define _ZFI_ZFCoreString_h_

#include "ZFCoreTypeDef_CharType.h"
#include "ZFCoreTypeDef_ClassType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_zfstr_bufSize (sizeof(void *) * 2)

// ============================================================
/** @cond ZFPrivateDoc */
template<typename T_Char>
zffinal zfclassPOD _zfstrD
{
public:
    union {
        struct {
            T_Char *s;
            zfuint capacity;
        } s;
        T_Char buf[_ZFP_zfstr_bufSize];
    } d;
    zfuint length;
    zfbool builtinBuf;

public:
    _zfstrD(void)
    : d()
    , length(0)
    , builtinBuf(zftrue)
    {
    }

public:
    inline T_Char *buf(void)
    {
        if(this->builtinBuf)
        {
            return d.buf;
        }
        else
        {
            return d.s.s;
        }
    }
    inline const T_Char *buf(void) const
    {
        if(this->builtinBuf)
        {
            return d.buf;
        }
        else
        {
            return d.s.s;
        }
    }
};
/** @endcond */
/**
 * @brief low level string container
 */
template<typename T_Char>
zfclassLikePOD ZF_ENV_EXPORT _zfstr
{
public:
    /** @brief construct an empty string */
    _zfstr(void)
    : d()
    {
    }
    /** @brief construct a string with specified capacity */
    _zfstr(ZF_IN zfindex capacity)
    : d()
    {
        _capacityRequire(capacity)[0] = '\0';
    }
    /** @brief copy content from another string */
    _zfstr(ZF_IN const _zfstr &s)
    : d()
    {
        T_Char *buf = _capacityRequire(s.length());
        d.length = (zfuint)s.length();
        zfmemcpy(buf, s.cString(), d.length * sizeof(T_Char));
        buf[d.length] = '\0';
    }
    /** @brief copy content from another string */
    _zfstr(ZF_IN const _zfstr &s, zfindex pos)
    : d()
    {
        if(pos < s.length())
        {
            T_Char *buf = _capacityRequire(s.length() - pos);
            d.length = (zfuint)(s.length() - pos);
            zfmemcpy(buf, s.cString() + pos, d.length * sizeof(T_Char));
            buf[d.length] = '\0';
        }
    }
    /** @brief copy content from another string */
    _zfstr(ZF_IN const _zfstr &s, zfindex pos, zfindex len)
    : d()
    {
        if(pos < s.length())
        {
            if(len > s.length() - pos)
            {
                len = s.length() - pos;
            }
            T_Char *buf = _capacityRequire(len);
            d.length = (zfuint)len;
            zfmemcpy(buf, s.cString() + pos, d.length * sizeof(T_Char));
            buf[d.length] = '\0';
        }
    }
    /** @brief copy content from another string */
    _zfstr(ZF_IN const T_Char *s)
    : d()
    {
        if(s)
        {
            zfindex len = zfslenT(s);
            T_Char *buf = _capacityRequire(len);
            d.length = (zfuint)len;
            zfmemcpy(buf, s, d.length * sizeof(T_Char));
            buf[d.length] = '\0';
        }
    }
    /** @brief copy content from another string */
    _zfstr(ZF_IN const T_Char *s, zfindex len)
    : d()
    {
        if(s)
        {
            if(len == zfindexMax())
            {
                len = zfslenT(s);
            }
            T_Char *buf = _capacityRequire(len);
            d.length = (zfuint)len;
            zfmemcpy(buf, s, d.length * sizeof(T_Char));
            buf[d.length] = '\0';
        }
    }
    ~_zfstr(void)
    {
        if(!d.builtinBuf)
        {
            zffree(d.d.s.s);
        }
    }

public:
    /** @cond ZFPrivateDoc */
    inline operator const T_Char * (void) const {return this->cString();}
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    inline _zfstr &operator = (ZF_IN const _zfstr &s)
    {
        if(&s == this)
        {
            return *this;
        }
        else
        {
            return this->assign(s);
        }
    }
    inline _zfstr &operator = (ZF_IN const T_Char *s) {return this->assign(s);}
    _zfstr &operator = (ZF_IN T_Char c)
    {
        T_Char *buf = d.buf();
        buf[0] = c;
        buf[1] = '\0';
        d.length = 1;
        return *this;
    }
    zfbool operator == (ZF_IN const _zfstr &ref) const
    {
        return (this->compare(ref) == 0);
    }
    zfbool operator != (ZF_IN const _zfstr &ref) const
    {
        return (this->compare(ref) != 0);
    }
    zfbool operator == (ZF_IN const T_Char *ref) const
    {
        return (this->compare(ref) == 0);
    }
    zfbool operator != (ZF_IN const T_Char *ref) const
    {
        return (this->compare(ref) != 0);
    }
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    inline _zfstr &operator += (ZF_IN const _zfstr &s) {return this->append(s);}
    inline _zfstr &operator += (ZF_IN const T_Char *s) {return this->append(s);}
    _zfstr &operator += (ZF_IN T_Char c)
    {
        T_Char *buf = _capacityRequire(d.length + 1);
        buf[d.length] = c;
        ++d.length;
        buf[d.length] = '\0';
        return *this;
    }
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    inline T_Char &operator [] (ZF_IN zfindex pos)
    {
        return d.buf()[pos];
    }
    inline const T_Char &operator [] (ZF_IN zfindex pos) const
    {
        return d.buf()[pos];
    }
    /** @endcond */

public:
    /**
     * @brief swrap internal data without deep copy,
     *   designed for performance
     */
    void swap(ZF_IN_OUT _zfstr &ref)
    {
        zfbyte buf[sizeof(d)];
        zfmemcpy(buf, &d, sizeof(d));
        zfmemcpy(&d, &ref.d, sizeof(d));
        zfmemcpy(&ref.d, buf, sizeof(d));
    }

public:
    /** @brief append string */
    inline _zfstr &append(ZF_IN const _zfstr &s) {this->append(s.cString(), s.length()); return *this;}
    /** @brief append string */
    inline _zfstr &append(ZF_IN const _zfstr &s, ZF_IN zfindex pos, ZF_IN zfindex len = zfindexMax())
    {
        return this->append(s.cString() + pos, (pos < s.length()) ? ((len > s.length() - pos) ? (s.length() - pos) : len) : 0);
    }
    /** @brief append string */
    inline _zfstr &append(ZF_IN const T_Char *s) {return this->append(s, zfindexMax());}
    /** @brief append string */
    _zfstr &append(ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        if(s)
        {
            if(len == zfindexMax())
            {
                len = zfslenT(s);
            }
            T_Char *buf = _capacityRequire(d.length + len);
            _safeCheck(buf, s);
            zfmemcpy(buf + d.length, s, len * sizeof(T_Char));
            d.length += (zfuint)len;
            buf[d.length] = '\0';
        }
        return *this;
    }

public:
    /** @brief replace all content of the string */
    inline _zfstr &assign(ZF_IN const _zfstr &s) {return this->assign(s.cString(), s.length());}
    /** @brief replace all content of the string */
    inline _zfstr &assign(ZF_IN const _zfstr &s, ZF_IN zfindex pos, ZF_IN zfindex len = zfindexMax())
    {
        return this->assign(s.cString() + pos, (pos < s.length()) ? ((len > s.length() - pos) ? (s.length() - pos) : len) : 0);
    }
    /** @brief replace all content of the string */
    inline _zfstr &assign(ZF_IN const T_Char *s) {return this->assign(s, zfindexMax());}
    /** @brief replace all content of the string */
    _zfstr &assign(ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        if(s)
        {
            if(len == zfindexMax())
            {
                len = zfslenT(s);
            }
            T_Char *buf = _capacityRequire(len);;
            _safeCheck(buf, s);
            zfmemcpy(buf, s, len * sizeof(T_Char));
            d.length = (zfuint)len;
            buf[d.length] = '\0';
        }
        else
        {
            d.length = 0;
            d.buf()[0] = '\0';
        }
        return *this;
    }

public:
    /** @brief insert string */
    inline _zfstr &insert(ZF_IN zfindex insertAt, ZF_IN const _zfstr &s) {return this->insert(insertAt, s.cString(), s.length());}
    /** @brief insert string */
    inline _zfstr &insert(ZF_IN zfindex insertAt, ZF_IN const _zfstr &s, ZF_IN zfindex pos, ZF_IN zfindex len = zfindexMax())
    {
        return this->insert(insertAt, s.cString() + pos, (pos < s.length()) ? ((len > s.length() - pos) ? (s.length() - pos) : len) : 0);
    }
    /** @brief insert string */
    inline _zfstr &insert(ZF_IN zfindex insertAt, ZF_IN const T_Char *s) {return this->insert(insertAt, s, zfindexMax());}
    /** @brief insert string */
    _zfstr &insert(ZF_IN zfindex insertAt, ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        if(insertAt >= d.length)
        {
            this->append(s, len);
        }
        else if(s)
        {
            if(len == zfindexMax())
            {
                len = zfslenT(s);
            }
            T_Char *buf = _capacityRequire(d.length + len);
            _safeCheck(buf, s);
            zfmemmove(buf + insertAt + len, buf + insertAt, (d.length - insertAt) * sizeof(T_Char));
            zfmemcpy(buf + insertAt, s, len * sizeof(T_Char));
            d.length += (zfuint)len;
            buf[d.length] = '\0';
        }
        return *this;
    }

public:
    /** @brief replace string in range */
    inline _zfstr &replace(ZF_IN zfindex replacePos, ZF_IN zfindex replaceLen, ZF_IN const _zfstr &s) {return this->replace(replacePos, replaceLen, s.cString(), s.length());}
    /** @brief replace string in range */
    _zfstr &replace(ZF_IN zfindex replacePos, ZF_IN zfindex replaceLen, ZF_IN const _zfstr &s, ZF_IN zfindex pos, ZF_IN zfindex len = zfindexMax())
    {
        return this->replace(replacePos, replaceLen, s.cString() + pos, (pos < s.length()) ? ((len > s.length() - pos) ? (s.length() - pos) : len) : 0);
    }
    /** @brief replace string in range */
    inline _zfstr &replace(ZF_IN zfindex replacePos, ZF_IN zfindex replaceLen, ZF_IN const T_Char *s) {return this->replace(replacePos, replaceLen, s, zfindexMax());}
    /** @brief replace string in range */
    _zfstr &replace(ZF_IN zfindex replacePos, ZF_IN zfindex replaceLen, ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        if(replacePos >= d.length)
        {
            this->append(s, len);
        }
        else if(s)
        {
            if(replaceLen > d.length - replacePos)
            {
                replaceLen = d.length - replacePos;
            }
            if(len == zfindexMax())
            {
                len = zfslenT(s);
            }
            T_Char *buf = _capacityRequire(d.length + len - replaceLen);
            _safeCheck(buf, s);
            zfmemmove(buf + replacePos + len, buf + replacePos + replaceLen, (d.length - replacePos - replaceLen) * sizeof(T_Char));
            zfmemcpy(buf + replacePos, s, len * sizeof(T_Char));
            d.length = (zfuint)(d.length + len - replaceLen);
            buf[d.length] = '\0';
        }
        return *this;
    }

public:
    /** @brief access string value */
    inline const T_Char *cString(void) const
    {
        return d.buf();
    }
    /** @brief length of the string */
    inline zfindex length(void) const {return d.length;}
    /** @brief true if empty */
    inline zfbool isEmpty(void) const {return (d.length == 0);}

public:
    /** @brief ensure the string's capacity */
    inline void capacitySet(ZF_IN zfindex capacity) {_capacityRequire(capacity);}
    /** @brief capacity of the string */
    zfindex capacity(void)
    {
        if(d.builtinBuf)
        {
            return _ZFP_zfstr_bufSize;
        }
        else
        {
            return (zfindex)d.d.s.capacity;
        }
    }
    /** @brief remove part of the string */
    void remove(ZF_IN_OPT zfindex pos = 0, ZF_IN_OPT zfindex len = zfindexMax())
    {
        if(pos < d.length)
        {
            if(len > d.length - pos)
            {
                len = d.length - pos;
            }
            if(len > 0)
            {
                T_Char *buf = d.buf();
                zfmemmove(buf + pos, buf + pos + len, (d.length - pos - len) * sizeof(T_Char));
                d.length -= (zfuint)len;
                buf[d.length] = '\0';
            }
        }
    }
    /** @brief remove all content of the string */
    inline void removeAll(void)
    {
        d.length = 0;
        d.buf()[0] = '\0';
    }

public:
    /** @brief compare with another string */
    inline zfint compare(ZF_IN const _zfstr &s) const {return zfscmpT(this->cString(), s.cString());}
    /** @brief compare with another string */
    zfint compare(ZF_IN const T_Char *s, ZF_IN zfindex len = zfindexMax()) const
    {
        const T_Char *buf = d.buf();
        if(s)
        {
            if(len == zfindexMax())
            {
                len = zfslenT(s);
            }
            if(len > d.length)
            {
                return zfsncmpT(buf, s, d.length + 1);
            }
            else
            {
                zfint t = zfsncmpT(buf, s, len);
                if(t)
                {
                    return t;
                }
                else
                {
                    return (zfint)buf[len];
                }
            }
        }
        else
        {
            return (zfint)buf[0];
        }
    }

private:
    _zfstrD<T_Char> d;
private:
    T_Char *_capacityRequire(ZF_IN zfindex size)
    {
        if(d.builtinBuf)
        {
            if(size >= _ZFP_zfstr_bufSize)
            {
                zfindex capacity = this->capacity();
                while(capacity <= size)
                {
                    capacity *= 2;
                }
                T_Char *buf = (T_Char *)zfmalloc(capacity * sizeof(T_Char));
                zfmemcpy(buf, d.d.buf, d.length * sizeof(T_Char));
                d.d.s.s = buf;
                d.d.s.capacity = (zfuint)capacity;
                d.builtinBuf = zffalse;
                return d.d.s.s;
            }
            return d.d.buf;
        }
        else
        {
            if((zfuint)size >= d.d.s.capacity)
            {
                do
                {
                    d.d.s.capacity *= 2;
                } while(d.d.s.capacity <= size);
                d.d.s.s = (T_Char *)zfrealloc(d.d.s.s, d.d.s.capacity * sizeof(T_Char));
            }
            return d.d.s.s;
        }
    }
    inline void _safeCheck(ZF_IN const T_Char *buf, ZF_IN const T_Char *s) const
    {
        // no overlapped copy supported
        if(s >= buf && s < buf + d.length)
        {
            abort();
        }
    }
};

// ============================================================
/** @brief see #_zfstr */
#ifndef _ZFT_ZFCoreStringA
    typedef _zfstr<zfcharA> ZFCoreStringA;
#else
    typedef _ZFT_ZFCoreStringA ZFCoreStringA;
#endif

/** @brief see #_zfstr */
#ifndef _ZFT_ZFCoreStringW
    typedef _zfstr<zfcharW> ZFCoreStringW;
#else
    typedef _ZFT_ZFCoreStringW ZFCoreStringW;
#endif

/**
 * @brief string types used in ZFFramework
 */
#ifndef _ZFT_ZFCoreString
    #if ZF_ENV_ZFCHAR_USE_CHAR_A
        typedef ZFCoreStringA ZFCoreString;
    #endif
    #if ZF_ENV_ZFCHAR_USE_CHAR_W
        typedef ZFCoreStringW ZFCoreString;
    #endif
#else
    typedef _ZFT_ZFCoreString ZFCoreString;
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreString_h_

