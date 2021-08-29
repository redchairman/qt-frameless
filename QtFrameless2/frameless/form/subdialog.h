#ifndef SUBDIALOG_H
#define SUBDIALOG_H

#include <QDialog>
#include "qframelessdialog.h"
namespace Ui {
class SubDialog;
}

class SubDialog : public QFramelessDialog
{
    Q_OBJECT

public:
    explicit SubDialog(QWidget *parent = nullptr);
    ~SubDialog();

    void initForm();
private slots:
    void on_btnMenu_Close_clicked();

    void on_pushButton_clicked();

private:
    Ui::SubDialog *ui;
};

#endif // SUBDIALOG_H
