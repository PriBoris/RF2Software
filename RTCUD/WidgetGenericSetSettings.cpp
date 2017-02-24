#include "WidgetGenericSetSettings.h"

#include "Utils.h"

#include <QDateTime>


//=================================================================================================
WidgetGenericSetSettings::~WidgetGenericSetSettings(){
    delete reportLogger;
}
//=================================================================================================
WidgetGenericSetSettings::WidgetGenericSetSettings(
	SerialPortTransceiver *serialPortTransceiver,
	QWidget *parent
	) : QWidget(parent){

	serialPortTransceiver_ = serialPortTransceiver;

    reportLogger = new ReportLogger("ReportGenericSetSettings.txt");

    {
        lblRxMessageCounter = new QLabel;
        lblTxMessageCounter = new QLabel;

        lblRxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));
        lblTxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));

        Utils::MessageCounterInitialize("Rx",rxMessageCounter,lblRxMessageCounter);
        Utils::MessageCounterInitialize("Tx",txMessageCounter,lblTxMessageCounter);

    }




	{
		wgtPause1 = new WidgetSettingsInteger("Pause1","GenericSet_Pause1",1000);
		wgtPause2 = new WidgetSettingsInteger("Pause2","GenericSet_Pause2",1000);
		wgtStartPositionRel = new WidgetSettingsInteger("StartPositionRel","GenericSet_StartPositionRel",5000);
		wgtMoveCount = new WidgetSettingsInteger("MoveCount","GenericSet_MoveCount",10);

		wgtPause1->setValidator(new QIntValidator(0,60000));
		wgtPause2->setValidator(new QIntValidator(0,60000));
		wgtStartPositionRel->setValidator(new QIntValidator(0,10000));
		wgtMoveCount->setValidator(new QIntValidator(1,50));

		for(int move=0;move<MOVE_COUNT_MAX;move++){
			wgtsMove.append(new WidgetGenericMoveSettings(
				"Move"+QString::number(move),
				"GenericSet_Move"+QString::number(move),
				5000,//destinationPositionRel
				6000//speed
				));
			wgtsMove.at(move)->setValidators(
				new QIntValidator(0,10000),
				new QIntValidator(1000,100000)
			 	);    

		}

	}

	{
		btnWriteSettings = new QPushButton("Send Generic Set Settings");
		btnWriteSettings->setFixedWidth(600);
		btnWriteSettings->setFont(QFont("Verdana",10,QFont::Bold,true));

	}

	{

		plotPositionVsTime = new QCustomPlot;
		plotPositionVsTime->addGraph();
		plotPositionVsTime->graph(0)->setPen(QPen(Qt::blue));
		plotPositionVsTime->yAxis->setVisible(true);
		plotPositionVsTime->setFixedSize(700,200);
        plotPositionVsTime->xAxis->setRange(0, 1.0);
		plotPositionVsTime->yAxis->setRange(0.0, 100.0);
		plotPositionVsTime->xAxis->setLabel("time [s]");
		plotPositionVsTime->yAxis->setLabel("position [%]");
        plotPositionVsTime->graph(0)->setLineStyle(QCPGraph::lsLine);
        plotPositionVsTime->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));


	}


	loMain = new QVBoxLayout;

	loMain->addWidget(lblRxMessageCounter);
	loMain->addWidget(lblTxMessageCounter);
	loMain->addSpacing(10);

	loMain->addWidget(wgtPause1);
	loMain->addWidget(wgtPause2);
	loMain->addWidget(wgtStartPositionRel);
	loMain->addWidget(wgtMoveCount);
	loMain->addSpacing(10);

	for(int move=0;move<MOVE_COUNT_MAX;move++){
		loMain->addWidget(wgtsMove.at(move));
	}
	loMain->addSpacing(10);
	loMain->addWidget(plotPositionVsTime);
	loMain->addSpacing(10);
	loMain->addWidget(btnWriteSettings);

	loMain->addStretch(1);

	this->setLayout(loMain);


    connect(wgtMoveCount,SIGNAL(signalEditingFinished()),this,SLOT(slotMoveCountUpdated()));
	slotMoveCountUpdated();

	connect(btnWriteSettings,SIGNAL(clicked(bool)),SLOT(slotWriteSettings()));


	connect(wgtPause1,SIGNAL(signalEditingFinished()),SLOT(slotEditingFinished()));
	connect(wgtPause2,SIGNAL(signalEditingFinished()),SLOT(slotEditingFinished()));
	connect(wgtStartPositionRel,SIGNAL(signalEditingFinished()),SLOT(slotEditingFinished()));
	connect(wgtMoveCount,SIGNAL(signalEditingFinished()),SLOT(slotEditingFinished()));
	for(int move=0;move<MOVE_COUNT_MAX;move++){
		connect(wgtsMove.at(move),SIGNAL(signalEditingFinished()),SLOT(slotEditingFinished()));
	}


    slotEditingFinished();


}
//=================================================================================================
void WidgetGenericSetSettings::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){

	if (tag==TLV::TAG_ReportGenericSetSettings){
		
        Utils::MessageCounterIncrement("Rx",rxMessageCounter,lblRxMessageCounter,value);

        {
            QDateTime dateTime(QDateTime::currentDateTime());
            QString pcTimeStr = dateTime.toString("yyyy-MM-dd HH:mm:ss");
            (reportLogger->stream) << pcTimeStr << "\n";
        }


		if (checkMessageLength(value.length())==true){


			Set newSet;
			memcpy(&newSet,value.data(),value.length());

			if (newSet.moveCount == getMoveCountFromLength(value.length())){

				wgtPause1->setReadValue(newSet.pause1);
				wgtPause2->setReadValue(newSet.pause2);
                wgtStartPositionRel->setReadValue(newSet.startPositionRel);
				wgtMoveCount->setReadValue(newSet.moveCount);

				(reportLogger->stream) << "pause1=" << QString::number(newSet.pause1) << "\n";
				(reportLogger->stream) << "pause2=" << QString::number(newSet.pause2) << "\n";
                (reportLogger->stream) << "startPositionRel=" << QString::number(newSet.startPositionRel) << "\n";
				(reportLogger->stream) << "moveCount=" << QString::number(newSet.moveCount) << "\n";

				for(int move=0;move<MOVE_COUNT_MAX;move++){

					if (move<newSet.moveCount){
						wgtsMove.at(move)->setReadValue(
                            newSet.moves[move].destinationPositionRel,
                            newSet.moves[move].duration
							);

						(reportLogger->stream) << "moveIndex=" << QString::number(move) << "\n";
                        (reportLogger->stream) << "\t" << "destinationPositionRel=" << QString::number(newSet.moves[move].destinationPositionRel) << "\n";
                        (reportLogger->stream) << "\t" << "duration=" << QString::number(newSet.moves[move].duration) << "\n";

					}else{

						wgtsMove.at(move)->setUnknownReadValues();
						
					}
				}
			}else{

				(reportLogger->stream) << "bad message\n";


				//bad message
				wgtPause1->setUnknownReadValue();
				wgtPause2->setUnknownReadValue();
				wgtStartPositionRel->setUnknownReadValue();
				wgtMoveCount->setUnknownReadValue();
				for(int move=0;move<MOVE_COUNT_MAX;move++){
					wgtsMove.at(move)->setUnknownReadValues();
				}
			}

		}else{
			//empty message

			wgtPause1->setUnknownReadValue();
			wgtPause2->setUnknownReadValue();
			wgtStartPositionRel->setUnknownReadValue();
			wgtMoveCount->setUnknownReadValue();
			for(int move=0;move<MOVE_COUNT_MAX;move++){
				wgtsMove.at(move)->setUnknownReadValues();
			}

		}

        (reportLogger->stream) << "\n";
        reportLogger->flush(true);





	}

}

//=================================================================================================
bool WidgetGenericSetSettings::checkMessageLength(int messageLength){

	const int moveLength = sizeof(qint32)*2;
	const int headerLength = sizeof(qint32)*4;

	int minLength = headerLength + 1*moveLength;
	int maxLength = headerLength + MOVE_COUNT_MAX*moveLength;

	if (messageLength<minLength){
		return false;
	}else if (messageLength>maxLength){
		return false;
	}else{
		return ((messageLength-headerLength)%moveLength)==0;
	}

}
//=================================================================================================
qint32 WidgetGenericSetSettings::getMoveCountFromLength(int messageLength){

	const int moveLength = sizeof(qint32)*2;
	const int headerLength = sizeof(qint32)*4;

	return (messageLength - headerLength) / moveLength;

}
//=================================================================================================
void WidgetGenericSetSettings::slotMoveCountUpdated(){

	qint32 moveCount = wgtMoveCount->getWriteValue();

	for(int move=0;move<MOVE_COUNT_MAX;move++){
		wgtsMove.at(move)->setVisible(move<moveCount);
	}

}
//=================================================================================================
void WidgetGenericSetSettings::slotWriteSettings(){


	if (serialPortTransceiver_->isPortOK()==true){

		QByteArray valueArray;

		qint32 pause1 = wgtPause1->getWriteValue();
		qint32 pause2 = wgtPause2->getWriteValue();
		qint32 startPositionRel = wgtStartPositionRel->getWriteValue();
		qint32 moveCount = wgtMoveCount->getWriteValue();
		
		valueArray.append((char*)&pause1,sizeof(pause1));
		valueArray.append((char*)&pause2,sizeof(pause2));
		valueArray.append((char*)&startPositionRel,sizeof(startPositionRel));
		valueArray.append((char*)&moveCount,sizeof(moveCount));

		for(qint32 move = 0;move<moveCount;move++){

			qint32 destinationPosition = wgtsMove.at(move)->getDestinationPositionWriteValue();
			qint32 speed = wgtsMove.at(move)->getSpeedWriteValue();

			valueArray.append((char*)&destinationPosition,sizeof(destinationPosition));
			valueArray.append((char*)&speed,sizeof(speed));
		}



		TLVWriter tlv(TLV::TAG_LoadGenericSetSettings,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;

        Utils::MessageCounterIncrement("Tx",txMessageCounter,lblTxMessageCounter,valueArray);






	}

	
}
//=================================================================================================
void WidgetGenericSetSettings::slotEditingFinished(){

	qint32 pause1 = wgtPause1->getWriteValue();
	qint32 pause2 = wgtPause2->getWriteValue();
	qint32 startPositionRel = wgtStartPositionRel->getWriteValue();
	qint32 moveCount = wgtMoveCount->getWriteValue();

    QVector<double> plotTime,plotPosition;
	double timeS = 0.0;

	plotTime.append(timeS);
    plotPosition.append((double)startPositionRel/100.0);

	timeS += (double)pause2/1000.0;
	plotTime.append(timeS);
    plotPosition.append((double)startPositionRel/100.0);

    for(qint32 move = 0;move<moveCount;move++){

        qint32 destinationPosition = wgtsMove.at(move)->getDestinationPositionWriteValue();
        qint32 speed = wgtsMove.at(move)->getSpeedWriteValue();

        timeS += (double)speed/1000.0;
        plotTime.append(timeS);
        plotPosition.append((double)destinationPosition/100.0);


    }



    plotPositionVsTime->xAxis->setRange(0.0, timeS);


	plotPositionVsTime->graph(0)->setData(plotTime, plotPosition);
	plotPositionVsTime->replot();


}
//=================================================================================================


