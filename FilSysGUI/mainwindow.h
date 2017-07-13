
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"mainFunctions.h"
#include"fileSystem.h"
#include <QMainWindow>
#include<QTreeWidgetItem>
#include<QTreeWidget>
#include<string>

using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initialize_buttons();//Disabling buttons
    void initialize_tree();
    QString getDirectory();

    void addTreeRoot(QTreeWidgetItem * &root);
    void addTreeChild(QTreeWidgetItem *parent,QTreeWidgetItem * &child);


private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_Delete_clicked();
    void on_copyfolderH_clicked();
    void on_copyfileH_clicked();
    void on_Freesize_clicked();
    void on_frag_clicked();
    void on_copyVtoH_clicked();
    void on_Addfile_clicked();
    void on_SetBlockSize_clicked();

    void on_DisplayList_clicked();

private:

    Ui::MainWindow *ui;
    QTreeWidget *tree;
    QString Name,type,date,ContainingFolder;
    double size;//,NOC;//NOC is number of childs
    QTreeWidgetItem *itemSelected;// Pointer to Selected Item
    inorderlist arbitlist;
    bool setblockseize_available;

};

#endif // MAINWINDOW_H
