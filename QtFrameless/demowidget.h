#ifndef DEMOWIDGET_H
#define DEMOWIDGET_H

#include <QWidget>
#include "framelesswidget.h"

namespace Ui {
class DemoWidget;
}

class DemoWidget : public FramelessWidget
{
    Q_OBJECT

public:
    explicit DemoWidget(int shadow, QWidget *parent = nullptr);
    ~DemoWidget();

private slots:
    void on_menuButton_Close_clicked();

private:
    Ui::DemoWidget *ui;
};

#endif // DEMOWIDGET_H
