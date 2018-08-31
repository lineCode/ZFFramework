/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFUIWidget_test.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFUIWidget_ZFUISerializable_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFUIWidget_ZFUISerializable_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        zfautoObject testObject = this->prepareTestObject();

        {
            zfstring s;
            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("xml:"));
            ZFObjectToXml(ZFOutputForString(s), testObject);
            ZFOutputForConsole() << s;
            this->testCaseOutput(zfText("xml re-serialized:"));
            ZFObjectToXml(ZFOutputForConsole(), ZFObjectFromXml(ZFInputForBuffer(s)));
        }

        {
            zfstring s;
            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("json:"));
            ZFObjectToJson(ZFOutputForString(s), testObject);
            ZFOutputForConsole() << s;
            this->testCaseOutput(zfText("json re-serialized:"));
            ZFObjectToJson(ZFOutputForConsole(), ZFObjectFromJson(ZFInputForBuffer(s)));
        }

        {
            zfstring s;
            this->testCaseOutputSeparator();
            this->testCaseOutput(zfText("zfsd:"));
            ZFObjectToZfsd(ZFOutputForString(s), testObject);
            ZFOutputForConsole() << s;
            this->testCaseOutput(zfText("zfsd re-serialized:"));
            ZFObjectToZfsd(ZFOutputForConsole(), ZFObjectFromZfsd(ZFInputForBuffer(s)));
        }

        this->testCaseStop();
    }

private:
    zfautoObject prepareTestObject(void)
    {
        zfblockedAlloc(ZFUIAutoLayout, parent);
        parent->viewAlphaSet(0.5f);

        zfblockedAlloc(ZFUIView, child0);
        child0->viewUIEnableTreeSet(zffalse);
        zfal_maker(child0, parent).left().toParentLeft();

        zfblockedAlloc(ZFUITextView, child1);
        child1->textSet(zfText("special chars: \r\n\t\"'-_=+<>()[]{}"));
        zfal_maker(child1, parent).right().toParentRight();

        return parent;
    }
};
ZFOBJECT_REGISTER(ZFUIWidget_ZFUISerializable_test)

ZF_NAMESPACE_GLOBAL_END

