#ifndef _ZFI_ZFImpl_sys_Qt_ZFUIKit_impl_h_
#define _ZFI_ZFImpl_sys_Qt_ZFUIKit_impl_h_

#include "ZFImpl.h"

#include "ZFImpl/sys_Qt/ZFImpl_sys_Qt_ZF_impl.h"
#include "ZFUIKit/ZFUITypeDef.h"

#if ZF_ENV_sys_Qt
#include <QPoint>
#include <QSize>
#include <QRect>
#include <QColor>
#include <QWidget>

ZF_NAMESPACE_GLOBAL_BEGIN

extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewInfoT(ZF_OUT zfstring &ret, ZF_IN QWidget *view);
inline zfstring ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewInfo(ZF_IN QWidget *view)
{
    zfstring ret;
    ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewInfoT(ret, view);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewTreeT(ZF_OUT zfstring &ret, ZF_IN QWidget *view);
inline zfstring ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewTree(ZF_IN QWidget *view)
{
    zfstring ret;
    ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewTreeT(ret, view);
    return ret;
}

// convert utility
extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIPointToQPointT(ZF_OUT QPoint &ret, ZF_IN const ZFUIPoint &point);
inline QPoint ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIPointToQPoint(ZF_IN const ZFUIPoint &point)
{
    QPoint ret;
    ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIPointToQPointT(ret, point);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIPointFromQPointT(ZF_OUT ZFUIPoint &ret, ZF_IN const QPoint &qPoint);
inline ZFUIPoint ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIPointFromQPoint(ZF_IN const QPoint &qPoint)
{
    ZFUIPoint ret = ZFUIPointZero();
    ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIPointFromQPointT(ret, qPoint);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUISizeToQSizeT(ZF_OUT QSize &ret, ZF_IN const ZFUISize &size);
inline QSize ZFImpl_sys_Qt_ZFUIKit_impl_ZFUISizeToQSize(ZF_IN const ZFUISize &size)
{
    QSize ret;
    ZFImpl_sys_Qt_ZFUIKit_impl_ZFUISizeToQSizeT(ret, size);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUISizeFromQSizeT(ZF_OUT ZFUISize &ret, ZF_IN const QSize &qSize);
inline ZFUISize ZFImpl_sys_Qt_ZFUIKit_impl_ZFUISizeFromQSize(ZF_IN const QSize &qSize)
{
    ZFUISize ret = ZFUISizeZero();
    ZFImpl_sys_Qt_ZFUIKit_impl_ZFUISizeFromQSizeT(ret, qSize);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIRectToQRectT(ZF_OUT QRect &ret, ZF_IN const ZFUIRect &rect);
inline QRect ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIRectToQRect(ZF_IN const ZFUIRect &rect)
{
    QRect ret;
    ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIRectToQRectT(ret, rect);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIRectFromQRectT(ZF_OUT ZFUIRect &ret, ZF_IN const QRect &qRect);
inline ZFUIRect ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIRectFromQRect(ZF_IN const QRect &qRect)
{
    ZFUIRect ret = ZFUIRectZero();
    ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIRectFromQRectT(ret, qRect);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIColorToQColor(ZF_IN QColor &ret, ZF_IN const ZFUIColor &color);
inline QColor ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIColorToQColor(ZF_IN const ZFUIColor &color)
{
    QColor ret;
    ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIColorToQColor(ret, color);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIColorFromQColorT(ZF_OUT ZFUIColor &ret, ZF_IN const QColor qColor);
inline ZFUIColor ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIColorFromQColor(ZF_IN const QColor qColor)
{
    ZFUIColor ret = ZFUIColorZero();
    ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIColorFromQColorT(ret, qColor);
    return ret;
}

extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIAlignFlagsToQAlignmentT(ZF_OUT Qt::Alignment &ret, ZF_IN const ZFUIAlignFlags &align);
inline Qt::Alignment ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIAlignFlagsToQAlignment(ZF_IN const ZFUIAlignFlags &align)
{
    Qt::Alignment ret = 0;
    ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIAlignFlagsToQAlignmentT(ret, align);
    return ret;
}
extern ZF_ENV_EXPORT void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIAlignFlagsFromQAlignmentT(ZF_OUT ZFUIAlignFlags &ret, ZF_IN const Qt::Alignment qAlign);
inline ZFUIAlignFlags ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIAlignFlagsFromQAlignment(ZF_IN const Qt::Alignment qAlign)
{
    ZFUIAlignFlags ret = 0;
    ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIAlignFlagsFromQAlignmentT(ret, qAlign);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Qt
#endif // #ifndef _ZFI_ZFImpl_sys_Qt_ZFUIKit_impl_h_

