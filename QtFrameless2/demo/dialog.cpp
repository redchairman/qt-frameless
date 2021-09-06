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
    //ui->labTitle->setText("无边框窗体示例-支持win、linux、mac等系统 (QQ: 517216493 WX: feiyangqingyun)");
    this->setWindowTitle(ui->label_title->text());
    this->framelessHelper()->setTitleBar(ui->widget_caption);

    //关联信号
    connect(framelessHelper(), SIGNAL(titleDblClick()), this, SLOT(titleDblClick()));
    connect(framelessHelper(), SIGNAL(windowStateChange(bool)), this, SLOT(windowStateChange(bool)));

    //设置样式表
//    QStringList list;
//    list << "#titleBar{background:#22cb64;}";
//    list << "#labTitle{color:#ffffff;}";
//    //list << "#titleBar{border-top-left-radius:8px;border-top-right-radius:8px;}";
//    list << "#widgetMain{border:1px solid #22cb64;background:#f1fcf3;}";
//    //list << "#widgetMain{border-bottom-left-radius:8px;border-bottom-right-radius:8px;}";
//    this->setStyleSheet(list.join(""));
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
#ifdef Q_OS_MACOS
    this->setWindowFlags(this->windowFlags() & ~Qt::FramelessWindowHint);
#endif
    this->showMinimized();
}

void Dialog::on_btnMenu_Max_clicked()
{
    if (this->isMaximized()) {
        this->showNormal();
        ui->btnMenu_Max->setProperty("type", "maxsize");
    } else {
        this->showMaximized();
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
