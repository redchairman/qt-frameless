#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include <QWidget>
#include <QFlag>
#include "shadowwidget.h"
#include "framelesshelper.h"

class FramelessBase
{
public:
    FramelessBase(QWidget *w);

    enum FramelessFlag
    {
        Moveable    = 0x00000001,
        Resizeable  = 0x00000002,
        ShadowBorder= 0x00000004
    };
    Q_DECLARE_FLAGS(FramelessFlags, FramelessFlag)

    enum ShadowFlag
    {
        DrawShadow  = 0,
        DwmShadow  = 1
    };

    enum CloseButtonAction
    {
        ActionNull = 0,
        ActionMinsize = 1,
        ActionClose = 2
    };

    FramelessFlags framelessFlags();
    void setFramelessFlags(FramelessFlags flags);
    void setShadowFlag(ShadowFlag flag);
    void setCloseButtonAction(CloseButtonAction action);
    void initFrameless();
    void setUIObject(QWidget *widgetTitle,
                     QAbstractButton *buttonClose,
                     QAbstractButton *buttonMax,
                     QAbstractButton *buttonMin);
    void setIconCtrl(FramelessIconCtrl* ctrl);

    void showDrawShadow();
    void hideDrawShadow();
protected:
    virtual void paintEvent(QPaintEvent *e);
#ifdef Q_OS_WIN
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif

private:
    QWidget* m_widget;
    FramelessFlags m_flags;
    ShadowFlag m_shadowFlag;
    CloseButtonAction m_closeButtonAction;
    ShadowImage *m_drawShadow;
#ifdef Q_OS_WIN
    WinDwmShadow *m_dwnShadow;
#endif
    FramelessHelper *m_framelessHelper;

    QWidget *m_titleWidget;
    QAbstractButton *m_minButton;
    QAbstractButton *m_maxButton;
    QAbstractButton *m_closeButton;
    FramelessIconCtrl *m_iconCtrl;

};
Q_DECLARE_OPERATORS_FOR_FLAGS(FramelessBase::FramelessFlags)

class FramelessWidget : public QWidget, public FramelessBase
{
    Q_OBJECT
public:
    explicit FramelessWidget(QWidget *parent = nullptr);

public slots:
    void sltShowDrawShadow();
    void sltHideDrawShadow();
protected:
    void paintEvent(QPaintEvent *e);
#ifdef Q_OS_WIN
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif

};

class FramelessDialog : public QDialog, public FramelessBase
{
    Q_OBJECT
public:
    explicit FramelessDialog(QWidget *parent = nullptr);

public slots:
    void sltShowDrawShadow();
    void sltHideDrawShadow();
protected:
    void paintEvent(QPaintEvent *e);
#ifdef Q_OS_WIN
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
#endif

};

#endif // FRAMELESSWIDGET_H
