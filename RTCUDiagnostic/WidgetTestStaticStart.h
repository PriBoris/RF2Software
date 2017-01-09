#ifndef WIDGETTESTSTATICSTART_H
#define WIDGETTESTSTATICSTART_H

#include <QWidget>
#include <QSettings>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QDebug>
class WidgetTestStaticStart : public QWidget
{
    Q_OBJECT

    QString legend_;

    QString registryNamePause_;
    qint32 defaultValuePause_;
    qint32 pauseValue_;

    QString registryNameDuration_;
    qint32 defaultValueDuration_;
    qint32 durationValue_;

    QString registryNamePositionRel_;
    qint32 defaultValuePositionRel_;
    qint32 positionRelValue_;


    QPushButton *btnStart;
    QLabel *lblPause;
    QLineEdit *edtPause;
    QLabel *lblDuration;
    QLineEdit *edtDuration;
    QLabel *lblPositionRel;
    QLineEdit *edtPositionRel;
    QHBoxLayout *lo;

    QSettings *settings;


public:
    explicit WidgetTestStaticStart(
            QString legend,
            QString registryNamePause,
            qint32 defaultValuePause,
            QString registryNameDuration,
            qint32 defaultValueDuration,
            QString registryNamePositionRel,
            qint32 defaultValuePositionRel,
            QWidget *parent = 0
            );

signals:
    void startButtonClicked(QByteArray &testSettings);


public slots:
    void pauseEditingFinished();
    void durationEditingFinished();
    void positionRelEditingFinished();
    void startButtonClicked();


};

#endif // WIDGETTESTSTATICSTART_H
