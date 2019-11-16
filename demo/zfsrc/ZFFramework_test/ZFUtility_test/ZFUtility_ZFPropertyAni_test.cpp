#include "ZFUtility_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUtility_ZFPropertyAni_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUtility_ZFPropertyAni_test, ZFFramework_test_TestCase)

    ZFPROPERTY_ASSIGN(zfint, v0)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_INLINE(zfint, v0)
    {
        zfLogTrimT() << zfLogCurTimeString() << "v0:" << propertyValue;
    }

    ZFPROPERTY_ASSIGN(zffloat, v1)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_INLINE(zffloat, v1)
    {
        zfLogTrimT() << zfLogCurTimeString() << "v1:" << propertyValue;
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

