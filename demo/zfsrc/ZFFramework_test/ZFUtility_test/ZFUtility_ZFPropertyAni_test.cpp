/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUtility_ZFPropertyAni_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUtility_ZFPropertyAni_test, ZFFramework_test_TestCase)

    ZFPROPERTY_ASSIGN(zfint, v0)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfint, v0)
    {
        zfLogTrimT() << zfLogCurTimeString() << zfText("v0:") << propertyValue;
    }

    ZFPROPERTY_ASSIGN(zffloat, v1)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zffloat, v1)
    {
        zfLogTrimT() << zfLogCurTimeString() << zfText("v1:") << propertyValue;
    }

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        ZFPropertyAniBegin();
        this->v0Set(100);
            ZFPropertyAniBegin();
            this->v1Set(200);
            ZFPropertyAniEnd();
        ZFPropertyAniEnd();

        ZFLISTENER_LOCAL(finishDelay, {
            userData->objectHolded<ZFTestCase *>()->testCaseStop();
        })
        ZFThreadExecuteInMainThreadAfterDelay(1000, finishDelay, this->objectHolder());
    }
};
ZFOBJECT_REGISTER(ZFUtility_ZFPropertyAni_test)

ZF_NAMESPACE_GLOBAL_END

