#ifndef WIDGET2_H
#define WIDGET2_H

#include <QWidget>
#include "shadowwidget.h"
namespace Ui {
class Widget2;
}

class Widget2 : public WinAPIShadowWidget
{
    Q_OBJECT

public:
    explicit Widget2(QWidget *parent = nullptr);
    ~Widget2();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Widget2 *ui;
};

#endif // WIDGET2_H
