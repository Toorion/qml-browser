#include "bookmarklinkitem.h"
#include <QVariant>

BookmarkLinkItem::BookmarkLinkItem(QVariantMap data)
    : m_itemData(std::move(data))
{
    if (!m_itemData.contains("bookmarkId") || m_itemData.value("bookmarkId").toInt() < 1) {
        m_itemData.insert("bookmarkId", 1);
    }
}


const QList<QByteArray> BookmarkLinkItem::fields()
{
    return QList<QByteArray>{
        "id",
        "bookmarkId",
        "title",
        "link",
        "icon",
        "description",
        "idx",
        "added",
        "updated",
        "pin",
        "shortcut",
        "hotkey",
        "html",
        "text"
    };
}

QVariant BookmarkLinkItem::data(int column) const
{
    return m_itemData.value(fields()[column]);
}

bool BookmarkLinkItem::setData(int column, const QVariant &value)
{
    m_itemData[fields()[column]].setValue(value);
    return true;
}

QVariant BookmarkLinkItem::value(const QByteArray &key) const
{
    return m_itemData.value(key);
}

void BookmarkLinkItem::setValue(const QByteArray &key, const QVariant &value)
{
    m_itemData[key].setValue(value);
}

int BookmarkLinkItem::id()
{
    return value("id").toUInt();
}

void BookmarkLinkItem::setId(int id)
{
    setValue("id", id);
}

