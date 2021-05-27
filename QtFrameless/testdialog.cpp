#include "testdialog.h"
#include "ui_testdialog.h"

TestDialog::TestDialog(QWidget *parent) :
    ShadowBaseDialog(parent),
    ui(new Ui::TestDialog)
{
    ui->setupUi(this);

    //connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(sltClick()));

}

TestDialog::~TestDialog()
{
    delete ui;
}

//void TestDialog::sltClick()
//{
//close();
//}
void TestDialog::on_pushButton_clicked()
{
    close();
}
