#include "dialog.h"
#include "ui_dialog.h"
#include<QDebug>
using namespace std;
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::SEt_end_one(float end)
{  qDebug()<<end;

   END=end;
     ui->widget->SetEnd(END);
}

void  Dialog::setTexts(QString text2,QString text1)
{

    ui->label->setText(text1);
    ui->label_2->setText(text2);



}
