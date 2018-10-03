/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZF_impl.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Qt
#include <string>

// ============================================================
class _ZFP_ZFImpl_sys_Qt_ValueWrapper
{
public:
    ZFObject *value;
public:
    _ZFP_ZFImpl_sys_Qt_ValueWrapper(void)
    : value(zfnull)
    {
    }
    _ZFP_ZFImpl_sys_Qt_ValueWrapper(ZF_IN ZFObject *obj)
    : value(zfRetain(obj))
    {
    }
    _ZFP_ZFImpl_sys_Qt_ValueWrapper(ZF_IN const _ZFP_ZFImpl_sys_Qt_ValueWrapper &ref)
    : value(zfRetain(ref.value))
    {
    }
    ~_ZFP_ZFImpl_sys_Qt_ValueWrapper(void)
    {
        zfRelease(this->value);
    }
    _ZFP_ZFImpl_sys_Qt_ValueWrapper &operator = (ZF_IN const _ZFP_ZFImpl_sys_Qt_ValueWrapper &ref)
    {
        zfRetainChange(this->value, ref.value);
        return *this;
    }
};
Q_DECLARE_METATYPE(_ZFP_ZFImpl_sys_Qt_ValueWrapper);

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
void ZFImpl_sys_Qt_QObjectTagSet(ZF_IN_OUT QObject *obj, ZF_IN const zfchar *name, ZF_IN QVariant const &tag)
{
    obj->setProperty(name, tag);
}
QVariant ZFImpl_sys_Qt_QObjectTagGet(ZF_IN_OUT QObject *obj, ZF_IN const zfchar *name)
{
    return obj->property(name);
}

void ZFImpl_sys_Qt_QObjectTagSetZFObject(ZF_IN_OUT QObject *obj, ZF_IN const zfchar *name, ZF_IN ZFObject *tag)
{
    if(tag == zfnull)
    {
        ZFImpl_sys_Qt_QObjectTagSet(obj, name, QVariant());
    }
    else
    {
        _ZFP_ZFImpl_sys_Qt_ValueWrapper valueWrapper(tag);
        ZFImpl_sys_Qt_QObjectTagSet(obj, name, QVariant::fromValue(valueWrapper));
    }
}
ZFObject *ZFImpl_sys_Qt_QObjectTagGetZFObject(ZF_IN_OUT QObject *obj, ZF_IN const zfchar *name)
{
    QVariant tag = ZFImpl_sys_Qt_QObjectTagGet(obj, name);
    return ((tag.isValid()) ? tag.value<_ZFP_ZFImpl_sys_Qt_ValueWrapper>().value : zfnull);
}

// ============================================================
void ZFImpl_sys_Qt_zfstringFromQString(ZF_IN_OUT zfstring &result, ZF_IN QString const &nativeString)
{
    std::string t = nativeString.toStdString();
    result += t.c_str();
}
QString ZFImpl_sys_Qt_zfstringToQString(ZF_IN const zfchar *s)
{
    return QString(s);
}

ZF_NAMESPACE_GLOBAL_END
#endif

