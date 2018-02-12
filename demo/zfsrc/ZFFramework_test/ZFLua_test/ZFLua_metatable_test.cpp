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

ZF_NAMESPACE_GLOBAL_BEGIN

zfclass ZFLua_metatable_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFLua_metatable_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        {
            ZFCoreStatisticInvokeTimeAccurateLoggerOneTime(zfText("run lua code"));
            ZFLuaExecute(zfText(
                    "print('\\n__add: (5 5 5)')\n"
                    "print(zfint(2) + zfint(3))\n"
                    "print(zfint(2) + 3)\n"
                    "print(2 + zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__sub: (-1 -1 -1)')\n"
                    "print(zfint(2) - zfint(3))\n"
                    "print(zfint(2) - 3)\n"
                    "print(2 - zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__mul: (6 6 6)')\n"
                    "print(zfint(2) * zfint(3))\n"
                    "print(zfint(2) * 3)\n"
                    "print(2 * zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__div: (0 0 0)')\n"
                    "print(zfint(2) / zfint(3))\n"
                    "print(zfint(2) / 3)\n"
                    "print(2 / zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__mod: (2 2 2)')\n"
                    "print(zfint(2) % zfint(3))\n"
                    "print(zfint(2) % 3)\n"
                    "print(2 % zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__unm: (-2)')\n"
                    "print(-zfint(2))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__band: (2 2 2)')\n"
                    "print(zfint(2) & zfint(3))\n"
                    "print(zfint(2) & 3)\n"
                    "print(2 & zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__bor: (3 3 3)')\n"
                    "print(zfint(2) | zfint(3))\n"
                    "print(zfint(2) | 3)\n"
                    "print(2 | zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__bxor: (1 1 1)')\n"
                    "print(zfint(2) ~ zfint(3))\n"
                    "print(zfint(2) ~ 3)\n"
                    "print(2 ~ zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__bnot: (4294967292)')\n"
                    "print(~zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__shl: (16 16 16)')\n"
                    "print(zfint(2) << zfint(3))\n"
                    "print(zfint(2) << 3)\n"
                    "print(2 << zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__shr: (0 0 0)')\n"
                    "print(zfint(2) >> zfint(3))\n"
                    "print(zfint(2) >> 3)\n"
                    "print(2 >> zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__concat: (23 23 23)')\n"
                    "print(zfstring(2) .. zfstring(3))\n"
                    "print(zfstring(2) .. 3)\n"
                    "print(2 .. zfstring(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__len: (3)')\n"
                    "print(#zfstring('123'))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__eq: (true false false)')\n"
                    "print(zfint(3) == zfint(3))\n"
                    "print(zfint(3) == 3)\n" // not compared by __eq
                    "print(3 == zfint(3))\n" // not compared by __eq
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__lt: (true true true)')\n"
                    "print(zfint(2) < zfint(3))\n"
                    "print(zfint(2) < 3)\n"
                    "print(2 < zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__le: (true true true)')\n"
                    "print(zfint(2) <= zfint(3))\n"
                    "print(zfint(2) <= 3)\n"
                    "print(2 <= zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__rt: (true true true)')\n"
                    "print(zfint(4) > zfint(3))\n"
                    "print(zfint(4) > 3)\n"
                    "print(4 > zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__re: (true true true)')\n"
                    "print(zfint(4) >= zfint(3))\n"
                    "print(zfint(4) >= 3)\n"
                    "print(4 >= zfint(3))\n"
                ));
            ZFLuaExecute(zfText(
                    "print('\\n__neq: (false true true)')\n"
                    "print(zfint(3) ~= zfint(3))\n"
                    "print(zfint(3) ~= 3)\n" // not compared by __eq
                    "print(3 ~= zfint(3))\n" // not compared by __eq
                ));
            ZFLuaExecute(zfText(
                    "print('\\nbool: (true false)')\n"
                    "if(zfbool('true')) then\n"
                    "    print('bool true')\n"
                    "else\n"
                    "    print('bool false')\n"
                    "end\n"
                    "if(zfbool('false')) then\n"
                    "    print('bool true')\n"
                    "else\n"
                    "    print('bool false')\n"
                    "end\n"
                ));
        }

        {
            ZFCoreStatisticInvokeTimeAccurateLoggerOneTime(zfText("lua gc"));
            ZFLuaGC();
        }

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFLua_metatable_test)

ZF_NAMESPACE_GLOBAL_END

