#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_widget_shadow_clicked();

    void on_pushButton_widget_dwm_clicked();

    void on_pushButton_dialog_shadow_clicked();

    void on_pushButton_dialog_dwm_clicked();

    void on_pushButton_widget_noshadow_clicked();

    void on_pushButton_dialog_noshadow_clicked();

    void on_pushButton_dialog_dwm_2_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
