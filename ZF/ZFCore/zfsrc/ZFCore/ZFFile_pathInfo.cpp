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

#include "ZFSTLWrapper/zfstl_map.h"
#include "ZFSTLWrapper/zfstl_string.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfbool ZFFilePathInfoCallbackIsExistDefault(ZF_IN const zfchar *pathData)
{
    return zffalse;
}
zfbool ZFFilePathInfoCallbackIsDirDefault(ZF_IN const zfchar *pathData)
{
    return zffalse;
}
zfbool ZFFilePathInfoCallbackPathGetDefault(ZF_OUT zfstring &path,
                                            ZF_IN const zfchar *pathData)
{
    return ZFFilePathFormat(path, pathData);
}
zfbool ZFFilePathInfoCallbackPathSetDefault(ZF_OUT zfstring &pathData,
                                            ZF_IN const zfchar *path)
{
    return ZFFilePathFormat(pathData, path);
}
zfbool ZFFilePathInfoCallbackPathCreateDefault(ZF_IN const zfchar *pathData,
                                               ZF_IN_OPT zfbool autoMakeParent /* = zftrue */,
                                               ZF_OUT_OPT zfstring *errPos /* = zfnull */)
{
    return zffalse;
}
zfbool ZFFilePathInfoCallbackRemoveDefault(ZF_IN const zfchar *pathData,
                                           ZF_IN_OPT zfbool isRecursive /* = zftrue */,
                                           ZF_IN_OPT zfbool isForce /* = zffalse */,
                                           ZF_IN_OPT zfstring *errPos /* = zfnull */)
{
    return zffalse;
}
zfbool ZFFilePathInfoCallbackFindFirstDefault(ZF_IN_OUT ZFFileFindData &fd,
                                              ZF_IN const zfchar *pathData)
{
    return zffalse;
}
zfbool ZFFilePathInfoCallbackFindNextDefault(ZF_IN_OUT ZFFileFindData &fd)
{
    return zffalse;
}
void ZFFilePathInfoCallbackFindCloseDefault(ZF_IN_OUT ZFFileFindData &fd)
{
}
ZFToken ZFFilePathInfoCallbackOpenDefault(ZF_IN const zfchar *pathData,
                                          ZF_IN_OPT ZFFileOpenOptionFlags flag /* = ZFFileOpenOption::e_Read */,
                                          ZF_IN_OPT zfbool autoCreateParent /* = zftrue */)
{
    return ZFTokenInvalid();
}
zfbool ZFFilePathInfoCallbackCloseDefault(ZF_IN ZFToken token)
{
    return zffalse;
}
zfindex ZFFilePathInfoCallbackTellDefault(ZF_IN ZFToken token)
{
    return zfindexMax();
}
zfbool ZFFilePathInfoCallbackSeekDefault(ZF_IN ZFToken token,
                                         ZF_IN zfindex byteSize,
                                         ZF_IN_OPT ZFSeekPos position /* = ZFSeekPosBegin */)
{
    return zffalse;
}
zfindex ZFFilePathInfoCallbackReadDefault(ZF_IN ZFToken token,
                                          ZF_IN void *buf,
                                          ZF_IN zfindex maxByteSize)
{
    return 0;
}
zfindex ZFFilePathInfoCallbackWriteDefault(ZF_IN ZFToken token,
                                           ZF_IN const void *src,
                                           ZF_IN_OPT zfindex maxByteSize /* = zfindexMax() */)
{
    return 0;
}
void ZFFilePathInfoCallbackFlushDefault(ZF_IN ZFToken token)
{
}
zfbool ZFFilePathInfoCallbackIsEofDefault(ZF_IN ZFToken token)
{
    return zftrue;
}
zfbool ZFFilePathInfoCallbackIsErrorDefault(ZF_IN ZFToken token)
{
    return zftrue;
}
zfindex ZFFilePathInfoCallbackSizeDefault(ZF_IN ZFToken token)
{
    return zfindexMax();
}

// ============================================================
static zfstlmap<zfstlstringZ, ZFFilePathInfoData> &_ZFP_ZFFilePathInfoDataMap(void)
{
    static zfstlmap<zfstlstringZ, ZFFilePathInfoData> d;
    return d;
}
static ZFFilePathInfoData *_ZFP_ZFFilePathInfoDataGet(ZF_IN const zfchar *pathType)
{
    zfstlmap<zfstlstringZ, ZFFilePathInfoData> &m = _ZFP_ZFFilePathInfoDataMap();
    zfstlmap<zfstlstringZ, ZFFilePathInfoData>::iterator it = m.find(pathType);
    if(it != m.end())
    {
        return &(it->second);
    }
    else
    {
        return zfnull;
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFilePathInfoIsExist,
                       ZFMP_IN(const ZFPathInfo &, pathInfo))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackIsExistDefault(pathInfo.pathData);
    }
    else
    {
        return data->callbackIsExist(pathInfo.pathData);
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFilePathInfoIsDir,
                       ZFMP_IN(const ZFPathInfo &, pathInfo))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackIsDirDefault(pathInfo.pathData);
    }
    else
    {
        return data->callbackIsDir(pathInfo.pathData);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoPathGet,
                       ZFMP_OUT(zfstring &, path),
                       ZFMP_IN(const ZFPathInfo &, pathInfo))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackPathGetDefault(path, pathInfo.pathData);
    }
    else
    {
        return data->callbackPathGet(path, pathInfo.pathData);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoPathSet,
                       ZFMP_IN_OUT(ZFPathInfo &, pathInfo),
                       ZFMP_IN(const zfchar *, path))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackPathSetDefault(pathInfo.pathData, path);
    }
    else
    {
        return data->callbackPathSet(pathInfo.pathData, path);
    }
}
ZFMETHOD_FUNC_DEFINE_3(zfbool, ZFFilePathInfoPathCreate,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(zfbool, autoMakeParent, zftrue),
                       ZFMP_OUT_OPT(zfstring *, errPos, zfnull))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackPathCreateDefault(pathInfo.pathData, autoMakeParent, errPos);
    }
    else
    {
        return data->callbackPathCreate(pathInfo.pathData, autoMakeParent, errPos);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFFilePathInfoRemove,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(zfbool, isRecursive, zftrue),
                       ZFMP_IN_OPT(zfbool, isForce, zffalse),
                       ZFMP_IN_OPT(zfstring *, errPos, zfnull))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackRemoveDefault(pathInfo.pathData, isRecursive, isForce, errPos);
    }
    else
    {
        return data->callbackRemove(pathInfo.pathData, isRecursive, isForce, errPos);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoFindFirst,
                       ZFMP_IN_OUT(ZFFileFindData &, fd),
                       ZFMP_IN(const ZFPathInfo &, pathInfo))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackFindFirstDefault(fd, pathInfo.pathData);
    }
    else
    {
        return data->callbackFindFirst(fd, pathInfo.pathData);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoFindNext,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackFindNextDefault(fd);
    }
    else
    {
        return data->callbackFindNext(fd);
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFFilePathInfoFindClose,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OUT(ZFFileFindData &, fd))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        ZFFilePathInfoCallbackFindCloseDefault(fd);
    }
    else
    {
        data->callbackFindClose(fd);
    }
}
ZFMETHOD_FUNC_DEFINE_3(ZFToken, ZFFilePathInfoOpen,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flag, ZFFileOpenOption::e_Read),
                       ZFMP_IN_OPT(zfbool, autoCreateParent, zftrue))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackOpenDefault(pathInfo.pathData, flag, autoCreateParent);
    }
    else
    {
        return data->callbackOpen(pathInfo.pathData, flag, autoCreateParent);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoClose,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackCloseDefault(token);
    }
    else
    {
        return data->callbackClose(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfindex, ZFFilePathInfoTell,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackTellDefault(token);
    }
    else
    {
        return data->callbackTell(token);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFFilePathInfoSeek,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token),
                       ZFMP_IN(zfindex, byteSize),
                       ZFMP_IN_OPT(ZFSeekPos, position, ZFSeekPosBegin))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackSeekDefault(token, byteSize, position);
    }
    else
    {
        return data->callbackSeek(token, byteSize, position);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfindex, ZFFilePathInfoRead,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token),
                       ZFMP_IN(void *, buf),
                       ZFMP_IN(zfindex, maxByteSize))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackReadDefault(token, buf, maxByteSize);
    }
    else
    {
        return data->callbackRead(token, buf, maxByteSize);
    }
}
ZFMETHOD_FUNC_DEFINE_4(zfindex, ZFFilePathInfoWrite,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token),
                       ZFMP_IN(const void *, src),
                       ZFMP_IN_OPT(zfindex, maxByteSize, zfindexMax()))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackWriteDefault(token, src, maxByteSize);
    }
    else
    {
        return data->callbackWrite(token, src, maxByteSize);
    }
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFFilePathInfoFlush,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        ZFFilePathInfoCallbackFlushDefault(token);
    }
    else
    {
        data->callbackFlush(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoIsEof,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackIsEofDefault(token);
    }
    else
    {
        return data->callbackIsEof(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFFilePathInfoIsError,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackIsErrorDefault(token);
    }
    else
    {
        return data->callbackIsError(token);
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfindex, ZFFilePathInfoSize,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(ZFToken, token))
{
    ZFFilePathInfoData *data = _ZFP_ZFFilePathInfoDataGet(pathInfo.pathType);
    if(data == zfnull)
    {
        return ZFFilePathInfoCallbackSizeDefault(token);
    }
    else
    {
        return data->callbackSize(token);
    }
}

// ============================================================
void _ZFP_ZFFilePathInfoRegister(ZF_IN const zfchar *pathType,
                                 ZF_IN const ZFFilePathInfoData &data)
{
    zfstlmap<zfstlstringZ, ZFFilePathInfoData> &m = _ZFP_ZFFilePathInfoDataMap();
    zfCoreAssertWithMessage(m.find(pathType) == m.end(),
        zfTextA("pathType \"%s\" already registered"),
        zfsCoreZ2A(pathType));
    zfCoreAssert(zftrue
            && data.callbackIsExist != zfnull
            && data.callbackIsDir != zfnull
            && data.callbackPathGet != zfnull
            && data.callbackPathSet != zfnull
            && data.callbackPathCreate != zfnull
            && data.callbackRemove != zfnull
            && data.callbackFindFirst != zfnull
            && data.callbackFindNext != zfnull
            && data.callbackFindClose != zfnull
            && data.callbackOpen != zfnull
            && data.callbackClose != zfnull
            && data.callbackTell != zfnull
            && data.callbackSeek != zfnull
            && data.callbackRead != zfnull
            && data.callbackWrite != zfnull
            && data.callbackFlush != zfnull
            && data.callbackIsEof != zfnull
            && data.callbackIsError != zfnull
            && data.callbackSize != zfnull
        );
    m[pathType] = data;
}
void _ZFP_ZFFilePathInfoUnregister(ZF_IN const zfchar *pathType)
{
    zfstlmap<zfstlstringZ, ZFFilePathInfoData> &m = _ZFP_ZFFilePathInfoDataMap();
    m.erase(pathType);
}
const ZFFilePathInfoData *ZFFilePathInfoDataGet(ZF_IN const zfchar *pathType)
{
    zfstlmap<zfstlstringZ, ZFFilePathInfoData> &m = _ZFP_ZFFilePathInfoDataMap();
    zfstlmap<zfstlstringZ, ZFFilePathInfoData>::iterator it = m.find(pathType);
    if(it == m.end())
    {
        return zfnull;
    }
    else
    {
        return &(it->second);
    }
}
void ZFFilePathInfoDataGetAllT(ZF_OUT ZFCoreArrayPOD<const zfchar *> &ret)
{
    zfstlmap<zfstlstringZ, ZFFilePathInfoData> &m = _ZFP_ZFFilePathInfoDataMap();
    for(zfstlmap<zfstlstringZ, ZFFilePathInfoData>::iterator it = m.begin(); it != m.end(); ++it)
    {
        ret.add(it->first.c_str());
    }
}

// ============================================================
zfindex ZFFilePathInfoBOMCheck(ZF_IN_OUT const ZFFilePathInfoData &impl,
                               ZF_IN_OUT ZFToken token,
                               ZF_IN_OPT const ZFFileBOMList &autoSkipBOMTable /* = ZFFileBOMListDefault() */)
{
    zfindex ret = 0;
    while(!autoSkipBOMTable.isEmpty())
    {
        zfbyte tmp[ZFFileBOMMaxSize] = {0};
        if(impl.callbackRead(token, tmp, ZFFileBOMMaxSize) == 0) {break;}

        for(zfindex i = 0; i < autoSkipBOMTable.count(); ++i)
        {
            const zfbyte *BOM = autoSkipBOMTable[i].BOM;
            zfindex len = zfslenA((const zfcharA *)BOM);
            if(zfmemcmp(tmp, BOM, len) == 0)
            {
                ret = len;
                break;
            }
        }

        impl.callbackSeek(token, ret, ZFSeekPosBegin);
        break;
    }
    return ret;
}
zfbool ZFFilePathInfoBOMSeek(ZF_IN_OUT const ZFFilePathInfoData &impl,
                             ZF_IN_OUT ZFToken token,
                             ZF_IN zfindex BOMSize,
                             ZF_IN zfindex byteSize,
                             ZF_IN ZFSeekPos pos)
{
    zfindex fileSize = impl.callbackSize(token);
    if(fileSize == zfindexMax())
    {
        return zffalse;
    }
    zfindex tmp = ZFIOCallbackCalcFSeek(BOMSize, fileSize, impl.callbackTell(token), byteSize, pos);
    return impl.callbackSeek(token, tmp, pos);
}
zfindex ZFFilePathInfoBOMTell(ZF_IN_OUT const ZFFilePathInfoData &impl,
                              ZF_IN ZFToken token,
                              ZF_IN zfindex BOMSize)
{
    zfindex ret = impl.callbackTell(token);
    if(ret == zfindexMax())
    {
        return zfindexMax();
    }
    else
    {
        return ret - BOMSize;
    }
}

// ============================================================
// ZFInputCallbackForPathInfo
zfclass _ZFP_I_ZFInputCallbackForPathInfoOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFInputCallbackForPathInfoOwner, ZFObject)

protected:
    zfoverride
    virtual void objectOnDealloc(void)
    {
        if(this->token != ZFTokenInvalid())
        {
            this->impl->callbackClose(this->token);
            this->token = ZFTokenInvalid();
        }
        zfsuper::objectOnDealloc();
    }

public:
    virtual zfbool openFile(ZF_IN const zfchar *pathType,
                            ZF_IN const zfchar *pathData,
                            ZF_IN ZFFileOpenOptionFlags flags,
                            ZF_IN const ZFFileBOMList &autoSkipBOMTable)
    {
        this->impl = ZFFilePathInfoDataGet(pathType);
        if(this->impl == zfnull)
        {
            return zffalse;
        }

        this->token = this->impl->callbackOpen(pathData, flags, zftrue);
        if(this->token != ZFTokenInvalid() && !autoSkipBOMTable.isEmpty())
        {
            this->BOMSize = ZFFilePathInfoBOMCheck(*(this->impl), this->token, autoSkipBOMTable);
        }
        return (this->token != ZFTokenInvalid());
    }

    ZFMETHOD_INLINE_2(zfindex, onInput,
                      ZFMP_IN(void *, buf),
                      ZFMP_IN(zfindex, count))
    {
        return this->impl->callbackRead(this->token, buf, count);
    }
    ZFMETHOD_INLINE_2(zfbool, ioSeek,
                      ZFMP_IN(zfindex, byteSize),
                      ZFMP_IN(ZFSeekPos, pos))
    {
        return ZFFilePathInfoBOMSeek(*(this->impl), this->token, this->BOMSize, byteSize, pos);
    }
    ZFMETHOD_INLINE_0(zfindex, ioTell)
    {
        return ZFFilePathInfoBOMTell(*(this->impl), this->token, this->BOMSize);
    }
    ZFMETHOD_INLINE_0(zfindex, ioSize)
    {
        return this->impl->callbackSize(this->token) - this->impl->callbackTell(this->token);
    }

private:
    const ZFFilePathInfoData *impl;
    ZFToken token;
    zfindex BOMSize;
protected:
    _ZFP_I_ZFInputCallbackForPathInfoOwner(void)
    : impl(zfnull)
    , token(ZFTokenInvalid())
    , BOMSize(0)
    {
    }
};
ZFMETHOD_FUNC_DEFINE_3(ZFInputCallback, ZFInputCallbackForPathInfo,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read),
                       ZFMP_IN_OPT(const ZFFileBOMList &, autoSkipBOMTable, ZFFileBOMListDefault()))
{
    ZFInputCallback ret;
    ZFInputCallbackForPathInfoT(ret, pathInfo.pathType, pathInfo.pathData, flags, autoSkipBOMTable);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_3(ZFInputCallback, ZFInputCallbackForPathInfoString,
                       ZFMP_IN(const zfchar *, pathInfoString),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read),
                       ZFMP_IN_OPT(const ZFFileBOMList &, autoSkipBOMTable, ZFFileBOMListDefault()))
{
    zfstring pathType;
    const zfchar *pathData = zfnull;
    if(!ZFPathInfoParse(pathInfoString, pathType, pathData))
    {
        return ZFCallbackNull();
    }
    ZFInputCallback ret;
    ZFInputCallbackForPathInfoT(ret, pathType, pathData, flags, autoSkipBOMTable);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFInputCallbackForPathInfoT,
                       ZFMP_IN_OUT(ZFCallback &, ret),
                       ZFMP_IN(const zfchar *, pathType),
                       ZFMP_IN(const zfchar *, pathData),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read),
                       ZFMP_IN_OPT(const ZFFileBOMList &, autoSkipBOMTable, ZFFileBOMListDefault()))
{
    zfblockedAlloc(_ZFP_I_ZFInputCallbackForPathInfoOwner, inputOwner);
    if(!inputOwner->openFile(pathType, pathData, flags, autoSkipBOMTable))
    {
        return zffalse;
    }
    zfbool needSerialize = (ret.callbackSerializeCustomType() == zfnull);
    ret = ZFCallbackForMemberMethod(
        inputOwner, ZFMethodAccess(_ZFP_I_ZFInputCallbackForPathInfoOwner, onInput));
    ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, inputOwner);

    ret.pathInfoSet(pathType, pathData);

    zfstring callbackId;
    ZFPathInfoToString(callbackId, *ret.pathInfo());
    if(autoSkipBOMTable.objectCompare(ZFFileBOMListDefault()) != ZFCompareTheSame)
    {
        ZFFileBOMListToString(callbackId, autoSkipBOMTable);
    }
    ret.callbackIdSet(callbackId);

    if(needSerialize)
    {
        zfbool success = zffalse;
        ZFSerializableData customData;
        customData.itemClassSet(ZFSerializableKeyword_node);
        do
        {
            ZFSerializableData pathInfoData;
            if(!ZFPathInfoToData(pathInfoData, *ret.pathInfo()))
            {
                break;
            }
            pathInfoData.categorySet(ZFSerializableKeyword_ZFFileCallback_pathInfo);
            customData.elementAdd(pathInfoData);

            if(flags != ZFFileOpenOption::e_Read)
            {
                ZFSerializableData flagsData;
                if(!ZFFileOpenOptionFlagsToData(flagsData, flags))
                {
                    break;
                }
                flagsData.categorySet(ZFSerializableKeyword_ZFFileCallback_flags);
                customData.elementAdd(flagsData);
            }

            if(autoSkipBOMTable.objectCompare(ZFFileBOMListDefault()) != ZFCompareTheSame)
            {
                ZFSerializableData autoSkipBOMTableData;
                if(!zfstringToData(autoSkipBOMTableData, ZFFileBOMListToString(autoSkipBOMTable)))
                {
                    break;
                }
                autoSkipBOMTableData.categorySet(ZFSerializableKeyword_ZFFileCallback_autoSkipBOMTable);
                customData.elementAdd(autoSkipBOMTableData);
            }

            success = zftrue;
        } while(zffalse);
        if(success)
        {
            ret.callbackSerializeCustomTypeSet(ZFCallbackSerializeCustomType_ZFInputCallbackForPathInfo);
            ret.callbackSerializeCustomDataSet(customData);
        }
        else
        {
            ret = ZFCallbackNull();
        }
    }

    return ret.callbackIsValid();
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFInputCallbackForPathInfo, ZFCallbackSerializeCustomType_ZFInputCallbackForPathInfo)
{
    const ZFSerializableData *pathInfoData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_pathInfo, outErrorHint, outErrorPos);
    if(pathInfoData == zfnull)
    {
        return zffalse;
    }
    ZFPathInfo pathInfo;
    if(!ZFPathInfoFromData(pathInfo, *pathInfoData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read;
    const ZFSerializableData *flagsData = ZFSerializableUtil::checkElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_flags);
    if(flagsData != zfnull && !ZFFileOpenOptionFlagsFromData(flags, *flagsData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ZFFileBOMList autoSkipBOMTable;
    const ZFSerializableData *autoSkipBOMTableData = ZFSerializableUtil::checkElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_autoSkipBOMTable);
    if(autoSkipBOMTableData != zfnull && !ZFCoreArrayFromData(autoSkipBOMTable, ZFFileBOMFromData, *autoSkipBOMTableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    else
    {
        autoSkipBOMTable = ZFFileBOMListDefault();
    }

    ret.callbackSerializeCustomDisable();
    ZFInputCallbackForPathInfoT(ret, pathInfo.pathType, pathInfo.pathData, flags, autoSkipBOMTable);
    if(!ret.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            zfText("failed to open file: %s"), ZFPathInfoToString(pathInfo).cString());
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
// ZFOutputCallbackForPathInfo
zfclass _ZFP_I_ZFOutputCallbackForPathInfoOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFOutputCallbackForPathInfoOwner, ZFObject)

protected:
    zfoverride
    virtual void objectOnDealloc(void)
    {
        if(this->token != ZFTokenInvalid())
        {
            this->impl->callbackClose(this->token);
            this->token = ZFTokenInvalid();
        }
        zfsuper::objectOnDealloc();
    }

public:
    virtual zfbool openFile(ZF_IN const zfchar *pathType,
                            ZF_IN const zfchar *pathData,
                            ZF_IN ZFFileOpenOptionFlags flags)
    {
        this->impl = ZFFilePathInfoDataGet(pathType);
        if(this->impl == zfnull)
        {
            return zffalse;
        }

        this->token = this->impl->callbackOpen(pathData, flags, zftrue);
        return (this->token != ZFTokenInvalid());
    }

    ZFMETHOD_INLINE_2(zfindex, onOutput,
                      ZFMP_IN(const void *, s),
                      ZFMP_IN(zfindex, count))
    {
        return this->impl->callbackWrite(this->token, s, count);
    }
    ZFMETHOD_INLINE_2(zfbool, ioSeek,
                      ZFMP_IN(zfindex, byteSize),
                      ZFMP_IN(ZFSeekPos, pos))
    {
        return ZFFilePathInfoBOMSeek(*(this->impl), this->token, 0, byteSize, pos);
    }
    ZFMETHOD_INLINE_0(zfindex, ioTell)
    {
        return ZFFilePathInfoBOMTell(*(this->impl), this->token, 0);
    }
    ZFMETHOD_INLINE_0(zfindex, ioSize)
    {
        return this->impl->callbackSize(this->token) - this->impl->callbackTell(this->token);
    }

private:
    const ZFFilePathInfoData *impl;
    ZFToken token;
protected:
    _ZFP_I_ZFOutputCallbackForPathInfoOwner(void)
    : impl(zfnull)
    , token(ZFTokenInvalid())
    {
    }
};
ZFMETHOD_FUNC_DEFINE_2(ZFOutputCallback, ZFOutputCallbackForPathInfo,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
{
    ZFOutputCallback ret;
    ZFOutputCallbackForPathInfoT(ret, pathInfo.pathType, pathInfo.pathData, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_2(ZFOutputCallback, ZFOutputCallbackForPathInfoString,
                       ZFMP_IN(const zfchar *, pathInfoString),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
{
    zfstring pathType;
    const zfchar *pathData = zfnull;
    if(!ZFPathInfoParse(pathInfoString, pathType, pathData))
    {
        return ZFCallbackNull();
    }
    ZFOutputCallback ret;
    ZFOutputCallbackForPathInfoT(ret, pathType, pathData, flags);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_4(zfbool, ZFOutputCallbackForPathInfoT,
                       ZFMP_IN_OUT(ZFCallback &, ret),
                       ZFMP_IN(const zfchar *, pathType),
                       ZFMP_IN(const zfchar *, pathData),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
{
    zfblockedAlloc(_ZFP_I_ZFOutputCallbackForPathInfoOwner, outputOwner);
    if(!outputOwner->openFile(pathType, pathData, flags))
    {
        return zffalse;
    }
    zfbool needSerialize = (ret.callbackSerializeCustomType() == zfnull);
    ret = ZFCallbackForMemberMethod(
        outputOwner, ZFMethodAccess(_ZFP_I_ZFOutputCallbackForPathInfoOwner, onOutput));
    ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, outputOwner);

    ret.pathInfoSet(pathType, pathData);

    zfstring callbackId;
    ZFPathInfoToString(callbackId, *ret.pathInfo());
    ret.callbackIdSet(callbackId);

    if(needSerialize)
    {
        zfbool success = zffalse;
        ZFSerializableData customData;
        customData.itemClassSet(ZFSerializableKeyword_node);
        do
        {
            ZFSerializableData pathInfoData;
            if(!ZFPathInfoToData(pathInfoData, *ret.pathInfo()))
            {
                break;
            }
            pathInfoData.categorySet(ZFSerializableKeyword_ZFFileCallback_pathInfo);
            customData.elementAdd(pathInfoData);

            if(flags != ZFFileOpenOption::e_Create)
            {
                ZFSerializableData flagsData;
                if(!ZFFileOpenOptionFlagsToData(flagsData, flags))
                {
                    break;
                }
                flagsData.categorySet(ZFSerializableKeyword_ZFFileCallback_flags);
                customData.elementAdd(flagsData);
            }

            success = zftrue;
        } while(zffalse);
        if(success)
        {
            ret.callbackSerializeCustomTypeSet(ZFCallbackSerializeCustomType_ZFOutputCallbackForPathInfo);
            ret.callbackSerializeCustomDataSet(customData);
        }
        else
        {
            ret = ZFCallbackNull();
        }
    }

    return ret.callbackIsValid();
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFOutputCallbackForPathInfo, ZFCallbackSerializeCustomType_ZFOutputCallbackForPathInfo)
{
    const ZFSerializableData *pathInfoData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_pathInfo, outErrorHint, outErrorPos);
    if(pathInfoData == zfnull)
    {
        return zffalse;
    }
    ZFPathInfo pathInfo;
    if(!ZFPathInfoFromData(pathInfo, *pathInfoData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Create;
    const ZFSerializableData *flagsData = ZFSerializableUtil::checkElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_flags);
    if(flagsData != zfnull && !ZFFileOpenOptionFlagsFromData(flags, *flagsData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ret.callbackSerializeCustomDisable();
    ZFOutputCallbackForPathInfoT(ret, pathInfo.pathType, pathInfo.pathData, flags);
    if(!ret.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            zfText("failed to open file: %s"), ZFPathInfoToString(pathInfo).cString());
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
static zfbool _ZFP_ZFFileCallbackForLocalFileGetAbsPath(ZF_OUT zfstring &pathDataAbs,
                                                        ZF_IN const ZFPathInfo &pathInfo,
                                                        ZF_IN const zfchar *localPath)
{
    const ZFFilePathInfoData *impl = ZFFilePathInfoDataGet(pathInfo.pathType);
    if(impl == zfnull)
    {
        return zffalse;
    }
    if(!impl->callbackPathGet(pathDataAbs, pathInfo.pathData))
    {
        return zffalse;
    }
    if(!impl->callbackIsDir(pathInfo.pathData))
    {
        if(ZFFilePathParentOf(pathDataAbs, pathDataAbs))
        {
            pathDataAbs += ZFFileSeparator();
        }
        pathDataAbs += localPath;
        return zftrue;
    }
    else
    {
        pathDataAbs += ZFFileSeparator();
        pathDataAbs += localPath;
        return impl->callbackPathSet(pathDataAbs, pathDataAbs);
    }
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_4(ZFInputCallback, ZFInputCallbackForLocalFile,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(const zfchar *, localPath),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read),
                       ZFMP_IN_OPT(const ZFFileBOMList &, autoSkipBOMTable, ZFFileBOMListDefault()))
{
    ZFInputCallback ret;
    ZFInputCallbackForLocalFileT(ret, pathInfo, localPath, flags, autoSkipBOMTable);
    return ret;
}
ZFMETHOD_FUNC_DEFINE_5(zfbool, ZFInputCallbackForLocalFileT,
                       ZFMP_OUT(ZFCallback &, ret),
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(const zfchar *, localPath),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Read),
                       ZFMP_IN_OPT(const ZFFileBOMList &, autoSkipBOMTable, ZFFileBOMListDefault()))
{
    if(zfsIsEmpty(localPath))
    {
        return zffalse;
    }

    zfstring pathDataAbs;
    if(!_ZFP_ZFFileCallbackForLocalFileGetAbsPath(pathDataAbs, pathInfo, localPath))
    {
        return zffalse;
    }
    ret.callbackSerializeCustomDisable();
    if(!ZFInputCallbackForPathInfoT(ret, pathInfo.pathType, pathDataAbs, flags, autoSkipBOMTable))
    {
        return zffalse;
    }

    {
        zfbool success = zffalse;
        ZFSerializableData customData;
        customData.itemClassSet(ZFSerializableKeyword_node);
        do
        {
            ZFSerializableData filePathData;
            if(!zfstringToData(filePathData, localPath))
            {
                return zffalse;
            }
            filePathData.categorySet(ZFSerializableKeyword_ZFFileCallback_filePath);
            customData.elementAdd(filePathData);

            if(flags != ZFFileOpenOption::e_Read)
            {
                ZFSerializableData flagsData;
                if(!ZFFileOpenOptionFlagsToData(flagsData, flags))
                {
                    break;
                }
                flagsData.categorySet(ZFSerializableKeyword_ZFFileCallback_flags);
                customData.elementAdd(flagsData);
            }

            if(autoSkipBOMTable.objectCompare(ZFFileBOMListDefault()) != ZFCompareTheSame)
            {
                ZFSerializableData autoSkipBOMTableData;
                if(!zfstringToData(autoSkipBOMTableData, ZFFileBOMListToString(autoSkipBOMTable)))
                {
                    break;
                }
                autoSkipBOMTableData.categorySet(ZFSerializableKeyword_ZFFileCallback_autoSkipBOMTable);
                customData.elementAdd(autoSkipBOMTableData);
            }

            success = zftrue;
        } while(zffalse);
        if(success)
        {
            ret.callbackSerializeCustomTypeSet(ZFCallbackSerializeCustomType_ZFInputCallbackForLocalFile);
            ret.callbackSerializeCustomDataSet(customData);
        }
        else
        {
            ret = ZFCallbackNull();
        }
    }

    return zftrue;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFInputCallbackForLocalFile, ZFCallbackSerializeCustomType_ZFInputCallbackForLocalFile)
{
    const ZFPathInfo *pathInfo = serializableData.pathInfoCheck();
    if(pathInfo == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            zfText("serializableData does not contain path info"));
        return zffalse;
    }

    const ZFSerializableData *filePathData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_filePath, outErrorHint, outErrorPos);
    if(filePathData == zfnull)
    {
        return zffalse;
    }
    const zfchar *localPath = zfnull;
    if(!zfstringFromData(localPath, *filePathData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Read;
    const ZFSerializableData *flagsData = ZFSerializableUtil::checkElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_flags);
    if(flagsData != zfnull && !ZFFileOpenOptionFlagsFromData(flags, *flagsData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ZFFileBOMList autoSkipBOMTable;
    const ZFSerializableData *autoSkipBOMTableData = ZFSerializableUtil::checkElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_autoSkipBOMTable);
    if(autoSkipBOMTableData != zfnull && !ZFCoreArrayFromData(autoSkipBOMTable, ZFFileBOMFromData, *autoSkipBOMTableData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    else
    {
        autoSkipBOMTable = ZFFileBOMListDefault();
    }

    ret.callbackSerializeCustomDisable();
    zfstring pathDataAbs;
    if(!_ZFP_ZFFileCallbackForLocalFileGetAbsPath(pathDataAbs, *pathInfo, localPath))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            zfText("failed to get file path: %s, localPath: %s"),
            ZFPathInfoToString(*pathInfo).cString(),
            localPath);
        return zffalse;
    }
    ZFInputCallbackForPathInfoT(ret, pathInfo->pathType, pathDataAbs, flags, autoSkipBOMTable);
    if(!ret.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            zfText("failed to open file: %s"),
            ZFPathInfoToString(ZFPathInfo(pathInfo->pathType, pathDataAbs)).cString());
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

// ============================================================
ZFMETHOD_FUNC_DEFINE_3(ZFOutputCallback, ZFOutputCallbackForLocalFile,
                       ZFMP_IN(const ZFPathInfo &, pathInfo),
                       ZFMP_IN(const zfchar *, localPath),
                       ZFMP_IN_OPT(ZFFileOpenOptionFlags, flags, ZFFileOpenOption::e_Create))
{
    if(zfsIsEmpty(localPath))
    {
        return ZFCallbackNull();
    }

    zfstring pathDataAbs;
    if(!_ZFP_ZFFileCallbackForLocalFileGetAbsPath(pathDataAbs, pathInfo, localPath))
    {
        return ZFCallbackNull();
    }
    ZFOutputCallback ret;
    ret.callbackSerializeCustomDisable();
    if(!ZFOutputCallbackForPathInfoT(ret, pathInfo.pathType, pathDataAbs, flags))
    {
        return ZFCallbackNull();
    }

    {
        zfbool success = zffalse;
        ZFSerializableData customData;
        customData.itemClassSet(ZFSerializableKeyword_node);
        do
        {
            ZFSerializableData filePathData;
            if(!zfstringToData(filePathData, localPath))
            {
                return ZFCallbackNull();
            }
            filePathData.categorySet(ZFSerializableKeyword_ZFFileCallback_filePath);
            customData.elementAdd(filePathData);

            if(flags != ZFFileOpenOption::e_Create)
            {
                ZFSerializableData flagsData;
                if(!ZFFileOpenOptionFlagsToData(flagsData, flags))
                {
                    break;
                }
                flagsData.categorySet(ZFSerializableKeyword_ZFFileCallback_flags);
                customData.elementAdd(flagsData);
            }

            success = zftrue;
        } while(zffalse);
        if(success)
        {
            ret.callbackSerializeCustomTypeSet(ZFCallbackSerializeCustomType_ZFOutputCallbackForLocalFile);
            ret.callbackSerializeCustomDataSet(customData);
        }
    }

    return ret;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFOutputCallbackForLocalFile, ZFCallbackSerializeCustomType_ZFOutputCallbackForLocalFile)
{
    const ZFPathInfo *pathInfo = serializableData.pathInfoCheck();
    if(pathInfo == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            zfText("serializableData does not contain path info"));
        return zffalse;
    }

    const ZFSerializableData *filePathData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_filePath, outErrorHint, outErrorPos);
    if(filePathData == zfnull)
    {
        return zffalse;
    }
    const zfchar *localPath = zfnull;
    if(!zfstringFromData(localPath, *filePathData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ZFFileOpenOptionFlags flags = ZFFileOpenOption::e_Create;
    const ZFSerializableData *flagsData = ZFSerializableUtil::checkElementByCategory(
        serializableData, ZFSerializableKeyword_ZFFileCallback_flags);
    if(flagsData != zfnull && !ZFFileOpenOptionFlagsFromData(flags, *flagsData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    ret.callbackSerializeCustomDisable();
    zfstring pathDataAbs;
    if(!_ZFP_ZFFileCallbackForLocalFileGetAbsPath(pathDataAbs, *pathInfo, localPath))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            zfText("failed to get file path: %s, localPath: %s"),
            ZFPathInfoToString(*pathInfo).cString(),
            localPath);
        return zffalse;
    }
    ZFOutputCallbackForPathInfoT(ret, pathInfo->pathType, pathDataAbs, flags);
    if(!ret.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            zfText("failed to open file: %s"),
            ZFPathInfoToString(ZFPathInfo(pathInfo->pathType, pathDataAbs)).cString());
        return zffalse;
    }
    serializableData.resolveMark();
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

