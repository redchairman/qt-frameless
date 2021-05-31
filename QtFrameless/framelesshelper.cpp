#include "framelesshelper.h"

/***** CursorPosCalculator *****/
CursorPosCalculator::CursorPosCalculator()
{
    reset();
}

void CursorPosCalculator::reset()
{
    m_bOnEdges = false;
    m_bOnLeftEdge = false;
    m_bOnRightEdge = false;
    m_bOnTopEdge = false;
    m_bOnBottomEdge = false;
    m_bOnTopLeftEdge = false;
    m_bOnBottomLeftEdge = false;
    m_bOnTopRightEdge  = false;
    m_bOnBottomRightEdge = false;
    m_nLeftOffset = 0;
    m_nRightOffset = 0;
    m_nTopOffset = 0;
    m_nBottomOffset = 0;
}

void CursorPosCalculator::recalculate(const QPoint &gMousePos, const QRect &frameRect, int nBorderWidth)
{
    qDebug() << "gMousePos" << gMousePos;
    qDebug() << "frameRect" << frameRect;

    int globalMouseX = gMousePos.x();
    int globalMouseY = gMousePos.y();

    int frameX = frameRect.x();
    int frameY = frameRect.y();

    int frameWidth = frameRect.width();
    int frameHeight = frameRect.height();

    m_bOnLeftEdge = (globalMouseX >= frameX &&
                     globalMouseX <= frameX + nBorderWidth );
    m_nLeftOffset = globalMouseX - frameX;

    m_bOnRightEdge = (globalMouseX >= frameX + frameWidth - nBorderWidth &&
                      globalMouseX <= frameX + frameWidth);
    m_nRightOffset = frameX + frameWidth - globalMouseX;

    m_bOnTopEdge = (globalMouseY >= frameY &&
                    globalMouseY <= frameY + nBorderWidth );
    m_nTopOffset = globalMouseY - frameY;

    m_bOnBottomEdge = (globalMouseY >= frameY + frameHeight - nBorderWidth &&
                       globalMouseY <= frameY + frameHeight);
    m_nBottomOffset = frameY + frameHeight - globalMouseY;

    m_bOnTopLeftEdge = m_bOnTopEdge && m_bOnLeftEdge;
    m_bOnBottomLeftEdge = m_bOnBottomEdge && m_bOnLeftEdge;
    m_bOnTopRightEdge = m_bOnTopEdge && m_bOnRightEdge;
    m_bOnBottomRightEdge = m_bOnBottomEdge && m_bOnRightEdge;

    m_bOnEdges = m_bOnLeftEdge || m_bOnRightEdge || m_bOnTopEdge || m_bOnBottomEdge;
}


/*****FramelessHelper*****/
FramelessHelper::FramelessHelper(QObject *parent)
    : QObject(parent)
{
    m_bWidgetMovable = true;
    m_bWidgetResizable = true;
    m_closeAction = CloseAction_Close;
    m_nBorderWidth = 4;
    m_nShadowWidth = 0;
    m_bLeftButtonPressed = false;
    m_bCursorShapeChanged = false;
    m_bLeftButtonTitlePressed = false;
    m_bMoveing = false;
    m_bMax = false;
    m_iconCtrl = nullptr;
    m_bNativeResize = false;

}

FramelessHelper::~FramelessHelper()
{
}

void FramelessHelper::setUIObject(QWidget *widget,
                                  QWidget *widgetTitle,
                                  QAbstractButton *buttonClose,
                                  QAbstractButton *buttonMax,
                                  QAbstractButton *buttonMin)
{
    m_pWidget = widget;
    m_pTitleWidget = widgetTitle;
    m_pMinButton = buttonMin;
    m_pMaxButton = buttonMax;
    m_pCloseButton = buttonClose;

    m_pWidget->setMouseTracking(true);
    m_pWidget->setAttribute(Qt::WA_Hover, true);
    m_pWidget->setWindowFlags(m_pWidget->windowFlags() |  Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);

//    if (m_bWidgetResizable)
//    {
//        m_pWidget->setWindowFlags(m_pWidget->windowFlags() | Qt::WindowMinMaxButtonsHint);
//    }
    m_pWidget->installEventFilter(this);
    //m_nTitleHeight = widgetTitle->height();

    if (m_pMinButton)
    {
        if (m_iconCtrl)
            m_iconCtrl->setIcon(FramelessIconCtrl::Button_MinSize, m_pMinButton);
        connect(m_pMinButton, SIGNAL(clicked()), this, SLOT(sltClickMin()));
    }

    if (m_pMaxButton)
    {
        if (m_iconCtrl)
            m_iconCtrl->setIcon(FramelessIconCtrl::Button_MaxSize, m_pMaxButton);
        connect(m_pMaxButton, SIGNAL(clicked()), this, SLOT(sltClickMaxRestore()));
    }

    if (m_pCloseButton)
    {
        if (m_iconCtrl)
            m_iconCtrl->setIcon(FramelessIconCtrl::Button_Close, m_pCloseButton);
        connect(m_pCloseButton, SIGNAL(clicked()), this, SLOT(sltClickClose()));
    }
}

void FramelessHelper::sltClickMin()
{
    m_pWidget->showMinimized();
}

void FramelessHelper::sltClickMaxRestore()
{
    handleMaxRestore();
}

void FramelessHelper::sltClickClose()
{
    switch ((CloseAction)m_closeAction)
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
            m_pWidget->close();
            break;
        }
    }
}

int FramelessHelper::getScreenIndex()
{
    int screenIndex = -1;
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

void FramelessHelper::setWidgetMovable(bool movable)
{
    m_bWidgetMovable = movable;
}

void FramelessHelper::setWidgetResizable(bool resizable)
{
    m_bWidgetResizable = resizable;
}

void FramelessHelper::setMinWhenClose(FramelessHelper::CloseAction action)
{
    m_closeAction = (int)action;
}

void FramelessHelper::setBorderWidth(int width)
{
    if (width > 0) {
        m_nBorderWidth = width;
    }
}

void FramelessHelper::setDrawShadowWidth(int width)
{
    m_nShadowWidth = width;
}

void FramelessHelper::setIconCtrl(FramelessIconCtrl *func)
{
    m_iconCtrl = func;
}

void FramelessHelper::setNativeResize(bool flag)
{
    m_bNativeResize = flag;
}

bool FramelessHelper::widgetMovable()
{
    return m_bWidgetMovable;
}

bool FramelessHelper::widgetResizable()
{
    return m_bWidgetResizable;
}


bool FramelessHelper::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseMove ||
        event->type() == QEvent::HoverMove ||
        event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonRelease ||
        event->type() == QEvent::MouseButtonDblClick ||
        event->type() == QEvent::Leave)
    {
        handleWidgetEvent(event);
        return true;
    }
    return QObject::eventFilter(obj, event);
}


void FramelessHelper::handleWidgetEvent(QEvent *event)
{
    switch (event->type()) {
    default:
        break;
    case QEvent::MouseButtonPress:
        handleMousePressEvent(static_cast<QMouseEvent *>(event));
        break;
    case QEvent::MouseButtonRelease:
        handleMouseReleaseEvent(static_cast<QMouseEvent *>(event));
        break;
    case QEvent::MouseMove:
        handleMouseMoveEvent(static_cast<QMouseEvent *>(event));
        break;
    case QEvent::Leave:
        handleLeaveEvent(static_cast<QMouseEvent *>(event));
        break;
    case QEvent::HoverMove:
        handleHoverMoveEvent(static_cast<QHoverEvent *>(event));
        break;
    case QEvent::MouseButtonDblClick:
        handleMouseDoubleClickEvent(static_cast<QMouseEvent *>(event));
        break;
    }
}

void FramelessHelper::updateCursorShape(const QPoint &gMousePos)
{
    if (m_pWidget->isFullScreen() || isMax()) {
        if (m_bCursorShapeChanged) {
            m_pWidget->unsetCursor();
        }
        return;
    }
    if (m_bMoveing)
        return;

    m_moveMousePos.recalculate(gMousePos, m_pWidget->frameGeometry(), m_nBorderWidth + m_nShadowWidth);

    if(m_moveMousePos.m_bOnTopLeftEdge || m_moveMousePos.m_bOnBottomRightEdge) {
        m_pWidget->setCursor( Qt::SizeFDiagCursor );
        m_bCursorShapeChanged = true;
    } else if(m_moveMousePos.m_bOnTopRightEdge || m_moveMousePos.m_bOnBottomLeftEdge) {
        m_pWidget->setCursor( Qt::SizeBDiagCursor );
        m_bCursorShapeChanged = true;
    } else if(m_moveMousePos.m_bOnLeftEdge || m_moveMousePos.m_bOnRightEdge) {
        m_pWidget->setCursor( Qt::SizeHorCursor );
        m_bCursorShapeChanged = true;
    } else if(m_moveMousePos.m_bOnTopEdge || m_moveMousePos.m_bOnBottomEdge) {
        m_pWidget->setCursor( Qt::SizeVerCursor );
        m_bCursorShapeChanged = true;
    } else {
        if (m_bCursorShapeChanged) {
            m_pWidget->unsetCursor();
            m_bCursorShapeChanged = false;
        }
    }
}

void FramelessHelper::resizeWidget(const QPoint &gMousePos)
{
    QRect origRect;

    origRect = m_pWidget->frameGeometry();

    int left = origRect.left();
    int top = origRect.top();
    int right = origRect.right();
    int bottom = origRect.bottom();
    origRect.getCoords(&left, &top, &right, &bottom);

    //int minWidth = m_pWidget->minimumWidth();
    //int minHeight = m_pWidget->minimumHeight();
    int minWidth = 40;
    int minHeight = 40;

    if (m_pressedMousePos.m_bOnTopLeftEdge) {
        left = gMousePos.x();
        top = gMousePos.y();

        left -= m_pressedMousePos.m_nLeftOffset;
        top -= m_pressedMousePos.m_nTopOffset;

        if (bottom - top < m_pWidget->minimumHeight())
            top = bottom - m_pWidget->minimumHeight();
        if (right - left < m_pWidget->minimumWidth())
            left = right - m_pWidget->minimumWidth();

    } else if (m_pressedMousePos.m_bOnBottomLeftEdge) {
        left = gMousePos.x();
        bottom = gMousePos.y();

        left -= m_pressedMousePos.m_nLeftOffset;
        bottom += m_pressedMousePos.m_nBottomOffset;

        if (bottom - top < m_pWidget->minimumHeight())
            bottom = top + m_pWidget->minimumHeight();
        if (right - left < m_pWidget->minimumWidth())
            left = right - m_pWidget->minimumWidth();
    } else if (m_pressedMousePos.m_bOnTopRightEdge) {
        right = gMousePos.x();
        top = gMousePos.y();
        right += m_pressedMousePos.m_nRightOffset;
        top -= m_pressedMousePos.m_nTopOffset;

        if (bottom - top < m_pWidget->minimumHeight())
            top = bottom - m_pWidget->minimumHeight();
        if (right - left < m_pWidget->minimumWidth())
            right = left + m_pWidget->minimumWidth();
    } else if (m_pressedMousePos.m_bOnBottomRightEdge) {
        right = gMousePos.x();
        bottom = gMousePos.y();
        right += m_pressedMousePos.m_nRightOffset;
        bottom += m_pressedMousePos.m_nBottomOffset;

        if (right - left < m_pWidget->minimumWidth())
            right = left + m_pWidget->minimumWidth();
        if (bottom - top < m_pWidget->minimumHeight())
            bottom = top + m_pWidget->minimumHeight();
    } else if (m_pressedMousePos.m_bOnLeftEdge) {
        left = gMousePos.x();
        left -= m_pressedMousePos.m_nLeftOffset;

        if (right - left < m_pWidget->minimumWidth())
            left = right - m_pWidget->minimumWidth();
    } else if (m_pressedMousePos.m_bOnRightEdge) {
        right = gMousePos.x();
        right += m_pressedMousePos.m_nRightOffset;

        if (right - left < m_pWidget->minimumWidth())
            right = left + m_pWidget->minimumWidth();
    } else if (m_pressedMousePos.m_bOnTopEdge) {
        top = gMousePos.y();
        top -= m_pressedMousePos.m_nTopOffset;

        if (bottom - top < m_pWidget->minimumHeight())
            top = bottom - m_pWidget->minimumHeight();
    } else if (m_pressedMousePos.m_bOnBottomEdge) {
        bottom = gMousePos.y();
        bottom += m_pressedMousePos.m_nBottomOffset;

        if (bottom - top < m_pWidget->minimumHeight())
            bottom = top + m_pWidget->minimumHeight();
    }

    QRect newRect(QPoint(left, top), QPoint(right, bottom));

    if (newRect.isValid()) {
        if (minWidth > newRect.width()) {
            if (left != origRect.left()) {
                newRect.setLeft(origRect.left());
            } else {
                newRect.setRight(origRect.right());
            }
        }
        if (minHeight > newRect.height()) {
            if (top != origRect.top()) {
                newRect.setTop(origRect.top());
            } else {
                newRect.setBottom(origRect.bottom());
            }
        }
        m_pWidget->setGeometry(newRect);
    }
}

void FramelessHelper::moveWidget(const QPoint &gMousePos)
{
    m_pWidget->move(gMousePos - m_ptDragPos);
}

void FramelessHelper::handleMousePressEvent(QMouseEvent *event)
{
    if (m_pWidget->isFullScreen())
    {
        return;
    }
    if (event->button() == Qt::LeftButton) {
        //QRect rect = m_pTitleWidget->rect();
        //if (rect.contains(event->pos()))
        {
            m_bLeftButtonPressed = true;
            //m_bLeftButtonTitlePressed = event->pos().y() < m_nTitleHeight;
            m_bLeftButtonTitlePressed = m_pTitleWidget->rect().contains(event->pos());

            QRect frameRect = m_pWidget->frameGeometry();
            QRect moveRect(frameRect.x(), frameRect.y(), frameRect.width(), 30);
            m_pressedMousePos.recalculate(event->globalPos(), frameRect, m_nBorderWidth + m_nShadowWidth);

            m_ptDragPos = event->globalPos() - frameRect.topLeft();
        }
    }
}

void FramelessHelper::handleMouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_bWidgetResizable == false)
    {
        return;
    }
    if (m_pMaxButton == nullptr || m_pMinButton == nullptr)
    {
        return;
    }
    if (m_pWidget->isFullScreen())
    {
        return;
    }
    if (event->buttons() & Qt::LeftButton)
    {
        QRect rect = m_pTitleWidget->rect();
        if (rect.contains(event->pos()))
        {
            handleMaxRestore();
        }
    }
    event->accept();
}

void FramelessHelper::handleMouseReleaseEvent(QMouseEvent *event)
{
    if (m_pWidget->isFullScreen())
    {
        return;
    }
    if (event->button() == Qt::LeftButton) {
        m_bLeftButtonPressed = false;
        m_bLeftButtonTitlePressed = false;
        m_bMoveing = false;
        m_pressedMousePos.reset();
    }
}

void FramelessHelper::handleMouseMoveEvent(QMouseEvent *event)
{
    if (m_pWidget->isFullScreen())
    {
        return;
    }

    if (m_bLeftButtonPressed) {
        if (isMax())
        {
            if (m_bLeftButtonTitlePressed)
            {
                if (!m_rcNormal.isNull())
                {
                    float fx = (float)event->pos().x() / (float)m_pWidget->rect().width();
                    //屏幕大小
                    int old_x = m_rcNormal.width() * fx + m_rcNormal.left();
                    int old_y = m_rcNormal.top() + event->pos().y();

                    QPoint pt_new(m_rcNormal.left() + event->globalPos().x() - old_x, m_rcNormal.top() + event->globalPos().y() - old_y);
                    m_rcNormal.moveTopLeft(pt_new);
                    //m_pWidget->setGeometry(m_rcNormal);
                    handleMaxRestore();
                    m_pWidget->move(pt_new);
                }
                else{
                    handleMaxRestore();
                }

                m_ptDragPos = event->globalPos() - m_pWidget->frameGeometry().topLeft();
                m_bMoveing = true;
            }
        }else
        {
            if (m_bWidgetResizable && m_pressedMousePos.m_bOnEdges && m_bMoveing == false && m_bNativeResize == false) {
                resizeWidget(event->globalPos());

            } else if (m_bWidgetMovable && m_bLeftButtonTitlePressed) {
                moveWidget(event->globalPos());
                m_bMoveing = true;
            }
        }

    } else if (m_bWidgetResizable && m_bNativeResize == false) {
            updateCursorShape(event->globalPos());
    }
}

void FramelessHelper::handleLeaveEvent(QEvent *event)
{
    Q_UNUSED(event)
    if (!m_bLeftButtonPressed) {
        m_pWidget->unsetCursor();
    }
}

void FramelessHelper::handleHoverMoveEvent(QHoverEvent *event)
{
    if (m_bWidgetResizable && m_bNativeResize == false) {
        updateCursorShape(m_pWidget->mapToGlobal(event->pos()));
    }
}

bool FramelessHelper::isMax()
{
    return (m_pWidget->isMaximized() || m_bMax);
}

void FramelessHelper::handleMin()
{
    m_pWidget->showMinimized();
}

void FramelessHelper::handleMaxRestore()
{
#ifdef Q_OS_WIN32
    if (m_pWidget->isMaximized())
    {
        emit sigShowDrawShadow();
        m_pWidget->showNormal();
        m_bMax = false;
    }
    else
    {
        emit sigHideDrawShadow();
        m_rcNormal = m_pWidget->geometry();
        int screenIndex = getScreenIndex();
        if (1)//screenIndex == 0)
        {
            m_pWidget->showMaximized();
            m_pWidget->setWindowState(Qt::WindowMaximized);
        }else{
            m_pWidget->setGeometry(qApp->desktop()->availableGeometry(screenIndex));    
        }
        m_bMax = true;
    }
#else
    if(isMax())
    {
        emit sigShowDrawShadow();
        if (m_rcNormal.isNull())
            m_pWidget->showNormal();
        else
            m_pWidget->setGeometry(m_rcNormal);
        m_bMax = false;
    }else
    {
        emit sigHideDrawShadow();
        m_rcNormal = m_pWidget->geometry();
        int screenIndex = getScreenIndex();
        QRect rc = qApp->desktop()->availableGeometry(screenIndex);
        m_pWidget->setGeometry(rc);
        m_bMax = true;
    }

#endif
    updateMaxIcon();
    return;
}

void FramelessHelper::updateMaxIcon()
{
    if (m_pMaxButton && m_iconCtrl)
    {
        m_iconCtrl->setIcon(isMax() ? FramelessIconCtrl::Button_Restore : FramelessIconCtrl::Button_MaxSize, m_pMaxButton);
    }
}

bool FramelessHelper::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef Q_OS_WIN
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)    ((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)    ((int)(short)HIWORD(lParam))
#endif

    if (eventType != "windows_generic_MSG")
        return false;
    if (m_bNativeResize == false)
        return false;

    MSG* msg = static_cast<MSG*>(message);
    QWidget* widget = QWidget::find(reinterpret_cast<WId>(msg->hwnd));
    if (!widget)
        return false;

    switch (msg->message)
    {
    case WM_NCCALCSIZE:
    {
        *result = 0;
        return true;
    }
    case WM_NCHITTEST: {
       int x = GET_X_LPARAM(msg->lParam);
       int y = GET_Y_LPARAM(msg->lParam);

       QPoint pt = m_pWidget->mapFromGlobal(QPoint(x, y));
       *result = calculateBorder(pt);
       if (*result == HTCLIENT) {
           QRect rcTitle = m_pTitleWidget->rect();
           rcTitle = QRect(m_pTitleWidget->mapTo(m_pWidget, rcTitle.topLeft()), rcTitle.size());

           QVector<QRect> btnsRect;
           QList<QAbstractButton *> btns = m_pTitleWidget->findChildren<QAbstractButton *>();
           for (int i=0; i<btns.size(); i++) {
               QRect rc = btns[i]->rect();
               rc = QRect(btns[i]->mapTo(m_pWidget, rc.topLeft()), rc.size());
               btnsRect.push_back(rc);
           }

           if (rcTitle.contains(pt))
            {
                bool containBtn = false;
                for (int i=0; i<btnsRect.size(); i++) {
                    if (btnsRect[i].contains(pt))
                    {
                        containBtn = true;
                        break;
                    }
                }
                if (containBtn == false)
                {
                    *result = HTCAPTION;
                }
            }
       }
       return true;
    }
    case WM_GETMINMAXINFO: {
        if (::IsZoomed(msg->hwnd)) {
            m_bMax = true;
            RECT frame = { 0, 0, 0, 0 };
            AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
            frame.left = abs(frame.left);
            frame.top = abs(frame.bottom);
            m_pWidget->setContentsMargins(frame.left, frame.top, frame.right, frame.bottom);
        }
        else {
            m_pWidget->setContentsMargins(0, 0, 0, 0);
            m_bMax = false;
        }
        updateMaxIcon();
        *result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
        return true;
    }
    break;

    default:
        return false;
    }

#endif
    return false;
}

#ifdef Q_OS_WIN

LRESULT FramelessHelper::calculateBorder(const QPoint &pt)
{
    if (::IsZoomed((HWND)m_pWidget->winId()) || isMax()) {
        return HTCLIENT;
    }
    int borderSize = 4;
    int cx = m_pWidget->size().width();
    int cy = m_pWidget->size().height();

    QRect rectTopLeft(0, 0, borderSize, borderSize);
    if (rectTopLeft.contains(pt)) {
        return HTTOPLEFT;
    }

    QRect rectLeft(0, borderSize, borderSize, cy - borderSize * 2);
    if (rectLeft.contains(pt)) {
        return HTLEFT;
    }

    QRect rectTopRight(cx - borderSize, 0, borderSize, borderSize);
    if (rectTopRight.contains(pt)) {
        return HTTOPRIGHT;
    }

    QRect rectRight(cx - borderSize, borderSize, borderSize, cy - borderSize * 2);
    if (rectRight.contains(pt)) {
        return HTRIGHT;
    }

    QRect rectTop(borderSize, 0, cx - borderSize * 2, borderSize);
    if (rectTop.contains(pt)) {
        return HTTOP;
    }

    QRect rectBottomLeft(0, cy - borderSize, borderSize, borderSize);
    if (rectBottomLeft.contains(pt)) {
        return HTBOTTOMLEFT;
    }

    QRect rectBottomRight(cx - borderSize, cy - borderSize, borderSize, borderSize);
    if (rectBottomRight.contains(pt)) {
        return HTBOTTOMRIGHT;
    }

    QRect rectBottom(borderSize, cy - borderSize, cx - borderSize * 2, borderSize);
    if (rectBottom.contains(pt)) {
        return HTBOTTOM;
    }

    return HTCLIENT;
}
#endif



