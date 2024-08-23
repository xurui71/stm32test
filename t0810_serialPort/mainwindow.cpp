#include "mainwindow.h"
#include "ui_mainwindow.h"

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial = new QSerialPort;

    QString description;
    QString manufacturer;
    QString serialNumber;

    //获取可以用的串口
    QList<QSerialPortInfo> serialPortInfos = QSerialPortInfo::availablePorts();

    //输出当前系统可以使用的串口个数
    qDebug() << "Total numbers of ports: " << serialPortInfos.count();

    //将所有可以使用的串口设备添加到ComboBox中
    for (const QSerialPortInfo &serialPortInfo : serialPortInfos)
    {
        QStringList list;
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();

        list << serialPortInfo.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << serialPortInfo.systemLocation()
             << (serialPortInfo.vendorIdentifier() ? QString::number(serialPortInfo.vendorIdentifier(), 16) : blankString)
             << (serialPortInfo.productIdentifier() ? QString::number(serialPortInfo.productIdentifier(), 16) : blankString);

        ui->comboBox_serialPort->addItem(list.first(), list);
    }

    ui->comboBox_serialPort->addItem(tr("custom"));

    //设置波特率
    ui->comboBox_baudRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->comboBox_baudRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->comboBox_baudRate->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->comboBox_baudRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->comboBox_baudRate->addItem(tr("Custom"));

    //设置数据位
    ui->comboBox_dataBits->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->comboBox_dataBits->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->comboBox_dataBits->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->comboBox_dataBits->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->comboBox_dataBits->setCurrentIndex(3);

    //设置奇偶校验位
    ui->comboBox_parity->addItem(tr("None"), QSerialPort::NoParity);
    ui->comboBox_parity->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->comboBox_parity->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->comboBox_parity->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->comboBox_parity->addItem(tr("Space"), QSerialPort::SpaceParity);

    //设置停止位
    ui->comboBox_stopBit->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->comboBox_stopBit->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    //添加流控
    ui->comboBox_flowBit->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->comboBox_flowBit->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->comboBox_flowBit->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);


    //禁用发送按钮
    ui->btn_send->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//打开串口按钮槽函数
void MainWindow::on_btn_openConsole_clicked()
{
    qDebug() << ui->btn_openConsole->text();

    if (ui->btn_openConsole->text() == tr("打开串口"))
    {


        //设置串口名字
        serial->setPortName(ui->comboBox_serialPort->currentText());

        //设置波特率
        serial->setBaudRate(ui->comboBox_baudRate->currentText().toInt());

        //设置数据位
        serial->setDataBits(QSerialPort::Data8);

        //设置奇偶校验位
        serial->setParity(QSerialPort::NoParity);

        //设置停止位
        serial->setStopBits(QSerialPort::OneStop);

        //设置流控
        serial->setFlowControl(QSerialPort::NoFlowControl);

        //打开串口
        if (serial->open(QIODevice::ReadWrite))
        {
            ui->comboBox_baudRate->setEnabled(false);
            ui->comboBox_dataBits->setEnabled(false);
            ui->comboBox_flowBit->setEnabled(false);
            ui->comboBox_parity->setEnabled(false);
            ui->comboBox_serialPort->setEnabled(false);
            ui->comboBox_stopBit->setEnabled(false);

            ui->btn_send->setEnabled(true);

            ui->btn_openConsole->setText(tr("关闭串口"));

            //信号与槽函数关联
            connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
        }
    }
    else
    {
        //关闭串口
        //serial->clear();
        serial->close();
        //serial->deleteLater();

        //恢复设置功能
        ui->comboBox_baudRate->setEnabled(true);
        ui->comboBox_dataBits->setEnabled(true);
        ui->comboBox_flowBit->setEnabled(true);
        ui->comboBox_parity->setEnabled(true);
        ui->comboBox_serialPort->setEnabled(true);
        ui->comboBox_stopBit->setEnabled(true);

        ui->btn_openConsole->setText(tr("打开串口"));
        ui->btn_send->setEnabled(false);
    }

}

//发送数据槽函数
void MainWindow::on_btn_send_clicked()
{
    serial->write(ui->textEdit_send->toPlainText().toLatin1());

}

//清空接收数据槽函数
void MainWindow::on_btn_clearRecv_clicked()
{
    ui->textEdit_recv->clear();
}

//清空发送区槽函数
void MainWindow::on_btn_clearSend_clicked()
{
    ui->textEdit_send->clear();
}

void MainWindow::readData()
{
    QByteArray buf;

    qDebug() << "readData: " << Qt::endl;

    buf = serial->readAll();
    if (!buf.isEmpty())
    {
        QString str = ui->textEdit_recv->toPlainText();

        str += tr(buf);
        ui->textEdit_recv->clear();
        ui->textEdit_recv->append(str);
    }
}




