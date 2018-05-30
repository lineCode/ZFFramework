/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFLua_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass _ZFP_ZFLua_gc_test_Object : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFLua_gc_test_Object, ZFObject)

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfLogT() << (void *)this;
        zfsuper::objectOnInit();
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfLogT() << (void *)this;
        zfsuper::objectOnDealloc();
    }
    zfoverride
    virtual void objectOnRetain(void)
    {
        zfsuper::objectOnRetain();
        zfLogT() << (void *)this << this->objectRetainCount();
    }
    zfoverride
    virtual void objectOnRelease(void)
    {
        zfsuper::objectOnRelease();
        zfLogT() << (void *)this << this->objectRetainCount();
    }
};
ZFOBJECT_REGISTER(_ZFP_ZFLua_gc_test_Object)

zfclass ZFLua_gc_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFLua_gc_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfLogTrimT() << zfText("normal begin");
        ZFLuaExecute(zfText(
                "local obj = _ZFP_ZFLua_gc_test_Object();\n"
            ));
        zfLogTrimT() << zfText("normal gc begin");
        ZFLuaGC();
        zfLogTrimT() << zfText("normal gc end");
        zfLogTrimT() << zfText("normal end");

        zfLogTrimT() << zfText("============================================================");
        zfLogTrimT() << zfText("note:");
        zfLogTrimT() << zfText("    if compiled without cpp exception,");
        zfLogTrimT() << zfText("    cpp object would leak when lua exception occurred");
        zfLogTrimT() << zfText("============================================================");
        zfLogTrimT() << zfText("lua exception begin");
        #if ZF_ENV_ZFLUA_USE_EXCEPTION
            ZFLuaExecute(zfText(
                    "local obj = _ZFP_ZFLua_gc_test_Object();\n"
                    "ZFObject.notExistFunc(obj);\n"
                ));
            zfLogTrimT() << zfText("lua exception gc begin");
            ZFLuaGC();
            zfLogTrimT() << zfText("lua exception gc end");
        #else
            zfLogTrimT() << zfText("lua exception disabled");
        #endif
        zfLogTrimT() << zfText("lua exception end");

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFLua_gc_test)

ZF_NAMESPACE_GLOBAL_END

