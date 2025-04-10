#ifndef BOOKMARKITEM_H
#define BOOKMARKITEM_H

#include <QVariantList>
#include "Api_global.h"

class API_EXPORT BookmarkItem
{

public:
    explicit BookmarkItem(QVariantMap data);

    static const QList<QByteArray> fields();

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);

    QVariant value(const QByteArray &key) const;

    void setValue(const QByteArray &key, const QVariant &value);

    int id();

    void setId(int id);

signals:

private:

    QVariantMap m_itemData;

};

#endif // BOOKMARKITEM_H
