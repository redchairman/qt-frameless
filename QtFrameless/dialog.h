#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "shadowbasedialog.h"
namespace Ui {
class Dialog;
}

class Dialog : public ShadowBaseDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
