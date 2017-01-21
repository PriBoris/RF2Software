#include "WidgetGenericSetSettings.h"


WidgetGenericSetSettings::WidgetGenericSetSettings(
	SerialPortTransceiver *serialPortTransceiver,
	QWidget *parent
	) : QWidget(parent){

	serialPortTransceiver_ = serialPortTransceiver;

	{
		rxMessageCounter = 0;
		lblRxMessageCounter = new QLabel("lblRxMessageCounter");
		lblRxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));
	}

	{
		wgtPause1 = new WidgetSettingsInteger("Pause1","GenericSet_Pause1",1000);
		wgtPause2 = new WidgetSettingsInteger("Pause2","GenericSet_Pause2",1000);
		wgtStartPositionRel = new WidgetSettingsInteger("StartPosition","GenericSet_StartPosition",5000);
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

	loMain = new QVBoxLayout;

	loMain->addWidget(lblRxMessageCounter);
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
	loMain->addWidget(btnWriteSettings);

	loMain->addStretch(1);

	this->setLayout(loMain);


	connect(wgtMoveCount,SIGNAL(signalEditingFinished()),this,SLOT(slotMoveCountUpdated()));
	slotMoveCountUpdated();

	connect(btnWriteSettings,SIGNAL(clicked(bool)),SLOT(slotWriteSettings()));


}
//=================================================================================================
void WidgetGenericSetSettings::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){

	if (tag==TLV::TAG_ReportGenericSetSettings){
		
		rxMessageCounter++;
		lblRxMessageCounter->setText(
					"Сообщений: "+QString::number(rxMessageCounter)+" "
					+"(длина последнего = "
					+QString::number(value.length())
					+" байт)"
					);

		if (checkMessageLength(value.length())==true){


			Set newSet;
			memcpy(&newSet,value.data(),value.length());

			if (newSet.moveCount == getMoveCountFromLength(value.length())){

				wgtPause1->setReadValue(newSet.pause1);
				wgtPause2->setReadValue(newSet.pause2);
				wgtStartPositionRel->setReadValue(newSet.startPosition);
				wgtMoveCount->setReadValue(newSet.moveCount);

                for(int move=0;move<MOVE_COUNT_MAX;move++){

                    if (move<newSet.moveCount){
                        wgtsMove.at(move)->setReadValue(
                        	newSet.moves[move].destinationPosition,
                        	newSet.moves[move].speed
                        	);
                    }else{
                        wgtsMove.at(move)->setUnknownReadValues();
                    }
                }
			}else{
				//bad message
			}

		}else{
			//bad message
		}






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



	}

	
}
//=================================================================================================


