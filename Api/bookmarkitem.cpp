#include "bookmarkitem.h"

BookmarkItem::BookmarkItem(QObject *parent) : QObject{parent}
{

}

void BookmarkItem::appendChild(std::unique_ptr<BookmarkItem> &&child)
{
    m_childItems.push_back(std::move(child));
}

BookmarkItem *BookmarkItem::child(int row)
{
    return row >= 0 && row < childCount() ? m_childItems.at(row).get() : nullptr;
}

int BookmarkItem::childCount() const
{
    return int(m_childItems.size());
}

int BookmarkItem::columnCount() const
{
    // return int(m_itemData.count());
    return 1;
}

QVariant BookmarkItem::data(int column) const
{
    return m_itemData.value(column);
}

int BookmarkItem::row() const
{
    if (m_parentItem == nullptr)
        return 0;
    const auto it = std::find_if(m_parentItem->m_childItems.cbegin(), m_parentItem->m_childItems.cend(),
                                 [this](const std::unique_ptr<BookmarkItem> &treeItem) {
                                     return treeItem.get() == this;
                                 });

    if (it != m_parentItem->m_childItems.cend())
        return std::distance(m_parentItem->m_childItems.cbegin(), it);
    Q_ASSERT(false); // should not happen
    return -1;
}

BookmarkItem *BookmarkItem::parentItem()
{

}
