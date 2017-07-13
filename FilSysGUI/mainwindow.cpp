#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QTreeWidget>
#include<QTreeWidgetItem>
#include<QMessageBox>
#include<QFileDialog>
#include"dialog.h"
#include"dialog2.h"
#include"pie.h"
#include"list_files.h"

#include<qdebug.h>

using namespace std;




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tree=ui->treeWidget;
    tree->setColumnCount(5);//name,type,size,date
    tree->setHeaderLabels(QStringList()<<"name"<<"Size"<<"Date"<<"Containing Folder"<<"Type");
    tree->setColumnWidth(0,150);
    tree->setColumnWidth(1,60);
    tree->setColumnWidth(2,200);
    tree->setColumnWidth(3,150);
    tree->setColumnWidth(4,20);

    initialize_buttons();
    itemSelected=NULL;

    Name="";
    size=0;
    date="";
    ContainingFolder="";//QString
    type="";//QString
    //NOC=0;

    setblockseize_available=true;
    initialize_tree();


}

void MainWindow::addTreeRoot(QTreeWidgetItem * &root) {

    QPixmap pix2("../FilSysGUI/fol_fil_Icons/Folder.png");
    QPixmap pix1("../FilSysGUI/fol_fil_Icons/file.png");
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(tree);

    if(type=="file"){
        QString s=QString("%1").arg(size);
        treeItem->setText(0, Name);
        treeItem->setText(1, s+" Bytes");
        treeItem->setText(2,date);
        treeItem->setText(3,ContainingFolder);
        treeItem->setText(4,type);
        treeItem->setIcon(0,QIcon(pix1));
    }
    else if(type=="folder"){
        treeItem->setText(0, Name);
        treeItem->setText(3,ContainingFolder);
        treeItem->setText(4,type);
        root=treeItem;//return root
        treeItem->setIcon(0,QIcon(pix2));

    }
    tree->sortItems(4, Qt::DescendingOrder);


}
void MainWindow::addTreeChild(QTreeWidgetItem *parent,QTreeWidgetItem * &child) {

    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    QPixmap pix2("../FilSysGUI/fol_fil_Icons/Folder.png");
    QPixmap pix1("../FilSysGUI/fol_fil_Icons/file.png");

    //get data
    QTreeWidgetItem *isparent = parent->parent();
    if(isparent)
    {
        QTreeWidgetItem *another_child = new QTreeWidgetItem();
        if(type=="file"){
            QString s=QString("%1").arg(size);
            another_child->setText(0, Name);
            another_child->setText(1, s+" Bytes");
            another_child->setText(2,date);
            another_child->setText(3,ContainingFolder);
            another_child->setText(4,type);
            another_child->setIcon(0,QIcon(pix1));

        }
        else if(type=="folder"){
            another_child->setText(0, Name);
            another_child->setText(3,ContainingFolder);
            another_child->setText(4,type);
            another_child->setIcon(0,QIcon(pix2));

        }
        isparent->addChild(another_child);
        parent->addChild( isparent->takeChild( isparent->indexOfChild( another_child ) ) );

        child=another_child;
    }else{
        if(type=="file"){
            QString s=QString("%1").arg(size);
            treeItem->setText(0, Name);
            treeItem->setText(1, s+" Bytes");
            treeItem->setText(2,date);
            treeItem->setText(3,ContainingFolder);
            treeItem->setText(4,type);
            treeItem->setIcon(0,QIcon(pix1));

        }
        else if(type=="folder"){
            treeItem->setText(0, Name);
            treeItem->setText(3,ContainingFolder);
            treeItem->setText(4,type);
            treeItem->setIcon(0,QIcon(pix2));


        }
        parent->addChild(treeItem);
        child=treeItem;
    }
    tree->sortItems(4, Qt::DescendingOrder);
}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    ui->copyVtoH->setEnabled(true);
    ui->Delete->setEnabled(true);
    itemSelected=item;
}

void MainWindow::initialize_buttons(){//called on clicked of any button

    //reinitialize button-Disabling
    ui->copyfolderH->setEnabled(true);
    ui->copyfileH->setEnabled(true);
    ui->copyVtoH->setEnabled(false);
    ui->Freesize->setEnabled(true);
    ui->frag->setEnabled(true);
    ui->Delete->setEnabled(false);

}

void MainWindow::on_Delete_clicked()//------------>Check file/Folder name with extension or not
{
    setblockseize_available=false;
        //get file/folder name
           QString file_folder=itemSelected->text(0);
           char *strname;
           //char* pathh;
           string pp =file_folder.toStdString();
           strname = new char [pp.size()+1];
           strcpy( strname, pp.c_str() );
           //get type (file/folder)
           QString type=itemSelected->text(4);
           //convert from Qstring to Char *



           char *containfold;
           string par=( itemSelected->text(3) ).toStdString();
           containfold=new char[par.size()+1];
           strcpy(containfold,par.c_str());



           //check file or folder
           if(type=="file"){
               if(itemSelected->text(3)=="")
                   containfold="*****";

               cout<<"Dname : "<<string(strname)<<endl;
               cout<<"Dparent : "<<string(containfold)<<endl;

               RemoveFile(strname,containfold);
               itemSelected->~QTreeWidgetItem(); //Delete Item
               QMessageBox::information(this,"Deletion",file_folder+" is Deleted");

           }
           else if(type=="folder"){
               if(itemSelected->text(3)!=""){
                  strcpy(strname,( string(containfold)+"/"+string(strname) ).c_str() );
               }

               cout<<"Dname : "<<string(strname)<<endl;
               cout<<"Dparent : "<<string(containfold)<<endl;

               RemoveFolder(strname);
               itemSelected->~QTreeWidgetItem(); //Delete Item
               QMessageBox::information(this,"Deletion",file_folder+" is Deleted with its containing files");
           }
    //Re-Disabling buttons
       initialize_buttons();
}

//new algorithm

void MainWindow::on_copyfolderH_clicked()
{
    setblockseize_available=false;

    QTreeWidgetItem *parentptr/*=new QTreeWidgetItem(tree)*/,*child/*=new QTreeWidgetItem(tree)*/;
    bool flag=false;
    inorderlist Files_list;
    fileslist Obj;
    QString folder_path ="";
    folder_path= QFileDialog::getExistingDirectory(this, tr("Select Folder"),
                                                          ".",
                                                          QFileDialog::ShowDirsOnly
                                                          | QFileDialog::DontResolveSymlinks);
    if(folder_path=="")
        return;
    QString foldername = QFileInfo(folder_path).fileName();

    //convert from QString path to char *
    char *path;

    string fname =folder_path.toStdString();
    path = new char [fname.size()+1];
    strcpy( path, fname.c_str() );


    double THput=CopyFolderFromHDtoVFS("*****",path,Files_list);
    if(THput==-1){
        QMessageBox::warning(this,"WARNING","  NO ENOUGH SPACE IN VFS !");
        initialize_buttons();
        return;
    }
    else if(THput==-2){
        QMessageBox::warning(this,"WARNING","  Path can't exceed 255 letters !");
        initialize_buttons();
        return;
    }

    size_t found;
    flag=Files_list.retrieve_the_first_element(Obj);

    while(flag){//looping over list of filses
        //-----------------test----------------//
        Obj.showData();
        //-----------------------------------//

        string cfolder=string(Obj.retFolder());
        found =cfolder.find("/");

        if(found==std::string::npos){//No slashes in containing folder

            QList<QTreeWidgetItem*> clist = tree->findItems(QString(Obj.retFolder()), Qt::MatchExactly, 0);

            if(!clist.empty() && clist.first()->text(3)==""){//folder is already created in containing folder

                if(QString(Obj.retName())!="*****"){//not empty folder
                    Name=QString(Obj.retName());
                    size=Obj.retSize();
                    date=QString(Obj.retDate());
                    ContainingFolder=QString(Obj.retFolder());//QString
                    type="file";//QString
                    addTreeChild(clist.first(),child);

                }
            }else{//no such folder created /create folder

                Name=QString(Obj.retFolder());
                type="folder";
                ContainingFolder="";
                addTreeRoot(parentptr);

                if(QString(Obj.retName())!="*****"){//not empty folder

                    Name=QString(Obj.retName());
                    size=Obj.retSize();
                    date=QString(Obj.retDate());
                    ContainingFolder=QString(Obj.retFolder());//QString
                    type="file";//QString
                    addTreeChild(parentptr,child);

                }// end if condition for non empty folder-creating file-

            }//end of creatining folder

        }//end of there is no slashes
        else{//there is slashes

            //-----------------test----------------//
            cout<<"1- "<<"slashe found "<<endl;
            //-----------------------------------//

            int counter=0;//number of slashes occurance
            int last_found;
            QString parent_folder="";
            while( found!=std::string::npos ){//getting number of slashes

                if(counter==0){
                    last_found=found;
                }
                counter++;
                found=cfolder.find("/",found+1,1);

            }

            string first_folder=cfolder.substr(0,last_found);

            //check if folder found
            QList<QTreeWidgetItem*> clist = tree->findItems(QString::fromUtf8(first_folder.c_str(),first_folder.length() )/*QString(Obj.retFolder())*/, Qt::MatchExactly, 0);

            bool flag_found=false;
            int index=0;

            if(!clist.empty()){

                for( int i=0; i< clist.size(); i++ ){

                    cout<<" yarab -first folder in the list :"<<( clist.at(i)->text(0) ).toStdString()<<endl;
                   if(clist.at(i)->text(3)==""){
                       index=i;
                       flag_found=true;
                       break;
                   }
                }

            }

            if(flag_found){//folder is already created in containing folder
                //-----------------test----------------//
                cout<<"2- "<<"first folder found "<<endl;
                //-----------------------------------//
                parentptr=clist.at(index);
                flag_found=false;
            }
            else {//no such folder created /create folder
                //-----------------test----------------//
                cout<<"3- "<<"first folder not found "<<endl;
                //-----------------------------------//

                Name=QString::fromUtf8(first_folder.c_str(),first_folder.length());
                type="folder";
                ContainingFolder="";
                addTreeRoot(parentptr);

            }//creating root folder
            parent_folder=QString::fromUtf8(first_folder.c_str(),first_folder.length());

            //-----------------test----------------//
            cout<<"4- counter :"<<counter<<endl;
            //-----------------------------------//
            for(int i=0;i<counter;i++){//loop over slashes

                found=cfolder.find("/",last_found+1,1);
                int end;
                if(found==std::string::npos){//no other slashes
                    end=cfolder.length();
                }else{//get new slash position
                    end=found;
                }

                first_folder=cfolder.substr(last_found+1,end-last_found-1);

                //-----------------test----------------//
                cout<<"5- curr folder is "<<first_folder<<endl;
                //-----------------------------------//
                //-----------------test----------------//
                cout<<"6- curr parent is "<<parent_folder.toStdString()<<endl;
                //-----------------------------------//


                last_found=found;
                Name=QString::fromUtf8(first_folder.c_str(),first_folder.length());
                type="folder";
                ContainingFolder=parent_folder;

                parent_folder=parent_folder+"/"+QString::fromUtf8(first_folder.c_str(),first_folder.length());//new parent folder


                QList<QTreeWidgetItem*> clist = tree->findItems(QString::fromUtf8(first_folder.c_str(),first_folder.length()), Qt::MatchContains | Qt::MatchRecursive, 0);

                ///////////////-----------------edit-------------////
                flag_found=false;

                if(!clist.empty()){

                    for( int i=0; i< clist.size(); i++ ){
                      // qDebug() << (*iter);
                       //int index = iter-clist.begin();
                        cout<<" yarab -folder in the list :"<<( clist.at(i)->text(0) ).toStdString()<<endl;
                       if(clist.at(i)->text(3)==ContainingFolder){
                           flag_found=true;
                           index=i;
                           break;
                       }
                    }

                }

                if(flag_found){//folder is already created in containing folder
                    //-----------------test----------------//
                    cout<<"7- curr folder found "<<first_folder<<endl;
                    //-----------------------------------//
                    parentptr=clist.at(index);
                    flag_found=false;
                }
                else {//no such folder created /create folder
                    //-----------------test----------------//
                    cout<<"8- curr folder not found "<<first_folder<<endl;
                    //-----------------------------------//
                    addTreeChild(parentptr,child);
                    parentptr=child;
                }//creating root folder
            }//looping over number of slashes to create other folders

            if(QString(Obj.retName())!="*****"){//not empty folder
                Name=QString(Obj.retName());
                size=Obj.retSize();
                date=QString(Obj.retDate());
                ContainingFolder=parent_folder;//QString
                type="file";//QString
                addTreeChild(parentptr,child);
            }
        }//end of if condition //there is slashes

        flag=Files_list.retrieve_the_next_element(Obj);
    }//end of looping over files

    //Message Box to confirm Copying operation
    QMessageBox::information(this,"Copying",foldername+" Copied from "+folder_path+tr(" to VFS With Throughput %1").arg(THput).append(" bytes/msec                      ") );

    initialize_buttons();
    //ListFilesDisplay();
}//end of func

void MainWindow::on_copyfileH_clicked()
{
    setblockseize_available=false;

    fileslist Obj;
    QString file_path ="";
    file_path =QFileDialog::getOpenFileName(this,
                                                tr("Choose File"),
                                                ".",
                                                "All File (*.*);;Text File (*.txt);;Music File (*.mp3);;Image(*.png)");
    if(file_path=="")
        return;

    QString filename = QFileInfo(file_path).fileName();

    //convert from QString path to char *
    const char *path;
    QByteArray ba;
    ba = file_path.toLatin1();
    path = ba.data();


    char *folder="*****";
    double THput=CopyFileFromHDtoVFS(folder,path,Obj);

    //check throuput
    if(THput==double(-1)){
        QMessageBox::warning(this,"WARNING","  NO ENOUGH SPACE IN VFS !");
        initialize_buttons();
        return;
    }

    Name=QString(Obj.retName());
    size=Obj.retSize();
    date=QString(Obj.retDate());
    ContainingFolder="";//QString
    type="file";//QString

    //NOC=0;

    //Add root file
    QTreeWidgetItem *treeitem;
    addTreeRoot(treeitem);

    //Message Box to confirm Copying operation
    QMessageBox::information(this,"Copying",filename+" Copied from "+file_path+tr(" to VFS With Throughput %1").arg(THput).append(" bytes/msec                      "));

    initialize_buttons();
    //ListFilesDisplay();


}

void MainWindow::initialize_tree(){//initialize tree at the constuctor by already existing files and folders
    QTreeWidgetItem *parentptr,*child;

    inorderlist List;
    ListFilesDisplay(List);

    fileslist Obj;
    bool flag ;
    flag=List.retrieve_the_first_element(Obj);

    while(flag)
    {
        setblockseize_available=false;

        if(QString(Obj.retFolder()) == "*****"){//file is the the root/has no containing folder
            Name=QString(Obj.retName());
            size=Obj.retSize();
            date=QString(Obj.retDate());
            ContainingFolder="";//QString
            type="file";//QString

            //Add root file
            QTreeWidgetItem *treeitem;
            addTreeRoot(treeitem);

        }//end of adding file to the root
        else{//file is contained in a folder
            size_t found=0;
            string cfolder=string(Obj.retFolder());
            found =cfolder.find("/");
//----------------------------------------------------------------------------------------//
            if(found==std::string::npos){//No slashes in containing folder

                QList<QTreeWidgetItem*> clist = tree->findItems(QString(Obj.retFolder()), Qt::MatchExactly, 0);

                if(!clist.empty() && clist.first()->text(3)==""){//folder is already created in containing folder

                    if(QString(Obj.retName())!="*****"){//not empty folder
                        Name=QString(Obj.retName());
                        size=Obj.retSize();
                        date=QString(Obj.retDate());
                        ContainingFolder=QString(Obj.retFolder());//QString
                        type="file";//QString
                        addTreeChild(clist.first(),child);

                    }
                }else{//no such folder created /create folder

                    Name=QString(Obj.retFolder());
                    type="folder";
                    ContainingFolder="";
                    addTreeRoot(parentptr);

                    if(QString(Obj.retName())!="*****"){//not empty folder

                        Name=QString(Obj.retName());
                        size=Obj.retSize();
                        date=QString(Obj.retDate());
                        ContainingFolder=QString(Obj.retFolder());//QString
                        type="file";//QString
                        addTreeChild(parentptr,child);

                    }// end if condition for non empty folder-creating file-

                }//end of creatining folder

            }//end of there is no slashes
            else{//there is slashes

                //-----------------test----------------//
                cout<<"1- "<<"slashe found "<<endl;
                //-----------------------------------//

                int counter=0;//number of slashes occurance
                int last_found;
                QString parent_folder="";
                while( found!=std::string::npos ){//getting number of slashes

                    if(counter==0){
                        last_found=found;
                    }
                    counter++;
                    found=cfolder.find("/",found+1,1);

                }

                string first_folder=cfolder.substr(0,last_found);

                //check if folder found
                QList<QTreeWidgetItem*> clist = tree->findItems(QString::fromUtf8(first_folder.c_str(),first_folder.length() )/*QString(Obj.retFolder())*/, Qt::MatchExactly, 0);

                bool flag_found=false;
                int index=0;

                if(!clist.empty()){

                    for( int i=0; i< clist.size(); i++ ){

                        cout<<" yarab -first folder in the list :"<<( clist.at(i)->text(0) ).toStdString()<<endl;
                       if(clist.at(i)->text(3)==""){
                           index=i;
                           flag_found=true;
                           break;
                       }
                    }

                }

                if(flag_found){//folder is already created in containing folder
                    //-----------------test----------------//
                    cout<<"2- "<<"first folder found "<<endl;
                    //-----------------------------------//
                    parentptr=clist.at(index);
                    flag_found=false;
                }
                else {//no such folder created /create folder
                    //-----------------test----------------//
                    cout<<"3- "<<"first folder not found "<<endl;
                    //-----------------------------------//

                    Name=QString::fromUtf8(first_folder.c_str(),first_folder.length());
                    type="folder";
                    ContainingFolder="";
                    addTreeRoot(parentptr);

                }//creating root folder
                parent_folder=QString::fromUtf8(first_folder.c_str(),first_folder.length());

                //-----------------test----------------//
                cout<<"4- counter :"<<counter<<endl;
                //-----------------------------------//
                for(int i=0;i<counter;i++){//loop over slashes

                    found=cfolder.find("/",last_found+1,1);
                    int end;
                    if(found==std::string::npos){//no other slashes
                        end=cfolder.length();
                    }else{//get new slash position
                        end=found;
                    }

                    first_folder=cfolder.substr(last_found+1,end-last_found-1);

                    //-----------------test----------------//
                    cout<<"5- curr folder is "<<first_folder<<endl;
                    //-----------------------------------//
                    //-----------------test----------------//
                    cout<<"6- curr parent is "<<parent_folder.toStdString()<<endl;
                    //-----------------------------------//


                    last_found=found;
                    Name=QString::fromUtf8(first_folder.c_str(),first_folder.length());
                    type="folder";
                    ContainingFolder=parent_folder;

                    parent_folder=parent_folder+"/"+QString::fromUtf8(first_folder.c_str(),first_folder.length());//new parent folder


                    QList<QTreeWidgetItem*> clist = tree->findItems(QString::fromUtf8(first_folder.c_str(),first_folder.length()), Qt::MatchContains | Qt::MatchRecursive, 0);

                    ///////////////-----------------edit-------------////
                    flag_found=false;

                    if(!clist.empty()){

                        for( int i=0; i< clist.size(); i++ ){
                          // qDebug() << (*iter);
                           //int index = iter-clist.begin();
                            cout<<" yarab -folder in the list :"<<( clist.at(i)->text(0) ).toStdString()<<endl;
                           if(clist.at(i)->text(3)==ContainingFolder){
                               flag_found=true;
                               index=i;
                               break;
                           }
                        }

                    }

                    if(flag_found){//folder is already created in containing folder
                        //-----------------test----------------//
                        cout<<"7- curr folder found "<<first_folder<<endl;
                        //-----------------------------------//
                        parentptr=clist.at(index);
                        flag_found=false;
                    }
                    else {//no such folder created /create folder
                        //-----------------test----------------//
                        cout<<"8- curr folder not found "<<first_folder<<endl;
                        //-----------------------------------//
                        addTreeChild(parentptr,child);
                        parentptr=child;
                    }//creating root folder
                }//looping over number of slashes to create other folders

                if(QString(Obj.retName())!="*****"){//not empty folder
                    Name=QString(Obj.retName());
                    size=Obj.retSize();
                    date=QString(Obj.retDate());
                    ContainingFolder=parent_folder;//QString
                    type="file";//QString
                    addTreeChild(parentptr,child);
                }
            }//end of if condition //there is slashes




        }//end of file is inside a folder
        flag=List.retrieve_the_next_element(Obj);

    }//while loop over all fat list of files
}//end of function

QString MainWindow::getDirectory()
{ QString path= QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                   "/home",
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);
 return path;
}



void MainWindow::on_Freesize_clicked()
{
    setblockseize_available=false;//-------------------->negotiated

    float free;
        VFS.calculateFB();
        free=FreeBlocks();
        float i=(free/((VFS.retBlocksLastLogicalPart())*1.00));
        Dialog wi;
        wi.SEt_end_one(i);
        QString text1=QString::number(free);
        QString text2=QString::number(VFS.retBlocksLastLogicalPart()-free);

        wi.setTexts("Free "+text1+" blocks","Occupied "+text2+" blocks");
        wi.setWindowTitle("Free Blocks");
        wi.setModal(true);
        wi.exec();

        //QString tmp = QString::number(free);
        //QMessageBox::information(this,"Free Blocks Size",tmp.append("  Bytes    "));

       //QString tmp = QString::number(free);
       //QMessageBox::information(this,"Free Blocks Size",tmp.append("  Bytes    "));

       initialize_buttons();

}

void MainWindow::on_frag_clicked()
{
//    setblockseize_available=false;

    double percent;
       percent=FragmentationPercentage();
       QString tmp = QString::number(percent);
       QMessageBox::information(this,"Fragmentation Percentage",tmp.append("%     "
                                                                           "                                              "));

       initialize_buttons();

}
void MainWindow::on_copyVtoH_clicked()
{

    setblockseize_available=false;

    QString path="";
    path= getDirectory();
    if(path=="")
        return;
    char* pathh;
    string pp =path.toStdString();
    pathh = new char [pp.size()+1];
    strcpy( pathh, pp.c_str() );

    QString FileorFolderName=ui->treeWidget->currentItem()->text(0);


    char* fileorfol;
    string fname =FileorFolderName.toStdString();
    fileorfol = new char [fname.size()+1];
    strcpy( fileorfol, fname.c_str() );

    char *containfold;
    string par=( ui->treeWidget->currentItem()->text(3) ).toStdString();
    containfold=new char[par.size()+1];
    strcpy(containfold,par.c_str());


    QString th="Throughput =  ";


    QString type=   ui->treeWidget->currentItem()->text(4);

    if(type.toStdString()=="file")
    {
        if(ui->treeWidget->currentItem()->text(3)=="")
            containfold="*****";


        cout<<"name : "<<string(fileorfol)<<endl;
        cout<<"parent : "<<string(containfold)<<endl;


        double Through=CopyFileFromVFStoHD(fileorfol,containfold, pathh);
        if(Through==-2){
            QMessageBox::warning(this,"WARNING","  Path can't exceed 255 letters !");
            initialize_buttons();
            return;
        }
        QString Throughput=QString::number(Through);
        QMessageBox::information(this,"Copy File Form VFS to HardDisk",th.append(Throughput+" bytes/msec                     "));
    }
    else if(type.toStdString()=="folder")
    {
        if(ui->treeWidget->currentItem()->text(3)!=""){
           strcpy(fileorfol,( string(containfold)+"/"+string(fileorfol) ).c_str() );
        }

        cout<<"name : "<<string(fileorfol)<<endl;
        cout<<"parent : "<<string(containfold)<<endl;

        double Through=CopyFolderFromVFStoHD( fileorfol, pathh);
        if(Through==-2){
            QMessageBox::warning(this,"WARNING","  Path can't exceed 255 letters !");
            initialize_buttons();
            return;
        }
        QString Throughput=QString::number(Through);

        QMessageBox::information(this,"Copy Folder Form VFS to HardDisk",th.append(Throughput+" bytes/msec                    "));

    }
    initialize_buttons();


}

void MainWindow::on_Addfile_clicked()
{
    setblockseize_available=false;

    Dialog2 x;
    x.setWindowTitle("Text File");
    x.setModal(true);
    x.exec();
    fileslist Obj;
    if(x.returnObj(Obj)){
        Name=QString(Obj.retName());
        size=Obj.retSize();
        date=QString(Obj.retDate());
        ContainingFolder="";//QString
        type="file";//QString

        //Add root file
        QTreeWidgetItem *treeitem;
        addTreeRoot(treeitem);
        QMessageBox ::information(this,"Add",Name+" successfully added to VFS.");

    }
    initialize_buttons();
}

void MainWindow::on_SetBlockSize_clicked()
{
    if(setblockseize_available){//its ok to set block size/at the beginning of the filesystem launch/empty  filesystem
        if( ui->BlockSize->text()=="" ){
            QMessageBox :: warning(this,"WARNING","Please fill the required settings. \n");

        }

        //---------------------------------------------->check is digit

        QString blockstring=(ui->BlockSize->text() );
        const char *blockchar;
        QByteArray ba;
        ba = blockstring.toLatin1();
        blockchar = ba.data();

        int counter=0;
        for(int i=0;i<blockstring.length();i++){//check if its digit
            if(isdigit( blockchar[i] ) )
                counter++;
            else break;
        }
        ////////////////////////////////////////////////////////////
        if(counter!=blockstring.length() ){// not digits
            QMessageBox :: warning(this,"WARNING","Please enter proper Block size. \n");

        }
        else{//digits
            double block=(ui->BlockSize->text() ).toDouble();
            if(block==256 || block==512 ||block==1024 || block== 2048){//check block size
                //------------------>call main function to set block size
                setblockseize_available=false;
                VFS.setBlockSize(block);
                QMessageBox :: information(this,"Edited",tr("Block size is set to be %1").arg(( ui->BlockSize->text() ).toDouble() ).append(" Bytes.") );
            }
            else{
                QMessageBox :: warning(this,"WARNING","Please enter proper Block size (256,512,1024,2048). \n");

            }
        }
    }
    else{//already set or filesystem has been used or file system is not empty
        QMessageBox ::critical(this, "Rejected", "Unable to Edit Block size \n");

    }
        initialize_buttons();
}



void MainWindow::on_DisplayList_clicked()
{
   list_files listss;
   listss.dispaly_files();
   listss.setWindowTitle("Display List");
   listss.setModal(true);
   listss.exec();
}
