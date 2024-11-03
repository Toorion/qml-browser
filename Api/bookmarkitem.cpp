#include "bookmarkitem.h"
#include <QVariant>

BookmarkItem::BookmarkItem(QVariantMap data)
    : m_itemData(std::move(data))
{}

const QList<QByteArray> BookmarkItem::fields()
{
    return QList<QByteArray>{
        "id",
        "parentId",
        "name",
        "color",
        "icon",
        "idx",
        "pin",
        "expanded",
        "depth",
        "childsCount",
        "visible",
    };
}

QVariant BookmarkItem::data(int column) const
{
    return m_itemData.value(fields()[column]);
}

bool BookmarkItem::setData(int column, const QVariant &value)
{
    m_itemData[fields()[column]].setValue(value);
    return true;
}

QVariant BookmarkItem::value(const QByteArray &key) const
{
    return m_itemData.value(key);
}

void BookmarkItem::setValue(const QByteArray &key, const QVariant &value)
{
    m_itemData[key].setValue(value);
}

int BookmarkItem::id()
{
    return value("id").toUInt();
}

void BookmarkItem::setId(int id)
{
    setValue("id", id);
}

