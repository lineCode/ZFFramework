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
zfclass ZFCore_ZFMethodFuncUserRegister_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFMethodFuncUserRegister_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        {
            this->testCaseOutputSeparator();
            this->testCaseOutput("try execute dynamically registered method");

            ZFMethodFuncUserRegisterDetail_1(method, zfself::func1, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic"
                , ZFMP_IN(zfstring const &, param0)
                );

            ZFMethodFuncUserRegisterDetail_0(method0, zfself::func0, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic0"
                );

            ZFMethodFuncUserRegisterDetail_1(method1, zfself::func1, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic1"
                , ZFMP_IN(zfstring const &, param0)
                );

            ZFMethodFuncUserRegisterDetail_2(method2, zfself::func2, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic2"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                );

            ZFMethodFuncUserRegisterDetail_3(method3, zfself::func3, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic3"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                );

            ZFMethodFuncUserRegisterDetail_4(method4, zfself::func4, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic4"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                );

            ZFMethodFuncUserRegisterDetail_5(method5, zfself::func5, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic5"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                );

            ZFMethodFuncUserRegisterDetail_6(method6, zfself::func6, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic6"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                );

            ZFMethodFuncUserRegisterDetail_7(method7, zfself::func7, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic7"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                , ZFMP_IN(zfstring const &, param6)
                );

            ZFMethodFuncUserRegisterDetail_8(method8, zfself::func8, "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                zfstring, "funcDynamic8"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                , ZFMP_IN(zfstring const &, param6)
                , ZFMP_IN(zfstring const &, param7)
                );

            zfstring ret = ZFMethodFuncGet("ZFCore_ZFMethodFuncUserRegister_testNamespace", "funcDynamic")
                ->execute<zfstring, zfstring const &>(this, "paramValue");
            this->testCaseOutput("execute result: %s", ret.cString());

            ZFMethodFuncUserUnregister(ZFMethodFuncGet("ZFCore_ZFMethodFuncUserRegister_testNamespace", "funcDynamic"));

            zfLogTrimT() << "after unregister, method:"
                << ZFMethodFuncGet("ZFCore_ZFMethodFuncUserRegister_testNamespace", "funcDynamic");

            this->testCaseOutput("all list:");
            for(zfindex i = 0; i <= 8; ++i)
            {
                this->testCaseOutput("  %s",
                    ZFMethodFuncGet(
                            "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                            zfstringWithFormat("funcDynamic%zi", i)
                        )->objectInfo().cString()
                    );
            }

            for(zfindex i = 0; i <= 8; ++i)
            {
                ZFMethodFuncUserUnregister(ZFMethodFuncGet("ZFCore_ZFMethodFuncUserRegister_testNamespace",
                        zfstringWithFormat("funcDynamic%zi", i))
                    );
            }
        }

        {
            this->testCaseOutputSeparator();
            this->testCaseOutput("try execute statically registered method");

            zfstring ret = ZFMethodFuncGet("ZFCore_ZFMethodFuncUserRegister_testNamespace", "funcStatic")
                ->execute<zfstring, zfstring const &>(this, "paramValue");
            this->testCaseOutput("execute result: %s", ret.cString());

            this->testCaseOutput("all list:");
            for(zfindex i = 0; i <= ZFMETHOD_MAX_PARAM; ++i)
            {
                this->testCaseOutput("  %s",
                    ZFMethodFuncGet(
                            "ZFCore_ZFMethodFuncUserRegister_testNamespace",
                            zfstringWithFormat("funcStatic%zi", i)
                        )->objectInfo().cString()
                    );
            }
        }

        this->testCaseStop();
    }

public:
    static zfstring func0(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj
        )
    {
        zfLogT();
        return "returnValue";
    }
    static zfstring func1(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj
        , ZF_IN zfstring const &param0
        )
    {
        zfLogT();
        return "returnValue";
    }
    static zfstring func2(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj
        , ZF_IN zfstring const &param0
        , ZF_IN zfstring const &param1
        )
    {
        zfLogT();
        return "returnValue";
    }
    static zfstring func3(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj
        , ZF_IN zfstring const &param0
        , ZF_IN zfstring const &param1
        , ZF_IN zfstring const &param2
        )
    {
        zfLogT();
        return "returnValue";
    }
    static zfstring func4(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj
        , ZF_IN zfstring const &param0
        , ZF_IN zfstring const &param1
        , ZF_IN zfstring const &param2
        , ZF_IN zfstring const &param3
        )
    {
        zfLogT();
        return "returnValue";
    }
    static zfstring func5(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj
        , ZF_IN zfstring const &param0
        , ZF_IN zfstring const &param1
        , ZF_IN zfstring const &param2
        , ZF_IN zfstring const &param3
        , ZF_IN zfstring const &param4
        )
    {
        zfLogT();
        return "returnValue";
    }
    static zfstring func6(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj
        , ZF_IN zfstring const &param0
        , ZF_IN zfstring const &param1
        , ZF_IN zfstring const &param2
        , ZF_IN zfstring const &param3
        , ZF_IN zfstring const &param4
        , ZF_IN zfstring const &param5
        )
    {
        zfLogT();
        return "returnValue";
    }
    static zfstring func7(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj
        , ZF_IN zfstring const &param0
        , ZF_IN zfstring const &param1
        , ZF_IN zfstring const &param2
        , ZF_IN zfstring const &param3
        , ZF_IN zfstring const &param4
        , ZF_IN zfstring const &param5
        , ZF_IN zfstring const &param6
        )
    {
        zfLogT();
        return "returnValue";
    }
    static zfstring func8(ZF_IN const ZFMethod *method, ZF_IN ZFObject *obj
        , ZF_IN zfstring const &param0
        , ZF_IN zfstring const &param1
        , ZF_IN zfstring const &param2
        , ZF_IN zfstring const &param3
        , ZF_IN zfstring const &param4
        , ZF_IN zfstring const &param5
        , ZF_IN zfstring const &param6
        , ZF_IN zfstring const &param7
        )
    {
        zfLogT();
        return "returnValue";
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFMethodFuncUserRegister_test)

ZF_NAMESPACE_BEGIN(ZFCore_ZFMethodFuncUserRegister_testNamespace)

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_1(ZFCore_ZFMethodFuncUserRegister_test_funcStatic,
    ZFCore_ZFMethodFuncUserRegister_test::func1,
    zfstring, "funcStatic"
    , ZFMP_IN(zfstring const &, param0)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_0(ZFCore_ZFMethodFuncUserRegister_test_funcStatic0,
    ZFCore_ZFMethodFuncUserRegister_test::func0,
    zfstring, "funcStatic0"
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_1(ZFCore_ZFMethodFuncUserRegister_test_funcStatic1,
    ZFCore_ZFMethodFuncUserRegister_test::func1,
    zfstring, "funcStatic1"
    , ZFMP_IN(zfstring const &, param0)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_2(ZFCore_ZFMethodFuncUserRegister_test_funcStatic2,
    ZFCore_ZFMethodFuncUserRegister_test::func2,
    zfstring, "funcStatic2"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_3(ZFCore_ZFMethodFuncUserRegister_test_funcStatic3,
    ZFCore_ZFMethodFuncUserRegister_test::func3,
    zfstring, "funcStatic3"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_4(ZFCore_ZFMethodFuncUserRegister_test_funcStatic4,
    ZFCore_ZFMethodFuncUserRegister_test::func4,
    zfstring, "funcStatic4"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_5(ZFCore_ZFMethodFuncUserRegister_test_funcStatic5,
    ZFCore_ZFMethodFuncUserRegister_test::func5,
    zfstring, "funcStatic5"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_6(ZFCore_ZFMethodFuncUserRegister_test_funcStatic6,
    ZFCore_ZFMethodFuncUserRegister_test::func6,
    zfstring, "funcStatic6"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_7(ZFCore_ZFMethodFuncUserRegister_test_funcStatic7,
    ZFCore_ZFMethodFuncUserRegister_test::func7,
    zfstring, "funcStatic7"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    , ZFMP_IN(zfstring const &, param6)
    )

ZFMETHOD_FUNC_USER_REGISTER_DETAIL_8(ZFCore_ZFMethodFuncUserRegister_test_funcStatic8,
    ZFCore_ZFMethodFuncUserRegister_test::func8,
    zfstring, "funcStatic8"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    , ZFMP_IN(zfstring const &, param6)
    , ZFMP_IN(zfstring const &, param7)
    )

ZF_NAMESPACE_END_WITH_REGISTER(ZFCore_ZFMethodFuncUserRegister_testNamespace, ZF_NAMESPACE_GLOBAL)

ZF_NAMESPACE_GLOBAL_END

