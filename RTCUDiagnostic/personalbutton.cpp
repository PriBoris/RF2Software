#include "personalbutton.h"

PersonalButton::PersonalButton(const QString & text, QWidget * parent)
    : QPushButton(text,parent)
{
    isPressed = false;
    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(holdInterval);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotHoldTimeout()));
    connect(this, SIGNAL(pressed()), this, SLOT(slotPressed()));
    connect(this, SIGNAL(released()), this, SLOT(slotReleased()));



}

void PersonalButton::slotPressed()
{
    //qDebug() << "PersonalButton::slotPressed";
    isPressed = true;
    timer->start();
}

void PersonalButton::slotReleased()
{
    isPressed = false;
    //qDebug() << "PersonalButton::slotReleased";
    //qDebug() << "";
}

void PersonalButton::slotHoldTimeout()
{
    if (isPressed==true)
    {
        //qDebug() << "PersonalButton::slotHoldTimeout";
        emit hold();
        timer->start();
    }
    else
    {

    }



}


