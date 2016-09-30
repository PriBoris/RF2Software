#include "widgetMode.h"


WidgetMode::~WidgetMode(){
    delete modeLogger;
}


WidgetMode::WidgetMode(
        SerialPortTransceiver *serialPortTransceiver,
        QString name,
        bool bbWidget,
        QWidget *parent
        ) : Widget(parent)
{

    rxMessageCounter = 0;

    serialPortTransceiver_ = serialPortTransceiver;
    name_ = name;
    bbWidget_ = bbWidget;

    modeLogger = new ModeLogger;



    lblRxMessageCounter = new QLabel("lblRxMessageCounter");
    lblDateTime = new QLabel("lblDateTime");
    lblMode = new QLabel("lblMode");
        lblMode->setStyleSheet("font: 16pt;");
    lblPhase = new QLabel("lblPhase");;
    lblError = new QLabel("lblError");
        lblError->setStyleSheet("font: 16pt;");
    lblMode->setStyleSheet("font: 16pt;");
    lblPosition = new QLabel("lblPosition");
    lblPositionRel = new QLabel("lblPositionRel");
    lblTimeToTest = new QLabel("lblTimeToTest");
    lblForceValue = new QLabel("lblForceValue");
        lblForceValue->setStyleSheet("font: 16pt;");

    lblSetIndex = new QLabel("lblSetIndex");
    lblRepIndex = new QLabel("lblRepIndex");
    lblRepDirection = new QLabel("lblRepDirection");
    lblTimeToSet = new QLabel("lblTimeToSet");
    btnEnableServo = new QPushButton("Enable SERVO");btnEnableServo->setFixedWidth(300);
    btnParking = new QPushButton("PARKING start");btnParking->setFixedWidth(300);
    btnPersonalStart = new QPushButton("PERSONAL start");btnPersonalStart->setFixedWidth(300);
    btnPersonalExit = new QPushButton("PERSONAL exit");btnPersonalExit->setFixedWidth(300);

    {
        btnPersonalMainPlus = new PersonalButton("Main+");
        btnPersonalMainMinus = new PersonalButton("Main-");
        btnPersonalAux1Plus = new PersonalButton("Aux1+");
        btnPersonalAux1Minus = new PersonalButton("Aux1-");
        btnPersonalAux2Plus = new PersonalButton("Aux2+");
        btnPersonalAux2Minus = new PersonalButton("Aux2-");
        btnPersonalAux3Plus = new PersonalButton("Aux3+");
        btnPersonalAux3Minus = new PersonalButton("Aux3-");
        btnPersonalMainPlus->setFixedWidth(50);
        btnPersonalMainMinus->setFixedWidth(50);
        btnPersonalAux1Plus->setFixedWidth(50);
        btnPersonalAux1Minus->setFixedWidth(50);
        btnPersonalAux2Plus->setFixedWidth(50);
        btnPersonalAux2Minus->setFixedWidth(50);
        btnPersonalAux3Plus->setFixedWidth(50);
        btnPersonalAux3Minus->setFixedWidth(50);

        loPersonal = new QHBoxLayout;
        loPersonal->addWidget(btnPersonalMainPlus);
        loPersonal->addWidget(btnPersonalMainMinus);
        loPersonal->addWidget(btnPersonalAux1Plus);
        loPersonal->addWidget(btnPersonalAux1Minus);
        loPersonal->addWidget(btnPersonalAux2Plus);
        loPersonal->addWidget(btnPersonalAux2Minus);
        loPersonal->addWidget(btnPersonalAux3Plus);
        loPersonal->addWidget(btnPersonalAux3Minus);
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



    btnResetError = new QPushButton("сброс ошибки");btnResetError->setFixedWidth(300);
    btnExcerciseIsokineticStart = new QPushButton("начать тренировку");btnExcerciseIsokineticStart->setFixedWidth(300);

    btnCancel = new QPushButton("CANCEL");btnCancel->setFixedWidth(300);

    lblOdometer = new QLabel("lblOdometer");


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
    connect(btnPersonalAux3Plus, SIGNAL(pressed()), signalMapperPressed, SLOT(map()));
    connect(btnPersonalAux3Minus, SIGNAL(pressed()), signalMapperPressed, SLOT(map()));
    signalMapperPressed->setMapping(btnPersonalMainPlus, (int)0);
    signalMapperPressed->setMapping(btnPersonalMainMinus, (int)1);
    signalMapperPressed->setMapping(btnPersonalAux1Plus, (int)2);
    signalMapperPressed->setMapping(btnPersonalAux1Minus, (int)3);
    signalMapperPressed->setMapping(btnPersonalAux2Plus, (int)4);
    signalMapperPressed->setMapping(btnPersonalAux2Minus, (int)5);
    signalMapperPressed->setMapping(btnPersonalAux3Plus, (int)6);
    signalMapperPressed->setMapping(btnPersonalAux3Minus, (int)7);
    connect(signalMapperPressed, SIGNAL(mapped(int)),this, SIGNAL(personalPressed(int)));
    connect(this, SIGNAL(personalPressed(int)),this, SLOT(slotPersonalPressed(int)));

    signalMapperReleased = new QSignalMapper(this);
    connect(btnPersonalMainPlus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
    connect(btnPersonalMainMinus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
    connect(btnPersonalAux1Plus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
    connect(btnPersonalAux1Minus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
    connect(btnPersonalAux2Plus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
    connect(btnPersonalAux2Minus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
    connect(btnPersonalAux3Plus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
    connect(btnPersonalAux3Minus, SIGNAL(released()), signalMapperReleased, SLOT(map()));
    signalMapperReleased->setMapping(btnPersonalMainPlus, (int)0);
    signalMapperReleased->setMapping(btnPersonalMainMinus, (int)1);
    signalMapperReleased->setMapping(btnPersonalAux1Plus, (int)2);
    signalMapperReleased->setMapping(btnPersonalAux1Minus, (int)3);
    signalMapperReleased->setMapping(btnPersonalAux2Plus, (int)4);
    signalMapperReleased->setMapping(btnPersonalAux2Minus, (int)5);
    signalMapperReleased->setMapping(btnPersonalAux3Plus, (int)6);
    signalMapperReleased->setMapping(btnPersonalAux3Minus, (int)7);
    connect(signalMapperReleased, SIGNAL(mapped(int)),this, SIGNAL(personalReleased(int)));
    connect(this, SIGNAL(personalReleased(int)),this, SLOT(slotPersonalReleased(int)));

    signalMapperHold = new QSignalMapper(this);
    connect(btnPersonalMainPlus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
    connect(btnPersonalMainMinus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
    connect(btnPersonalAux1Plus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
    connect(btnPersonalAux1Minus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
    connect(btnPersonalAux2Plus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
    connect(btnPersonalAux2Minus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
    connect(btnPersonalAux3Plus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
    connect(btnPersonalAux3Minus, SIGNAL(hold()), signalMapperHold, SLOT(map()));
    signalMapperHold->setMapping(btnPersonalMainPlus, (int)0);
    signalMapperHold->setMapping(btnPersonalMainMinus, (int)1);
    signalMapperHold->setMapping(btnPersonalAux1Plus, (int)2);
    signalMapperHold->setMapping(btnPersonalAux1Minus, (int)3);
    signalMapperHold->setMapping(btnPersonalAux2Plus, (int)4);
    signalMapperHold->setMapping(btnPersonalAux2Minus, (int)5);
    signalMapperHold->setMapping(btnPersonalAux3Plus, (int)6);
    signalMapperHold->setMapping(btnPersonalAux3Minus, (int)7);
    connect(signalMapperHold, SIGNAL(mapped(int)),this, SIGNAL(personalHold(int)));
    connect(this, SIGNAL(personalHold(int)),this, SLOT(slotPersonalHold(int)));


    connect(btnPersonalStart,SIGNAL(clicked(bool)),SLOT(slotPersonalStart()));
    connect(btnPersonalExit,SIGNAL(clicked(bool)),SLOT(slotPersonalExit()));

    connect(btnTestConcentricStart,SIGNAL(startButtonClicked(qint32)),SLOT(slotTestConcentricStart(qint32)));
    connect(btnTestEccentricStart,SIGNAL(startButtonClicked(qint32)),SLOT(slotTestEccentricStart(qint32)));
    connect(btnTestStaticStart,SIGNAL(startButtonClicked(QByteArray &)),SLOT(slotTestStaticStart(QByteArray &)));

    connect(btnResetError,SIGNAL(clicked(bool)),SLOT(slotResetError()));
    connect(btnExcerciseIsokineticStart,SIGNAL(clicked(bool)),SLOT(slotExcerciseIsokineticStart()));
    connect(btnCancel,SIGNAL(clicked(bool)),SLOT(slotCancel()));


    //debugTimer = new QTimer(this);
    //connect(debugTimer, SIGNAL(timeout()), this, SLOT(debugTimerTimeout()));
    //debugTimer->start(300);

    connect(btnPlotForceReset,SIGNAL(clicked(bool)),SLOT(slotPlotForceReset()));

}
//=========================================================================================================
void WidgetMode::newMessageReceived(quint8 tag,quint32 msgID,QByteArray &value)
{
    if (tag==TLV::TAG_ReportCurrentMode)
    {
        rxMessageCounter++;
        lblRxMessageCounter->setText(
                    "Сообщений: "+QString::number(rxMessageCounter)+" "
                    +"(длина последнего="
                    +QString::number(value.length())
                    +")"
                    );
        (modeLogger->stream) << "id=" << QString::number(msgID) << ";";


        lblPhase->setText("");
        lblDateTime->setText(
            QString("%1").arg(2000+(quint32)value.at(1),4,10,QChar('0'))+
            "/"+
            QString("%1").arg(value.at(2),2,10,QChar('0'))+
            "/"+
            QString("%1").arg(value.at(3),2,10,QChar('0'))+
            " "+
            QString("%1").arg(value.at(4),2,10,QChar('0'))+
            ":"+
            QString("%1").arg(value.at(5),2,10,QChar('0'))+
            ":"+
            QString("%1").arg(value.at(6),2,10,QChar('0'))
            );

        qint32 odometerTime;
        qint32 odometerWay;
        memcpy(&odometerTime,&(value.data()[7]),sizeof(odometerTime));
        memcpy(&odometerWay,&(value.data()[11]),sizeof(odometerWay));
        lblOdometer->setText("Odometer: "+QString::number(odometerTime) + "min / " + QString::number(odometerWay)+"m");


        switch(value.at(0))
        {
        //------------------------------------------------------------------------
        default:
            lblMode->setText("Режим: ???");
            lblError->setVisible(false);
            lblPosition->setVisible(false);
            btnEnableServo->setVisible(false);
            btnParking->setVisible(false);
            btnPersonalStart->setVisible(false);
            btnPersonalExit->setVisible(false);
            wgtPersonal->setVisible(false);

            btnResetError->setVisible(false);
            btnExcerciseIsokineticStart->setVisible(false);
            lblSetIndex->setVisible(false);
            lblRepIndex->setVisible(false);
            lblRepDirection->setVisible(false);
            btnCancel->setVisible(false);
            break;
        //------------------------------------------------------------------------
        case MODE_INITIALIZING:

            (modeLogger->stream) << "mode=INITIALIZING" << ";";

            lblMode->setText("Режим: INITIALIZING");
            lblError->setVisible(false);
            lblPosition->setVisible(false);
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
            lblSetIndex->setVisible(false);
            lblTimeToSet->setVisible(false);
            lblRepIndex->setVisible(false);
            lblRepDirection->setVisible(false);
            btnCancel->setVisible(false);
            break;
            //------------------------------------------------------------------------
        case MODE_IDLE:

            (modeLogger->stream) << "mode=IDLE" << ";";

            lblMode->setText("Режим: IDLE");
            lblError->setVisible(false);
            lblPosition->setVisible(true);
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

            lblPosition->setText(
                        "Позиция: "+
                        QString::number(getInt(value,1+6+8+0*4))+" / "+
                        QString::number(getInt(value,1+6+8+1*4))+" / "+
                        QString::number(getInt(value,1+6+8+2*4))+" / "+
                        QString::number(getInt(value,1+6+8+3*4))
                        );

            btnResetError->setVisible(false);
            btnExcerciseIsokineticStart->setVisible(false);
            lblSetIndex->setVisible(false);
            lblTimeToSet->setVisible(false);
            lblRepIndex->setVisible(false);
            lblRepDirection->setVisible(false);
            btnCancel->setVisible(false);
            break;
        //------------------------------------------------------------------------
        case MODE_WAITING:
            (modeLogger->stream) << "mode=WAITING" << ";";


            lblMode->setText("Режим: WAITING");
            lblError->setVisible(false);
            lblPosition->setVisible(true);
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

            lblPosition->setText(
                        "Позиция: "+
                        QString::number(getInt(value,1+6+8+0*4))+" / "+
                        QString::number(getInt(value,1+6+8+1*4))+" / "+
                        QString::number(getInt(value,1+6+8+2*4))+" / "+
                        QString::number(getInt(value,1+6+8+3*4))
                        );
            btnResetError->setVisible(false);
            btnExcerciseIsokineticStart->setVisible(true);
            lblSetIndex->setVisible(false);
            lblTimeToSet->setVisible(false);
            lblRepIndex->setVisible(false);
            lblRepDirection->setVisible(false);
            btnCancel->setVisible(false);
            break;
            //------------------------------------------------------------------------
        case MODE_PARKING:

            (modeLogger->stream) << "mode=PARKING" << ";";

            lblMode->setText("Режим: PARKING");
            lblError->setVisible(false);
            lblPosition->setVisible(true);
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
            lblPosition->setText(
                        "Позиция: "+
                        QString::number(getInt(value,1+6+8+0*4))+" / "+
                        QString::number(getInt(value,1+6+8+1*4))+" / "+
                        QString::number(getInt(value,1+6+8+2*4))+" / "+
                        QString::number(getInt(value,1+6+8+3*4))
                        );
            btnResetError->setVisible(false);
            btnExcerciseIsokineticStart->setVisible(false);
            lblSetIndex->setVisible(false);
            lblTimeToSet->setVisible(false);
            lblRepIndex->setVisible(false);
            lblRepDirection->setVisible(false);
            btnCancel->setVisible(true);
            break;

            //------------------------------------------------------------------------
        case MODE_PERSONAL:

            (modeLogger->stream) << "mode=PERSONAL" << ";";

            lblMode->setText("Режим: PERSONAL");
            lblError->setVisible(false);
            lblPosition->setVisible(true);
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
            lblPosition->setText(
                        "Позиция: "+
                        QString::number(getInt(value,1+6+8+0*4))+" / "+
                        QString::number(getInt(value,1+6+8+1*4))+" / "+
                        QString::number(getInt(value,1+6+8+2*4))+" / "+
                        QString::number(getInt(value,1+6+8+3*4))
                        );

            btnResetError->setVisible(false);
            btnExcerciseIsokineticStart->setVisible(false);
            lblSetIndex->setVisible(false);
            lblTimeToSet->setVisible(false);
            lblRepIndex->setVisible(false);
            lblRepDirection->setVisible(false);
            btnCancel->setVisible(false);
            break;
            //------------------------------------------------------------------------
        case MODE_TEST_CONCENTRIC:

            (modeLogger->stream) << "mode=TEST_CONCENTRIC" << ";";

            lblMode->setText("Режим: TEST_CONCENTRIC");
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
                        QString::number(getInt(value,1+6+8+0*4))+" / "+
                        QString::number(getInt(value,1+6+8+1*4))+" / "+
                        QString::number(getInt(value,1+6+8+2*4))+" / "+
                        QString::number(getInt(value,1+6+8+3*4))
                        );
            {
                QString phaseInfo = "Фаза: ";
                switch(getInt(value,23+8)){
                default:
                    lblPhase->setText(phaseInfo+"???");
                    break;
                case PHASE_HOMING:
                    lblPhase->setText(phaseInfo+"PHASE_HOMING");
                    break;
                case PHASE_PAUSE:
                    lblPhase->setText(phaseInfo+"PHASE_PAUSE");
                    break;
                case PHASE_TEST_CONCENTRIC:
                    lblPhase->setText(phaseInfo+"PHASE_TEST_CONCENTRIC");
                    break;
                }
            }

            switch(value.length())
            {
                case 27+8:
                    lblPositionRel->setVisible(false);
                    lblTimeToTest->setVisible(false);
                    lblForceValue->setVisible(false);
                    break;
                case 31+8:
                    lblPositionRel->setVisible(false);
                    lblTimeToTest->setVisible(true);
                    lblForceValue->setVisible(false);

                    lblTimeToTest->setText(
                                "До начала теста осталось: "+
                                QString::number(getInt(value,27+8))+" мс"
                                );
                    break;
                case 35+8:
                    lblPositionRel->setVisible(true);
                    lblTimeToTest->setVisible(false);
                    lblForceValue->setVisible(true);

                    lblPositionRel->setText(
                                "Относительное положение: "+
                                QString::number(getInt(value,27+8))+" ???"
                                );
                    lblForceValue->setText(
                                "Усилие: "+
                                QString::number(getInt(value,31+8))+"g"
                                );
                    {
                        qint32 positionRel = getInt(value,27+8);
                        qint32 force = getInt(value,31+8);
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
            btnResetError->setVisible(false);
            btnExcerciseIsokineticStart->setVisible(false);
            lblSetIndex->setVisible(false);
            lblTimeToSet->setVisible(false);
            lblRepIndex->setVisible(false);
            lblRepDirection->setVisible(false);
            btnCancel->setVisible(true);




            break;
            //------------------------------------------------------------------------
        case MODE_TEST_ECCENTRIC:

            (modeLogger->stream) << "mode=TEST_ECCENTRIC" << ";";


            lblMode->setText("Режим: TEST_ECCENTRIC");
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
                        QString::number(getInt(value,1+6+8+0*4))+" / "+
                        QString::number(getInt(value,1+6+8+1*4))+" / "+
                        QString::number(getInt(value,1+6+8+2*4))+" / "+
                        QString::number(getInt(value,1+6+8+3*4))
                        );
            {
                QString phaseInfo = "Фаза: ";
                switch(getInt(value,23+8)){
                default:
                    lblPhase->setText(phaseInfo+"???");
                    break;
                case PHASE_HOMING:
                    lblPhase->setText(phaseInfo+"PHASE_HOMING");
                    break;
                case PHASE_PAUSE:
                    lblPhase->setText(phaseInfo+"PHASE_PAUSE");
                    break;
                case PHASE_TEST_ECCENTRIC:
                    lblPhase->setText(phaseInfo+"PHASE_TEST_ECCENTRIC");
                    break;
                }
            }

            switch(value.length())
            {
                case 27+8:
                    lblPositionRel->setVisible(false);
                    lblTimeToTest->setVisible(false);
                    lblForceValue->setVisible(false);
                    break;
                case 31+8:
                    lblPositionRel->setVisible(false);
                    lblTimeToTest->setVisible(true);
                    lblForceValue->setVisible(false);

                    lblTimeToTest->setText(
                                "До начала теста осталось: "+
                                QString::number(getInt(value,27+8))+" мс"
                                );
                    break;
                case 35+8:
                    lblPositionRel->setVisible(true);
                    lblTimeToTest->setVisible(false);
                    lblForceValue->setVisible(true);

                    lblPositionRel->setText(
                                "Относительное положение: "+
                                QString::number(getInt(value,27+8))+" ???"
                                );
                    lblForceValue->setText(
                                "Усилие: "+
                                QString::number(getInt(value,31+8))+"g"
                                );

                    {
                        qint32 positionRel = getInt(value,27+8);
                        qint32 force = getInt(value,31+8);
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
            btnResetError->setVisible(false);
            btnExcerciseIsokineticStart->setVisible(false);
            lblSetIndex->setVisible(false);
            lblTimeToSet->setVisible(false);
            lblRepIndex->setVisible(false);
            lblRepDirection->setVisible(false);
            btnCancel->setVisible(true);
            break;
        //------------------------------------------------------------------------
        case MODE_TEST_STATIC:

            (modeLogger->stream) << "mode=TEST_STATIC" << ";";


            lblMode->setText("Режим: TEST_STATIC");
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
                        QString::number(getInt(value,1+6+8+0*4))+" / "+
                        QString::number(getInt(value,1+6+8+1*4))+" / "+
                        QString::number(getInt(value,1+6+8+2*4))+" / "+
                        QString::number(getInt(value,1+6+8+3*4))
                        );

            {
                lblPositionRel->setVisible(true);
                qint32 relPos = getInt(value,35);
                lblPositionRel->setText(
                            "Относительное положение: "+
                            QString::number(relPos)+" ???" + " / " +
                            QString::number((double)relPos/100.0)+"%"
                            );
            }


            {
                QString phaseInfo = "Фаза: ";
                switch(getInt(value,31)){
                default:
                    lblPhase->setText(phaseInfo+"???");
                    break;
                case PHASE_HOMING:
                    lblPhase->setText(phaseInfo+"PHASE_HOMING");

                    lblTimeToTest->setVisible(false);
                    lblForceValue->setVisible(false);


                    break;
                case PHASE_PAUSE:
                    lblPhase->setText(phaseInfo+"PHASE_PAUSE");

                    lblTimeToTest->setVisible(true);
                    lblForceValue->setVisible(false);

                    lblTimeToTest->setText(
                                "До начала теста осталось: "+
                                QString::number(getInt(value,39))+" мс"
                                );
                    break;
                case PHASE_TEST_STATIC:
                    lblPhase->setText(phaseInfo+"PHASE_TEST_STATIC");

                    lblTimeToTest->setVisible(true);
                    lblForceValue->setVisible(true);

                    lblForceValue->setText(
                                "Усилие: "+
                                QString::number(getInt(value,39))+"g"
                                );
                    lblTimeToTest->setText(
                                "До окончания теста осталось: "+
                                QString::number(getInt(value,43))+" мс"
                                );

                    {
                        qint32 positionRel = getInt(value,35);
                        qint32 force = getInt(value,39);
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
            lblSetIndex->setVisible(false);
            lblTimeToSet->setVisible(false);
            lblRepIndex->setVisible(false);
            lblRepDirection->setVisible(false);
            btnCancel->setVisible(true);
            break;
        //------------------------------------------------------------------------
        case MODE_EXCERCISE_ISOKINETIC:

            (modeLogger->stream) << "mode=EXCERCISE_ISOKINETIC" << ";";


            lblMode->setText("Режим: EXCERCISE_ISOKINETIC");
            lblError->setVisible(false);
            lblPosition->setVisible(true);
            btnEnableServo->setVisible(false);
            btnParking->setVisible(false);
            btnTestConcentricStart->setVisible(false);
            btnTestEccentricStart->setVisible(false);
            btnTestStaticStart->setVisible(false);
            btnPersonalStart->setVisible(false);
            btnPersonalExit->setVisible(false);

            lblPosition->setText(
                        "Позиция: "+
                        QString::number(getInt(value,1+6+8+0*4))+" / "+
                        QString::number(getInt(value,1+6+8+1*4))+" / "+
                        QString::number(getInt(value,1+6+8+2*4))+" / "+
                        QString::number(getInt(value,1+6+8+3*4))
                        );

            lblSetIndex->setVisible(true);
            lblSetIndex->setText("Set index = "+QString::number(getInt(value,35)));


            {
                QString phaseInfo = "Фаза: ";
                qint32 phase = getInt(value,31);
                switch(phase){
                default:
                    lblPhase->setText(phaseInfo+"??? ("+QString::number(phase)+")");
                    break;
                case PHASE_HOMING:

                    (modeLogger->stream) << "phase=HOMING" << ";";


                    lblPhase->setText(phaseInfo+"PHASE_HOMING");

                    lblPositionRel->setVisible(false);
                    lblTimeToSet->setVisible(false);
                    lblRepIndex->setVisible(false);
                    lblRepDirection->setVisible(false);
                    lblForceValue->setVisible(false);


                    break;
                case PHASE_PAUSE:

                    (modeLogger->stream) << "phase=PAUSE" << ";";

                    lblPhase->setText(phaseInfo+"PHASE_PAUSE");

                    lblPositionRel->setVisible(false);

                    lblTimeToSet->setVisible(true);
                    lblTimeToSet->setText("Осталось "+QString::number(getInt(value,39)));
                    (modeLogger->stream) << "timeToSet=" << QString::number(getInt(value,39)) << ";";

                    lblRepIndex->setVisible(false);
                    lblRepDirection->setVisible(false);
                    lblForceValue->setVisible(false);

                    break;
                case PHASE_ISOKINETIC_FIRSTMOVE:

                    (modeLogger->stream) << "phase=FIRSTMOVE" << ";";

                    lblPhase->setText(phaseInfo+"PHASE_ISOKINETIC_FIRSTMOVE");


                    break;
                case PHASE_ISOKINETIC_FIRSTINTERRUPTION:

                    (modeLogger->stream) << "phase=FIRSTINTERRUPTION" << ";";

                    lblPhase->setText(phaseInfo+"PHASE_ISOKINETIC_FIRSTINTERRUPTION");
                    break;
                case PHASE_ISOKINETIC_SECONDMOVE:

                    (modeLogger->stream) << "phase=SECONDMOVE" << ";";

                    lblPhase->setText(phaseInfo+"PHASE_ISOKINETIC_SECONDMOVE");


                    break;
                case PHASE_ISOKINETIC_SECONDINTERRUPTION:

                    (modeLogger->stream) << "phase=SECONDINTERRUPTION" << ";";

                    lblPhase->setText(phaseInfo+"PHASE_ISOKINETIC_SECONDINTERRUPTION");
                    break;
                }

                if ((phase==PHASE_ISOKINETIC_FIRSTMOVE)||(phase==PHASE_ISOKINETIC_SECONDMOVE)){

                    lblTimeToSet->setVisible(false);

                    lblPositionRel->setVisible(true);
                    lblPositionRel->setText("Position REL = "+QString::number(getInt(value,47)));
                    (modeLogger->stream) << "posRel=" << QString::number(getInt(value,47)) << ";";



                    lblRepIndex->setVisible(true);
                    lblRepIndex->setText("Rep index = "+QString::number(getInt(value,39)));
                    (modeLogger->stream) << "repIndex=" << QString::number(getInt(value,39)) << ";";


                    lblRepDirection->setVisible(true);

                    {
                        qint32 repDirection = getInt(value,43);
                        lblRepDirection->setText(
                                    "Rep direction = "+
                                    QString::number(repDirection)+
                                    (repDirection==0?" ЭКСЦЕНТРИКА":" КОНЦЕНТРИКА")
                                    );
                    }


                    lblForceValue->setVisible(true);
                    lblForceValue->setText("Force = "+QString::number(getInt(value,51))+"g");
                    (modeLogger->stream) << "force=" << QString::number(getInt(value,51)) << ";";


                    {
                        qint32 positionRel = getInt(value,47);
                        qint32 force = getInt(value,51);
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



                }else if ((phase==PHASE_ISOKINETIC_FIRSTINTERRUPTION)||(phase==PHASE_ISOKINETIC_SECONDINTERRUPTION)){

                    lblTimeToSet->setVisible(true);
                    lblTimeToSet->setText("Осталось "+QString::number(getInt(value,39)));
                    (modeLogger->stream) << "timeToRep=" << QString::number(getInt(value,39)) << ";";


                    lblPositionRel->setVisible(false);
                    lblRepIndex->setVisible(false);
                    lblRepDirection->setVisible(false);
                    lblForceValue->setVisible(false);



                }




            }

            btnResetError->setVisible(false);
            btnExcerciseIsokineticStart->setVisible(false);
            btnCancel->setVisible(true);
            break;
            //------------------------------------------------------------------------
        case MODE_ERROR:

            (modeLogger->stream) << "mode=ERROR" << ";";


            lblMode->setText("Режим: ERROR");
            lblError->setVisible(true);
            lblPosition->setVisible(false);
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
            lblSetIndex->setVisible(false);
            lblTimeToSet->setVisible(false);
            btnCancel->setVisible(false);

            lblRepIndex->setVisible(false);
            lblRepDirection->setVisible(false);

            break;
            //------------------------------------------------------------------------
        }

        (modeLogger->stream) << "\n";
        modeLogger->flush();
    }

}

//============================================================================================================
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
//============================================================================================================
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
//============================================================================================================
void WidgetMode::slotPersonalPressed(int buttonID)
{
    qDebug() << "WidgetHmiMode::slotPersonalPressed" << buttonID;
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
void WidgetMode::slotPersonalHold(int buttonID)
{
    qDebug() << "WidgetHmiMode::slotPersonalHold" << buttonID;
    if (serialPortTransceiver_->isPortOK()==true)
    {
        QByteArray valueArray;
        valueArray.push_back((char)buttonID);
        TLVWriter tlv(TLV::TAG_PersonalButtonHold,valueArray);
        QByteArray *txArray = tlv.getStuffedArray();
        serialPortTransceiver_->write(*txArray);
        delete txArray;
    }
}
void WidgetMode::slotPersonalReleased(int buttonID)
{
    qDebug() << "WidgetHmiMode::slotPersonalReleased" << buttonID;
    qDebug() << "";
    if (serialPortTransceiver_->isPortOK()==true)
    {
        QByteArray valueArray;
        valueArray.push_back((char)buttonID);
        TLVWriter tlv(TLV::TAG_PersonalButtonReleased,valueArray);
        QByteArray *txArray = tlv.getStuffedArray();
        serialPortTransceiver_->write(*txArray);
        delete txArray;
    }
}
//============================================================================================================
void WidgetMode::slotPersonalStart()
{
    if (serialPortTransceiver_->isPortOK()==true)
    {
        QByteArray valueArray;
        TLVWriter tlv(TLV::TAG_Personal,valueArray);
        QByteArray *txArray = tlv.getStuffedArray();
        serialPortTransceiver_->write(*txArray);
        delete txArray;
    }

}
//============================================================================================================
void WidgetMode::slotPersonalExit()
{
    qDebug() << "WidgetHmiMode::slotPersonalExit";
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
void WidgetMode::slotTestConcentricStart(qint32 pauseBeforeTest)
{
    qDebug() << "WidgetHmiMode::slotTestConcentricStart" << QString::number(pauseBeforeTest);
    if (serialPortTransceiver_->isPortOK()==true)
    {
        QByteArray valueArray;
        valueArray.append((char*)&pauseBeforeTest,4);
        TLVWriter tlv(TLV::TAG_TestConcentric,valueArray);
        QByteArray *txArray = tlv.getStuffedArray();
        serialPortTransceiver_->write(*txArray);
        delete txArray;
    }

}
//============================================================================================================
void WidgetMode::slotTestEccentricStart(qint32 pauseBeforeTest)
{
    qDebug() << "WidgetHmiMode::slotTestEccentricStart" << QString::number(pauseBeforeTest);
    if (serialPortTransceiver_->isPortOK()==true)
    {
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

    qDebug() << "WidgetHmiMode::slotTestStaticStart";
    if (serialPortTransceiver_->isPortOK()==true)
    {
        TLVWriter tlv(TLV::TAG_TestStatic,testSettings);
        QByteArray *txArray = tlv.getStuffedArray();
        serialPortTransceiver_->write(*txArray);
        delete txArray;
    }

}
//============================================================================================================
void WidgetMode::slotResetError()
{
    qDebug() << "WidgetHmiMode::slotResetError";
    if (serialPortTransceiver_->isPortOK()==true)
    {
        QByteArray valueArray;
        TLVWriter tlv(TLV::TAG_ResetError,valueArray);
        QByteArray *txArray = tlv.getStuffedArray();
        serialPortTransceiver_->write(*txArray);
        delete txArray;
    }

}
//============================================================================================================
void WidgetMode::slotExcerciseIsokineticStart()
{
    qDebug() << "WidgetHmiMode::slotExcerciseIsokineticStart";
    if (serialPortTransceiver_->isPortOK()==true)
    {
        QByteArray valueArray;
        TLVWriter tlv(TLV::TAG_ExcerciseIsokinetic,valueArray);
        QByteArray *txArray = tlv.getStuffedArray();
        serialPortTransceiver_->write(*txArray);
        delete txArray;
    }


}
//============================================================================================================
void WidgetMode::slotCancel()
{
    qDebug() << "WidgetHmiMode::slotCancel";
    if (serialPortTransceiver_->isPortOK()==true)
    {
        QByteArray valueArray;
        TLVWriter tlv(TLV::TAG_Cancel,valueArray);
        QByteArray *txArray = tlv.getStuffedArray();
        serialPortTransceiver_->write(*txArray);
        delete txArray;
    }


}
//============================================================================================================
void WidgetMode::debugTimerTimeout(void){

    qDebug() << "WidgetMode::debugTimerTimeout";

    QByteArray value(43,(char)0);


    quint8 mode = 5;
    memcpy(&value.data()[0],&mode,sizeof(mode));

    qint32 phase = 2;
    memcpy(&value.data()[31],&phase,sizeof(value));


    static qint32 positionRel = 0;
    positionRel = ((positionRel+345+((qrand() % 100))))%10000;
    memcpy(&value.data()[34],&positionRel,sizeof(positionRel));

    qint32 force = ((qrand() % 200)-100);
    memcpy(&value.data()[39],&force,sizeof(force));



    newMessageReceived(TLV::TAG_ReportCurrentMode,0,value);




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



