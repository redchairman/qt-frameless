#ifndef SHADOWBASEDIALOG_H
#define SHADOWBASEDIALOG_H

#include <QDialog>

class ShadowBaseDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ShadowBaseDialog(QWidget *parent = nullptr);
    QWidget* clientWidget()
    {
        return shadowClientWidget;
    }
private:
    QWidget* shadowClientWidget;

signals:

};

#endif // SHADOWBASEDIALOG_H
