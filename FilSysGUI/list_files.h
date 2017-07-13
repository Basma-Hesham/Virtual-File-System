#ifndef LIST_FILES_H
#define LIST_FILES_H

#include <QDialog>

namespace Ui {
class list_files;
}

class list_files : public QDialog
{
    Q_OBJECT

public:
    explicit list_files(QWidget *parent = 0);
     void dispaly_files();
    ~list_files();

private slots:
     void on_pushButton_clicked();

     void on_pushButton_2_clicked();

private:
    Ui::list_files *ui;
};

#endif // LIST_FILES_H
