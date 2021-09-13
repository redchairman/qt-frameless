#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qframelessmainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QFramelessMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(bool resizeEnable = true,
                        bool shadowBorder = true,
                        bool winNativeEvent = true,
                        QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void initForm();
private slots:
    void maximizedChanged(bool max);

private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
    void on_pushButton_QDialog_clicked();
    void on_pushButton_QWidget_clicked();
    void on_pushButton_QMainWindow_clicked();
};

#endif // MAINWINDOW_H
