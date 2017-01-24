#include "WidgetMode.h"

#include <QDateTime>

#include "RTCU.h"

//=================================================================================================
WidgetMode::~WidgetMode(){
	delete reportLogger;
}
//=================================================================================================
WidgetMode::WidgetMode(
		SerialPortTransceiver *serialPortTransceiver,
		QWidget *parent
		) : QWidget(parent){


	serialPortTransceiver_ = serialPortTransceiver;

	reportLogger = new ReportLogger("ReportCurrentMode.txt");


	{
		rxMessageCounter = 0;
		lblRxMessageCounter = new QLabel("lblRxMessageCounter");
		lblRxMessageCounter->setFont(QFont("Verdana",10,QFont::Normal,true));
	}

	{
		lblDateTime = new QLabel("lblDateTime");
		lblDateTime->setFont(QFont("Verdana",10,QFont::Normal,false));
	}

	{
		lblMode = new QLabel("lblMode");
		lblMode->setFont(QFont("Verdana",16,QFont::Bold,false));
	}

	{
		lblPhase = new QLabel("lblPhase");
		lblPhase->setFont(QFont("Verdana",10,QFont::Normal,false));
	}

	{
		lblError = new QLabel("lblError");
		lblError->setStyleSheet("font: 16pt;");
	}

	{
		lblPosition = new QLabel("lblPosition");
		lblPosition->setFont(QFont("Verdana",10,QFont::Normal,false));
	}
	{
		lblPositionRel = new QLabel("lblPositionRel");
		lblPositionRel->setFont(QFont("Verdana",10,QFont::Normal,false));
	}
	{
		lblTimeToTest = new QLabel("lblTimeToTest");
		lblTimeToTest->setFont(QFont("Verdana",10,QFont::Normal,false));
	}
	{
		lblForceValue = new QLabel("lblForceValue");
		lblForceValue->setFont(QFont("Verdana",16,QFont::Normal,false));
	}
	{
		lblOdometer = new QLabel("lblOdometer");
		lblOdometer->setFont(QFont("Verdana",10,QFont::Normal,false));
	}


	{
		lblSetIndex = new QLabel("lblSetIndex");
		lblSetIndex->setFont(QFont("Verdana",10,QFont::Normal,true));
	}
	{
		lblRepIndex = new QLabel("lblRepIndex");
		lblRepIndex->setFont(QFont("Verdana",10,QFont::Normal,true));
	}
	{
		lblRepDirection = new QLabel("lblRepDirection");
		lblRepDirection->setFont(QFont("Verdana",10,QFont::Normal,true));
	}
	{
		lblTimeToSet = new QLabel("lblTimeToSet");
		lblTimeToSet->setFont(QFont("Verdana",10,QFont::Normal,true));
	}

	{
		btnEnableServo = new QPushButton("Enable SERVO");btnEnableServo->setFixedWidth(300);
		btnParking = new QPushButton("PARKING start");btnParking->setFixedWidth(300);
		btnPersonalStart = new QPushButton("PERSONAL start");btnPersonalStart->setFixedWidth(300);
		btnPersonalExit = new QPushButton("PERSONAL exit");btnPersonalExit->setFixedWidth(300);

		btnEnableServo->setFont(QFont("Verdana",10,QFont::Bold,true));
		btnParking->setFont(QFont("Verdana",10,QFont::Bold,true));
		btnPersonalStart->setFont(QFont("Verdana",10,QFont::Bold,true));
		btnPersonalExit->setFont(QFont("Verdana",10,QFont::Bold,true));

	}

	{
		btnPersonalMainPlus = new PersonalButton("Main+");
		btnPersonalMainMinus = new PersonalButton("Main-");
		btnPersonalAux1Plus = new PersonalButton("Aux1+");
		btnPersonalAux1Minus = new PersonalButton("Aux1-");
		btnPersonalAux2Plus = new PersonalButton("Aux2+");
		btnPersonalAux2Minus = new PersonalButton("Aux2-");
		//btnPersonalAux3Plus = new PersonalButton("Aux3+");
		//btnPersonalAux3Minus = new PersonalButton("Aux3-");

		btnPersonalMainPlus->setFixedWidth(75);
		btnPersonalMainMinus->setFixedWidth(75);
		btnPersonalAux1Plus->setFixedWidth(75);
		btnPersonalAux1Minus->setFixedWidth(75);
		btnPersonalAux2Plus->setFixedWidth(75);
		btnPersonalAux2Minus->setFixedWidth(75);
		//btnPersonalAux3Plus->setFixedWidth(50);
		//btnPersonalAux3Minus->setFixedWidth(50);

		btnPersonalMainPlus->setFont(QFont("Verdana",10,QFont::Bold,true));
		btnPersonalMainMinus->setFont(QFont("Verdana",10,QFont::Bold,true));
		btnPersonalAux1Plus->setFont(QFont("Verdana",10,QFont::Bold,true));
		btnPersonalAux1Minus->setFont(QFont("Verdana",10,QFont::Bold,true));
		btnPersonalAux2Plus->setFont(QFont("Verdana",10,QFont::Bold,true));
		btnPersonalAux2Minus->setFont(QFont("Verdana",10,QFont::Bold,true));

		loPersonal = new QHBoxLayout;
		loPersonal->addWidget(btnPersonalMainPlus);
		loPersonal->addWidget(btnPersonalMainMinus);
		loPersonal->addWidget(btnPersonalAux1Plus);
		loPersonal->addWidget(btnPersonalAux1Minus);
		loPersonal->addWidget(btnPersonalAux2Plus);
		loPersonal->addWidget(btnPersonalAux2Minus);
		//loPersonal->addWidget(btnPersonalAux3Plus);
		//loPersonal->addWidget(btnPersonalAux3Minus);
		loPersonal->addStretch(1);
		wgtPersonal = new QWidget;
		wgtPersonal->setLayout(loPersonal);
	}


	btnTestConcentricStart = new WidgetTestDynamicStart(
				"Test Concentric Start",
				"TestConcentricPause",
				3000
				);
	btnTestEccentricStart = new WidgetTestDynamicStart(
				"Test Eccentric Start",
				"TestEccentricPause",
				3000
				);
	btnTestStaticStart = new WidgetTestStaticStart(
				"Test Static Start",
				"TestStaticPause",
				2000,
				"TestStaticDuration",
				6000,
				"TestStaticPositionRel",
				4500
				);


	{
		btnResetError = new QPushButton("Reset Error");
		btnResetError->setFixedWidth(300);
		btnResetError->setFont(QFont("Verdana",10,QFont::Bold,true));
	}

	{    
		btnExcerciseIsokineticStart = new QPushButton("Start Isokinetic Excercise");
		btnExcerciseIsokineticStart->setFixedWidth(300);
		btnExcerciseIsokineticStart->setFont(QFont("Verdana",10,QFont::Bold,true));
	}

	{    
		btnGenericSetStart = new QPushButton("Start Generic Set");
		btnGenericSetStart->setFixedWidth(300);
		btnGenericSetStart->setFont(QFont("Verdana",10,QFont::Bold,true));
	}




	{
		btnCancel = new QPushButton("CANCEL");btnCancel->setFixedWidth(300);
		btnCancel->setFont(QFont("Verdana",10,QFont::Bold,true));
	}



	{
		plotForceVsPosition = new QCustomPlot;
		plotForceVsPosition->addGraph();
		plotForceVsPosition->graph(0)->setPen(QPen(Qt::blue));
		plotForceVsPosition->yAxis->setVisible(true);
		plotForceVsPosition->setFixedSize(700,200);
		plotForceVsPosition->xAxis->setRange(-20, 120);
		plotForceVsPosition->yAxis->setLabel("force [kg]");
		plotForceVsPosition->xAxis->setLabel("relative position [%]");
		plotForceVsPosition->graph(0)->setLineStyle(QCPGraph::lsNone);
		plotForceVsPosition->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

		plotForceVsTime = new QCustomPlot;
		plotForceVsTime->addGraph();
		plotForceVsTime->graph(0)->setPen(QPen(Qt::red));
		plotForceVsTime->yAxis->setVisible(true);
		plotForceVsTime->setFixedSize(700,200);
		plotForceVsTime->xAxis->setRange(0, 1);
		plotForceVsTime->yAxis->setLabel("force [kg]");
		plotForceVsTime->xAxis->setLabel("time [1/10s]");
		plotForceVsTime->graph(0)->setLineStyle(QCPGraph::lsNone);
		plotForceVsTime->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));


		btnPlotForceReset = new QPushButton("clear plot");btnPlotForceReset->setFixedWidth(300);
		btnPlotForceReset->setFont(QFont("Verdana",10,QFont::Normal,true));
		plotTime = 0;

	}


	//--------------------LAYOUT---------------------------


	loMain = new QVBoxLayout;
	loMain->addWidget(lblRxMessageCounter);
	loMain->addWidget(lblDateTime);
	loMain->addWidget(lblOdometer);
	loMain->addWidget(lblMode);
	loMain->addWidget(lblPhase);
	loMain->addWidget(lblError);
	loMain->addWidget(lblPosition);
	loMain->addWidget(lblPositionRel);
	loMain->addWidget(lblTimeToTest);
	loMain->addWidget(lblForceValue);
	loMain->addWidget(lblSetIndex);
	loMain->addWidget(lblRepIndex);
	loMain->addWidget(lblRepDirection);
	loMain->addWidget(lblTimeToSet);
	loMain->addWidget(btnEnableServo);
	loMain->addWidget(btnParking);
	loMain->addWidget(wgtPersonal);
	loMain->addWidget(btnPersonalStart);
	loMain->addWidget(btnPersonalExit);
	loMain->addWidget(btnTestConcentricStart);
	loMain->addWidget(btnTestEccentricStart);
	loMain->addWidget(btnTestStaticStart);
	loMain->addWidget(btnResetError);
	loMain->addWidget(btnExcerciseIsokineticStart);
	loMain->addWidget(btnGenericSetStart);
	loMain->addWidget(btnCancel);

	loMain->addWidget(plotForceVsPosition);
	loMain->addWidget(plotForceVsTime);
	loMain->addWidget(btnPlotForceReset);


	loMain->addStretch(1);

	this->setLayout(loMain);

	connect(btnEnableServo,SIGNAL(clicked(bool)),SLOT(slotEnableServo()));
	connect(btnParking,SIGNAL(clicked(bool)),SLOT(slotParking()));


	signalMapperPressed = new QSignalMapper(this);
	connect(btnPersonalMainPlus, SIGNAL(pressed()), signalMapperPressed, SLOT(map()));
	connect(btnPersonalMainMinus, SIGNAL(pressed()), signalMapperPressed, SLOT(map()));
	connect(btnPersonalAux1Plus, SIGNAL(pressed()), signalMapperPressed, SLOT(map()));
	connect(btnPersonalAux1Minus, SIGNAL(pressed()), signalMapperPressed, SLOT(map()));
	connect(btnPersonalAux2Plus, SIGNAL(pressed()), signalMapperPressed, SLOT(map()));
	connect(btnPersonalAux2Minus, SIGNAL(pressed()), signalMapperPressed, SLOT(map()));
	//connect(btnPersonalAux3Plus, SIGNAL(pressed()), signalMapperPressed, SLOT(map()));
	//connect(btnPersonalAux3Minus, SIGNAL(pressed()), signalMapperPressed, SLOT(map()));
	signalMapperPressed->setMapping(btnPersonalMainPlus, (int)0);
	signalMapperPressed->setMapping(btnPersonalMainMinus, (int)1);
	signalMapperPressed->setMapping(btnPersonalAux1Plus, (int)2);
	signalMapperPressed->setMapping(btnPersonalAux1Minus, (int)3);
	signalMapperPressed->setMapping(btnPersonalAux2Plus, (int)4);
	signalMapperPressed->setMapping(btnPersonalAux2Minus, (int)5);
	//signalMapperPressed->setMapping(btnPersonalAux3Plus, (int)6);
	//signalMapperPressed->setMapping(btnPersonalAux3Minus, (int)7);
	connect(signalMapperPressed, SIGNAL(mapped(int)),this, SIGNAL(personalPressed(int)));
	connect(this, SIGNAL(personalPressed(int)),this, SLOT(slotPersonalPressed(int)));

	signalMapperReleased = new QSignalMapper(this);
	connect(btnPersonalMainPlus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
	connect(btnPersonalMainMinus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
	connect(btnPersonalAux1Plus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
	connect(btnPersonalAux1Minus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
	connect(btnPersonalAux2Plus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
	connect(btnPersonalAux2Minus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
	//connect(btnPersonalAux3Plus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
	//connect(btnPersonalAux3Minus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
	signalMapperReleased->setMapping(btnPersonalMainPlus, (int)0);
	signalMapperReleased->setMapping(btnPersonalMainMinus, (int)1);
	signalMapperReleased->setMapping(btnPersonalAux1Plus, (int)2);
	signalMapperReleased->setMapping(btnPersonalAux1Minus, (int)3);
	signalMapperReleased->setMapping(btnPersonalAux2Plus, (int)4);
	signalMapperReleased->setMapping(btnPersonalAux2Minus, (int)5);
	//signalMapperReleased->setMapping(btnPersonalAux3Plus, (int)6);
	//signalMapperReleased->setMapping(btnPersonalAux3Minus, (int)7);
	connect(signalMapperReleased, SIGNAL(mapped(int)),this, SIGNAL(personalReleased(int)));
	connect(this, SIGNAL(personalReleased(int)),this, SLOT(slotPersonalReleased(int)));

	signalMapperHold = new QSignalMapper(this);
	connect(btnPersonalMainPlus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
	connect(btnPersonalMainMinus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
	connect(btnPersonalAux1Plus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
	connect(btnPersonalAux1Minus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
	connect(btnPersonalAux2Plus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
	connect(btnPersonalAux2Minus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
	//connect(btnPersonalAux3Plus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
	//connect(btnPersonalAux3Minus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
	signalMapperHold->setMapping(btnPersonalMainPlus, (int)0);
	signalMapperHold->setMapping(btnPersonalMainMinus, (int)1);
	signalMapperHold->setMapping(btnPersonalAux1Plus, (int)2);
	signalMapperHold->setMapping(btnPersonalAux1Minus, (int)3);
	signalMapperHold->setMapping(btnPersonalAux2Plus, (int)4);
	signalMapperHold->setMapping(btnPersonalAux2Minus, (int)5);
	//signalMapperHold->setMapping(btnPersonalAux3Plus, (int)6);
	//signalMapperHold->setMapping(btnPersonalAux3Minus, (int)7);
	connect(signalMapperHold, SIGNAL(mapped(int)),this, SIGNAL(personalHold(int)));
	connect(this, SIGNAL(personalHold(int)),this, SLOT(slotPersonalHold(int)));


	connect(btnPersonalStart,SIGNAL(clicked(bool)),SLOT(slotPersonalStart()));
	connect(btnPersonalExit,SIGNAL(clicked(bool)),SLOT(slotPersonalExit()));

	connect(btnTestConcentricStart,SIGNAL(startButtonClicked(qint32)),SLOT(slotTestConcentricStart(qint32)));
	connect(btnTestEccentricStart,SIGNAL(startButtonClicked(qint32)),SLOT(slotTestEccentricStart(qint32)));
	connect(btnTestStaticStart,SIGNAL(startButtonClicked(QByteArray &)),SLOT(slotTestStaticStart(QByteArray &)));

	connect(btnResetError,SIGNAL(clicked(bool)),SLOT(slotResetError()));
	connect(btnExcerciseIsokineticStart,SIGNAL(clicked(bool)),SLOT(slotExcerciseIsokineticStart()));
	connect(btnGenericSetStart,SIGNAL(clicked(bool)),SLOT(slotGenericSetStart()));
	connect(btnCancel,SIGNAL(clicked(bool)),SLOT(slotCancel()));

	connect(btnPlotForceReset,SIGNAL(clicked(bool)),SLOT(slotPlotForceReset()));

}
//=================================================================================================
void WidgetMode::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value){

	if (tag==TLV::TAG_ReportCurrentMode){

		{
			rxMessageCounter++;
			lblRxMessageCounter->setText(
						"Сообщений: "+QString::number(rxMessageCounter)+" "
						+"(длина последнего="
						+QString::number(value.length())
						+")"
						);
			(reportLogger->stream) << "mdgID=" << QString::number(msgID) << ";";
		}


		{
			QString machineTimeStr = TLV::getDateTimeStr(value,1,true);
			lblDateTime->setText("Machine time: "+machineTimeStr);
		}            

		{
			QDateTime dateTime(QDateTime::currentDateTime());
			QString pcTimeStr = dateTime.toString("(HH:mm:ss.zzz)");
			QString machineTimeStr = TLV::getDateTimeStr(value,1,false);
			(reportLogger->stream) << machineTimeStr << pcTimeStr << ";";
		}

		{
			qint32 odometerTime;
			qint32 odometerWay;
			memcpy(&odometerTime,&(value.data()[7]),sizeof(odometerTime));
			memcpy(&odometerWay,&(value.data()[11]),sizeof(odometerWay));
			lblOdometer->setText("Odometer: "+QString::number(odometerTime) + "min / " + QString::number(odometerWay)+"m");
		}

		lblPhase->setText("");


		uint8_t mode = value.at(0);

		switch(mode){
		default:
		case (RTCU::Mode::INITIALIZING):
			lblPosition->setVisible(false);
			lblPosition->setText("");
			break;
		case (RTCU::Mode::IDLE):
		case (RTCU::Mode::WAITING):
		case (RTCU::Mode::PARKING):
		case (RTCU::Mode::PERSONAL):
		case (RTCU::Mode::TEST_CONCENTRIC):
		case (RTCU::Mode::TEST_ECCENTRIC):
		case (RTCU::Mode::EXCERCISE_ISOKINETIC):
		case (RTCU::Mode::FAULT):
		case (RTCU::Mode::TEST_STATIC):
		case (RTCU::Mode::GENERIC_SET):
			lblPosition->setVisible(true);
			{
				QString posStr = 
					QString::number(TLV::getInt32(value,1+6+8+0*4))+" ... "+
					QString::number(TLV::getInt32(value,1+6+8+1*4))+" ... "+
					QString::number(TLV::getInt32(value,1+6+8+2*4))/*+""+
					QString::number(TLV::getInt32(value,1+6+8+3*4))*/
					;
					lblPosition->setText("Позиция:   "+posStr); 
					(reportLogger->stream) << "pos=" << posStr << ";";
			}
			break;




		}


		{
			QString modeStr = RTCU::Mode::getTitle(mode);
			(reportLogger->stream) << "mode=" << modeStr << ";";
			lblMode->setText("Режим: "+modeStr);
		}

		switch(mode){
		//------------------------------------------------------------------------
		default:
		   lblError->setVisible(false);
			btnEnableServo->setVisible(false);
			btnParking->setVisible(false);
			btnPersonalStart->setVisible(false);
			btnPersonalExit->setVisible(false);
			wgtPersonal->setVisible(false);

			btnResetError->setVisible(false);
			btnExcerciseIsokineticStart->setVisible(false);
			btnGenericSetStart->setVisible(false);
			lblSetIndex->setVisible(false);
			lblRepIndex->setVisible(false);
			lblRepDirection->setVisible(false);
			btnCancel->setVisible(false);
			break;
		//------------------------------------------------------------------------
		case RTCU::Mode::INITIALIZING:

			lblError->setVisible(false);
			lblPositionRel->setVisible(false);
			lblTimeToTest->setVisible(false);
			lblForceValue->setVisible(false);
			btnEnableServo->setVisible(false);
			btnParking->setVisible(false);
			btnPersonalStart->setVisible(false);
			btnPersonalExit->setVisible(false);
			wgtPersonal->setVisible(false);
			btnTestConcentricStart->setVisible(false);
			btnTestEccentricStart->setVisible(false);
			btnTestStaticStart->setVisible(false);

			btnResetError->setVisible(false);
			btnExcerciseIsokineticStart->setVisible(false);
			btnGenericSetStart->setVisible(false);
			lblSetIndex->setVisible(false);
			lblTimeToSet->setVisible(false);
			lblRepIndex->setVisible(false);
			lblRepDirection->setVisible(false);
			btnCancel->setVisible(false);
			break;
			//------------------------------------------------------------------------
		case RTCU::Mode::IDLE:

			lblError->setVisible(false);
			lblPositionRel->setVisible(false);
			lblTimeToTest->setVisible(false);
			lblForceValue->setVisible(false);
			btnEnableServo->setVisible(true);
			btnParking->setVisible(false);
			btnPersonalStart->setVisible(false);
			btnPersonalExit->setVisible(false);
			wgtPersonal->setVisible(false);
			btnTestConcentricStart->setVisible(false);
			btnTestEccentricStart->setVisible(false);
			btnTestStaticStart->setVisible(false);

 
			btnResetError->setVisible(false);
			btnExcerciseIsokineticStart->setVisible(false);
			btnGenericSetStart->setVisible(false);
			lblSetIndex->setVisible(false);
			lblTimeToSet->setVisible(false);
			lblRepIndex->setVisible(false);
			lblRepDirection->setVisible(false);
			btnCancel->setVisible(false);
			break;
		//------------------------------------------------------------------------
		case RTCU::Mode::WAITING:
			lblError->setVisible(false);
			lblPositionRel->setVisible(false);
			lblTimeToTest->setVisible(false);
			lblForceValue->setVisible(false);
			btnEnableServo->setVisible(false);
			btnParking->setVisible(true);
			btnPersonalStart->setVisible(true);
			btnPersonalExit->setVisible(false);
			wgtPersonal->setVisible(false);
			wgtPersonal->setVisible(false);
			btnTestConcentricStart->setVisible(true);
			btnTestEccentricStart->setVisible(true);
			btnTestStaticStart->setVisible(true);

			btnResetError->setVisible(false);
			btnExcerciseIsokineticStart->setVisible(true);
			btnGenericSetStart->setVisible(true);
			lblSetIndex->setVisible(false);
			lblTimeToSet->setVisible(false);
			lblRepIndex->setVisible(false);
			lblRepDirection->setVisible(false);
			btnCancel->setVisible(false);
			break;
			//------------------------------------------------------------------------
		case RTCU::Mode::PARKING:

			lblError->setVisible(false);
			lblPositionRel->setVisible(false);
			lblTimeToTest->setVisible(false);
			lblForceValue->setVisible(false);
			btnEnableServo->setVisible(false);
			btnParking->setVisible(false);
			btnPersonalStart->setVisible(false);
			btnPersonalExit->setVisible(false);
			btnTestConcentricStart->setVisible(false);
			btnTestEccentricStart->setVisible(false);
			btnTestStaticStart->setVisible(false);
			wgtPersonal->setVisible(false);
			btnResetError->setVisible(false);
			btnExcerciseIsokineticStart->setVisible(false);
			btnGenericSetStart->setVisible(false);
			lblSetIndex->setVisible(false);
			lblTimeToSet->setVisible(false);
			lblRepIndex->setVisible(false);
			lblRepDirection->setVisible(false);
			btnCancel->setVisible(true);
			break;

			//------------------------------------------------------------------------
		case RTCU::Mode::PERSONAL:

			lblError->setVisible(false);
			lblPositionRel->setVisible(false);
			lblTimeToTest->setVisible(false);
			lblForceValue->setVisible(false);
			btnEnableServo->setVisible(false);
			btnParking->setVisible(false);
			btnPersonalStart->setVisible(false);
			btnPersonalExit->setVisible(true);
			btnTestConcentricStart->setVisible(false);
			btnTestEccentricStart->setVisible(false);
			btnTestStaticStart->setVisible(false);
			wgtPersonal->setVisible(true);

			btnResetError->setVisible(false);
			btnExcerciseIsokineticStart->setVisible(false);
			btnGenericSetStart->setVisible(false);
			lblSetIndex->setVisible(false);
			lblTimeToSet->setVisible(false);
			lblRepIndex->setVisible(false);
			lblRepDirection->setVisible(false);
			btnCancel->setVisible(false);
			break;
		//------------------------------------------------------------------------
		case RTCU::Mode::TEST_CONCENTRIC:

			lblError->setVisible(false);
			btnEnableServo->setVisible(false);
			btnParking->setVisible(false);
			btnTestConcentricStart->setVisible(false);
			btnTestEccentricStart->setVisible(false);
			btnTestStaticStart->setVisible(false);
			btnPersonalStart->setVisible(false);
			btnPersonalExit->setVisible(false);
			wgtPersonal->setVisible(false);

			{
				qint32 phase = TLV::getInt32(value,23+8);
				switch(phase){
				default:
					{
						QString phaseStr = "???";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					break;
				case Phase::HOMING:
					{
						QString phaseStr = "HOMING";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					lblPositionRel->setVisible(false);
					lblTimeToTest->setVisible(false);
					lblForceValue->setVisible(false);

					break;
				case Phase::PAUSE:
					{
						QString phaseStr = "PAUSE";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					lblPositionRel->setVisible(false);
					lblTimeToTest->setVisible(true);
					lblForceValue->setVisible(false);

					lblTimeToTest->setText(
								"До начала теста осталось: "+
								QString::number(TLV::getInt32(value,27+8))+" мс"
								);

					break;
				case Phase::TEST_CONCENTRIC:
					{
						QString phaseStr = "TEST_CONCENTRIC";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					lblPositionRel->setVisible(true);
					lblTimeToTest->setVisible(false);
					lblForceValue->setVisible(true);

					{
						double relPos = (double)TLV::getInt32(value,27+8)/100.0;
						QString relPosStr = QString::number(relPos,'f',3)+"%";
						lblPositionRel->setText("Относительное положение: "+relPosStr);
						(reportLogger->stream) << "relPos=" << relPosStr << ";";
					}

					{
						QString forceStr = QString::number(TLV::getInt32(value,31+8))+"g";
						lblForceValue->setText("Усилие: "+forceStr);
						(reportLogger->stream) << "force=" << forceStr << ";";
					}

					{
						qint32 positionRel = TLV::getInt32(value,27+8);
						qint32 force = TLV::getInt32(value,31+8);
						plotX.append((double)positionRel/100.0);
						plotY.append((double)force/1000.0);
						plotTime += 1.0;
						plotT.append(plotTime);
						plotForceVsPosition->yAxis->setRange(getMin(plotY), getMax(plotY));
						plotForceVsPosition->graph(0)->setData(plotX, plotY);
						plotForceVsPosition->replot();
						plotForceVsTime->xAxis->setRange(getMin(plotT), getMax(plotT));
						plotForceVsTime->yAxis->setRange(getMin(plotY), getMax(plotY));
						plotForceVsTime->graph(0)->setData(plotT, plotY);
						plotForceVsTime->replot();
					}

					break;
				}
			}

 
			btnResetError->setVisible(false);
			btnExcerciseIsokineticStart->setVisible(false);
			btnGenericSetStart->setVisible(false);
			lblSetIndex->setVisible(false);
			lblTimeToSet->setVisible(false);
			lblRepIndex->setVisible(false);
			lblRepDirection->setVisible(false);
			btnCancel->setVisible(true);




			break;
			//------------------------------------------------------------------------
		case RTCU::Mode::TEST_ECCENTRIC:

			lblError->setVisible(false);
			lblPosition->setVisible(true);
			btnEnableServo->setVisible(false);
			btnParking->setVisible(false);
			btnTestConcentricStart->setVisible(false);
			btnTestEccentricStart->setVisible(false);
			btnTestStaticStart->setVisible(false);
			btnPersonalStart->setVisible(false);
			btnPersonalExit->setVisible(false);
			wgtPersonal->setVisible(false);

			lblPosition->setText(
						"Позиция: "+
						QString::number(TLV::getInt32(value,1+6+8+0*4))+" / "+
						QString::number(TLV::getInt32(value,1+6+8+1*4))+" / "+
						QString::number(TLV::getInt32(value,1+6+8+2*4))+" / "+
						QString::number(TLV::getInt32(value,1+6+8+3*4))
						);
			{
				qint32 phase = TLV::getInt32(value,23+8);
				switch(phase){
				default:
					{
						QString phaseStr = "???";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					break;
				case Phase::HOMING:
					{
						QString phaseStr = "HOMING";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					lblPositionRel->setVisible(false);
					lblTimeToTest->setVisible(false);
					lblForceValue->setVisible(false);
					break;
				case Phase::PAUSE:
					{
						QString phaseStr = "PAUSE";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					lblPositionRel->setVisible(false);
					lblTimeToTest->setVisible(true);
					lblForceValue->setVisible(false);

					lblTimeToTest->setText(
								"До начала теста осталось: "+
								QString::number(TLV::getInt32(value,27+8))+" мс"
								);
					break;
				case Phase::TEST_ECCENTRIC:
					{
						QString phaseStr = "TEST_ECCENTRIC";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					lblPositionRel->setVisible(true);
					lblTimeToTest->setVisible(false);
					lblForceValue->setVisible(true);

					{
						double relPos = (double)TLV::getInt32(value,27+8)/100.0;
						QString relPosStr = QString::number(relPos,'f',3)+"%";
						lblPositionRel->setText("Относительное положение: "+relPosStr);
						(reportLogger->stream) << "relPos=" << relPosStr << ";";
					}
					{
						QString forceStr = QString::number(TLV::getInt32(value,31+8))+"g";
						lblForceValue->setText("Усилие: "+forceStr);
						(reportLogger->stream) << "force=" << forceStr << ";";
					}
					{
						qint32 positionRel = TLV::getInt32(value,27+8);
						qint32 force = TLV::getInt32(value,31+8);
						plotX.append((double)positionRel/100.0);
						plotY.append((double)force/1000.0);
						plotTime += 1.0;
						plotT.append(plotTime);
						plotForceVsPosition->yAxis->setRange(getMin(plotY), getMax(plotY));
						plotForceVsPosition->graph(0)->setData(plotX, plotY);
						plotForceVsPosition->replot();
						plotForceVsTime->xAxis->setRange(getMin(plotT), getMax(plotT));
						plotForceVsTime->yAxis->setRange(getMin(plotY), getMax(plotY));
						plotForceVsTime->graph(0)->setData(plotT, plotY);
						plotForceVsTime->replot();
					}
					break;
				}
			}

			btnResetError->setVisible(false);
			btnExcerciseIsokineticStart->setVisible(false);
			btnGenericSetStart->setVisible(false);
			lblSetIndex->setVisible(false);
			lblTimeToSet->setVisible(false);
			lblRepIndex->setVisible(false);
			lblRepDirection->setVisible(false);
			btnCancel->setVisible(true);
			break;
		//------------------------------------------------------------------------
		case RTCU::Mode::TEST_STATIC:

			lblError->setVisible(false);
			btnEnableServo->setVisible(false);
			btnParking->setVisible(false);
			btnTestConcentricStart->setVisible(false);
			btnTestEccentricStart->setVisible(false);
			btnTestStaticStart->setVisible(false);
			btnPersonalStart->setVisible(false);
			btnPersonalExit->setVisible(false);
			wgtPersonal->setVisible(false);


			{
				qint32 phase = TLV::getInt32(value,23+8);
				switch(phase){
				default:
					{
						QString phaseStr = "???";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					break;
				case Phase::HOMING:
					{
						QString phaseStr = "HOMING";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					{
						lblPositionRel->setVisible(true);
						double relPos = (double)TLV::getInt32(value,27+8)/100.0;
						QString relPosStr = QString::number(relPos,'f',3)+"%";
						lblPositionRel->setText("Относительное положение: "+relPosStr);
						(reportLogger->stream) << "relPos=" << relPosStr << ";";
					}

					lblTimeToTest->setVisible(false);
					lblForceValue->setVisible(false);


					break;
				case Phase::PAUSE:
					{
						QString phaseStr = "PAUSE";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					{
						lblPositionRel->setVisible(true);
						double relPos = (double)TLV::getInt32(value,27+8)/100.0;
						QString relPosStr = QString::number(relPos,'f',3)+"%";
						lblPositionRel->setText("Относительное положение: "+relPosStr);
						(reportLogger->stream) << "relPos=" << relPosStr << ";";
					}

					lblTimeToTest->setVisible(true);
					lblForceValue->setVisible(false);

					lblTimeToTest->setText(
								"До начала теста осталось: "+
								QString::number(TLV::getInt32(value,39))+" мс"
								);
					break;
				case Phase::TEST_STATIC:
					{
						QString phaseStr = "TEST_STATIC";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					{
						lblPositionRel->setVisible(true);
						double relPos = (double)TLV::getInt32(value,27+8)/100.0;
						QString relPosStr = QString::number(relPos,'f',3)+"%";
						lblPositionRel->setText("Относительное положение: "+relPosStr);
						(reportLogger->stream) << "relPos=" << relPosStr << ";";
					}

					lblTimeToTest->setVisible(true);
					lblForceValue->setVisible(true);

					{
						QString forceStr = QString::number(TLV::getInt32(value,31+8))+"g";
						lblForceValue->setText("Усилие: "+forceStr);
						(reportLogger->stream) << "force=" << forceStr << ";";
					}
					lblTimeToTest->setText(
								"До окончания теста осталось: "+
								QString::number(TLV::getInt32(value,43))+" мс"
								);

					{
						qint32 positionRel = TLV::getInt32(value,35);
						qint32 force = TLV::getInt32(value,39);
						plotX.append((double)positionRel/100.0);
						plotY.append((double)force/1000.0);
						plotTime += 1.0;
						plotT.append(plotTime);
						plotForceVsPosition->yAxis->setRange(getMin(plotY), getMax(plotY));
						plotForceVsPosition->graph(0)->setData(plotX, plotY);
						plotForceVsPosition->replot();
						plotForceVsTime->xAxis->setRange(getMin(plotT), getMax(plotT));
						plotForceVsTime->yAxis->setRange(getMin(plotY), getMax(plotY));
						plotForceVsTime->graph(0)->setData(plotT, plotY);
						plotForceVsTime->replot();
					}

					break;
				}
			}


			btnResetError->setVisible(false);
			btnExcerciseIsokineticStart->setVisible(false);
			btnGenericSetStart->setVisible(false);
			lblSetIndex->setVisible(false);
			lblTimeToSet->setVisible(false);
			lblRepIndex->setVisible(false);
			lblRepDirection->setVisible(false);
			btnCancel->setVisible(true);
			break;
		//------------------------------------------------------------------------
		case RTCU::Mode::EXCERCISE_ISOKINETIC:

			lblError->setVisible(false);
			btnEnableServo->setVisible(false);
			btnParking->setVisible(false);
			btnTestConcentricStart->setVisible(false);
			btnTestEccentricStart->setVisible(false);
			btnTestStaticStart->setVisible(false);
			btnPersonalStart->setVisible(false);
			btnPersonalExit->setVisible(false);

			lblSetIndex->setVisible(true);

			{
				QString setIndexStr = QString::number(TLV::getInt32(value,35));
				lblSetIndex->setText("Set index = "+setIndexStr);
				(reportLogger->stream) << "set=" << setIndexStr << ";";
			}


			{
				QString phaseInfo = "Фаза: ";
				qint32 phase = TLV::getInt32(value,31);
				switch(phase){
				default:
					{
						QString phaseStr = "???";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
				   break;
				case Phase::HOMING:
					{
						QString phaseStr = "HOMING";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}

					lblPositionRel->setVisible(false);
					lblTimeToSet->setVisible(false);
					lblRepIndex->setVisible(false);
					lblRepDirection->setVisible(false);
					lblForceValue->setVisible(false);

					break;
				case Phase::PAUSE:
					{
						QString phaseStr = "PAUSE";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
 
					lblPositionRel->setVisible(false);
					lblTimeToSet->setVisible(true);

					{
						lblTimeToSet->setText("Осталось "+QString::number(TLV::getInt32(value,39)));
						(reportLogger->stream) << "timeToSet=" << QString::number(TLV::getInt32(value,39)) << ";";
					}

					lblRepIndex->setVisible(false);
					lblRepDirection->setVisible(false);
					lblForceValue->setVisible(false);

					break;
				case Phase::ISOKINETIC_FIRSTMOVE:
					{
						QString phaseStr = "FIRSTMOVE";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}

 

					break;
				case Phase::ISOKINETIC_FIRSTINTERRUPTION:
					{
						QString phaseStr = "FIRSTINTERRUPTION";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					{
						(reportLogger->stream) << "timeToMove=" << QString::number(TLV::getInt32(value,39)) << ";";
					}


					break;
				case Phase::ISOKINETIC_SECONDMOVE:
					{
						QString phaseStr = "SECONDMOVE";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}

 

					break;
				case Phase::ISOKINETIC_SECONDINTERRUPTION:
					{
						QString phaseStr = "SECONDINTERRUPTION";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					{
						(reportLogger->stream) << "timeToMove=" << QString::number(TLV::getInt32(value,39)) << ";";
					}

					break;
				}

				if ((phase==Phase::ISOKINETIC_FIRSTMOVE)||(phase==Phase::ISOKINETIC_SECONDMOVE)){

					lblTimeToSet->setVisible(false);
					lblPositionRel->setVisible(true);
					lblRepIndex->setVisible(true);
					lblRepDirection->setVisible(true);

					{
						QString repIndexStr = QString::number(TLV::getInt32(value,39));
						lblRepIndex->setText("Rep index = "+repIndexStr);
						(reportLogger->stream) << "repIndex=" << repIndexStr << ";";
					}
					{
						qint32 repDir = TLV::getInt32(value,43);
						QString repDirStr = (repDir==0)?"AB":"BA";
						lblRepDirection->setText("Rep direction = "+repDirStr);
						(reportLogger->stream) << "repDir=" << repDirStr << ";";
					}

					{
						double relPos = (double)TLV::getInt32(value,47)/100.0;
						QString relPosStr = QString::number(relPos,'f',3)+"%";
						lblPositionRel->setText("Относительное положение: "+relPosStr);
						(reportLogger->stream) << "relPos=" << relPosStr << ";";
					}

					{
						QString forceStr = QString::number(TLV::getInt32(value,51))+"g";
						lblForceValue->setText("Усилие: "+forceStr);
						(reportLogger->stream) << "force=" << forceStr << ";";
					}
 


 

					{
						qint32 positionRel = TLV::getInt32(value,47);
						qint32 force = TLV::getInt32(value,51);
						plotX.append((double)positionRel/100.0);
						plotY.append((double)force/1000.0);
						plotTime += 1.0;
						plotT.append(plotTime);
						plotForceVsPosition->yAxis->setRange(getMin(plotY), getMax(plotY));
						plotForceVsPosition->graph(0)->setData(plotX, plotY);
						plotForceVsPosition->replot();
						plotForceVsTime->xAxis->setRange(getMin(plotT), getMax(plotT));
						plotForceVsTime->yAxis->setRange(getMin(plotY), getMax(plotY));
						plotForceVsTime->graph(0)->setData(plotT, plotY);
						plotForceVsTime->replot();
					}



				}else if ((phase==Phase::ISOKINETIC_FIRSTINTERRUPTION)||(phase==Phase::ISOKINETIC_SECONDINTERRUPTION)){

					lblTimeToSet->setVisible(true);
					lblTimeToSet->setText("Осталось "+QString::number(TLV::getInt32(value,39)));


					lblPositionRel->setVisible(false);
					lblRepIndex->setVisible(false);
					lblRepDirection->setVisible(false);
					lblForceValue->setVisible(false);



				}




			}

			btnResetError->setVisible(false);
			btnExcerciseIsokineticStart->setVisible(false);
			btnGenericSetStart->setVisible(false);
			btnCancel->setVisible(true);
			break;
		//------------------------------------------------------------------------
		case RTCU::Mode::GENERIC_SET:



			lblError->setVisible(false);
			btnEnableServo->setVisible(false);
			btnParking->setVisible(false);
			btnTestConcentricStart->setVisible(false);
			btnTestEccentricStart->setVisible(false);
			btnTestStaticStart->setVisible(false);
			btnPersonalStart->setVisible(false);
			btnPersonalExit->setVisible(false);

			lblSetIndex->setVisible(false);
			lblTimeToTest->setVisible(false);
			wgtPersonal->setVisible(false);
			lblRepDirection->setVisible(false);


			lblPhase->setVisible(true);
			{
				qint32 phase = TLV::getInt32(value,23+8);
				switch(phase){
				default:
					{
						QString phaseStr = "???";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					lblPositionRel->setVisible(false);
					lblTimeToSet->setVisible(false);
					lblRepDirection->setVisible(false);
					lblRepIndex->setVisible(false);
					lblForceValue->setVisible(false);




					break;
				case Phase::PAUSE1:
					{
						QString phaseStr = "PAUSE1";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					lblPositionRel->setVisible(false);
					lblRepIndex->setVisible(false);
					lblForceValue->setVisible(false);

					{
						lblTimeToSet->setVisible(true);
						QString pauseTimeRemainingStr = QString::number(TLV::getInt32(value,35))+"ms";
						lblTimeToSet->setText("Осталось "+pauseTimeRemainingStr);
						(reportLogger->stream) << "pauseTimeRemaining=" << pauseTimeRemainingStr << ";";
					}

					break;
				case Phase::HOMING:
					{
						QString phaseStr = "HOMING";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					lblPositionRel->setVisible(false);
					lblTimeToSet->setVisible(false);
					lblRepIndex->setVisible(false);
					lblForceValue->setVisible(false);

					break;
				case Phase::PAUSE2:
					{
						QString phaseStr = "PAUSE2";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					lblPositionRel->setVisible(false);
					lblRepIndex->setVisible(false);
					lblForceValue->setVisible(false);

					{
						lblTimeToSet->setVisible(true);
						QString pauseTimeRemainingStr = QString::number(TLV::getInt32(value,35))+"ms";
						lblTimeToSet->setText("Осталось "+pauseTimeRemainingStr);
						(reportLogger->stream) << "pauseTimeRemaining=" << pauseTimeRemainingStr << ";";
					}


					break;
				case Phase::GENERIC_MOVE:
					{
						QString phaseStr = "GENERIC_MOVE";
						lblPhase->setText("Фаза: "+phaseStr);
						(reportLogger->stream) << "phase=" << phaseStr << ";";
					}
					lblPositionRel->setVisible(true);
					lblTimeToSet->setVisible(true);
					lblRepIndex->setVisible(true);
					lblForceValue->setVisible(true);

					break;



				}

			}




			btnResetError->setVisible(false);
			btnExcerciseIsokineticStart->setVisible(false);
			btnGenericSetStart->setVisible(false);
			btnCancel->setVisible(true);

			break;
		//------------------------------------------------------------------------
		case RTCU::Mode::FAULT:

			lblError->setVisible(true);
			lblPositionRel->setVisible(false);
			lblTimeToTest->setVisible(false);
			lblForceValue->setVisible(false);
			btnEnableServo->setVisible(false);
			btnParking->setVisible(false);
			btnPersonalStart->setVisible(false);
			btnPersonalExit->setVisible(false);
			btnTestConcentricStart->setVisible(false);
			btnTestEccentricStart->setVisible(false);
			btnTestStaticStart->setVisible(false);
			wgtPersonal->setVisible(false);

			quint8 errorType = value.at(1+6+8);
			(reportLogger->stream) << "type=" << (int)errorType << ";";

			switch(errorType)
			{
			default:
				lblError->setText("Ошибка: ???");
				break;
			case ERROR_ServoInternal:
				lblError->setText("Ошибка: ERROR_ServoInternal");
				break;
			case ERROR_ServoConnectionLost:
				lblError->setText("Ошибка: ERROR_ServoConnectionLost");
				break;
			case ERROR_ServoVoltageLost:
				lblError->setText("Ошибка: ERROR_ServoVoltageLost");
				break;
			case ERROR_ServoControlLost:
				lblError->setText("Ошибка: ERROR_ServoControlLost");
				break;

			case ERROR_RtcuLogicError:
				lblError->setText("Ошибка: ERROR_RtcuLogicError");
				break;
			case ERROR_AuxMoved:
				lblError->setText("Ошибка: ERROR_AuxMoved");
				break;
			case ERROR_HmiParametersError:
				lblError->setText("Ошибка: ERROR_HmiParametersError");
				break;
			case ERROR_StrainGaugeLost:
				lblError->setText("Ошибка: ERROR_StrainGaugeLost");
				break;
			case ERROR_EmergencyStop:
				lblError->setText("Ошибка: ERROR_EmergencyStop");
				break;
			case ERROR_PositionMainSensorLost:
				lblError->setText("Ошибка: ERROR_PositionMainSensorLost");
				break;
			case ERROR_RfidReaderLost:
				lblError->setText("Ошибка: ERROR_RfidReaderLost");
				break;
			case ERROR_Unknown:
				lblError->setText("Ошибка: ERROR_Unknown");
				break;


			}

			btnResetError->setVisible(true);
			btnExcerciseIsokineticStart->setVisible(false);
			btnGenericSetStart->setVisible(false);
			lblSetIndex->setVisible(false);
			lblTimeToSet->setVisible(false);
			btnCancel->setVisible(false);

			lblRepIndex->setVisible(false);
			lblRepDirection->setVisible(false);

			break;
			//------------------------------------------------------------------------
		}

		(reportLogger->stream) << "\n";
		reportLogger->flush();
	}

}
//=================================================================================================
void WidgetMode::slotEnableServo()
{

	if (serialPortTransceiver_->isPortOK()==true)
	{

		QByteArray valueArray;
		TLVWriter tlv(TLV::TAG_EnableServo,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;

	}

}
//=================================================================================================
void WidgetMode::slotParking()
{
	if (serialPortTransceiver_->isPortOK()==true)
	{

		QByteArray valueArray;
		TLVWriter tlv(TLV::TAG_Parking,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;

	}
}
//=================================================================================================
void WidgetMode::slotPersonalPressed(int buttonID)
{
	if (serialPortTransceiver_->isPortOK()==true)
	{
		QByteArray valueArray;
		valueArray.push_back((char)buttonID);
		TLVWriter tlv(TLV::TAG_PersonalButtonPressed,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;
	}
}
//=================================================================================================
void WidgetMode::slotPersonalHold(int buttonID){

	if (serialPortTransceiver_->isPortOK()==true){

		QByteArray valueArray;
		valueArray.push_back((char)buttonID);
		TLVWriter tlv(TLV::TAG_PersonalButtonHold,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;
	}
}
//=================================================================================================
void WidgetMode::slotPersonalReleased(int buttonID){

	if (serialPortTransceiver_->isPortOK()==true){
		
		QByteArray valueArray;
		valueArray.push_back((char)buttonID);
		TLVWriter tlv(TLV::TAG_PersonalButtonReleased,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;
	}
}
//=================================================================================================
void WidgetMode::slotPersonalStart(){

	if (serialPortTransceiver_->isPortOK()==true){
		QByteArray valueArray;
		TLVWriter tlv(TLV::TAG_Personal,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;
	}

}
//=================================================================================================
void WidgetMode::slotPersonalExit(){

	if (serialPortTransceiver_->isPortOK()==true)
	{
		QByteArray valueArray;
		TLVWriter tlv(TLV::TAG_PersonalExit,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;
	}

}
//============================================================================================================
void WidgetMode::slotTestConcentricStart(qint32 pauseBeforeTest){

	if (serialPortTransceiver_->isPortOK()==true){

		QByteArray valueArray;
		valueArray.append((char*)&pauseBeforeTest,4);
		TLVWriter tlv(TLV::TAG_TestConcentric,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;
	}

}
//============================================================================================================
void WidgetMode::slotTestEccentricStart(qint32 pauseBeforeTest){

	if (serialPortTransceiver_->isPortOK()==true){

		QByteArray valueArray;
		valueArray.append((char*)&pauseBeforeTest,4);
		TLVWriter tlv(TLV::TAG_TestEccentric,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;
	}

}
//============================================================================================================
void WidgetMode::slotTestStaticStart(QByteArray &testSettings){

	if (serialPortTransceiver_->isPortOK()==true){

		TLVWriter tlv(TLV::TAG_TestStatic,testSettings);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;
	}

}
//============================================================================================================
void WidgetMode::slotResetError(){

	if (serialPortTransceiver_->isPortOK()==true){

		QByteArray valueArray;
		TLVWriter tlv(TLV::TAG_ResetError,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;
	}

}
//============================================================================================================
void WidgetMode::slotExcerciseIsokineticStart(){

	if (serialPortTransceiver_->isPortOK()==true){

		QByteArray valueArray;
		TLVWriter tlv(TLV::TAG_ExcerciseIsokinetic,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;
	}


}
//============================================================================================================
void WidgetMode::slotGenericSetStart(){

	if (serialPortTransceiver_->isPortOK()==true){

		QByteArray valueArray;
		TLVWriter tlv(TLV::TAG_GenericSet,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;
	}


}
//============================================================================================================
void WidgetMode::slotCancel(){

	if (serialPortTransceiver_->isPortOK()==true){

		QByteArray valueArray;
		TLVWriter tlv(TLV::TAG_Cancel,valueArray);
		QByteArray *txArray = tlv.getStuffedArray();
		serialPortTransceiver_->write(*txArray);
		delete txArray;
	}
}
//============================================================================================================
void WidgetMode::slotPlotForceReset(){

	plotT.clear();
	plotX.clear();
	plotY.clear();
	plotForceVsPosition->graph(0)->setData(plotX, plotY);
	plotForceVsPosition->replot();
	plotForceVsTime->graph(0)->setData(plotT, plotY);
	plotForceVsTime->replot();

}
//============================================================================================================
double WidgetMode::getMax(QVector<double> &data){
	if (data.length()==0){
		return 10.0;
	}else{
		double result = data.at(0);
		for(int i=1;i<data.length();i++){
			if (data.at(i)>result){
				result = data.at(i);
			}
		}
		return result;
   }
}
//============================================================================================================
double WidgetMode::getMin(QVector<double> &data){
	if (data.length()==0){
		return -10.0;
	}else{
		double result = data.at(0);
		for(int i=1;i<data.length();i++){
			if (data.at(i)<result){
				result = data.at(i);
			}
		}
		return result;
	}
}
//============================================================================================================



