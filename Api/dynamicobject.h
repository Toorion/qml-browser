#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include <QQmlPropertyMap>
#include "Api_global.h"

class API_EXPORT DynamicObject : public QQmlPropertyMap
{
public:
    explicit DynamicObject(QObject *baseObject, QObject *parent = nullptr);

private:
    void changeValue(const QString &key, const QVariant &value);
    QObject *m_baseObject;
};

#endif // DYNAMICOBJECT_H
