#include "ZFLua_test.h"
#include "ZFUIKit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFLua_app_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFLua_app_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfautoObject luaResult = ZFLuaExecute(ZFInputForResFile("test_ZFLua_app.lua"));
        ZFUIWindow *window = luaResult;
        zfCoreAssert(window != zfnull);
        window->observerAdd(
            ZFUIWindow::EventWindowOnHide(),
            ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, windowOnHide)));
    }

private:
    ZFLISTENER_INLINE(windowOnHide)
    {
        this->testCaseStop(ZFResultType::e_Success);
        ZFLuaGC();
    }
};
ZFOBJECT_REGISTER(ZFLua_app_test)

ZF_NAMESPACE_GLOBAL_END

