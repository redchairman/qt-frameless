#include "framelesshelper.h"
#include "qdatetime.h"
#include "qevent.h"
#include "qdebug.h"
#include "shadowhelper.h"
#include <QPainter>
#ifdef Q_OS_WIN
#include <windows.h>
#include <dwmapi.h>
#pragma comment (lib,"user32.lib")
#pragma comment (lib,"dwmapi.lib")
#endif

#define TIMEMS qPrintable(QTime::currentTime().toString("HH:mm:ss zzz"))

FramelessHelper::FramelessHelper(QWidget* w, QObject *parent)
    : QObject(parent)
    , widget(w)
{
    padding = 8;
    moveEnable = true;
    resizeEnable = true;
    nativeResize = false;
#ifdef Q_OS_WIN
    nativeResize = true;
#endif
    shadowEnable = true;
    drawShadow = 0;

    mousePressed = false;
    mousePoint = QPoint(0, 0);
    mouseRect = QRect(0, 0, 0, 0);

    for (int i = 0; i < 8; ++i) {
        pressedArea << false;
        pressedRect << QRect(0, 0, 0, 0);
    }

    isMin = false;
    flags = widget->windowFlags();
    titleBar = 0;

    //设置背景透明 官方在5.3以后才彻底修复 WA_TranslucentBackground+FramelessWindowHint 并存不绘制的BUG
#if (QT_VERSION >= QT_VERSION_CHECK(5,3,0))
    //this->setAttribute(Qt::WA_TranslucentBackground);
#endif
    //设置无边框属性
    widget->setWindowFlags(flags | Qt::FramelessWindowHint);
    //安装事件过滤器识别拖动
    widget->installEventFilter(this);

    //设置属性产生win窗体效果,移动到边缘半屏或者最大化等
    //设置以后会产生标题栏需要在下面拦截消息重新去掉
#ifdef Q_OS_WIN
    if (nativeResize){
        HWND hwnd = (HWND)widget->winId();
        DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
        if (resizeEnable) {
            ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
        }else{
            ::SetWindowLong(hwnd, GWL_STYLE, style | WS_THICKFRAME | WS_CAPTION);
        }
        if (shadowEnable)
        {
            //we better left 1 piexl width of border untouch, so OS can draw nice shadow around it
            const MARGINS shadow = { 1, 1, 1, 1 };
            DwmExtendFrameIntoClientArea(HWND(widget->winId()), &shadow);
        }
    }
#endif

    if (nativeResize == false){
        widget->setAttribute(Qt::WA_Hover, true);
        if (shadowEnable){
            drawShadow = new DrawShadowHelper(widget, 10, this);
            setPadding(padding + 5);
        }
    }
}

void FramelessHelper::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(widget);
    painter.save();
    if (drawShadow)
        drawShadow->paint(&painter);
    painter.restore();
}

void FramelessHelper::doShowEvent(QEvent *event)
{
    //解决有时候窗体重新显示的时候假死不刷新的BUG
    widget->setAttribute(Qt::WA_Mapped);
    //QDialog::showEvent(event);
}

void FramelessHelper::doWindowStateChange(QEvent *event)
{
    //非最大化才能移动和拖动大小
    if (widget->windowState() == Qt::WindowNoState) {
        moveEnable = true;
        resizeEnable = true;
    } else {
        moveEnable = false;
        resizeEnable = false;
    }

    if (drawShadow)
    {
        if (widget->windowState() == Qt::WindowMaximized || widget->windowState() == Qt::WindowFullScreen)
        {
            drawShadow->hide();
        }else{
            drawShadow->show();
        }
    }

    //发出最大化最小化等改变事件,以便界面上更改对应的信息比如右上角图标和文字
    emit windowStateChange(!moveEnable);

    //解决mac系统上无边框最小化失效的BUG
#ifdef Q_OS_MACOS
    if (widget->windowState() & Qt::WindowMinimized) {
        isMin = true;
    } else {
        if (isMin) {
            //设置无边框属性
            widget->setWindowFlags(flags | Qt::FramelessWindowHint);
            widget->setVisible(true);
            isMin = false;
        }
    }
#endif
}

void FramelessHelper::doResizeEvent(QEvent *event)
{
    //非win系统的无边框拉伸,win系统上已经采用了nativeEvent来处理拉伸
    //为何不统一用计算的方式因为在win上用这个方式往左拉伸会发抖妹的
#ifdef Q_OS_WIN
    if (nativeResize)
        return;
#endif
    if (event->type() == QEvent::Resize) {
        //重新计算八个描点的区域,描点区域的作用还有就是计算鼠标坐标是否在某一个区域内
        int width = widget->width();
        int height = widget->height();

        //左侧描点区域
        pressedRect[0] = QRect(0, padding, padding, height - padding * 2);
        //右侧描点区域
        pressedRect[1] = QRect(width - padding, padding, padding, height - padding * 2);
        //上侧描点区域
        pressedRect[2] = QRect(padding, 0, width - padding * 2, padding);
        //下侧描点区域
        pressedRect[3] = QRect(padding, height - padding, width - padding * 2, padding);

        //左上角描点区域
        pressedRect[4] = QRect(0, 0, padding, padding);
        //右上角描点区域
        pressedRect[5] = QRect(width - padding, 0, padding, padding);
        //左下角描点区域
        pressedRect[6] = QRect(0, height - padding, padding, padding);
        //右下角描点区域
        pressedRect[7] = QRect(width - padding, height - padding, padding, padding);
    } else if (event->type() == QEvent::HoverMove) {
        //设置对应鼠标形状,这个必须放在这里而不是下面,因为可以在鼠标没有按下的时候识别
        QHoverEvent *hoverEvent = (QHoverEvent *)event;
        QPoint point = hoverEvent->pos();
        if (resizeEnable) {
            if (pressedRect.at(0).contains(point)) {
                widget->setCursor(Qt::SizeHorCursor);
            } else if (pressedRect.at(1).contains(point)) {
                widget->setCursor(Qt::SizeHorCursor);
            } else if (pressedRect.at(2).contains(point)) {
                widget->setCursor(Qt::SizeVerCursor);
            } else if (pressedRect.at(3).contains(point)) {
                widget->setCursor(Qt::SizeVerCursor);
            } else if (pressedRect.at(4).contains(point)) {
                widget->setCursor(Qt::SizeFDiagCursor);
            } else if (pressedRect.at(5).contains(point)) {
                widget->setCursor(Qt::SizeBDiagCursor);
            } else if (pressedRect.at(6).contains(point)) {
                widget->setCursor(Qt::SizeBDiagCursor);
            } else if (pressedRect.at(7).contains(point)) {
                widget->setCursor(Qt::SizeFDiagCursor);
            } else {
                widget->setCursor(Qt::ArrowCursor);
            }
        }

        //根据当前鼠标位置,计算XY轴移动了多少
        int offsetX = point.x() - mousePoint.x();
        int offsetY = point.y() - mousePoint.y();

        //根据按下处的位置判断是否是移动控件还是拉伸控件
        if (moveEnable && mousePressed) {
            widget->move(widget->x() + offsetX, widget->y() + offsetY);
        }

        //最大化时，点击标题栏或者顶部锚点，移动鼠标，则还原窗口
        if ((pressedArea[2] || mousePressed) &&
            widget->windowState() == Qt::WindowMaximized)
        {
            QRect normalRect = widget->normalGeometry();
            float fx = (float)point.x() / (float)widget->rect().width();
            int offsetY = point.y();
            emit titleDblClick();

            int x1 = fx * (float)normalRect.width();
            x1 = point.x() - x1;
            int y1 = point.y() - offsetY;
            //点击顶部锚点时候，必须将y坐标向下移动一点，这样窗口还原后，鼠标不会继续触发顶部锚点
            if (pressedArea[2]){
                y1 -= 8;
                if (drawShadow)
                    y1 -= drawShadow->shadowSize();
            }
            widget->move(x1, y1);
            //重新计算移动起点和大小
            mousePoint = widget->mapFromGlobal(point);
            mouseRect = widget->geometry();
            mousePressed = true;
            pressedArea[2] = false;
        }

        if (resizeEnable) {
            int rectX = mouseRect.x();
            int rectY = mouseRect.y();
            int rectW = mouseRect.width();
            int rectH = mouseRect.height();

            if (pressedArea.at(0)) {
                int resizeW = widget->width() - offsetX;
                if (widget->minimumWidth() <= resizeW) {
                    widget->setGeometry(widget->x() + offsetX, rectY, resizeW, rectH);
                }
            } else if (pressedArea.at(1)) {
                widget->setGeometry(rectX, rectY, rectW + offsetX, rectH);
            } else if (pressedArea.at(2)) {
                int resizeH = widget->height() - offsetY;
                if (widget->minimumHeight() <= resizeH) {
                    widget->setGeometry(rectX, widget->y() + offsetY, rectW, resizeH);
                }
            } else if (pressedArea.at(3)) {
                widget->setGeometry(rectX, rectY, rectW, rectH + offsetY);
            } else if (pressedArea.at(4)) {
                int resizeW = widget->width() - offsetX;
                int resizeH = widget->height() - offsetY;
                if (widget->minimumWidth() <= resizeW) {
                    widget->setGeometry(widget->x() + offsetX, widget->y(), resizeW, resizeH);
                }
                if (widget->minimumHeight() <= resizeH) {
                    widget->setGeometry(widget->x(), widget->y() + offsetY, resizeW, resizeH);
                }
            } else if (pressedArea.at(5)) {
                int resizeW = rectW + offsetX;
                int resizeH = widget->height() - offsetY;
                if (widget->minimumHeight() <= resizeH) {
                    widget->setGeometry(widget->x(), widget->y() + offsetY, resizeW, resizeH);
                }
            } else if (pressedArea.at(6)) {
                int resizeW = widget->width() - offsetX;
                int resizeH = rectH + offsetY;
                if (widget->minimumWidth() <= resizeW) {
                    widget->setGeometry(widget->x() + offsetX, widget->y(), resizeW, resizeH);
                }
                if (widget->minimumHeight() <= resizeH) {
                    widget->setGeometry(widget->x(), widget->y(), resizeW, resizeH);
                }
            } else if (pressedArea.at(7)) {
                int resizeW = rectW + offsetX;
                int resizeH = rectH + offsetY;
                widget->setGeometry(widget->x(), widget->y(), resizeW, resizeH);
            }
        }
    } else if (event->type() == QEvent::MouseButtonPress) {
        //记住鼠标按下的坐标+窗体区域
        QMouseEvent *mouseEvent = (QMouseEvent *)event;
        mousePoint = mouseEvent->pos();
        mouseRect = widget->geometry();

        //判断按下的手柄的区域位置
        if (pressedRect.at(0).contains(mousePoint)) {
            pressedArea[0] = true;
        } else if (pressedRect.at(1).contains(mousePoint)) {
            pressedArea[1] = true;
        } else if (pressedRect.at(2).contains(mousePoint)) {
            pressedArea[2] = true;
        } else if (pressedRect.at(3).contains(mousePoint)) {
            pressedArea[3] = true;
        } else if (pressedRect.at(4).contains(mousePoint)) {
            pressedArea[4] = true;
        } else if (pressedRect.at(5).contains(mousePoint)) {
            pressedArea[5] = true;
        } else if (pressedRect.at(6).contains(mousePoint)) {
            pressedArea[6] = true;
        } else if (pressedRect.at(7).contains(mousePoint)) {
            pressedArea[7] = true;
        } else {
            //点击标题栏，才可以移动窗体，如果希望点击窗体即可移动，则去掉下面判断条件
            QRect titleRect = titleBar->rect();
            titleRect = QRect(titleBar->mapTo(widget, titleRect.topLeft()), titleRect.size());
            if (titleRect.contains(mousePoint)) {
                mousePressed = true;
            }
        }
    } else if (event->type() == QEvent::MouseMove) {
        //改成用HoverMove识别
    } else if (event->type() == QEvent::MouseButtonRelease) {
        //恢复所有
        widget->setCursor(Qt::ArrowCursor);
        mousePressed = false;
        for (int i = 0; i < 8; ++i) {
            pressedArea[i] = false;
        }
    }

}

bool FramelessHelper::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == widget) {
        qDebug() << event;
        if (event->type() == QEvent::WindowStateChange) {
            doWindowStateChange(event);
        } else if (event->type() == QEvent::Show) {
            doShowEvent(event);
        } else if (event->type() == QEvent::WindowActivate) {
            //qDebug() << event;
            //widget->activateWindow();
        } else if (event->type() == QEvent::WindowDeactivate) {
            qDebug() << event;
        } else {
            doResizeEvent(event);
        }
    } else if (watched == titleBar) {
        //双击标题栏发出双击信号给主界面
        //下面的 *result = HTCAPTION; 标志位也会自动识别双击标题栏
#ifdef Q_OS_WIN
        //qDebug() << "title bar hwnd: " << titleBar->winId();
        if (nativeResize == false)
#endif
        {
            if (event->type() == QEvent::MouseButtonDblClick) {
                emit titleDblClick();
            } else if (event->type() == QEvent::NonClientAreaMouseButtonDblClick) {
                emit titleDblClick();
            }
        }
    }

    return false;
//    return QDialog::eventFilter(watched, event);
}

#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
bool FramelessHelper::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
#else
bool FramelessHelper::nativeEvent(const QByteArray &eventType, void *message, long *result)
#endif
{
    if (eventType == "windows_generic_MSG") {
#ifdef Q_OS_WIN
        if (nativeResize == false) {
            return false;
        }
        MSG *msg = static_cast<MSG *>(message);
        //qDebug() << TIMEMS << "nativeEvent" << msg->wParam << msg->message;

        //不同的消息类型和参数进行不同的处理
        if (msg->message == WM_NCCALCSIZE) {
            *result = 0;
            return true;
        } /*else if (msg->message == WM_NCACTIVATE) {
            *result = 0;
            return true;
        } */else if (msg->message == WM_NCHITTEST) {
            //计算鼠标对应的屏幕坐标
            long x = LOWORD(msg->lParam);
            long y = HIWORD(msg->lParam);
            QPoint pos = widget->mapFromGlobal(QPoint(x, y));
            qDebug() << "WM_NCHITTEST " << pos;
            //判断当前鼠标位置在哪个区域
            bool left = pos.x() < padding;
            bool right = pos.x() > widget->width() - padding;
            bool top = pos.y() < padding;
            bool bottom = pos.y() > widget->height() - padding;

            //鼠标移动到四个角,这个消息是当鼠标移动或者有鼠标键按下时候发出的
            *result = 0;
            if (resizeEnable) {
                if (left && top) {
                    *result = HTTOPLEFT;
                } else if (left && bottom) {
                    *result = HTBOTTOMLEFT;
                } else if (right && top) {
                    *result = HTTOPRIGHT;
                } else if (right && bottom) {
                    *result = HTBOTTOMRIGHT;
                } else if (left) {
                    *result = HTLEFT;
                } else if (right) {
                    *result = HTRIGHT;
                } else if (top) {
                    *result = HTTOP;
                } else if (bottom) {
                    *result = HTBOTTOM;
                }
            }

            //先处理掉拉伸
            if (0 != *result) {
                return true;
            }

            //识别标题栏拖动产生半屏全屏效果
            if (titleBar != 0 && titleBar->rect().contains(pos)) {
                QWidget *child = titleBar->childAt(pos);
                if (!child) {
                    *result = HTCAPTION;
                    return true;
                }
            }

            *result = HTCLIENT;
            return true;

        } else if (msg->message == WM_GETMINMAXINFO) {
            if (::IsZoomed(msg->hwnd)) {
                RECT frame = { 0, 0, 0, 0 };
                AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
                frame.left = abs(frame.left);
                frame.top = abs(frame.bottom);
                widget->setContentsMargins(frame.left, frame.top, frame.right, frame.bottom);
            }
            else {
                widget->setContentsMargins(0, 0, 0, 0);
            }
            return false;
//            *result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
//            return true;
        }else if (msg->wParam == PBT_APMSUSPEND && msg->message == WM_POWERBROADCAST) {
            //系统休眠的时候自动最小化可以规避程序可能出现的问题
            widget->showMinimized();
        } else if (msg->wParam == PBT_APMRESUMEAUTOMATIC) {
            //休眠唤醒后自动打开
            //this->showNormal();
        }
#endif
    } else if (eventType == "NSEvent") {
#ifdef Q_OS_MACOS
#endif
    } else if (eventType == "xcb_generic_event_t") {
#ifdef Q_OS_LINUX
#endif
    }
    return false;
}

//#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
//#ifdef Q_OS_WIN
//bool FramelessHelper::winEvent(MSG *message, long *result)
//{
//    return nativeEvent("windows_generic_MSG", message, result);
//}
//#endif
//#endif

void FramelessHelper::setPadding(int padding)
{
    this->padding = padding;
}

void FramelessHelper::setMoveEnable(bool moveEnable)
{
    this->moveEnable = moveEnable;
}

void FramelessHelper::setResizeEnable(bool resizeEnable)
{
    this->resizeEnable = resizeEnable;
}

void FramelessHelper::setTitleBar(QWidget *titleBar)
{
    this->titleBar = titleBar;
    this->titleBar->installEventFilter(this);
}


