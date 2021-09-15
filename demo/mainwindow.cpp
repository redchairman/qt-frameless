#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "head.h"
#include "dialog.h"
#include "widget.h"

MainWindow::MainWindow(bool resizeEnable,
                       bool shadowBorder,
                       bool winNativeEvent,
                       QWidget *parent)
    : QFramelessMainWindow(parent, resizeEnable, shadowBorder, winNativeEvent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (resizeEnable == false)
    {
        ui->btnMenu_Min->hide();
        ui->btnMenu_Max->hide();
    }
    this->initForm();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initForm()
{
    //设置标题栏控件
    this->setWindowTitle(ui->label_title->text());
    this->framelessHelper()->setTitleBar(ui->widget_titleBar);

    //关联信号
    connect(framelessHelper(), SIGNAL(maximizedChanged(bool)), this, SLOT(maximizedChanged(bool)));
}

void MainWindow::maximizedChanged(bool max)
{
    if (!max) {
        ui->btnMenu_Max->setProperty("type", "maxsize");
    } else {
        ui->btnMenu_Max->setProperty("type", "restore");
    }
    ui->btnMenu_Max->style()->unpolish(ui->btnMenu_Max);
    ui->btnMenu_Max->style()->polish(ui->btnMenu_Max);
}

void MainWindow::on_btnMenu_Min_clicked()
{
    framelessHelper()->showMinimized();
}

void MainWindow::on_btnMenu_Max_clicked()
{
    framelessHelper()->switchMaximizedNormal();
}

void MainWindow::on_btnMenu_Close_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_QDialog_clicked()
{
    bool resizeEnable = ui->checkBox_resizeEnable->isChecked();
    bool shadowBorder = ui->checkBox_shadowBorder->isChecked();
    bool winNativeEvent = ui->checkBox_winNativeEvent->isChecked();

    Dialog dlg(resizeEnable, shadowBorder, winNativeEvent, this);
    dlg.exec();
}

void MainWindow::on_pushButton_QWidget_clicked()
{
    bool resizeEnable = ui->checkBox_resizeEnable->isChecked();
    bool shadowBorder = ui->checkBox_shadowBorder->isChecked();
    bool winNativeEvent = ui->checkBox_winNativeEvent->isChecked();

    Widget *w = new Widget(resizeEnable, shadowBorder, winNativeEvent);
    //w->setWindowModality(Qt::ApplicationModal);
    w->show();
}

void MainWindow::on_pushButton_QMainWindow_clicked()
{
    bool resizeEnable = ui->checkBox_resizeEnable->isChecked();
    bool shadowBorder = ui->checkBox_shadowBorder->isChecked();
    bool winNativeEvent = ui->checkBox_winNativeEvent->isChecked();

    MainWindow *w = new MainWindow(resizeEnable, shadowBorder, winNativeEvent);
    w->show();
}
