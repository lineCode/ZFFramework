/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFCore_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFCore_ZFFilePath_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFFilePath_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutput(zfText("ZFFilePathForModule:"));
        this->testCaseOutput(ZFFilePathForModule());

        this->testCaseOutput(zfText("ZFFilePathForModuleFile:"));
        this->testCaseOutput(ZFFilePathForModuleFile());

        this->testCaseOutput(zfText("ZFFilePathForSetting:"));
        this->testCaseOutput(ZFFilePathForSetting());

        this->testCaseOutput(zfText("ZFFilePathForStorage:"));
        this->testCaseOutput(ZFFilePathForStorage());

        this->testCaseOutput(zfText("ZFFilePathForStorageShared:"));
        this->testCaseOutput(ZFFilePathForStorageShared());

        this->testCaseOutput(zfText("ZFFilePathForCache:"));
        this->testCaseOutput(ZFFilePathForCache());

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFFilePath_test)

ZF_NAMESPACE_GLOBAL_END

