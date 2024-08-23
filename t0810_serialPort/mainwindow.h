#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>         // 提供访问串口的功能
#include <QtSerialPort/QSerialPortInfo>     // 提供系统中存在的串口信息
#include <QList>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_btn_openConsole_clicked();

    void on_btn_send_clicked();

    void on_btn_clearRecv_clicked();

    void on_btn_clearSend_clicked();

    void readData();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
};
#endif // MAINWINDOW_H   
