#include "subdialog.h"
#include "ui_subdialog.h"

SubDialog::SubDialog(QWidget *parent) :
    QFramelessDialog(parent, false),
    ui(new Ui::SubDialog)
{
    ui->setupUi(this);
    initForm();
}

SubDialog::~SubDialog()
{
    delete ui;
}

void SubDialog::initForm()
{
    //设置标题栏控件
    ui->labTitle->setText("无边框窗体示例-支持win、linux、mac等系统 (QQ: 517216493 WX: feiyangqingyun)");
    this->setWindowTitle(ui->labTitle->text());
    this->framelessHelper()->setTitleBar(ui->labTitle);

    framelessHelper()->setResizeEnable(false);
    //关联信号
//    connect(framelessHelper(), SIGNAL(titleDblClick()), this, SLOT(titleDblClick()));
//    connect(framelessHelper(), SIGNAL(windowStateChange(bool)), this, SLOT(windowStateChange(bool)));

    //设置样式表
    QStringList list;
    list << "#titleBar{background:#BBBBBB;}";
    //list << "#titleBar{border-top-left-radius:8px;border-top-right-radius:8px;}";
    list << "#widgetMain{border:1px solid #BBBBBB;background:#FFFFFF;}";
    //list << "#widgetMain{border-bottom-left-radius:8px;border-bottom-right-radius:8px;}";
    this->setStyleSheet(list.join(""));
}

void SubDialog::on_btnMenu_Close_clicked()
{
    close();
}

void SubDialog::on_pushButton_clicked()
{
    SubDialog dlg(this);
    dlg.exec();
}
