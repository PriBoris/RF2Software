#ifndef RTCU_H
#define RTCU_H

#include <QObject>
#include <QMap>

class RTCU{

public:
    static void init();


    class Mode{
    private:
    public:
        static const quint8 INITIALIZING = 0;
        static const quint8 IDLE = 1;
        static const quint8 WAITING = 2;
        static const quint8 PARKING = 3;
        static const quint8 PERSONAL = 4;
        static const quint8 TEST_CONCENTRIC = 5;
        static const quint8 TEST_ECCENTRIC = 6;
        static const quint8 EXCERCISE_ISOKINETIC = 7;
        static const quint8 FAULT = 8;
        static const quint8 TEST_STATIC = 9;
        static const quint8 GENERIC_EXERCISE_ISOKINETIC = 10;

        static QMap<quint8,QString> titles;
        static QString getTitle(quint8 index);
    };
    
};

#endif // RTCU_H
