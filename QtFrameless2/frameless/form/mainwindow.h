#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "framelessmainwindow.h"
#include "qframelessmainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QFramelessMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void initForm();
    void titleDblClick();
    void windowStateChange(bool max);

private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void on_pushButton_clicked();
    void on_pushButton_QDialog_clicked();
    void on_pushButton_QWidget_clicked();
    void on_pushButton_QMainWindow_clicked();
};

#endif // MAINWINDOW_H
