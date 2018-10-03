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

zfclass ZF_ENV_EXPORT _ZFP_ZFCore_ZFPropertyDynamic_test_Object : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFCore_ZFPropertyDynamic_test_Object, ZFObject)

public:
    zfint myProp(void) const
    {
        return _ZFP_myProp.zfv;
    }
    void myPropSet(ZF_IN zfint v)
    {
        _ZFP_myProp.zfv = v;
    }
private:
    zftValue<zfint> _ZFP_myProp;
};

// ============================================================
zfclass ZFCore_ZFPropertyDynamic_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFPropertyDynamic_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        const ZFClass *cls = _ZFP_ZFCore_ZFPropertyDynamic_test_Object::ClassData();

        const ZFMethod *setterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .methodOwnerClassSet(cls)
                .methodGenericInvokerSet(_setterGI)
                .methodTypeSet(ZFMethodTypeNormal)
                .methodNameSet("myPropSet")
                .methodParamAdd(ZFTypeId_zfint(), "zfint const &")
            );
        const ZFMethod *getterMethod = ZFMethodDynamicRegister(ZFMethodDynamicRegisterParam()
                .methodOwnerClassSet(cls)
                .methodGenericInvokerSet(_getterGI)
                .methodTypeSet(ZFMethodTypeNormal)
                .methodNameSet("myProp")
                .methodReturnTypeIdSet(ZFTypeId_zfint())
            );

        const ZFProperty *property = ZFPropertyDynamicRegister(ZFPropertyDynamicRegisterParam()
                .propertyOwnerClassSet(cls)
                .propertyTypeIdSet(ZFTypeId_zfint())
                .propertyNameSet("myProp")
                .propertyCustomImplSet(setterMethod, getterMethod, _callbackIsValueAccessed, _callbackIsInitValue, _callbackValueReset)
            );

        this->testCaseOutput("property: %s", property->objectInfo().cString());

        zfblockedAlloc(_ZFP_ZFCore_ZFPropertyDynamic_test_Object, obj);
        obj->myPropSet(123);
        this->testCaseOutput("obj: %s", ZFClassUtil::objectInfo(obj).cString());

        ZFPropertyDynamicUnregister(property);
        ZFMethodDynamicUnregister(setterMethod);
        ZFMethodDynamicUnregister(getterMethod);
        this->testCaseStop();
    }
private:
    static const zfchar *_valueKey(void)
    {
        return "ZFCore_ZFPropertyDynamic_test_myProp";
    }
    static zfbool _setterGI(ZFMETHOD_GENERIC_INVOKER_PARAMS)
    {
        _ZFP_ZFCore_ZFPropertyDynamic_test_Object *obj = invokerObject->toAny();
        obj->myPropSet(paramList[0]->to<v_zfint *>()->zfv);
        obj->tagSet(_valueKey(), paramList[0]->to<ZFCopyable *>()->copy());
        return zftrue;
    }
    static zfbool _getterGI(ZFMETHOD_GENERIC_INVOKER_PARAMS)
    {
        _ZFP_ZFCore_ZFPropertyDynamic_test_Object *obj = invokerObject->toAny();
        zfautoObject tag = zflineAlloc(v_zfint, obj->myProp());
        obj->tagSet(_valueKey(), tag);
        ret = tag;
        return zftrue;
    }
    static zfbool _callbackIsValueAccessed(ZF_IN const ZFProperty *property,
                                           ZF_IN ZFObject *ownerObj)
    {
        return !_callbackIsInitValue(property, ownerObj);
    }
    static zfbool _callbackIsInitValue(ZF_IN const ZFProperty *property,
                                       ZF_IN ZFObject *ownerObj,
                                       ZF_OUT_OPT void *outInitValue = zfnull)
    {
        _ZFP_ZFCore_ZFPropertyDynamic_test_Object *obj = ownerObj->toAny();
        if(outInitValue != zfnull)
        {
            *(zfint *)outInitValue = 0;
        }
        return (obj->myProp() == 0);
    }
    static void _callbackValueReset(ZF_IN const ZFProperty *property,
                                    ZF_IN ZFObject *ownerObj)
    {
        _ZFP_ZFCore_ZFPropertyDynamic_test_Object *obj = ownerObj->toAny();
        obj->tagRemove(_valueKey());
        obj->myPropSet(0);
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFPropertyDynamic_test)

ZF_NAMESPACE_GLOBAL_END

