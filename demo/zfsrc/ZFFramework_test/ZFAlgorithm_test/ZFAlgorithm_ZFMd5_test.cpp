/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFAlgorithm_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFAlgorithm_ZFMd5_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFMd5_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        const zfchar *testString = "123abc";
        zfstring testValue = "A906449D5769FA7361D7ECC6AA3F6D28"; // testString's MD5 to verify
        zfstring value;

        value = zfMd5Calc((const zfbyte *)testString, zfslen(testString) * sizeof(zfchar));
        this->testCaseOutput("MD5 of array \"%s\": %s", testString, value.cString());
        ZFTestCaseAssert(value == testValue);

        zfstring tmpFilePath = this->testCaseUseTmpFile("ZFMd5_Md5.txt");
        ZFToken fp = ZFFileFileOpen(tmpFilePath, ZFFileOpenOption::e_Write);
        if(fp != ZFTokenInvalid())
        {
            ZFFileFileWrite(fp, testString);
            ZFFileFileClose(fp);
            fp = ZFTokenInvalid();
        }
        this->testCaseOutput("write it to file %s, file's MD5: %s",
                tmpFilePath.cString(),
                zfMd5Calc(ZFInputForFile(tmpFilePath)).cString());

        this->testCaseOutputSeparator();
        tmpFilePath = this->testCaseUseTmpFile("ZFMd5_Md5_big.txt");
        fp = ZFFileFileOpen(tmpFilePath, ZFFileOpenOption::e_Write);
        zfindex fileSize = 0;
        if(fp != zfnull)
        {
            for(zfindex i = 0; i < 1000; i++)
            {
                for(zfindex j = 0; j < 1000; j++)
                {
                    ZFFileFileWrite(fp, testString);
                }
            }
            fileSize = ZFFileFileTell(fp);
            ZFFileFileClose(fp);
            fp = zfnull;
        }
        ZFTimeValue tv1 = ZFTime::currentTimeValue();
        zfstring MD5BigFile = zfMd5Calc(ZFInputForFile(tmpFilePath.cString()));
        ZFTimeValue tv2 = ZFTimeValueDec(ZFTime::currentTimeValue(), tv1);
        this->testCaseOutput("write it 1000*1000 times to file %s, file's size: %zi, MD5: %s, time: %s.%03s %03s",
            tmpFilePath.cString(),
            fileSize,
            MD5BigFile.cString(),
            zfsFromInt(tv2.sec).cString(),
            zfsFromInt(tv2.usec / 1000).cString(),
            zfsFromInt(tv2.usec % 1000).cString());

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFMd5_test)

ZF_NAMESPACE_GLOBAL_END

