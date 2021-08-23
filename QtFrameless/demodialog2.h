#ifndef DEMODIALOG2_H
#define DEMODIALOG2_H

#include <QDialog>
#include "QFramelessDialog.h"

namespace Ui {
class DemoDialog2;
}

class DemoDialog2 : public QFramelessDialog
{
    Q_OBJECT

public:
    explicit DemoDialog2(QWidget *parent = nullptr);
    ~DemoDialog2();

private:
    Ui::DemoDialog2 *ui;
};

#endif // DEMODIALOG2_H
