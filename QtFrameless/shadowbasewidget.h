#ifndef SHADOWBASEWIDGET_H
#define SHADOWBASEWIDGET_H

#include <QWidget>

class ShadowBaseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShadowBaseWidget(QWidget *parent = nullptr);
    QWidget* clientWidget()
    {
        return shadowClientWidget;
    }
private:
    QWidget* shadowClientWidget;

signals:

};

#endif // SHADOWBASEWIDGET_H
