#ifndef WIDGETSETTINGSFLOAT_H
#define WIDGETSETTINGSFLOAT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QBoxLayout>
#include <QValidator>

#include <QSettings>
#include <QDebug>


class WidgetSettingsFloat : public QWidget{

    Q_OBJECT

    QLabel *lblLegend;
    QLineEdit *edtWriteValue;
    QLabel *lblReadValue;
    QHBoxLayout *lo;

    QString legend_;
    QString registryName_;

    QSettings *settings;
    float value;

public:
   explicit WidgetSettingsFloat(
            QString legend,
            QString registryName,
            float defaultValue,
            QWidget *parent = 0
            );
    float getWriteValue();
    void setReadValue(float newValue);
    void clearReadValue();
    void setUnknownReadValue();
    void setValidator(QValidator *validator);


signals:

public slots:

    void editingFinished();

};

#endif // WIDGETSETTINGSFLOAT_H
