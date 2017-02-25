#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QLabel>

class Utils{


public:

    static void MessageCounterIncrement(
            QString prefix,
            quint32 &counter,
            QLabel *lbl,
            QByteArray &messageValue
            );
    static void MessageCounterInitialize(
            QString prefix,
            quint32 &counter,
            QLabel *lbl
            );


};

#endif // UTILS_H
