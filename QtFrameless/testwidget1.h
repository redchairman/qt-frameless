#ifndef TESTWIDGET1_H
#define TESTWIDGET1_H

#include <QWidget>
#include "shadowbasewidget.h"

namespace Ui {
class TestWidget1;
}

class TestWidget1 : public ShadowBaseWidget
{
    Q_OBJECT

public:
    explicit TestWidget1(QWidget *parent = nullptr);
    ~TestWidget1();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TestWidget1 *ui;
};

#endif // TESTWIDGET1_H
