#ifndef MODELOGGER_H
#define MODELOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>



class ReportLogger
{
    QFile *file;

public:
    ReportLogger(QString fileName);
    ~ReportLogger();

    QTextStream stream;
    void flush(bool force);

};

#endif // MODELOGGER_H
