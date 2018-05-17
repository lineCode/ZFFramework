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

zfclass ZFCore_ZFFileIO_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFFileIO_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfLogTrimT() << zfText("============================================================");
        zfLogTrimT() << zfText("res tree:");
        ZFFilePathInfoTreePrint(ZFPathInfo(ZFPathType_res, zfText("test_ZFFileIO")), ZFOutputDefault(), zfText("  "));

        zfLogTrimT() << zfText("try read content:");
        ZFInputReadToOutput(ZFOutputDefault(),
            ZFInputForResFile(zfText("test_ZFFileIO/fileExist")));
        ZFInputReadToOutput(ZFOutputDefault(),
            ZFInputForResFile(zfText("test_ZFFileIO/dirExist/fileExist2")));

        zfLogTrimT() << zfText("============================================================");
        zfLogTrimT() << zfText("copy to cache dir, tree:");
        ZFFileResCopy(zfText("test_ZFFileIO"), zfstringWithFormat(zfText("%s/test_ZFFileIO"), ZFFilePathForCache()));
        ZFFilePathInfoTreePrint(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO")), ZFOutputDefault(), zfText("  "));

        zfLogTrimT() << zfText("try read content:");
        ZFInputReadToOutput(ZFOutputDefault(),
            ZFInputForPathInfo(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/fileExist"))));
        ZFInputReadToOutput(ZFOutputDefault(),
            ZFInputForPathInfo(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/dirExist/fileExist2"))));

        zfLogTrimT() << zfText("============================================================");
        zfLogTrimT() << zfText("file type check:");

        zfLogTrimT() << zfText("  res isExist:");
        zfLogTrimT() << zfText("    ") << ZFFileResIsExist(zfText("test_ZFFileIO/fileExist"));
        zfLogTrimT() << zfText("    ") << ZFFileResIsExist(zfText("test_ZFFileIO/dirExist"));
        zfLogTrimT() << zfText("    ") << ZFFileResIsExist(zfText("test_ZFFileIO/dirExist/fileExist2"));
        zfLogTrimT() << zfText("    ") << !ZFFileResIsExist(zfText("test_ZFFileIO/fileNotExist"));
        zfLogTrimT() << zfText("    ") << !ZFFileResIsExist(zfText("test_ZFFileIO/dirNotExist"));
        zfLogTrimT() << zfText("    ") << !ZFFileResIsExist(zfText("test_ZFFileIO/dirExist/fileNotExist"));

        zfLogTrimT() << zfText("  res isDir:");
        zfLogTrimT() << zfText("    ") << !ZFFileResIsDir(zfText("test_ZFFileIO/fileExist"));
        zfLogTrimT() << zfText("    ") << ZFFileResIsDir(zfText("test_ZFFileIO/dirExist"));
        zfLogTrimT() << zfText("    ") << !ZFFileResIsDir(zfText("test_ZFFileIO/dirExist/fileExist2"));
        zfLogTrimT() << zfText("    ") << !ZFFileResIsDir(zfText("test_ZFFileIO/fileNotExist"));
        zfLogTrimT() << zfText("    ") << !ZFFileResIsDir(zfText("test_ZFFileIO/dirNotExist"));
        zfLogTrimT() << zfText("    ") << !ZFFileResIsDir(zfText("test_ZFFileIO/dirExist/fileNotExist"));

        zfLogTrimT() << zfText("  file isExist:");
        zfLogTrimT() << zfText("    ") << ZFFilePathInfoIsExist(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/fileExist")));
        zfLogTrimT() << zfText("    ") << ZFFilePathInfoIsExist(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/dirExist")));
        zfLogTrimT() << zfText("    ") << ZFFilePathInfoIsExist(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/dirExist/fileExist2")));
        zfLogTrimT() << zfText("    ") << !ZFFilePathInfoIsExist(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/fileNotExist")));
        zfLogTrimT() << zfText("    ") << !ZFFilePathInfoIsExist(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/dirNotExist")));
        zfLogTrimT() << zfText("    ") << !ZFFilePathInfoIsExist(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/dirExist/fileNotExist")));

        zfLogTrimT() << zfText("  file isDir:");
        zfLogTrimT() << zfText("    ") << !ZFFilePathInfoIsDir(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/fileExist")));
        zfLogTrimT() << zfText("    ") << ZFFilePathInfoIsDir(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/dirExist")));
        zfLogTrimT() << zfText("    ") << !ZFFilePathInfoIsDir(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/dirExist/fileExist2")));
        zfLogTrimT() << zfText("    ") << !ZFFilePathInfoIsDir(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/fileNotExist")));
        zfLogTrimT() << zfText("    ") << !ZFFilePathInfoIsDir(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/dirNotExist")));
        zfLogTrimT() << zfText("    ") << !ZFFilePathInfoIsDir(ZFPathInfo(ZFPathType_cachePath, zfText("test_ZFFileIO/dirExist/fileNotExist")));

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFFileIO_test)

ZF_NAMESPACE_GLOBAL_END

