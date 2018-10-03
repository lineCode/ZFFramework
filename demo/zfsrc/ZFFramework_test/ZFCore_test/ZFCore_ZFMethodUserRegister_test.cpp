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
zfclass ZFCore_ZFMethodUserRegister_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFMethodUserRegister_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        {
            this->testCaseOutputSeparator();
            this->testCaseOutput("try execute dynamically registered method");

            ZFMethodUserRegisterDetail_1(method, zfself::func1, zfself::ClassData(),
                public, ZFMethodTypeNormal,
                zfstring, "funcDynamic"
                , ZFMP_IN(zfstring const &, param0)
                );

            ZFMethodUserRegisterDetail_0(method0, zfself::func0, zfself::ClassData(),
                public, ZFMethodTypeNormal,
                zfstring, "funcDynamic0"
                );

            ZFMethodUserRegisterDetail_1(method1, zfself::func1, zfself::ClassData(),
                public, ZFMethodTypeNormal,
                zfstring, "funcDynamic1"
                , ZFMP_IN(zfstring const &, param0)
                );

            ZFMethodUserRegisterDetail_2(method2, zfself::func2, zfself::ClassData(),
                public, ZFMethodTypeNormal,
                zfstring, "funcDynamic2"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                );

            ZFMethodUserRegisterDetail_3(method3, zfself::func3, zfself::ClassData(),
                public, ZFMethodTypeNormal,
                zfstring, "funcDynamic3"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                );

            ZFMethodUserRegisterDetail_4(method4, zfself::func4, zfself::ClassData(),
                public, ZFMethodTypeNormal,
                zfstring, "funcDynamic4"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                );

            ZFMethodUserRegisterDetail_5(method5, zfself::func5, zfself::ClassData(),
                public, ZFMethodTypeNormal,
                zfstring, "funcDynamic5"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                );

            ZFMethodUserRegisterDetail_6(method6, zfself::func6, zfself::ClassData(),
                public, ZFMethodTypeNormal,
                zfstring, "funcDynamic6"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                );

            ZFMethodUserRegisterDetail_7(method7, zfself::func7, zfself::ClassData(),
                public, ZFMethodTypeNormal,
                zfstring, "funcDynamic7"
                , ZFMP_IN(zfstring const &, param0)
                , ZFMP_IN(zfstring const &, param1)
                , ZFMP_IN(zfstring const &, param2)
                , ZFMP_IN(zfstring const &, param3)
                , ZFMP_IN(zfstring const &, param4)
                , ZFMP_IN(zfstring const &, param5)
                , ZFMP_IN(zfstring const &, param6)
                );

            ZFMethodUserRegisterDetail_8(method8, zfself::func8, zfself::ClassData(),
                public, ZFMethodTypeNormal,
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

            zfstring ret = zfself::ClassData()->methodForName("funcDynamic")->execute<zfstring, zfstring const &>(this, "paramValue");
            this->testCaseOutput("execute result: %s", ret.cString());

            ZFMethodUserUnregister(zfself::ClassData()->methodForName("funcDynamic"));

            zfLogTrimT() << "after unregister, method:" << zfself::ClassData()->methodForName("funcDynamic");

            this->testCaseOutput("all list:");
            for(zfindex i = 0; i <= 8; ++i)
            {
                this->testCaseOutput("  %s",
                    zfself::ClassData()->methodForName(zfstringWithFormat("funcDynamic%zi", i))->objectInfo().cString());
            }

            for(zfindex i = 0; i <= 8; ++i)
            {
                ZFMethodUserUnregister(zfself::ClassData()->methodForName(zfstringWithFormat("funcDynamic%zi", i)));
            }
        }

        {
            this->testCaseOutputSeparator();
            this->testCaseOutput("try execute statically registered method");

            zfstring ret = zfself::ClassData()->methodForName("funcStatic")->execute<zfstring, zfstring const &>(this, "paramValue");
            this->testCaseOutput("execute result: %s", ret.cString());

            this->testCaseOutput("all list:");
            for(zfindex i = 0; i <= ZFMETHOD_MAX_PARAM; ++i)
            {
                this->testCaseOutput("  %s",
                    zfself::ClassData()->methodForName(zfstringWithFormat("funcStatic%zi", i))->objectInfo().cString());
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
ZFOBJECT_REGISTER(ZFCore_ZFMethodUserRegister_test)

ZFMETHOD_USER_REGISTER_DETAIL_1(ZFCore_ZFMethodUserRegister_test_funcStatic,
    ZFCore_ZFMethodUserRegister_test::func1, ZFCore_ZFMethodUserRegister_test::ClassData(),
    public, ZFMethodTypeStatic,
    zfstring, "funcStatic"
    , ZFMP_IN(zfstring const &, param0)
    )

ZFMETHOD_USER_REGISTER_DETAIL_0(ZFCore_ZFMethodUserRegister_test_funcStatic0,
    ZFCore_ZFMethodUserRegister_test::func0, ZFCore_ZFMethodUserRegister_test::ClassData(),
    public, ZFMethodTypeStatic,
    zfstring, "funcStatic0"
    )

ZFMETHOD_USER_REGISTER_DETAIL_1(ZFCore_ZFMethodUserRegister_test_funcStatic1,
    ZFCore_ZFMethodUserRegister_test::func1, ZFCore_ZFMethodUserRegister_test::ClassData(),
    public, ZFMethodTypeStatic,
    zfstring, "funcStatic1"
    , ZFMP_IN(zfstring const &, param0)
    )

ZFMETHOD_USER_REGISTER_DETAIL_2(ZFCore_ZFMethodUserRegister_test_funcStatic2,
    ZFCore_ZFMethodUserRegister_test::func2, ZFCore_ZFMethodUserRegister_test::ClassData(),
    public, ZFMethodTypeStatic,
    zfstring, "funcStatic2"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    )

ZFMETHOD_USER_REGISTER_DETAIL_3(ZFCore_ZFMethodUserRegister_test_funcStatic3,
    ZFCore_ZFMethodUserRegister_test::func3, ZFCore_ZFMethodUserRegister_test::ClassData(),
    public, ZFMethodTypeStatic,
    zfstring, "funcStatic3"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    )

ZFMETHOD_USER_REGISTER_DETAIL_4(ZFCore_ZFMethodUserRegister_test_funcStatic4,
    ZFCore_ZFMethodUserRegister_test::func4, ZFCore_ZFMethodUserRegister_test::ClassData(),
    public, ZFMethodTypeStatic,
    zfstring, "funcStatic4"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    )

ZFMETHOD_USER_REGISTER_DETAIL_5(ZFCore_ZFMethodUserRegister_test_funcStatic5,
    ZFCore_ZFMethodUserRegister_test::func5, ZFCore_ZFMethodUserRegister_test::ClassData(),
    public, ZFMethodTypeStatic,
    zfstring, "funcStatic5"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    )

ZFMETHOD_USER_REGISTER_DETAIL_6(ZFCore_ZFMethodUserRegister_test_funcStatic6,
    ZFCore_ZFMethodUserRegister_test::func6, ZFCore_ZFMethodUserRegister_test::ClassData(),
    public, ZFMethodTypeStatic,
    zfstring, "funcStatic6"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    )

ZFMETHOD_USER_REGISTER_DETAIL_7(ZFCore_ZFMethodUserRegister_test_funcStatic7,
    ZFCore_ZFMethodUserRegister_test::func7, ZFCore_ZFMethodUserRegister_test::ClassData(),
    public, ZFMethodTypeStatic,
    zfstring, "funcStatic7"
    , ZFMP_IN(zfstring const &, param0)
    , ZFMP_IN(zfstring const &, param1)
    , ZFMP_IN(zfstring const &, param2)
    , ZFMP_IN(zfstring const &, param3)
    , ZFMP_IN(zfstring const &, param4)
    , ZFMP_IN(zfstring const &, param5)
    , ZFMP_IN(zfstring const &, param6)
    )

ZFMETHOD_USER_REGISTER_DETAIL_8(ZFCore_ZFMethodUserRegister_test_funcStatic8,
    ZFCore_ZFMethodUserRegister_test::func8, ZFCore_ZFMethodUserRegister_test::ClassData(),
    public, ZFMethodTypeStatic,
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

ZF_NAMESPACE_GLOBAL_END

