#include "widget.h"
#include "ui_widget.h"
#include "demodialog.h"
#include "demowidget.h"
#include "demodialog2.h"

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


void Widget::on_pushButton_widget_noshadow_clicked()
{
    DemoWidget *widget = new DemoWidget(0);
    widget->setWindowModality(Qt::ApplicationModal);//设置窗体模态，要求该窗体没有父类，否则无效
    widget->show();
}

void Widget::on_pushButton_widget_shadow_clicked()
{
    DemoWidget *widget = new DemoWidget(1);
    widget->setWindowModality(Qt::ApplicationModal);//设置窗体模态，要求该窗体没有父类，否则无效
    widget->show();
}

void Widget::on_pushButton_widget_dwm_clicked()
{
    DemoWidget *widget = new DemoWidget(2);
    widget->setWindowModality(Qt::ApplicationModal);//设置窗体模态，要求该窗体没有父类，否则无效
    widget->show();
}

void Widget::on_pushButton_dialog_noshadow_clicked()
{
    DemoDialog *widget = new DemoDialog(0, this);
    widget->exec();
}

void Widget::on_pushButton_dialog_shadow_clicked()
{
    DemoDialog *widget = new DemoDialog(1, this);
    widget->exec();
}

void Widget::on_pushButton_dialog_dwm_clicked()
{
    DemoDialog *widget = new DemoDialog(2, this);
    widget->exec();
}

void Widget::on_pushButton_dialog_dwm_2_clicked()
{
    DemoDialog2 dlg(this);
    dlg.exec();
}
