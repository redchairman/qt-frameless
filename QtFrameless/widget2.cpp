#include "widget2.h"
#include "ui_widget2.h"

Widget2::Widget2(QWidget *parent) :
    WinAPIShadowWidget(parent),
    ui(new Ui::Widget2)
{
    ui->setupUi(this);
}

Widget2::~Widget2()
{
    delete ui;
}

void Widget2::on_pushButton_clicked()
{
    close();
}
