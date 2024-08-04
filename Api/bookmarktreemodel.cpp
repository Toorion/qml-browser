#include "bookmarktreemodel.h"

BookmarkTreeModel::BookmarkTreeModel(QObject *parent)
    : QAbstractItemModel{parent}
{}

BookmarkTreeModel::~BookmarkTreeModel()
{

}

QVariant BookmarkTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};

    const auto *item = static_cast<const BookmarkItem*>(index.internalPointer());
    return item->data(index.column());
}

Qt::ItemFlags BookmarkTreeModel::flags(const QModelIndex &index) const
{
    return index.isValid()
    ? QAbstractItemModel::flags(index) : Qt::ItemFlags(Qt::NoItemFlags);
}

QVariant BookmarkTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return orientation == Qt::Horizontal && role == Qt::DisplayRole
               ? rootItem->data(section) : QVariant{};
}

QModelIndex BookmarkTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};

    BookmarkItem *parentItem = parent.isValid()
                               ? static_cast<BookmarkItem*>(parent.internalPointer())
                               : rootItem.get();

    if (auto *childItem = parentItem->child(row))
        return createIndex(row, column, childItem);
    return {};
}

QModelIndex BookmarkTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    auto *childItem = static_cast<BookmarkItem*>(index.internalPointer());
    BookmarkItem *parentItem = childItem->parentItem();

    return parentItem != rootItem.get()
               ? createIndex(parentItem->row(), 0, parentItem) : QModelIndex{};
}

int BookmarkTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const BookmarkItem *parentItem = parent.isValid()
                                     ? static_cast<const BookmarkItem*>(parent.internalPointer())
                                     : rootItem.get();

    return parentItem->childCount();
}

int BookmarkTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<BookmarkItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

