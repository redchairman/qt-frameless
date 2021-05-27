#include "testwidget1.h"
#include "ui_testwidget1.h"

TestWidget1::TestWidget1(QWidget *parent) :
    ShadowBaseWidget(parent),
    ui(new Ui::TestWidget1)
{
//    ShadowBaseWidget * shadow = new ShadowBaseWidget(10, this);
    ui->setupUi(this);
}

TestWidget1::~TestWidget1()
{
    delete ui;
}

void TestWidget1::on_pushButton_clicked()
{
    close();
}
