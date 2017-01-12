#include "ReportLogger.h"

ReportLogger::ReportLogger(QString fileName){
    file = new QFile(fileName);
    file->open(QIODevice::WriteOnly | QIODevice::Append);
    stream.setDevice(file);
    stream << "\n\n\n";
}

ReportLogger::~ReportLogger(){

    file->close();
    delete file;

}
void ReportLogger::flush(){

    static int counter = 0;

    counter++;
    if (counter==20){
        counter = 0;
        file->close();
        file->open(QIODevice::WriteOnly | QIODevice::Append);
        stream.setDevice(file);
    }

}


