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
#include "ZFUIKit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFLua_UI_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFLua_UI_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfautoObject result = ZFLuaExecute(
                "local window = ZFUIWindow()\n"
                "window:windowShow()\n"
                "window:viewBackgroundColorSet(ZFUIColorRandom())\n"
                "local button = ZFUIButtonBasic()\n"
                "window:childAdd(button)\n"
                "button:layoutParam():layoutAlignSet(ZFUIAlign.e_TopInner() | ZFUIAlign.e_RightInner())\n"
                "button:viewBackgroundColorSet(ZFUIColorRandom())\n"
                "button:buttonLabelTextSet('close')\n"
                "button:observerAdd(ZFUIButton.EventButtonOnClick(), function (listenerData, userData)\n"
                "        userData:objectHolded():windowHide()\n"
                "    end, window:objectHolder())\n"
                "return window\n"
            );
        zfCoreAssert(result != zfnull);

        ZFLISTENER_LOCAL(windowOnHide, {
            ZFLISTENER_LOCAL(testCaseStopDelay, {
                ZFLuaGC();
                ZFTestCase *testCase = userData->objectHolded();
                testCase->testCaseStop();
            })
            ZFThreadTaskRequest(testCaseStopDelay, userData);
        })
        result->observerAdd(ZFUIWindow::EventWindowOnHide(), windowOnHide, this->objectHolder());
    }
};
ZFOBJECT_REGISTER(ZFLua_UI_test)

ZF_NAMESPACE_GLOBAL_END

