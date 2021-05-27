#include "widget.h"
#include "ui_widget.h"
#include "testdialog.h"
#include "testwidget1.h"
#include "dialog.h"
#include "dialog2.h"
#include "widget2.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_widget_shadow_clicked()
{
    TestWidget1 *widget = new TestWidget1();
    widget->setWindowModality(Qt::ApplicationModal);//设置窗体模态，要求该窗体没有父类，否则无效
    widget->show();
}

void Widget::on_pushButton_widget_dwm_clicked()
{
    Widget2 *widget = new Widget2();
    widget->setWindowModality(Qt::ApplicationModal);//设置窗体模态，要求该窗体没有父类，否则无效
    widget->show();
}

void Widget::on_pushButton_dialog_shadow_clicked()
{
    Dialog *widget = new Dialog(this);
    widget->exec();
}

void Widget::on_pushButton_dialog_dwm_clicked()
{
    Dialog2 *widget = new Dialog2(this);
    widget->exec();
}
