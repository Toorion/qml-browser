#ifndef QMLVALUE_H
#define QMLVALUE_H

#include <QObject>
#include <QVariant>

class QmlValue : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant *value READ value WRITE setValue NOTIFY valueChanged)
public:
    explicit QmlValue(QObject *parent = nullptr);

    QVariant *value() {
        return m_value;
    }

    void setValue(QVariant *value) {
        if(value == m_value) {
            return;
        }
        m_value = value;
        emit valueChanged(*value);
    }

signals:
    void valueChanged(const QVariant &value);

private:
    QVariant *m_value;
};

#endif // QMLVALUE_H
