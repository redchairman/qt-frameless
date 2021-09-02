#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "head.h"
#include "dialog.h"

MainWindow::MainWindow(bool resizeEnable,
                       bool shadowBorder,
                       bool winNativeEvent,
                       QWidget *parent)
    : QFramelessMainWindow(parent, resizeEnable, shadowBorder, winNativeEvent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initForm();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initForm()
{
    //设置标题栏控件
    ui->labTitle->setText("无边框窗体示例-支持");
    this->setWindowTitle(ui->labTitle->text());
    this->framelessHelper()->setTitleBar(ui->labTitle);

    //关联信号
    connect(framelessHelper(), SIGNAL(titleDblClick()), this, SLOT(titleDblClick()));
    connect(framelessHelper(), SIGNAL(windowStateChange(bool)), this, SLOT(windowStateChange(bool)));

    //设置样式表
    QStringList list;
    list << "#titleBar{background:#BBBBBB;}";
    list << "#titleBar{border-top-left-radius:8px;border-top-right-radius:8px;}";
    list << "#widgetMain{border:2px solid #BBBBBB;background:#FFFFFF;}";
    //list << "#widgetMain{border-bottom-left-radius:8px;border-bottom-right-radius:8px;}";
    this->setStyleSheet(list.join(""));
}

void MainWindow::titleDblClick()
{
    on_btnMenu_Max_clicked();
}

void MainWindow::windowStateChange(bool max)
{
    ui->btnMenu_Max->setText(max ? "还原" : "最大");
}

void MainWindow::on_btnMenu_Min_clicked()
{
#ifdef Q_OS_MACOS
    this->setWindowFlags(this->windowFlags() & ~Qt::FramelessWindowHint);
#endif
    this->showMinimized();
}

void MainWindow::on_btnMenu_Max_clicked()
{
    if (this->isMaximized()) {
        this->showNormal();
        ui->btnMenu_Max->setText("最大");
    } else {
        this->showMaximized();
        ui->btnMenu_Max->setText("还原");
    }
}

void MainWindow::on_btnMenu_Close_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_clicked()
{
    Dialog dlg(this);
    dlg.exec();
//    QMessageBox::warning(this, "error", "eeeeeeeeeeeeee");
}

void MainWindow::on_pushButton_QDialog_clicked()
{

}

void MainWindow::on_pushButton_QWidget_clicked()
{

}

void MainWindow::on_pushButton_QMainWindow_clicked()
{

}
