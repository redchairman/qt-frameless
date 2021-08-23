#include "framelesstitlebar.h"
#include "framelesshelper.h"
FramelessTitlebar::FramelessTitlebar(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<FramelessTitlebar::SizeType>("FramelessTitlebar::SizeType");
}

void FramelessTitlebar::setWidgets(QWidget *mainWidget,
                                   QWidget *titleBar,
                                   QAbstractButton *closeButton,
                                   QAbstractButton *maxButton,
                                   QAbstractButton *minButton)
{
    m_mainWidget = mainWidget;
    m_titleBar = titleBar;
    m_closeButton = closeButton;
    m_maxButton = maxButton;
    m_minButton = minButton;

    if (m_minButton)
    {
        if (m_iconCtrl)
            m_iconCtrl->setIcon(FramelessIconCtrl::Button_MinSize, m_minButton);
        connect(m_minButton, SIGNAL(clicked()), this, SLOT(sltClickMin()));
    }

    if (m_maxButton)
    {
        if (m_iconCtrl)
            m_iconCtrl->setIcon(FramelessIconCtrl::Button_MaxSize, m_maxButton);
        connect(m_maxButton, SIGNAL(clicked()), this, SLOT(sltClickMaxRestore()));
    }

    if (m_closeButton)
    {
        if (m_iconCtrl)
            m_iconCtrl->setIcon(FramelessIconCtrl::Button_Close, m_closeButton);
        connect(m_closeButton, SIGNAL(clicked()), this, SLOT(sltClickClose()));
    }
}

void FramelessTitlebar::setMinWhenClose(FramelessTitlebar::CloseAction action)
{
    m_closeAction = action;
}

void FramelessTitlebar::sltClickMin()
{
    m_mainWidget->showMinimized();
}

void FramelessTitlebar::sltClickMaxRestore()
{
    handleMaxRestore();
}

void FramelessTitlebar::sltClickClose()
{
    switch (m_closeAction)
    {
        case CloseAction_Null:
        {
            break;
        }
        case CloseAction_Min:
        {
            sltClickMin();
            break;
        }
        case CloseAction_Close:
        {
            m_mainWidget->close();
            break;
        }
    }
}

bool FramelessTitlebar::isMax()
{
    return (m_mainWidget->isMaximized() || m_isMax);
}

void FramelessTitlebar::handleMin()
{
    m_mainWidget->showMinimized();
}


int FramelessTitlebar::getScreenIndex()
{
    int screenIndex = 0;
    int screenCount = qApp->desktop()->screenCount();
    if(screenCount > 1)
    {
        QPoint pos = QCursor::pos();
        for(int i=0;i < screenCount;++i)
        {
            if(qApp->desktop()->screenGeometry(i).contains(pos))
            {
                screenIndex = i;
                break;
            }
        }
    }
    return screenIndex;
}

void FramelessTitlebar::handleMaxRestore()
{
#ifdef Q_OS_WIN32
    if (m_mainWidget->isMaximized())
    {
        emit sigShowDrawShadow();
        m_mainWidget->showNormal();
        m_isMax = false;
    }
    else
    {
        emit sigHideDrawShadow();
        m_rcNormal = m_mainWidget->geometry();
        int screenIndex = getScreenIndex();
        if (1)//screenIndex == 0)
        {
            m_mainWidget->showMaximized();
            m_mainWidget->setWindowState(Qt::WindowMaximized);
        }else{
            m_mainWidget->setGeometry(qApp->desktop()->availableGeometry(screenIndex));
        }
        m_isMax = true;
    }
#else
    if(isMax())
    {
        emit sigShowDrawShadow();
        if (m_rcNormal.isNull())
            m_mainWidget->showNormal();
        else
            m_mainWidget->setGeometry(m_rcNormal);
        m_isMax = false;
    }else
    {
        emit sigHideDrawShadow();
        m_rcNormal = m_mainWidget->geometry();
        int screenIndex = getScreenIndex();
        QRect rc = qApp->desktop()->availableGeometry(screenIndex);
        m_mainWidget->setGeometry(rc);
        m_isMax = true;
    }

#endif
    updateMaxIcon();
    return;
}

void FramelessTitlebar::updateMaxIcon()
{
    if (m_maxButton && m_iconCtrl)
    {
        m_iconCtrl->setIcon(isMax() ? FramelessIconCtrl::Button_Restore : FramelessIconCtrl::Button_MaxSize, m_pMaxButton);
    }
}

