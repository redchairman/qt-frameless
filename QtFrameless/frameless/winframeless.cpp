#include "winframeless.h"

WinFrameless::WinFrameless(QObject *parent) : QObject(parent)
{

}


bool WinFrameless::nativeEvent(const QByteArray &eventType, void *message, long *result)
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
    if (m_bWidgetResizable == false)
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

LRESULT WinFrameless::calculateBorder(const QPoint &pt)
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
