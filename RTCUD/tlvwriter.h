#ifndef TLVWRITER_H
#define TLVWRITER_H

#include <QObject>

#include "TLV.h"

#include "ReportLogger.h"


class TLVWriter : public QObject,TLV
{
	Q_OBJECT

	QByteArray tagLenValueCrc_;
	QByteArray tagLenValueCrcStuffed_;

	static quint32 txID;

	ReportLogger *reportLogger;


public:
	explicit TLVWriter(const quint8 tag,QByteArray &value,QObject *parent = 0);

	QByteArray* getStuffedArray();


signals:

public slots:
};

#endif // TLVWRITER_H
