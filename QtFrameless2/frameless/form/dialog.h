﻿#ifndef DIALOG_H
#define DIALOG_H

#include "framelessdialog.h"
#include "qframelessdialog.h"
namespace Ui {
class Dialog;
}

class Dialog : public QFramelessDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;

private slots:
    void initForm();
    void titleDblClick();
    void windowStateChange(bool max);

private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void on_pushButton_clicked();
};

#endif // DIALOG_H