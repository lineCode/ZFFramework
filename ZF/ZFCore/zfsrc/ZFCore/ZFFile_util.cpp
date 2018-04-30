/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFFile_impl.cpp"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
static zfbool _ZFP_ZFFilePathFormat(ZF_IN_OUT zfstring &ret,
                                    ZF_IN const zfchar *src,
                                    ZF_IN const zfchar *srcEnd)
{
    const zfchar *p = src;

    while(p < srcEnd)
    {
        if(*p == '/')
        {
            do
            {
                ++p;
            } while(p < srcEnd && *p == '/');
            if(ret.isEmpty() || ret[ret.length() - 1] != ZFFileSeparator())
            {
                ret += ZFFileSeparator();
            }
        }
        else if(*p == '\\')
        {
            ++p;
            if(*p == ' '
                || *p == '"')
            {
                ret += *p;
                ++p;
            }
            else
            {
                while(p < srcEnd && (*p == '/' || *p == '\\'))
                {
                    ++p;
                }
                if(ret.isEmpty() || ret[ret.length() - 1] != ZFFileSeparator())
                {
                    ret += ZFFileSeparator();
                }
            }
        }
        else if(*p == '.')
        {
            if(p == src || *(p-1) == '/')
            {
                if(p + 1 == srcEnd)
                {
                    if(p != src)
                    { // "[xxx]/."
                        ret.remove(ret.length() - 1);
                        ++p;
                    }
                    else
                    { // "."
                        ret += '.';
                    }
                    break;
                }
                else if(p[1] == '/'
                    || (p[1] == '\\' && p[1] != ' ' && p[1] != '"'))
                { // "[xxx]/./"
                    ++p;
                    while(p < srcEnd && (*p == '/' || *p == '\\')) {++p;}
                    continue;
                }
            }
            zfcharAppendAndMoveNext(ret, p);
        }
        else
        {
            zfcharAppendAndMoveNext(ret, p);
        }
    }
    while(!ret.isEmpty())
    {
        zfchar last = ret[ret.length() - 1];
        if(last == '/' || last == ' ' || last == '\t')
        {
            ret.remove(ret.length() - 1);
        }
        else
        {
            break;
        }
    }
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFilePathFormat,
                       ZFMP_IN_OUT(zfstring &, ret),
                       ZFMP_IN(const zfchar *, src),
                       ZFMP_IN_OPT(zfindex, srcLen, zfindexMax()))
{
    if(src == zfnull) {return zffalse;}
    while(*src == ' ' || *src == '\t') {++src;}
    const zfchar *srcEnd = src + (srcLen == zfindexMax() ? zfslen(src) : srcLen);
    if(srcEnd == src) {return zftrue;}
    while(*(srcEnd-1) == ' ' || *(srcEnd - 1) == '\t') {--srcEnd;}
    if(src >= ret.cString() && src < ret.cString() + ret.length())
    {
        zfstring tmp;
        ret.swap(tmp);
        return _ZFP_ZFFilePathFormat(ret, tmp.cString(), tmp.cString() + tmp.length());
    }
    else
    {
        return _ZFP_ZFFilePathFormat(ret, src, srcEnd);
    }
}

ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFileNameOf,
                       ZFMP_OUT(zfstring &, ret),
                       ZFMP_IN(const zfchar *, src))
{
    if(zfsIsEmpty(src) || zfscmpTheSame(src, zfText(".")))
    {
        return zffalse;
    }
    zfindex pos = zfstringFindReversely(src, zfindexMax(), ZFFileSeparatorString());
    if(src >= ret.cString() && src < ret.cString() + ret.length())
    {
        if(pos != zfindexMax())
        {
            ret.remove(0, (src + pos + 1) - ret.cString());
        }
        else
        {
            // nothing to do
        }
        return zftrue;
    }
    else
    {
        if(pos != zfindexMax())
        {
            ret += (src + pos + 1);
        }
        else
        {
            ret += src;
        }
        return zftrue;
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFFileNameOf,
                       ZFMP_IN(const zfchar *, src))
{
    zfstring ret;
    ZFFileNameOf(ret, src);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFileNameOfWithoutExt,
                       ZFMP_OUT(zfstring &, ret),
                       ZFMP_IN(const zfchar *, src))
{
    if(zfsIsEmpty(src) || zfscmpTheSame(src, zfText(".")))
    {
        return zffalse;
    }
    zfindex len = zfslen(src);
    zfindex pos = zfstringFindReversely(src, len, ZFFileSeparatorString());
    if(pos != zfindexMax())
    {
        ++pos;
    }
    else
    {
        pos = 0;
    }
    zfindex dotPos = zfstringFindReversely(src + pos, len - pos, zfText("."), 1);
    if(dotPos < pos)
    {
        dotPos = zfindexMax();
    }
    if(src >= ret.cString() && src < ret.cString() + ret.length())
    {
        pos += src - ret.cString();
        if(dotPos != zfindexMax())
        {
            ret.remove(src - ret.cString() + dotPos);
        }
        if(pos != 0)
        {
            ret.remove(0, pos);
        }
        return zftrue;
    }
    else
    {
        if(dotPos != zfindexMax())
        {
            ret.append(src + pos, dotPos - pos);
        }
        else
        {
            ret += (src + pos);
        }
        return zftrue;
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFFileNameOfWithoutExt,
                       ZFMP_IN(const zfchar *, src))
{
    zfstring ret;
    ZFFileNameOfWithoutExt(ret, src);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFileExtOf,
                       ZFMP_OUT(zfstring &, ret),
                       ZFMP_IN(const zfchar *, src))
{
    if(zfsIsEmpty(src) || zfscmpTheSame(src, zfText(".")))
    {
        return zffalse;
    }
    zfindex pos = zfstringFindReversely(src, zfindexMax(), ZFFileSeparatorString());
    zfindex dotPos = zfstringFindReversely(src, zfindexMax(), zfText("."), 1);
    if(pos != zfindexMax() && dotPos < pos)
    {
        dotPos = zfindexMax();
    }
    if(src >= ret.cString() && src < ret.cString() + ret.length())
    {
        if(dotPos != zfindexMax())
        {
            ret.remove(0, (src + dotPos + 1) - ret.cString());
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }
    else
    {
        if(dotPos != zfindexMax())
        {
            ret += (src + dotPos + 1);
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFFileExtOf,
                       ZFMP_IN(const zfchar *, src))
{
    zfstring ret;
    ZFFileExtOf(ret, src);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathOfWithoutExt,
                       ZFMP_OUT(zfstring &, ret),
                       ZFMP_IN(const zfchar *, src))
{
    if(zfsIsEmpty(src))
    {
        return zffalse;
    }
    zfindex dotPos = zfslen(src) - 1;
    for( ; dotPos != zfindexMax(); --dotPos)
    {
        if(src[dotPos] == '.')
        {
            break;
        }
        else if(src[dotPos] == '/' || src[dotPos] == '\\')
        {
            dotPos = zfindexMax();
            break;
        }
    }
    if(src >= ret.cString() && src < ret.cString() + ret.length())
    {
        if(dotPos != zfindexMax())
        {
            ret.remove(src - ret.cString() + dotPos, zfslen(src) - dotPos);
        }
    }
    else
    {
        ret.append(src, dotPos);
    }
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFFilePathOfWithoutExt,
                       ZFMP_IN(const zfchar *, src))
{
    zfstring ret;
    ZFFilePathOfWithoutExt(ret, src);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathParentOf,
                       ZFMP_OUT(zfstring &, ret),
                       ZFMP_IN(const zfchar *, src))
{
    if(zfsIsEmpty(src) || zfscmpTheSame(src, zfText(".")))
    {
        return zffalse;
    }
    zfindex pos = zfstringFindReversely(src, zfindexMax(), ZFFileSeparatorString());
    if(src >= ret.cString() && src < ret.cString() + ret.length())
    {
        if(pos != zfindexMax())
        {
            ret.remove(pos);
            return zftrue;
        }
        else
        {
            ret.removeAll();
            return zffalse;
        }
    }
    else
    {
        if(pos != zfindexMax())
        {
            ret.append(src, pos);
            return zftrue;
        }
        else
        {
            return zffalse;
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfstring, ZFFilePathParentOf,
                       ZFMP_IN(const zfchar *, src))
{
    zfstring ret;
    ZFFilePathParentOf(ret, src);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathComponentsOf,
                       ZFMP_OUT(ZFCoreArray<zfstring> &, ret),
                       ZFMP_IN(const zfchar *, src))
{
    if(zfsIsEmpty(src) || zfscmpTheSame(src, zfText(".")))
    {
        return zffalse;
    }
    zfindex len = zfslen(src);
    zfindex posL = 0;
    zfindex posR = len;
    do
    {
        posL = zfstringFindReversely(src, posR, ZFFileSeparatorString());
        if(posL == zfindexMax())
        {
            if(posR > 0)
            {
                ret.add(zfstring(src, posR));
            }
            break;
        }
        else
        {
            if(posL != posR - 1)
            {
                ret.add(zfstring(src, posL + 1, posR - posL - 1));
            }
            posR = posL;
        }
    } while(zftrue);
    return zftrue;
}
ZFMETHOD_FUNC_DEFINE_1(ZFCoreArray<zfstring>, ZFFilePathComponentsOf,
                       ZFMP_IN(const zfchar *, src))
{
    ZFCoreArray<zfstring> ret;
    ZFFilePathComponentsOf(ret, src);
    return ret;
}

// ============================================================
static void _ZFP_ZFFileTreePrint(ZF_IN const zfchar *pathData,
                                 ZF_IN const ZFOutputCallback &outputCallback,
                                 ZF_IN const zfchar *headToken,
                                 ZF_IN const zfchar *indentToken,
                                 ZF_IN zfindex indentLevel,
                                 ZF_IN ZFFilePathInfoData const &fileImpl)
{
    ZFFileFindData fd;
    if(fileImpl.callbackFindFirst(fd, pathData))
    {
        do
        {
            if(headToken != zfnull)
            {
                outputCallback << headToken;
            }
            if(indentToken != zfnull)
            {
                for(zfindex i = 0; i < indentLevel; i++)
                {
                    outputCallback << indentToken;
                }
            }

            if(fd.fileIsDir())
            {
                outputCallback << fd.fileName() << zfText("/\n");
                zfstring pathDataChild;
                if(fileImpl.callbackToChild(pathData, pathDataChild, fd.fileName()))
                {
                    _ZFP_ZFFileTreePrint(pathDataChild, outputCallback, headToken, indentToken, indentLevel + 1, fileImpl);
                }
            }
            else
            {
                outputCallback << fd.fileName();
                outputCallback << zfText("\n");
            }
        } while(fileImpl.callbackFindNext(fd));
        fileImpl.callbackFindClose(fd);
    }
}

ZFMETHOD_FUNC_DEFINE_4(void, ZFFilePathInfoTreePrint,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(const ZFOutputCallback &, outputCallback, ZFOutputCallbackDefault()),
                       ZFMP_IN_OPT(const zfchar *, headToken, zfnull),
                       ZFMP_IN_OPT(const zfchar *, indentToken, zfText("  ")))
{
    const ZFFilePathInfoData *data = ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data != zfnull
        && data->callbackFindFirst
        && data->callbackFindNext
        && data->callbackFindClose
        )
    {
        _ZFP_ZFFileTreePrint(pathInfo.pathData, outputCallback, headToken, indentToken, 0, *data);
    }
}

// ============================================================
zfclass _ZFP_ZFIOBufferedCallbackUsingTmpFileOutputIOOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFIOBufferedCallbackUsingTmpFileOutputIOOwner, ZFObject)
public:
    _ZFP_ZFIOBufferedCallbackUsingTmpFilePrivate *d;

    ZFMETHOD_DECLARE_2(zfbool, ioSeek,
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN(ZFSeekPos, pos))
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
};
zfclass _ZFP_ZFIOBufferedCallbackUsingTmpFilePrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFIOBufferedCallbackUsingTmpFilePrivate, ZFObject)

public:
    ZFToken token;
    zfstring tmpFilePath;
    zfindex outputIndex;
    zfindex inputIndex;
    zfindex fileSize;
    _ZFP_ZFIOBufferedCallbackUsingTmpFileOutputIOOwner *outputIOOwner;

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        zfstringAppend(this->tmpFilePath, zfText("%s%cZFIOBufferedCallbackUsingTmpFile_%s"),
            ZFFilePathForCache(),
            ZFFileSeparator(),
            zfsFromInt(zfidentityCalcPointer(this)).cString());
        this->token = ZFFileFileOpen(this->tmpFilePath.cString(),
            ZFFileOpenOption::e_Create | ZFFileOpenOption::e_Read | ZFFileOpenOption::e_Write);
        this->outputIndex = 0;
        this->inputIndex = 0;
        this->fileSize = 0;
        this->outputIOOwner = zfAlloc(_ZFP_ZFIOBufferedCallbackUsingTmpFileOutputIOOwner);
        this->outputIOOwner->d = this;
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        if(this->token != ZFTokenInvalid())
        {
            ZFFileFileClose(this->token);
            this->token = ZFTokenInvalid();
        }
        ZFFileFileRemove(this->tmpFilePath.cString(), zfHint("recursive")zffalse, zfHint("force")zftrue);
        this->outputIOOwner->d = zfnull;
        zfRelease(this->outputIOOwner);
        zfsuper::objectOnDealloc();
    }

public:
    void resetInput(void)
    {
        this->inputIndex = 0;
    }
    void resetOutput(void)
    {
        this->inputIndex = 0;
        this->outputIndex = 0;
        this->fileSize = 0;
    }

public:
    ZFMETHOD_INLINE_2(zfindex, onInput,
                      ZFMP_IN(void *, buf),
                      ZFMP_IN(zfindex, count))
    {
        if(buf == zfnull)
        {
            return this->fileSize - this->inputIndex;
        }
        else
        {
            ZFFileFileSeek(this->token, this->inputIndex);
            zfindex read = ZFFileFileRead(this->token, buf, zfmMin(count, this->fileSize - this->inputIndex));
            this->inputIndex += read;
            return read;
        }
    }
    ZFMETHOD_INLINE_2(zfindex, onOutput,
                      ZFMP_IN(const void *, buf),
                      ZFMP_IN(zfindex, count))
    {
        ZFFileFileSeek(this->token, this->outputIndex);
        zfindex written = ZFFileFileWrite(this->token, buf, count);
        this->outputIndex += written;
        if(this->outputIndex > this->fileSize)
        {
            this->fileSize = this->outputIndex;
        }
        return written;
    }

    // input IO
    ZFMETHOD_INLINE_2(zfbool, ioSeek,
                      ZFMP_IN(zfindex, byteSize),
                      ZFMP_IN(ZFSeekPos, pos))
    {
        this->inputIndex = ZFIOCallbackCalcFSeek(0, this->fileSize, this->inputIndex, byteSize, pos);
        return zftrue;
    }
    ZFMETHOD_INLINE_0(zfindex, ioTell)
    {
        return this->inputIndex;
    }
    ZFMETHOD_INLINE_0(zfindex, ioSize)
    {
        return this->fileSize;
    }
};
// output IO
ZFMETHOD_DEFINE_2(_ZFP_ZFIOBufferedCallbackUsingTmpFileOutputIOOwner, zfbool, ioSeek,
                  ZFMP_IN(zfindex, byteSize),
                  ZFMP_IN(ZFSeekPos, pos))
{
    d->outputIndex = ZFIOCallbackCalcFSeek(0, d->fileSize, d->outputIndex, byteSize, pos);
    return zftrue;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferedCallbackUsingTmpFileOutputIOOwner, zfindex, ioTell)
{
    return d->outputIndex;
}
ZFMETHOD_DEFINE_0(_ZFP_ZFIOBufferedCallbackUsingTmpFileOutputIOOwner, zfindex, ioSize)
{
    return d->fileSize;
}

/** @cond ZFPrivateDoc */
ZFIOBufferedCallbackUsingTmpFile::ZFIOBufferedCallbackUsingTmpFile(void)
: ZFIOBufferedCallback()
{
    d = zfAlloc(_ZFP_ZFIOBufferedCallbackUsingTmpFilePrivate);
}
ZFIOBufferedCallbackUsingTmpFile::ZFIOBufferedCallbackUsingTmpFile(ZF_IN const ZFIOBufferedCallbackUsingTmpFile &ref)
: ZFIOBufferedCallback(ref)
{
    d = zfRetain(ref.d);
}
ZFIOBufferedCallbackUsingTmpFile &ZFIOBufferedCallbackUsingTmpFile::operator = (ZF_IN const ZFIOBufferedCallbackUsingTmpFile &ref)
{
    zfRetain(ref.d);
    zfRelease(d);
    d = ref.d;
    return *this;
}
/** @endcond */
ZFIOBufferedCallbackUsingTmpFile::~ZFIOBufferedCallbackUsingTmpFile(void)
{
    zfRelease(d);
    d = zfnull;
}

ZFInputCallback ZFIOBufferedCallbackUsingTmpFile::inputCallback(void)
{
    if(d->token == ZFTokenInvalid())
    {
        return ZFCallbackNull();
    }
    else
    {
        ZFInputCallback ret = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFIOBufferedCallbackUsingTmpFilePrivate, onInput));
        ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, d);
        return ret;
    }
}
ZFOutputCallback ZFIOBufferedCallbackUsingTmpFile::outputCallback(void)
{
    if(d->token == ZFTokenInvalid())
    {
        return ZFCallbackNull();
    }
    else
    {
        ZFOutputCallback ret = ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFIOBufferedCallbackUsingTmpFilePrivate, onOutput));
        ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, d->outputIOOwner);
        return ret;
    }
}

void ZFIOBufferedCallbackUsingTmpFile::resetInput(void)
{
    d->resetInput();
}
void ZFIOBufferedCallbackUsingTmpFile::resetOutput(void)
{
    d->resetOutput();
}

ZF_NAMESPACE_GLOBAL_END

