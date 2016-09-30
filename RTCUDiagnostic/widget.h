#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = 0);
    virtual QString getName()=0;
    virtual bool isBBWidget()=0;

signals:

public slots:
};

#endif // WIDGET_H
