#ifndef FRAMELESSHELPER_H
#define FRAMELESSHELPER_H

#include <QtGui>
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))
#include <QtWidgets>
#endif

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


class CursorPosCalculator
{
public:
    explicit CursorPosCalculator();
    void reset();
    void recalculate(const QPoint &globalMousePos, const QRect &frameRect, int nBorderWidth);

public:
bool m_bOnEdges              :
    true;
bool m_bOnLeftEdge           :
    true;
bool m_bOnRightEdge          :
    true;
bool m_bOnTopEdge            :
    true;
bool m_bOnBottomEdge         :
    true;
bool m_bOnTopLeftEdge        :
    true;
bool m_bOnBottomLeftEdge     :
    true;
bool m_bOnTopRightEdge       :
    true;
bool m_bOnBottomRightEdge    :
    true;

    int m_nLeftOffset;
    int m_nRightOffset;
    int m_nTopOffset;
    int m_nBottomOffset;

};

class FramelessIconCtrl
{
public:
    enum ButtonType
    {
        Button_MinSize = 0,
        Button_MaxSize,
        Button_Restore,
        Button_Close
    };
    virtual void setIcon(ButtonType type, QAbstractButton* button) = 0;
};

class FramelessHelper : public QObject
{
    Q_OBJECT

public:

    enum CloseAction
    {
        CloseAction_Null = 0,
        CloseAction_Min,
        CloseAction_Close
    };

    explicit FramelessHelper(QObject *parent = 0);
    ~FramelessHelper();
    // 设置窗体移动
    void setWidgetMovable(bool movable);
    // 设置窗体缩放
    void setWidgetResizable(bool resizable);
    // 点击关闭按钮时
    void setMinWhenClose(CloseAction action);

    bool widgetResizable();
    bool widgetMovable();
    uint borderWidth();
    uint titleHeight();
    bool minWhenClose();

    //设置标题栏、按钮等，用于实现窗口最大化、最小化等
    void setUIObject(QWidget *widget,
                     QWidget *widgetTitle,
                     QAbstractButton *buttonClose,
                     QAbstractButton *buttonMax = nullptr,
                     QAbstractButton *buttonMin = nullptr);
    void setBorderWidth(int width);
    void setDrawShadowWidth(int width);
    void setIconCtrl(FramelessIconCtrl *func);
    void setNativeResize(bool flag);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#ifdef Q_OS_WIN
    LRESULT calculateBorder(const QPoint &pt);
#endif
signals:
    void sigShowDrawShadow();
    void sigHideDrawShadow();

public slots:
    void sltClickMaxRestore();
    void sltClickMin();

    void sltClickClose();
protected:
    // 事件过滤，进行移动、缩放等
    virtual bool eventFilter(QObject *obj, QEvent *event);
    int getScreenIndex();
    // 处理鼠标事件-划过、厉害、按下、释放、移动
    void handleWidgetEvent(QEvent *event);

    void handleMaxRestore();
    void handleMin();

private:
    // 更新鼠标样式
    void updateCursorShape(const QPoint &gMousePos);
    // 重置窗体大小
    void resizeWidget(const QPoint &gMousePos);
    // 移动窗体
    void moveWidget(const QPoint &gMousePos);
    // 处理鼠标按下
    void handleMousePressEvent(QMouseEvent *event);
    // 处理鼠标释放
    void handleMouseReleaseEvent(QMouseEvent *event);
    // 处理鼠标移动
    void handleMouseMoveEvent(QMouseEvent *event);
    // 处理鼠标离开
    void handleLeaveEvent(QEvent *event);
    // 处理鼠标进入
    void handleHoverMoveEvent(QHoverEvent *event);
    void handleMouseDoubleClickEvent(QMouseEvent *event);
    bool isMax();


private:
    FramelessIconCtrl * m_iconCtrl;
    QWidget *m_pWidget;

    CursorPosCalculator m_pressedMousePos;
    CursorPosCalculator m_moveMousePos;
    bool m_bLeftButtonPressed;
    bool m_bCursorShapeChanged;
    bool m_bLeftButtonTitlePressed;
    QPoint m_ptDragPos;
    QRect m_rcNormal;
    bool m_bMax;
    bool m_bMoveing;

    bool m_bWidgetMovable;
    bool m_bWidgetResizable;
    int m_nTitleHeight;
    QWidget *m_pTitleWidget;
    QAbstractButton *m_pMinButton;
    QAbstractButton *m_pMaxButton;
    QAbstractButton *m_pCloseButton;

    int m_closeAction;
    int m_nBorderWidth;
    int m_nShadowWidth;

    //windows native event
    bool m_bNativeResize;


};

//固定大小对话框，常用于模态弹出窗口，不可Resize，点击关闭按钮，直接关闭窗口
#define FRAMELESS_HELPER_FIXSIZE()       \
    FramelessHelper* framelessHelper = new FramelessHelper(this);\
    framelessHelper->setUIObject(this, ui->widget_title, ui->menuButton_Close);\
    framelessHelper->setWidgetResizable(false);\
    framelessHelper->setMinWhenClose(FramelessHelper::CloseAction_Close);

#define FRAMELESS_HELPER_FIXSIZE_SHADOW()       \
    setAttribute(Qt::WA_TranslucentBackground);\
    ui->widget_shadow->setShadowSize(10);\
    FramelessHelper* framelessHelper = new FramelessHelper(this);\
    framelessHelper->setUIObject(this, ui->widget_title, ui->menuButton_Close);\
    framelessHelper->setWidgetResizable(false);\
    framelessHelper->setMinWhenClose(FramelessHelper::CloseAction_Close);

//Resize对话框1，常用于主窗口，支持Resize，点击关闭按钮，最小化窗口
#define FRAMELESS_HELPER_RESIZE1()       \
    FramelessHelper* framelessHelper = new FramelessHelper(this);\
    framelessHelper->setUIObject(this, ui->widget_title, ui->menuButton_Close, ui->menuButton_Max, ui->menuButton_Min);    \
    framelessHelper->setWidgetResizable(true);\
    framelessHelper->setMinWhenClose(FramelessHelper::CloseAction_Min);

#define FRAMELESS_HELPER_RESIZE1_SHADOW()       \
    setAttribute(Qt::WA_TranslucentBackground);\
    ui->widget_shadow->setShadowSize(10);\
    FramelessHelper* framelessHelper = new FramelessHelper(this);\
    framelessHelper->setUIObject(this, ui->widget_title, ui->menuButton_Close, ui->menuButton_Max, ui->menuButton_Min);    \
    framelessHelper->setWidgetResizable(true);\
    framelessHelper->setMinWhenClose(FramelessHelper::CloseAction_Min);

//Resize对话框2，常用于主窗口，支持Resize，点击关闭按钮，关闭窗口
#define FRAMELESS_HELPER_RESIZE2()       \
    FramelessHelper* framelessHelper = new FramelessHelper(this);\
    framelessHelper->setUIObject(this, ui->widget_title, ui->menuButton_Close, ui->menuButton_Max, ui->menuButton_Min);    \
    framelessHelper->setWidgetResizable(true);\
    framelessHelper->setMinWhenClose(FramelessHelper::CloseAction_Close);

#define FRAMELESS_HELPER_RESIZE2_SHADOW()       \
    setAttribute(Qt::WA_TranslucentBackground);\
    ui->widget_shadow->setShadowSize(10);\
    FramelessHelper* framelessHelper = new FramelessHelper(this);\
    framelessHelper->setUIObject(this, ui->widget_title, ui->menuButton_Close, ui->menuButton_Max, ui->menuButton_Min);    \
    framelessHelper->setWidgetResizable(true);\
    framelessHelper->setMinWhenClose(FramelessHelper::CloseAction_Close);

//Resize对话框3，常用于主窗口，支持Resize，点击关闭按钮，什么也不做，由调用者自行控制关闭
#define FRAMELESS_HELPER_RESIZE3()      \
    FramelessHelper* framelessHelper = new FramelessHelper(this);\
    framelessHelper->setUIObject(this, ui->widget_title, ui->menuButton_Close, ui->menuButton_Max, ui->menuButton_Min);    \
    framelessHelper->setWidgetResizable(true);\
    framelessHelper->setMinWhenClose(FramelessHelper::CloseAction_Null);

#define FRAMELESS_HELPER_RESIZE3_SHADOW()      \
    setAttribute(Qt::WA_TranslucentBackground);\
    ui->widget_shadow->setShadowSize(10);\
    FramelessHelper* framelessHelper = new FramelessHelper(this);\
    framelessHelper->setUIObject(this, ui->widget_title, ui->menuButton_Close, ui->menuButton_Max, ui->menuButton_Min);    \
    framelessHelper->setWidgetResizable(true);\
    framelessHelper->setMinWhenClose(FramelessHelper::CloseAction_Null);

#endif // FRAMELESSHELPER_H
