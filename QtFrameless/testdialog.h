#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QWidget>
#include "shadowbasedialog.h"

namespace Ui {
class TestDialog;
}

class TestDialog : public ShadowBaseDialog
{
    Q_OBJECT

public:
    explicit TestDialog(QWidget *parent = nullptr);
    ~TestDialog();

public slots:
//    void sltClick();
private slots:
    void on_pushButton_clicked();

private:
    Ui::TestDialog *ui;
};

#endif // TESTDIALOG_H
