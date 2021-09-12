#include "dialog.h"
#include "ui_dialog.h"
#include "head.h"
#include "widget.h"
#include "mainwindow.h"

Dialog::Dialog(bool resizeEnable,
               bool shadowBorder,
               bool winNativeEvent,
               QWidget *parent)
    : QFramelessDialog(parent, resizeEnable, shadowBorder, winNativeEvent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    if (resizeEnable == false)
    {
        ui->btnMenu_Min->hide();
        ui->btnMenu_Max->hide();
    }
    this->initForm();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::initForm()
{
    //设置标题栏控件
    this->setWindowTitle(ui->label_title->text());
    this->framelessHelper()->setTitleBar(ui->widget_titleBar);

    //关联信号
    connect(framelessHelper(), SIGNAL(titleDblClick()), this, SLOT(titleDblClick()));
    connect(framelessHelper(), SIGNAL(windowStateChange(bool)), this, SLOT(windowStateChange(bool)));
}

void Dialog::titleDblClick()
{
    on_btnMenu_Max_clicked();
}

void Dialog::windowStateChange(bool max)
{
    if (!max) {
        ui->btnMenu_Max->setProperty("type", "maxsize");
    } else {
        ui->btnMenu_Max->setProperty("type", "restore");
    }
    ui->btnMenu_Max->style()->unpolish(ui->btnMenu_Max);
    ui->btnMenu_Max->style()->polish(ui->btnMenu_Max);
}

void Dialog::on_btnMenu_Min_clicked()
{
    framelessHelper()->showMinimized();
}

void Dialog::on_btnMenu_Max_clicked()
{
    if (framelessHelper()->isMaximized()) {
        framelessHelper()->showNormal();
        ui->btnMenu_Max->setProperty("type", "maxsize");
    } else {
        framelessHelper()->showMaximized();
        ui->btnMenu_Max->setProperty("type", "restore");
    }
    ui->btnMenu_Max->style()->unpolish(ui->btnMenu_Max);
    ui->btnMenu_Max->style()->polish(ui->btnMenu_Max);
}

void Dialog::on_btnMenu_Close_clicked()
{
    this->close();
}

void Dialog::on_pushButton_QDialog_clicked()
{
    bool resizeEnable = ui->checkBox_resizeEnable->isChecked();
    bool shadowBorder = ui->checkBox_shadowBorder->isChecked();
    bool winNativeEvent = ui->checkBox_winNativeEvent->isChecked();

    Dialog dlg(resizeEnable, shadowBorder, winNativeEvent, this);
    dlg.exec();
}

void Dialog::on_pushButton_QWidget_clicked()
{
    bool resizeEnable = ui->checkBox_resizeEnable->isChecked();
    bool shadowBorder = ui->checkBox_shadowBorder->isChecked();
    bool winNativeEvent = ui->checkBox_winNativeEvent->isChecked();

    Widget *w = new Widget(resizeEnable, shadowBorder, winNativeEvent);
    //w->setWindowModality(Qt::ApplicationModal);
    w->show();
}

void Dialog::on_pushButton_QMainWindow_clicked()
{
    bool resizeEnable = ui->checkBox_resizeEnable->isChecked();
    bool shadowBorder = ui->checkBox_shadowBorder->isChecked();
    bool winNativeEvent = ui->checkBox_winNativeEvent->isChecked();

    MainWindow *w = new MainWindow(resizeEnable, shadowBorder, winNativeEvent);
    w->show();
}
