/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFPropertySerializable.h"
#include "ZFObjectImpl.h"
#include "ZFSerializableUtil.h"

#include "ZFPropertyDeclare.h"
#include "ZFPropertyUserRegister.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFTYPEID_DEFINE(ZFProperty, const ZFProperty *, {
        if(zfscmpTheSame(serializableData.itemClass(), ZFSerializableKeyword_null))
        {
            v = zfnull;
            serializableData.resolveMark();
            return zftrue;
        }
        if(ZFSerializableUtil::requireItemClass(serializableData, ZFTypeId_ZFProperty(), outErrorHint, outErrorPos) == zfnull)
        {
            return zffalse;
        }

        const zfchar *tmpValue = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFProperty_owner, outErrorHint, outErrorPos);
        if(tmpValue == zfnull)
        {
            return zffalse;
        }
        const ZFClass *ownerClass = ZFClass::classForName(tmpValue);
        if(ownerClass == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "no such class \"%s\"", tmpValue);
            return zffalse;
        }

        tmpValue = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFProperty_property, outErrorHint, outErrorPos);
        if(tmpValue == zfnull)
        {
            return zffalse;
        }
        v = ownerClass->propertyForName(tmpValue);
        if(v == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
                "no such property \"%s\" in class \"%s\"", tmpValue, ownerClass->classNameFull());
            return zffalse;
        }

        serializableData.resolveMark();
        return zftrue;
    }, {
        if(v == zfnull)
        {
            serializableData.itemClassSet(ZFSerializableKeyword_null);
            return zftrue;
        }

        serializableData.itemClassSet(ZFTypeId_ZFProperty());

        serializableData.attributeSet(ZFSerializableKeyword_ZFProperty_owner, v->propertyOwnerClass()->classNameFull());
        serializableData.attributeSet(ZFSerializableKeyword_ZFProperty_property, v->propertyName());

        return zftrue;
    }, {
        ZFCoreArrayPOD<ZFIndexRange> pos;
        if(!zfCoreDataPairSplitString(pos, 3, src, srcLen, ":", zfnull, zfnull, zftrue)) {return zffalse;}
        const ZFClass *cls = ZFClass::classForName(zfstring(src + pos[0].start, pos[0].count));
        if(cls == zfnull)
        {
            return zffalse;
        }
        v = cls->propertyForName(zfstring(src + pos[2].start, pos[2].count));
        return (v != zfnull);
    }, {
        if(v)
        {
            s += v->propertyOwnerClass()->classNameFull();
            s += "::";
            s += v->propertyName();
        }
        return zftrue;
    })

ZF_NAMESPACE_GLOBAL_END

