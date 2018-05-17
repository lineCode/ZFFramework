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
zfclass ZFCore_ZFTypeId_test : zfextends ZFFramework_test_TestCase
{
    ZFOBJECT_DECLARE(ZFCore_ZFTypeId_test, ZFFramework_test_TestCase)

protected:
    zfoverride
    virtual void testCaseOnStart(void)
    {
        zfsuper::testCaseOnStart();

        ZFObject *v = zfnull;

        zfblockedAlloc(v_zfstring, test_zfstring, zfText("string type"));
        v = test_zfstring;

        // for aliased type, when accessed, a new instance would be created and stored as tag in holder object
        typedef const zfchar * _ZFP_AliasedChar;
        zfLogTrimT() << zfText("============================================================");
        zfLogTrimT() << zfText("const zfchar *");
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar>::access(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar &>::access(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar &>::access(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar *>::access(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar *>::access(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar * &>::access(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<_ZFP_AliasedChar * const &>::access(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar * &>::access(v);
        zfLogTrimT() << ZFTypeId<_ZFP_AliasedChar>::Value<const _ZFP_AliasedChar * const &>::access(v);

        // for wrapped type, all should be able to access
        zfLogTrimT() << zfText("============================================================");
        zfLogTrimT() << zfText("zfstring");
        zfLogTrimT() << ZFTypeId<zfstring>::Value<zfstring>::access(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<const zfstring &>::access(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<zfstring &>::access(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<zfstring *>::access(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<const zfstring *>::access(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<zfstring * &>::access(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<zfstring * const &>::access(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<const zfstring * &>::access(v);
        zfLogTrimT() << ZFTypeId<zfstring>::Value<const zfstring * const &>::access(v);

        // zfautoObject processed as normal raw type
        zfblockedAlloc(ZFString, test_String, zfText("zfautoObject"));
        zfblockedAlloc(v_zfautoObject, test_zfautoObject, zfautoObject(test_String));
        v = test_zfautoObject;
        zfLogTrimT() << zfText("============================================================");
        zfLogTrimT() << zfText("zfautoObject");
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<zfautoObject>::access(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<const zfautoObject &>::access(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<zfautoObject &>::access(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<zfautoObject *>::access(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<const zfautoObject *>::access(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<zfautoObject * &>::access(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<zfautoObject * const &>::access(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<const zfautoObject * &>::access(v);
        zfLogTrimT() << ZFTypeId<zfautoObject>::Value<const zfautoObject * const &>::access(v);

        // ZFAny processed as aligned type
        zfblockedAlloc(ZFString, test_ZFAny, zfText("ZFAny"));
        v = test_ZFAny;
        zfLogTrimT() << zfText("============================================================");
        zfLogTrimT() << zfText("ZFAny");
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<ZFAny>::access(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<const ZFAny &>::access(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<ZFAny &>::access(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<ZFAny *>::access(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<const ZFAny *>::access(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<ZFAny * &>::access(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<ZFAny * const &>::access(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<const ZFAny * &>::access(v);
        zfLogTrimT() << ZFTypeId<ZFAny>::Value<const ZFAny * const &>::access(v);

        this->testCaseStop();
    }
};
ZFOBJECT_REGISTER(ZFCore_ZFTypeId_test)

ZF_NAMESPACE_GLOBAL_END

