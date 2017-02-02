#include "ReportLogger.h"

ReportLogger::ReportLogger(QString fileName){
    file = new QFile(fileName);
    file->open(QIODevice::WriteOnly | QIODevice::Append);
    stream.setDevice(file);
}

ReportLogger::~ReportLogger(){

    file->close();
    delete file;

}
void ReportLogger::flush(bool force){

    static int counter = 0;

    if (force==true){
        file->close();
        file->open(QIODevice::WriteOnly | QIODevice::Append);
        stream.setDevice(file);
    }else{
        counter++;
        if (counter==20){
            counter = 0;
            file->close();
            file->open(QIODevice::WriteOnly | QIODevice::Append);
            stream.setDevice(file);
        }

    }


}


