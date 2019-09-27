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

zfclass ZFCore_ZFLambda_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFLambda_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfstring capture = "init";
        ZFLAMBDA_1(refCapture
                , zfstring &, capture
            , {
                capture = "changed";
            }, void
            )
        this->testCaseOutput("before invoke: %s", capture.cString());
        refCapture.executeExact<void>();
        this->testCaseOutput("after invoke: %s", capture.cString());

        ZFCallback outter;
        {
            zfstring value = "init";
            ZFTestCase *testCase = this;
            ZFLAMBDA_2(valueCapture
                    , zfstring, value
                    , ZFTestCase *, testCase
                , {
                    testCase->testCaseOutput("value capture: %s", value.cString());
                }, void
                )
            outter = valueCapture;
        }
        outter.executeExact<void>();


        ZFLAMBDA_0(lambdaWithZeroParam
            , {
            }, void
            )
        lambdaWithZeroParam.executeExact<void>();

        ZFLISTENER_LAMBDA_1(lambdaListener
                , zfstring &, capture
            , {
            })
        lambdaListener.executeExact<void>();
        ZFLISTENER_LAMBDA_0(lambdaListenerWithZeroParam
            , {
            })
        lambdaListenerWithZeroParam.executeExact<void>();

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFLambda_test)

ZF_NAMESPACE_GLOBAL_END

