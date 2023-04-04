#include "dynamicobject.h"
#include <QMetaProperty>


DynamicObject::DynamicObject(QObject *baseObject, QObject *parent)
    : QQmlPropertyMap{parent},
      m_baseObject(baseObject)
{
    const QMetaObject *metaObject = baseObject->metaObject();
    int count = metaObject->propertyCount();
    for (int i=0; i<count; ++i) {
        QMetaProperty metaProperty = metaObject->property(i);
        const char *name = metaProperty.name();
        QVariant value = baseObject->property(name);
        insert(name, value);
    }

    connect(this, &DynamicObject::valueChanged, this, &DynamicObject::changeValue);
}

void DynamicObject::changeValue(const QString &key, const QVariant &value)
{
    QByteArray bKey = key.toLocal8Bit();
    m_baseObject->setProperty(bKey.data(), value);
}
