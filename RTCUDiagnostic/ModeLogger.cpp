#include "ModeLogger.h"

ModeLogger::ModeLogger(){
    file = new QFile("ReportCurrentMode.txt");
    file->open(QIODevice::WriteOnly | QIODevice::Append);
    stream.setDevice(file);
    stream << "\n\n\n";
}

ModeLogger::~ModeLogger(){

    file->close();
    delete file;

}
void ModeLogger::flush(){

    static int counter = 0;

    counter++;
    if (counter==20){
        counter = 0;
        file->close();
        file->open(QIODevice::WriteOnly | QIODevice::Append);
        stream.setDevice(file);
    }

}


