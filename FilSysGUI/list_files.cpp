#include "list_files.h"
#include "ui_list_files.h"
#include"fileSystem.h"
#include"mainFunctions.h"
#include<QListWidget>
#include <QAbstractTableModel>
#include<QMessageBox>
#include<QDebug>
list_files::list_files(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::list_files)
{
    ui->setupUi(this);
    qDebug()<<"creating ui";


}

list_files::~list_files()
{
    delete ui;
    qDebug()<<"deleted ui";

}

void list_files::dispaly_files()
{
   ui->tableWidget->update();
   fileslist ob;
   char* name;
   double filesizes;
   double Num_of_blocks;
   double first_block;
   inorderlist displaylist;
   ListFilesDisplay( displaylist);
   bool exist;
   exist=displaylist.retrieve_the_first_element(ob);
   for(int rowNumber=0;exist;rowNumber++)
   {
       name=ob.retName();
       string names(name);
       filesizes=ob.retSize();
       Num_of_blocks=ob.retNumberofblocks();
       QString Nu_blocks=QString::number( Num_of_blocks);
       string empty="*****";
       first_block=ob.retFirstBlock();
       QString first_block_nu=QString::number(first_block);
       QString last_block=QString::number(first_block+ Num_of_blocks-1);



       if(names != empty && filesizes!=0)
       {
           ui-> tableWidget->insertRow( rowNumber );
           ui-> tableWidget->setItem( rowNumber,0, new QTableWidgetItem( name));
           ui-> tableWidget->setItem( rowNumber,1, new QTableWidgetItem(  Nu_blocks));
           ui-> tableWidget->setItem( rowNumber, 2, new QTableWidgetItem(first_block_nu));
           ui-> tableWidget->setItem( rowNumber, 3, new QTableWidgetItem(last_block));
       }
       else{
           rowNumber--;
       }

       exist=displaylist.retrieve_the_next_element(ob);
   }
   ui->tableWidget->sortItems(2,Qt::AscendingOrder);


    //displaylist.~inorderlist();
}

void list_files::on_pushButton_clicked()
{ bool done;
    done= VFS.MoveUp();
       if(done)
       {QMessageBox::information(this,"Move UP","Failed");}
        else{QMessageBox::information(this,"Move UP","Done");}
}

void list_files::on_pushButton_2_clicked()
{
    ui->tableWidget->clearContents();
    dispaly_files();


}
