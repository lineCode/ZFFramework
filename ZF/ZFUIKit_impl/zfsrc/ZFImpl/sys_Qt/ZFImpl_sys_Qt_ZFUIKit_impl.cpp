/* ====================================================================== *
 * Copyright (c) 2010-2018 ZFFramework
 * Github repo: https://github.com/ZFFramework/ZFFramework
 * Home page: http://ZFFramework.com
 * Blog: http://zsaber.com
 * Contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/LICENSE
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"

#if ZF_ENV_sys_Qt

#include <QLayout>
#include <QLabel>
#include <QTextEdit>

ZF_NAMESPACE_GLOBAL_BEGIN

void _ZFP_ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewInfoT(ZF_IN_OUT zfstring &s, ZF_IN QWidget *view)
{
    // class name
    s += view->metaObject()->className();

    // instance
    zfstringAppend(s, " %p", view);

    // frame
    {
        const QRect &frame = view->geometry();
        zfstringAppend(s, " (%d, %d, %d, %d)", (zfint)frame.x(), (zfint)frame.y(), (zfint)frame.width(), (zfint)frame.height());
    }

    // text
    {
        QLabel *tmp = qobject_cast<QLabel *>(view);
        if(tmp != NULL)
        {
            zfstringAppend(s, " %s", tmp->text().toStdString().c_str());
        }
    }
    {
        QTextEdit *tmp = qobject_cast<QTextEdit *>(view);
        if(tmp != NULL)
        {
            zfstringAppend(s, " %s", tmp->toPlainText().toStdString().c_str());
        }
    }
}

void ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewInfoT(ZF_OUT zfstring &ret, ZF_IN QWidget *view)
{
    if(view == zfnull)
    {
        ret = ZFTOKEN_zfnull;
    }
    else
    {
        _ZFP_ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewInfoT(ret, view);
    }
}

static void _ZFP_ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewTree_recursive(ZF_IN_OUT zfstring &s, QWidget *view, zfindex depth, zfindex siblingIndex)
{
    zfstringAppend(s, "|%2d ", siblingIndex);
    for(zfindex i = 0; i < depth; ++i)
    {
        s += "| ";
    }

    _ZFP_ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewInfoT(s, view);

    s += "\n";

    if(view->layout() != zfnull)
    {
        for(int i = 0; i < view->layout()->count(); ++i)
        {
            _ZFP_ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewTree_recursive(s, view->layout()->itemAt(i)->widget(), depth + 1, i);
        }
    }
}
void ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewTreeT(ZF_OUT zfstring &ret, ZF_IN QWidget *view)
{
    ret += "==================== QWidget tree begin ====================\n";
    if(view != zfnull)
    {
        _ZFP_ZFImpl_sys_Qt_ZFUIKit_impl_QWidgetGetViewTree_recursive(ret, view, 0, 0);
    }
    ret += "==================== QWidget tree  end  ====================\n";
}

// convert utility
void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIPointToQPointT(ZF_OUT QPoint &ret, ZF_IN const ZFUIPoint &point)
{
    ret = QPoint(point.x, point.y);
}
void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIPointFromQPointT(ZF_OUT ZFUIPoint &ret, ZF_IN const QPoint &qPoint)
{
    ret.x = qPoint.x();
    ret.y = qPoint.y();
}
void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUISizeToQSizeT(ZF_OUT QSize &ret, ZF_IN const ZFUISize &size)
{
    ret = QSize(size.width, size.height);
}
void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUISizeFromQSizeT(ZF_OUT ZFUISize &ret, ZF_IN const QSize &qSize)
{
    ret.width = qSize.width();
    ret.height = qSize.height();
}
void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIRectToQRectT(ZF_OUT QRect &ret, ZF_IN const ZFUIRect &rect)
{
    ret = QRect(rect.point.x, rect.point.y, rect.size.width, rect.size.height);
}
void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIRectFromQRectT(ZF_OUT ZFUIRect &ret, ZF_IN const QRect &qRect)
{
    ret.point.x = qRect.x();
    ret.point.y = qRect.y();
    ret.size.width = qRect.width();
    ret.size.height = qRect.height();
}

void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIColorToQColor(ZF_OUT QColor &ret, ZF_IN const ZFUIColor &color)
{
    ret.setRgbF(
            (qreal)ZFUIColorGetR(color),
            (qreal)ZFUIColorGetG(color),
            (qreal)ZFUIColorGetB(color),
            (qreal)ZFUIColorGetA(color)
        );
}
void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIColorFromQColorT(ZF_OUT ZFUIColor &ret, ZF_IN const QColor &qColor)
{
    ret = ZFUIColorMake(
            (zffloat)qColor.redF(),
            (zffloat)qColor.greenF(),
            (zffloat)qColor.blueF(),
            (zffloat)qColor.alphaF()
        );
}

void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIAlignFlagsToQAlignmentT(ZF_OUT Qt::Alignment &ret, ZF_IN const ZFUIAlignFlags &align)
{
    ret = 0;

    if(ZFBitTest(align, ZFUIAlign::e_LeftInner))
    {
        ZFBitSet(ret, Qt::AlignLeft);
    }
    else if(ZFBitTest(align, ZFUIAlign::e_RightInner))
    {
        ZFBitSet(ret, Qt::AlignRight);
    }
    else
    {
        ZFBitSet(ret, Qt::AlignHCenter);
    }

    if(ZFBitTest(align, ZFUIAlign::e_TopInner))
    {
        ZFBitSet(ret, Qt::AlignTop);
    }
    else if(ZFBitTest(align, ZFUIAlign::e_BottomInner))
    {
        ZFBitSet(ret, Qt::AlignBottom);
    }
    else
    {
        ZFBitSet(ret, Qt::AlignVCenter);
    }
}
void ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIAlignFlagsFromQAlignmentT(ZF_OUT ZFUIAlignFlags &ret, ZF_IN const Qt::Alignment qAlign)
{
    ret = ZFUIAlign::e_Center;

    if(ZFBitTest(qAlign, Qt::AlignLeft))
    {
        ZFBitSet(ret, ZFUIAlign::e_LeftInner);
    }
    else if(ZFBitTest(qAlign, Qt::AlignRight))
    {
        ZFBitSet(ret, ZFUIAlign::e_RightInner);
    }

    if(ZFBitTest(qAlign, Qt::AlignTop))
    {
        ZFBitSet(ret, ZFUIAlign::e_TopInner);
    }
    else if(ZFBitTest(qAlign, Qt::AlignBottom))
    {
        ZFBitSet(ret, ZFUIAlign::e_BottomInner);
    }
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Qt

