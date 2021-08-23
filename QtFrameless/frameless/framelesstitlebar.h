#ifndef FRAMELESSTITLEBAR_H
#define FRAMELESSTITLEBAR_H

#include <QObject>
#include <QAbstractButton>
class FramelessIconCtrl;
class FramelessTitlebar : public QObject
{
    Q_OBJECT
public:
    explicit FramelessTitlebar(QObject *parent = nullptr);

    enum CloseAction
    {
        CloseAction_Null = 0,
        CloseAction_Min,
        CloseAction_Close
    };

    enum SizeType
    {
        MinSize = 0,
        MaxSize = 1,
        Restore = 2,
        Close = 3
    };
    Q_ENUM(SizeType);

    void setWidgets(QWidget *mainWidget,
                    QWidget *titleBar,
                      QAbstractButton *closeButton,
                      QAbstractButton *maxButton,
                      QAbstractButton *minButton);

    void setMinWhenClose(CloseAction action);


public slots:
    void sltClickMin();
    void sltClickMaxRestore();
    void sltClickClose();
signals:
    void sigSizeChanged(FramelessTitlebar::SizeType);
protected:
    void updateMaxIcon();
    void handleMaxRestore();
    bool isMax();
    void handleMin();
private:
    FramelessIconCtrl * m_iconCtrl;
    QWidget *m_mainWidget;
    QWidget *m_titleBar;
    QAbstractButton *m_minButton;
    QAbstractButton *m_maxButton;
    QAbstractButton *m_closeButton;
    CloseAction m_closeAction;
    bool m_isMax;
    QRect m_rcNormal;

};
Q_DECLARE_METATYPE(FramelessTitlebar::SizeType)

#endif // FRAMELESSTITLEBAR_H
