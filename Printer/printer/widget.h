#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QString m_barcode;
    QString m_combinationID;
    QString m_notes;
    QString m_lpnCode;
    QString m_workOrderID;
    QString m_sequence;
    QString m_finished;
    bool searchFile(QString, QString);
    void print();

private slots:
    void on_pushButton_clicked();
};

#endif // WIDGET_H
