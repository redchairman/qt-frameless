#include "demowidget.h"
#include "ui_demowidget.h"
#include "myframelessiconctrl.h"

DemoWidget::DemoWidget(int shadow, QWidget *parent) :
    FramelessWidget(parent),
    ui(new Ui::DemoWidget)
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

DemoWidget::~DemoWidget()
{
    delete ui;
}

void DemoWidget::on_menuButton_Close_clicked()
{
    close();
}
