#ifndef LIST_OF_FILES_H
#define LIST_OF_FILES_H

#include <QWidget>

class list_of_files : public QWidget
{
    Q_OBJECT
public:
    explicit list_of_files(QWidget *parent = 0);
    void display_files();

signals:

public slots:
};

#endif // LIST_OF_FILES_H
