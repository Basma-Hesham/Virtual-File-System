#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    void setTexts(QString text1,QString text2);
      void SEt_end_one(float end);
    ~Dialog();

private:
     float END;
    Ui::Dialog *ui;
};

#endif // DIALOG_H
