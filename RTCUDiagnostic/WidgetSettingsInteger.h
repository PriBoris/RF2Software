#ifndef WIDGETSETTINGSINTEGER_H
#define WIDGETSETTINGSINTEGER_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QBoxLayout>
#include <QValidator>
#include <QSettings>



class WidgetSettingsInteger : public QWidget{
    
    Q_OBJECT

    QLabel *lblLegend;
    QLineEdit *edtWriteValue;
    QLabel *lblReadValue;
    QHBoxLayout *lo;

    QString legend_;
    QString registryName_;

    QSettings *settings;
    qint32 value;

public:
    
    explicit WidgetSettingsInteger(
            QString legend,
            QString registryName,
            qint32 defaultValue,
            QWidget *parent = 0
            );

    qint32 getWriteValue();

    void setReadValue(qint32 newValue);

    //void clearReadValue();

    void setUnknownReadValue();

    void setReadOnly();

    void setValidator(QValidator *validator);

signals:

    void signalEditingFinished();

public slots:
    void editingFinished();


};

#endif // WIDGETSETTINGSINTEGER_H
