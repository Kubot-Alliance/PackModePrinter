#include "widget.h"
#include "ui_widget.h"
#include "QPrinter"
#include "QPrintDialog"
#include "QPainter"
#include "QMessageBox"
#include "QFile"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->fileNameLineEdit->setText(tr("/home/shaojian/Kubot-System/cmake-build-debug/src/station1_print_info.csv"));

//    m_barcode = "ROBOT-WAVE20181107008_1";
//    m_combinationID = "43";
//    m_notes = "1/4";
//    m_lpnCode = "YM34355";
//    m_workOrderID = "434534666";
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QString inputLpnCode = ui->lpnLineEdit->text();
    QRegularExpression re("^YM\\d{5}$");
    QRegularExpressionMatch match = re.match(inputLpnCode);
    if (!match.hasMatch())
    {
        QMessageBox::information(this, tr("提示"), tr("LPN码格式错误"), QMessageBox::Ok);
            return;
    }

    QString inputFileName = ui->fileNameLineEdit->text();
    if (inputFileName.isEmpty())
    {
        QMessageBox::information(this, tr("提示"), tr("文件名不能为空"), QMessageBox::Ok);
        return;
    }

    if (!searchFile(inputLpnCode, inputFileName))
    {
        QMessageBox::information(this, tr("提示"), tr("没有该箱子的封箱记录"), QMessageBox::Ok);
        return;
    }

//    print();
}

bool Widget::searchFile(QString searchLpnCode, QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("提示"), tr("无法打开记录文件"), QMessageBox::Ok);
        return false;
    }

    QStringList wordList;
    bool found = false;
    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        if (line.split(',').first() == searchLpnCode)
        {
            m_lpnCode = line.split(',').at(0);
            m_barcode = line.split(',').at(1);
            m_combinationID = line.split(',').at(2);
            m_notes = line.split(',').at(3);
            m_workOrderID = line.split(',').at(4);
            m_sequence = line.split(',').at(5);
            m_finished = line.split(',').at(6);

            found = true;
            print();

//            return true;
        }
    }

    return found;
}

void Widget::print()
{
    QPrinter printer(QPrinter::ScreenResolution);
    QPrintDialog printDialog(&printer, 0);
    if (printDialog.exec()) {
        QPainter painter;
        painter.begin(&printer);

        QString completeStr = (m_finished == "1\n") ? "最后一箱" : "提前封箱";

        painter.setFont(QFont("Code 128", 30));
        painter.drawText(0, 20, m_barcode);

        painter.setFont(QFont("Times New Roman", 40));
        painter.drawText(0, 80, m_combinationID);

        painter.setFont(QFont("Times New Roman", 20));
        painter.drawText(0, 140, tr("第") + m_notes.at(0) + "份 （共" + m_notes.at(2) + "份）");

        painter.drawText(0, 200, tr("第") + m_notes.at(0) + "份的第" + m_sequence + "箱 " + completeStr);

        painter.drawText(0, 260, tr("LPN码：") + m_lpnCode);

        painter.drawText(0, 320, tr("工单号码：") + m_workOrderID);

        painter.end();
    }
}


