#include "Utils.h"

#include <QDebug>

//=================================================================================================
void Utils::MessageCounterIncrement(
        QString prefix,
        quint32 &counter,
        QLabel *lbl,
        QByteArray &messageValue
        ){

    counter++;
    lbl->setText(
                prefix + " messages counter: "+QString::number(counter)+" "
                +"(latest length = "
                +QString::number(messageValue.length())
                +" bytes)"
                );


}
//=================================================================================================
void Utils::MessageCounterInitialize(
        QString prefix,
        quint32 &counter,
        QLabel *lbl
        ){

    counter = 0;
    lbl->setText(prefix+" messages counter: "+QString::number(counter));


}
//=================================================================================================

