#include "dialog2.h"
#include "ui_dialog2.h"
#include<QMessageBox>
#include"mainFunctions.h"
#include"mainwindow.h"
Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);
    added=false;
}

Dialog2::~Dialog2()
{
    delete ui;
}

void Dialog2::on_buttonBox_2_accepted()
{
    QString tit =ui->title->text();
    QString cont=ui->content->toPlainText();

    //------------------------------------------------------------->check name constrains
    if(tit.length()>255 || tit=="*****" || tit.toStdString().find('/')!=std::string::npos ){
        added=false;
        QMessageBox ::warning(this,"WARNING","Name can't contain (/,*) and can't exceed 255 letters !") ;//check max
        //close();
        //return;
    }else{//title is fine

        double THput= add_txtFile(tit.toStdString(),cont.toStdString(),fileObj);
        if(THput==-1){//no enough space
            QMessageBox ::warning(this,"WARNING","THERE IS NO ENOUGH SPACE!") ;
            added=false;
            //close();
            //return;
        }else{//there is space, add file and close window

            added=true;
            close();
        }
    }
}

void Dialog2::on_buttonBox_2_rejected()
{
    added=false;
    close();
}
bool Dialog2::returnObj(fileslist &Obj){
    if(added)
        Obj=fileObj;
    return added;
}
