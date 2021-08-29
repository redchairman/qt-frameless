#include "qframelesswidget.h"

QFramelessWidget::QFramelessWidget(QWidget *parent)
    : QWidget(parent)
    , m_framelessHelper(new FramelessHelper(this, this))
{    
}

FramelessHelper *QFramelessWidget::framelessHelper()
{
    return m_framelessHelper;
}

void QFramelessWidget::paintEvent(QPaintEvent *e)
{
    m_framelessHelper->paintEvent(e);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
bool QFramelessWidget::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else
bool QFramelessWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
#endif
{
    return m_framelessHelper->nativeEvent(eventType, message, result);
}

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#ifdef Q_OS_WIN
bool QFramelessWidget::winEvent(MSG *message, long *result)
{
    return nativeEvent("windows_generic_MSG", message, result);
}
#endif
#endif

