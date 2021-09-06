#ifndef WIDGET_H
#define WIDGET_H

#include "qframelesswidget.h"

namespace Ui {
class Widget;
}

class Widget : public QFramelessWidget
{
    Q_OBJECT

public:
    explicit Widget(bool resizeEnable = true,
                    bool shadowBorder = true,
                    bool winNativeEvent = true,
                    QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

private slots:
    void initForm();
    void titleDblClick();
    void windowStateChange(bool max);

private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void on_pushButton_QDialog_clicked();
    void on_pushButton_QWidget_clicked();
    void on_pushButton_QMainWindow_clicked();
};

#endif // WIDGET_H
