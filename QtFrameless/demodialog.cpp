#include "demodialog.h"
#include "ui_demodialog.h"
#include "myframelessiconctrl.h"

DemoDialog::DemoDialog(int shadow, QWidget *parent) :
    FramelessDialog(parent),
    ui(new Ui::DemoDialog)
{
    ui->setupUi(this);
    if (shadow == 0)
    {
        setFramelessFlags(Moveable | Resizeable);
    }
    else
    {
        setFramelessFlags(Moveable | Resizeable | ShadowBorder);
        setShadowFlag((shadow == 1) ? FramelessWidget::DrawShadow : FramelessWidget::DwmShadow);
    }
    setCloseButtonAction(FramelessWidget::ActionClose);
    setUIObject(ui->widget_title,
                ui->menuButton_Close,
                ui->menuButton_Max,
                ui->menuButton_Min);
    setIconCtrl(new MyFramelessIconCtrl(this));
    initFrameless();
}

DemoDialog::~DemoDialog()
{
    delete ui;
}

void DemoDialog::on_menuButton_Close_clicked()
{
    close();
}
