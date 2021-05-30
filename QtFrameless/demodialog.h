#ifndef DEMODIALOG_H
#define DEMODIALOG_H

#include <QDialog>
#include "shadowwidget.h"
#include "framelesswidget.h"

namespace Ui {
class DemoDialog;
}

class DemoDialog : public FramelessDialog
{
    Q_OBJECT

public:
    explicit DemoDialog(int shadow, QWidget *parent = nullptr);
    ~DemoDialog();

private slots:
    void on_menuButton_Close_clicked();

private:
    Ui::DemoDialog *ui;
};

#endif // DEMODIALOG_H
