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
// cwd
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForCwd)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFFileCwd) *impl = ZFPROTOCOL_TRY_ACCESS(ZFFileCwd);
    if(impl != zfnull)
    {
        return impl->pathForCwd();
    }
    else
    {
        return zfnull;
    }
}
ZFMETHOD_FUNC_DEFINE_1(zfbool, ZFFilePathForCwdChange,
                       ZFMP_IN(const zfchar *, path))
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFFileCwd) *impl = ZFPROTOCOL_TRY_ACCESS(ZFFileCwd);
    if(impl != zfnull)
    {
        return impl->pathForCwdChange(path);
    }
    else
    {
        return zffalse;
    }
}
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForCwdOrModule)
{
    const zfchar *ret = ZFFilePathForCwd();
    if(ret != zfnull)
    {
        return ret;
    }
    else
    {
        return ZFFilePathForModule();
    }
}

// ============================================================
// module
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForModule)
{
    return _ZFP_ZFFilePathImpl->pathForModule();
}
ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForModuleFile)
{
    return _ZFP_ZFFilePathImpl->pathForModuleFile();
}

// ============================================================
// setting
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFFilePathForSettingOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForSetting)
{
    return _ZFP_ZFFilePathImpl->pathForSetting();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFilePathForSettingSet,
                       ZFMP_IN_OPT(const zfchar *, path, zfnull))
{
    zfstring old = _ZFP_ZFFilePathImpl->pathForSetting();
    _ZFP_ZFFilePathImpl->pathForSettingSet(path);
    if(!old.isEmpty())
    {
        ZFPointerHolder *t = ZFPointerHolder::cacheGet();
        t->holdedData = old.cString();
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFFilePathForSettingOnChange(), t);
        ZFPointerHolder::cacheAdd(t);
    }
}

// ============================================================
// storage
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFFilePathForStorageOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForStorage)
{
    return _ZFP_ZFFilePathImpl->pathForStorage();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFilePathForStorageSet,
                       ZFMP_IN_OPT(const zfchar *, path, zfnull))
{
    zfstring old = _ZFP_ZFFilePathImpl->pathForStorage();
    _ZFP_ZFFilePathImpl->pathForStorageSet(path);
    if(!old.isEmpty())
    {
        ZFPointerHolder *t = ZFPointerHolder::cacheGet();
        t->holdedData = old.cString();
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFFilePathForStorageOnChange(), t);
        ZFPointerHolder::cacheAdd(t);
    }
}

// ============================================================
// storage shared
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFFilePathForStorageSharedOnChange)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForStorageShared)
{
    return _ZFP_ZFFilePathImpl->pathForStorageShared();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFilePathForStorageSharedSet,
                       ZFMP_IN_OPT(const zfchar *, path, zfnull))
{
    zfstring old = _ZFP_ZFFilePathImpl->pathForStorageShared();
    _ZFP_ZFFilePathImpl->pathForStorageSharedSet(path);
    if(!old.isEmpty())
    {
        ZFPointerHolder *t = ZFPointerHolder::cacheGet();
        t->holdedData = old.cString();
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFFilePathForStorageSharedOnChange(), t);
        ZFPointerHolder::cacheAdd(t);
    }
}

// ============================================================
// cache
ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFFilePathForCacheOnChange)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFFilePathForCacheBeforeClear)
ZFOBSERVER_EVENT_GLOBAL_REGISTER(ZFFilePathForCacheAfterClear)
ZF_NAMESPACE_END(ZFGlobalEvent)

ZFMETHOD_FUNC_DEFINE_0(const zfchar *, ZFFilePathForCache)
{
    return _ZFP_ZFFilePathImpl->pathForCache();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFFilePathForCacheSet,
                       ZFMP_IN_OPT(const zfchar *, path, zfnull))
{
    zfstring old = _ZFP_ZFFilePathImpl->pathForCache();
    _ZFP_ZFFilePathImpl->pathForCacheSet(path);
    if(!old.isEmpty())
    {
        ZFPointerHolder *t = ZFPointerHolder::cacheGet();
        t->holdedData = old.cString();
        ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFFilePathForCacheOnChange(), t);
        ZFPointerHolder::cacheAdd(t);
    }
}

ZFMETHOD_FUNC_DEFINE_0(void, ZFFilePathForCacheClear)
{
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFFilePathForCacheBeforeClear());
    _ZFP_ZFFilePathImpl->pathForCacheClear();
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventZFFilePathForCacheAfterClear());
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFFilePathForCacheAutoClear, ZFLevelZFFrameworkLow)
{
    ZFFilePathForCacheClear();
}
ZF_GLOBAL_INITIALIZER_END(ZFFilePathForCacheAutoClear)

ZF_NAMESPACE_GLOBAL_END

