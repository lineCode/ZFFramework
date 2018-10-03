/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFCallbackSerializable.h"
#include "ZFObjectImpl.h"

#include "../ZFSTLWrapper/zfstl_string.h"
#include "../ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// serialize routine
ZFTYPEID_DEFINE_BY_SERIALIZABLE_CONVERTER(ZFCallback, ZFCallback, {
        { // custom serialize logic
            const zfchar *customType = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFCallback_callbackType);
            if(customType != zfnull)
            {
                const ZFSerializableData *customData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFCallback_callbackData);
                if(customData == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                        "missing %s", ZFSerializableKeyword_ZFCallback_callbackData);
                    return zffalse;
                }

                _ZFP_ZFCallbackSerializeCustomCallback serializeCallback = _ZFP_ZFCallbackSerializeCustomTypeGet(customType);
                if(serializeCallback == zfnull)
                {
                    ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                        "no such callback custom serialize type: %s", customType);
                    return zffalse;
                }
                if(!serializeCallback(v, *customData, outErrorHint, outErrorPos))
                {
                    return zffalse;
                }
                v.callbackSerializeCustomTypeSet(customType);
                v.callbackSerializeCustomDataSet(customData);

                serializableData.resolveMark();
                return zftrue;
            }
        }

        if(zfscmpTheSame(serializableData.itemClass(), ZFSerializableKeyword_null))
        {
            v = ZFCallbackNull();
            serializableData.resolveMark();
            return zftrue;
        }
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFCallback(), outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        const ZFSerializableData *methodData = ZFSerializableUtil::requireElementByCategory(serializableData, ZFSerializableKeyword_ZFCallback_method, outErrorHint, outErrorPos);
        if(methodData == zfnull)
        {
            return zffalse;
        }
        const ZFMethod *method = zfnull;
        if(!ZFMethodFromData(method, *methodData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }

        if(method->methodOwnerClass() == zfnull)
        {
            v = ZFCallbackForMethod(method);
        }
        else if(method->methodType() == ZFMethodTypeStatic)
        {
            v = ZFCallbackForMethod(method);
        }
        else
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "member method callback is not supported");
            return zffalse;
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        if(v.callbackSerializeCustomType() != zfnull)
        {
            if(v.callbackSerializeCustomDisabled())
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, "callback was marked as not serializable");
                return zffalse;
            }

            if(v.callbackSerializeCustomData() == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, "missing callback serialize custom data");
                return zffalse;
            }
            serializableData.itemClassSet(ZFTypeId_ZFCallback());

            serializableData.attributeSet(ZFSerializableKeyword_ZFCallback_callbackType, v.callbackSerializeCustomType());

            ZFSerializableData customData = v.callbackSerializeCustomData()->copy();
            customData.categorySet(ZFSerializableKeyword_ZFCallback_callbackData);
            serializableData.elementAdd(customData);

            return zftrue;
        }

        switch(v.callbackType())
        {
            case ZFCallbackTypeDummy:
                serializableData.itemClassSet(ZFSerializableKeyword_null);
                break;
            case ZFCallbackTypeMethod:
            {
                serializableData.itemClassSet(ZFTypeId_ZFCallback());
                ZFSerializableData methodData;
                if(!ZFMethodToData(methodData, v.callbackMethod(), outErrorHint))
                {
                    return zffalse;
                }
                methodData.categorySet(ZFSerializableKeyword_ZFCallback_method);
                serializableData.elementAdd(methodData);
            }
                break;
            case ZFCallbackTypeMemberMethod:
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    "member method callback is not supported");
                return zffalse;
            }
                break;
            case ZFCallbackTypeRawFunction:
                serializableData.itemClassSet(ZFTypeId_ZFCallback());
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    "raw function is not supported");
                return zffalse;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }

        return zftrue;
    })

// ============================================================
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFListener, ZFListener)
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFIOCallback, ZFIOCallback)
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFOutput, ZFOutput)
ZFTYPEID_ALIAS_DEFINE(ZFCallback, ZFCallback, ZFInput, ZFInput)

// ============================================================
// custom serialize logic
static zfstlmap<zfstlstringZ, _ZFP_ZFCallbackSerializeCustomCallback> &_ZFP_ZFCallbackSerializeCustomCallbackMap(void)
{
    static zfstlmap<zfstlstringZ, _ZFP_ZFCallbackSerializeCustomCallback> d;
    return d;
}
void _ZFP_ZFCallbackSerializeCustomTypeRegister(ZF_IN const zfchar *customType,
                                                ZF_IN _ZFP_ZFCallbackSerializeCustomCallback serializeCallback)
{
    zfstlmap<zfstlstringZ, _ZFP_ZFCallbackSerializeCustomCallback> &m = _ZFP_ZFCallbackSerializeCustomCallbackMap();
    zfCoreAssert(!zfsIsEmpty(customType) && serializeCallback != zfnull);
    zfCoreAssertWithMessage(m.find(customType) == m.end(), "custom callback serialize type \"%s\" already registered", customType);

    m[customType] = serializeCallback;
}
void _ZFP_ZFCallbackSerializeCustomTypeUnregister(ZF_IN const zfchar *customType)
{
    _ZFP_ZFCallbackSerializeCustomCallbackMap().erase(customType);
}
_ZFP_ZFCallbackSerializeCustomCallback _ZFP_ZFCallbackSerializeCustomTypeGet(ZF_IN const zfchar *customType)
{
    zfstlmap<zfstlstringZ, _ZFP_ZFCallbackSerializeCustomCallback> &m = _ZFP_ZFCallbackSerializeCustomCallbackMap();
    zfstlmap<zfstlstringZ, _ZFP_ZFCallbackSerializeCustomCallback>::iterator it = m.find(customType);
    if(it != m.end())
    {
        return it->second;
    }
    else
    {
        return zfnull;
    }
}

ZF_NAMESPACE_GLOBAL_END

