#ifndef WIDGETSETTINGSINTEGER_H
#define WIDGETSETTINGSINTEGER_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QBoxLayout>
#include <QIntValidator>

#include <QSettings>
#include <QDebug>



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

    qint32 getWriteValue() {
        return edtWriteValue->text().toInt();
    };

    void setReadValue(qint32 newValue) {
        value = newValue;
        lblReadValue->setText(QString::number(value));
    };

    void clearReadValue() {
        lblReadValue->setText("---");
    }

    void setUnknownReadValue() {
        lblReadValue->setText("не задано");
    }

signals:

public slots:
    void editingFinished();


};

#endif // WIDGETSETTINGSINTEGER_H
