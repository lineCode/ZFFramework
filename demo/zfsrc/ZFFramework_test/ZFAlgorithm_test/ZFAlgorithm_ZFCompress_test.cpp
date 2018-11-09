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

zfclass ZFAlgorithm_ZFCompress_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFAlgorithm_ZFCompress_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput("compress buffer");
        {
            ZFIOBufferedCallbackUsingBuffer io;
            ZFCompress(io, ZFInputForBuffer("uncompressed text"));
            ZFDecompress(ZFOutputDefault(), io);
        }

        this->testCaseOutputSeparator();
        this->testCaseOutput("compress tree");
        {
            ZFPathInfo pathInfoSrc(ZFPathType_res(), ".");
            ZFPathInfo pathInfoDst(ZFPathType_cachePath(), "ZFCompress_test");

            this->testCaseOutput("original src tree:");
            ZFFilePathInfoTreePrint(pathInfoSrc, ZFOutputDefault(), "    ");
            this->testCaseOutput("original dst tree:");
            ZFFilePathInfoTreePrint(pathInfoDst, ZFOutputDefault(), "    ");

            ZFIOBufferedCallbackUsingTmpFile io;
            ZFCompressDir(io, pathInfoSrc);

            ZFDecompressDir(pathInfoDst, io);
            this->testCaseOutput("decompressed dst tree:");
            ZFFilePathInfoTreePrint(pathInfoDst, ZFOutputDefault(), "    ");
        }

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFAlgorithm_ZFCompress_test)

ZF_NAMESPACE_GLOBAL_END

