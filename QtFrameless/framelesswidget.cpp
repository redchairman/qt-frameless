#include "framelesswidget.h"

FramelessBase::FramelessBase(QWidget *w)
    : m_widget(w)
    , m_drawShadow(nullptr)
    #ifdef Q_OS_WIN
    , m_dwnShadow(nullptr)
    #endif
    , m_framelessHelper(nullptr)
    , m_titleWidget(nullptr)
    , m_minButton(nullptr)
    , m_maxButton(nullptr)
    , m_closeButton(nullptr)
{
    m_flags = Moveable | ShadowBorder;
#ifdef Q_OS_WIN
    m_shadowFlag = DwmShadow;
#else
    m_shadowFlag = DrawShadow;
#endif
    m_closeButtonAction = ActionClose;
}

FramelessBase::FramelessFlags FramelessBase::framelessFlags()
{
    return m_flags;
}

void FramelessBase::setFramelessFlags(FramelessFlags flags)
{
    m_flags = flags;
}

void FramelessBase::setShadowFlag(FramelessBase::ShadowFlag flag)
{
    m_shadowFlag = flag;
}

void FramelessBase::setCloseButtonAction(FramelessBase::CloseButtonAction action)
{
    m_closeButtonAction = action;
}

void FramelessBase::setUIObject(QWidget *widgetTitle,
                                  QAbstractButton *buttonClose,
                                  QAbstractButton *buttonMax,
                                  QAbstractButton *buttonMin)
{
    m_titleWidget = widgetTitle;
    m_minButton = buttonMin;
    m_maxButton = buttonMax;
    m_closeButton = buttonClose;
}

void FramelessBase::setIconCtrl(FramelessIconCtrl *ctrl)
{
    m_iconCtrl = ctrl;
}

void FramelessBase::initFrameless()
{
    if (m_flags & ShadowBorder)
    {
    #ifdef Q_OS_WIN
        if (m_shadowFlag == DwmShadow)
        {
            m_dwnShadow = new WinDwmShadow(m_widget);
        }
        else
        {
            m_drawShadow = new ShadowImage(m_widget);
        }
    #else
        m_drawShadow = new ShadowImage(this);
    #endif
    }

    m_framelessHelper = new FramelessHelper(m_widget);
    m_framelessHelper->setWidgetResizable(m_flags & Resizeable);
    m_framelessHelper->setWidgetMovable(m_flags & Moveable);
    if (m_closeButtonAction == ActionClose)
        m_framelessHelper->setMinWhenClose(FramelessHelper::CloseAction_Close);
    else if (m_closeButtonAction == ActionMinsize)
        m_framelessHelper->setMinWhenClose(FramelessHelper::CloseAction_Min);
    else
        m_framelessHelper->setMinWhenClose(FramelessHelper::CloseAction_Null);
    if (m_drawShadow)
    {
        QObject::connect(m_framelessHelper, SIGNAL(sigShowDrawShadow()), m_widget, SLOT(sltShowDrawShadow()));
        QObject::connect(m_framelessHelper, SIGNAL(sigHideDrawShadow()), m_widget, SLOT(sltHideDrawShadow()));
        showDrawShadow();
    }
    if (m_dwnShadow)
    {
        m_framelessHelper->setNativeResize(true);
    }
    m_framelessHelper->setIconCtrl(m_iconCtrl);
    m_framelessHelper->setUIObject(m_widget, m_titleWidget, m_closeButton, m_maxButton, m_minButton);
}

void FramelessBase::showDrawShadow()
{
    if (m_drawShadow)
    {
        m_framelessHelper->setDrawShadowWidth(m_drawShadow->shadowSize());
        m_drawShadow->show();
    }
}

void FramelessBase::hideDrawShadow()
{
    if (m_drawShadow)
    {
        m_framelessHelper->setDrawShadowWidth(0);
        m_drawShadow->hide();
    }
}

void FramelessBase::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(m_widget);
    painter.save();
    if (m_drawShadow)
        m_drawShadow->paint(&painter);
    painter.restore();
}

bool FramelessBase::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
    if (eventType != "windows_generic_MSG")
        return false;
    if (m_framelessHelper)
        return m_framelessHelper->nativeEvent(eventType, message, result);
#endif
    return false;
}

FramelessWidget::FramelessWidget(QWidget *parent)
    : QWidget(parent)
    , FramelessBase(this)
{

}
void FramelessWidget::sltShowDrawShadow()
{
    showDrawShadow();
}

void FramelessWidget::sltHideDrawShadow()
{
    hideDrawShadow();
}

void FramelessWidget::paintEvent(QPaintEvent *e)
{
    FramelessBase::paintEvent(e);
}

bool FramelessWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    return FramelessBase::nativeEvent(eventType, message, result);
}

FramelessDialog::FramelessDialog(QWidget *parent)
    : QDialog(parent)
    , FramelessBase(this)
{

}
void FramelessDialog::sltShowDrawShadow()
{
    showDrawShadow();
}

void FramelessDialog::sltHideDrawShadow()
{
    hideDrawShadow();
}

void FramelessDialog::paintEvent(QPaintEvent *e)
{
    FramelessBase::paintEvent(e);
}

bool FramelessDialog::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    return FramelessBase::nativeEvent(eventType, message, result);
}

