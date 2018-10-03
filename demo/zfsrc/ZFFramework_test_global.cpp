/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFFramework_test_global.h"

#include "ZFCore/ZFFile.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFFramework_test_TestCase)

void ZFFramework_test_TestCase::objectOnInit(void)
{
    zfsuper::objectOnInit();
    this->_testCaseTmpPath = zfstringWithFormat("%s%cZFFramework_test%c%s",
        ZFFilePathForCache(),
        ZFFileSeparator(),
        ZFFileSeparator(),
        this->classData()->classNameFull());
}
void ZFFramework_test_TestCase::objectOnDealloc(void)
{
    zfsuper::objectOnDealloc();
}
void ZFFramework_test_TestCase::objectOnDeallocPrepare(void)
{
    ZFFileFileRemove(this->testCaseTmpPath());
    zfsuper::objectOnDeallocPrepare();
}

void ZFFramework_test_TestCase::testCaseOnStop(ZF_IN ZFResultTypeEnum testCaseResult)
{
    for(zfindex i = 0; i < this->_filesToRemove.count(); ++i)
    {
        ZFFileFileRemove(this->_filesToRemove[i]);
    }
    zfsuper::testCaseOnStop(testCaseResult);
}

void ZFFramework_test_TestCase::testCaseOutputSeparator(void)
{
    this->testCaseOutput("----------------------------------------");
}
const zfchar *ZFFramework_test_TestCase::testCaseTmpPath(void)
{
    return this->_testCaseTmpPath;
}
zfstring ZFFramework_test_TestCase::testCaseUseTmpFile(ZF_IN const zfchar *fileName)
{
    zfstring ret = this->testCaseTmpPath();
    ret += ZFFileSeparator();
    ret += fileName;
    this->testCaseAddFileToRemove(ret.cString());
    return ret;
}
void ZFFramework_test_TestCase::testCaseAddFileToRemove(ZF_IN const zfchar *filePath)
{
    this->_filesToRemove.add(filePath);
}

ZF_NAMESPACE_GLOBAL_END

