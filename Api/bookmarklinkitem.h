#ifndef BOOKMARKLINKITEM_H
#define BOOKMARKLINKITEM_H

#include "Api_global.h"
#include <QVariantList>

class API_EXPORT BookmarkLinkItem
{
public:
    explicit BookmarkLinkItem(QVariantMap data);
    static const QList<QByteArray> fields();

    QVariant data(int column) const;
    bool setData(int column, const QVariant &value);

    QVariant value(const QByteArray &key) const;

    void setValue(const QByteArray &key, const QVariant &value);

    int id();

    void setId(int id);


private:

    QVariantMap m_itemData;

};

#endif // BOOKMARKLINKITEM_H
