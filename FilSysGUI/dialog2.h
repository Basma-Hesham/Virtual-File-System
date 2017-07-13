#ifndef DIALOG2_H
#define DIALOG2_H
#include"mainFunctions.h"
#include <QDialog>

namespace Ui {
class Dialog2;
}

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget *parent = 0);
    ~Dialog2();
    bool returnObj(fileslist &Obj);


private slots:
    void on_buttonBox_2_accepted();

    void on_buttonBox_2_rejected();

private:
    fileslist fileObj;
    bool added;
    Ui::Dialog2 *ui;
};

#endif // DIALOG2_H
