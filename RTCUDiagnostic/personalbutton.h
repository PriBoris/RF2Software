#ifndef PERSONALBUTTON_H
#define PERSONALBUTTON_H

#include <QPushButton>
#include <QTimer>
#include <QDebug>

class PersonalButton : public QPushButton
{
    Q_OBJECT

    QTimer *timer;
    static const int holdInterval = 100;
    bool isPressed;


public:
    PersonalButton(const QString & text, QWidget * parent = 0);

signals:
    void hold();

public slots:

    void slotPressed();
    void slotReleased();
    void slotHoldTimeout();



};

#endif // PERSONALBUTTON_H
