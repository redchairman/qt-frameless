#include "demodialog2.h"
#include "ui_demodialog2.h"

DemoDialog2::DemoDialog2(QWidget *parent) :
    QFramelessDialog(parent),
    ui(new Ui::DemoDialog2)
{
    ui->setupUi(this);
    InitFrameless(ui->widget_title,
                ui->menuButton_Close,
                ui->menuButton_Max,
                ui->menuButton_Min, true, true);

}

DemoDialog2::~DemoDialog2()
{
    delete ui;
}
