#ifndef MODELOGGER_H
#define MODELOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>



class ModeLogger
{
    QFile *file;

public:
    ModeLogger();
    ~ModeLogger();

    QTextStream stream;
    void flush();

};

#endif // MODELOGGER_H
