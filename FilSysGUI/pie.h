#ifndef PIE_H
#define PIE_H

#include <QWidget>

class pie : public QWidget
{
    Q_OBJECT
public:
    explicit pie(QWidget *parent = 0);
    void SetEnd(float end_one);

 protected:
    void paintEvent(QPaintEvent*);

private:
float end_One;

signals:

public slots:
};

#endif // PIE_H
