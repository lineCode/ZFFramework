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

// ============================================================
zfclass ZFCore_ZFDynamic_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFDynamic_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFDynamic"));

        ZFLISTENER_LOCAL(methodCallback, {
                ZFDynamicMethodData *d = listenerData.param0->toAny();
                zfblockedAlloc(v_zfstring, ret);
                ret->zfv += d->param0->to<v_zfstring *>()->zfv;
                ret->zfv += zfText("(modified)");
                zfautoRelease(zfRetain(ret)); // take care of this
                d->ret = ret;
            })
        ZFDynamic d = ZFDynamic().errorCallbackAdd()
            .classBegin(zfText("ZFDynamicTest"), zfText("ZFObject"))
                .property(ZFTypeId_zfstring(), zfText("testProp"))
                .method(methodCallback, zfnull, ZFTypeId_zfstring(), zfText("testMethod"), ZFTypeId_zfstring())
            .classEnd()
            .NSBegin(zfText("ZFDynamicTestNS"))
                .method(methodCallback, zfnull, ZFTypeId_zfstring(), zfText("testMethod"), ZFTypeId_zfstring())
            .NSEnd()
            ;
        const ZFClass *cls = ZFClass::classForName(zfText("ZFDynamicTest"));
        zfautoObject obj = cls->newInstance();

        obj->invoke(zfText("testPropSet"), zflineAlloc(v_zfstring, zfText("testValue")));
        zfLogT() << obj->invoke(zfText("testProp"));
        zfLogT() << obj->invoke(zfText("testMethod"), zflineAlloc(v_zfstring, zfText("testParam")));

        const ZFMethod *method = ZFMethodFuncGet(zfText("ZFDynamicTestNS"), zfText("testMethod"));
        zfLogT() << method;
        zfLogT() << method->methodGenericInvoke(zfnull, zflineAlloc(v_zfstring, zfText("testParam")));

        d.removeAll();
        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFDynamic_test)

ZF_NAMESPACE_GLOBAL_END

