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

zfclass _ZFP_ZFCore_ZFSerializable_test_TestClass : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFSerializable_test_TestClass, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

public:
    ZFPROPERTY_ASSIGN(zfstring, stringInParent)

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        ZFClassUtil::objectPropertyInfo(ret, this);
    }
};

zfclass _ZFP_ZFCore_ZFSerializable_test_TestClassChild : zfextends _ZFP_ZFCore_ZFSerializable_test_TestClass
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFSerializable_test_TestClassChild, _ZFP_ZFCore_ZFSerializable_test_TestClass)

    ZFPROPERTY_ASSIGN(zfstring, stringInChild)
};

zfclass _ZFP_ZFCore_ZFSerializable_test_TestClassContainer : zfextends ZFObject, zfimplements ZFSerializable
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFSerializable_test_TestClassContainer, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable)

public:
    ZFPROPERTY_RETAIN(ZFObject *, serializableMember)

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        ret += this->classData()->classNameFull();
        ZFClassUtil::objectPropertyInfo(ret, this);
    }
};

// ============================================================
zfclass ZFCore_ZFSerializable_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFSerializable_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFSerializable: normal serializable object"));
        this->test(this->obj);

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFSerializable: inherit serializable object"));
        this->test(this->objChild);

        this->testCaseOutputSeparator();
        this->testCaseOutput(zfText("ZFSerializable: serializable object that contains another serializable object"));
        this->test(this->objContainer);

        this->testCaseStop();
    }
protected:
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->memberPrepare();
    }
    virtual void objectOnDealloc(void)
    {
        this->memberDestroy();
        zfsuper::objectOnDealloc();
    }
private:
    _ZFP_ZFCore_ZFSerializable_test_TestClass *obj;
    _ZFP_ZFCore_ZFSerializable_test_TestClassChild *objChild;
    _ZFP_ZFCore_ZFSerializable_test_TestClassContainer *objContainer;
    void memberPrepare(void)
    {
        this->obj = zfAlloc(_ZFP_ZFCore_ZFSerializable_test_TestClass);
        this->obj->stringInParentSet(zfText("base's string, with unicode chars: \"啊哦\""));

        this->objChild = zfAlloc(_ZFP_ZFCore_ZFSerializable_test_TestClassChild);
        this->objChild->stringInParentSet(zfText("child's string"));
        this->objChild->stringInChildSet(zfText("child's string in child"));

        this->objContainer = zfAlloc(_ZFP_ZFCore_ZFSerializable_test_TestClassContainer);
        _ZFP_ZFCore_ZFSerializable_test_TestClassChild *objTmp = zfAlloc(_ZFP_ZFCore_ZFSerializable_test_TestClassChild);
        objTmp->stringInParentSet(zfText("container's string"));
        objTmp->stringInChildSet(zfText("container's string"));
        this->objContainer->serializableMemberSet(objTmp);
        zfRelease(objTmp);
    }
    void memberDestroy(void)
    {
        zfRelease(this->obj);
        this->obj = zfnull;
        zfRelease(this->objChild);
        this->objChild = zfnull;
        zfRelease(this->objContainer);
        this->objContainer = zfnull;
    }
    void test(ZFSerializable *serializableObj)
    {
        zfstring encodedData;

        this->testCaseOutput(zfText("object:\n%s\n"), serializableObj->toObject()->objectInfo().cString());

        {
            ZFSerializableData serializableData;
            ZFObjectToData(serializableData, serializableObj->toObject());
            ZFSerializableDataToZfsd(encodedData, serializableData);
            this->testCaseOutput(zfText("encodedData:\n%s\n"), encodedData.cString());
        }

        {
            ZFSerializableData serializableData;
            serializableObj->serializeToData(serializableData);
            this->testCaseOutput(zfText("serializableData:\n%s\n"), serializableData.objectInfo().cString());
        }

        {
            ZFSerializableData serializableData;
            ZFSerializableDataFromZfsd(serializableData, encodedData);
            zfautoObject newSerializableObj = ZFObjectFromData(serializableData);
            this->testCaseOutput(zfText("re-serialize from encodedData, result:\n%s\n"), newSerializableObj.toObject()->objectInfo().cString());
        }
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFSerializable_test)

ZF_NAMESPACE_GLOBAL_END

