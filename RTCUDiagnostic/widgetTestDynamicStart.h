#ifndef WIDGETTESTSTART_H
#define WIDGETTESTSTART_H

#include <QWidget>
#include <QSettings>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QDebug>



class WidgetTestDynamicStart : public QWidget
{
    Q_OBJECT

    QString legend_;
    QString registryName_;
    qint32 defaultValue_;
    qint32 pauseValue_;

    QPushButton *btnStart;
    QLabel *lblPause;
    QLineEdit *edtPause;
    QHBoxLayout *lo;

    QSettings *settings;


public:
    explicit WidgetTestDynamicStart(
            QString legend,
            QString registryName,
            qint32 defaultValue,
            QWidget *parent = 0
            );

signals:
    void startButtonClicked(qint32 pauseBeforeTest);


public slots:
    void pauseEditingFinished();
    void startButtonClicked();



};

#endif // WIDGETTESTSTART_H
