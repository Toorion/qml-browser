#include "bookmarkitemmodel.h"
#include "bookmarkdb.h"

BookmarkItemModel::BookmarkItemModel(QObject *parent)
    : QAbstractListModel{parent}
{}

BookmarkItemModel &BookmarkItemModel::instance(QObject *parent)
{
    static BookmarkItemModel instance(parent);
    if (instance.m_items.count() == 0) {
        instance.populate();
    }
    return instance;
}

BookmarkItemModel::~BookmarkItemModel()
{

}

void BookmarkItemModel::populate()
{
    m_items.clear();
    appendChilds(0, BookmarkDb::list(), 0);
}

uint BookmarkItemModel::appendChilds(const uint parentId, const QList<QVariantMap> *list, const uint depth)
{
    uint childsCount = 0;
    for (int i = 0; i < list->size(); i++) {
        auto row = list->at(i);
        if (row.value("parentId").toUInt() != parentId) {
            continue;
        }
        BookmarkItem* item = new BookmarkItem(row);
        m_items.append(item);
        item->setValue("childsCount", appendChilds(item->id(), list, depth + 1));
        item->setValue("depth", depth);
        item->setValue("expanded", false);
        item->setValue("visible", depth == 0);
        childsCount++;
    }
    return childsCount;
}


QHash<int, QByteArray> BookmarkItemModel::roleNames() const {
    QHash<int, QByteArray> roles;
    for (int i = 0; i < BookmarkItem::fields().count(); i++) {
        roles[Qt::UserRole+i+1] = BookmarkItem::fields().at(i);
    }
    return roles;
}


void BookmarkItemModel::addItem(BookmarkItem *item)
{
    int id = BookmarkDb::insert(*item);
    item->setId(id);
    QModelIndex idx = QModelIndex();
    beginInsertRows(idx, 0, 0);
    m_items.prepend(item);
    endInsertRows();
}

void BookmarkItemModel::add(const QVariantMap &data)
{
    auto item = new BookmarkItem(data);
    int id = BookmarkDb::insert(*item);
    item->setId(id);
    beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
    m_items.append(item);
    endInsertRows();
}

void BookmarkItemModel::insert(int row, const QVariantMap &data)
{
    auto item = new BookmarkItem(data);
    int id = BookmarkDb::insert(*item);
    item->setId(id);
    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, item);
    endInsertRows();
}

void BookmarkItemModel::updateItem(BookmarkItem *item)
{
    BookmarkDb::update(*item);
    for (int i=0; i<m_items.count(); ++i) {
        if(item->id() == m_items.at(i)->id()) {
            QModelIndex topLeft = createIndex(0,0);
            emit dataChanged(topLeft, topLeft);
            break;
        }
    }
}

bool BookmarkItemModel::removeItem(const qint64 index)
{
    if(index >= 0 && index < m_items.count()) {
        beginRemoveRows(QModelIndex(), index, index);
        BookmarkDb::remove(m_items.at(index)->id());
        m_items.removeAt(index);
        endRemoveRows();
        emit layoutChanged(); // required for reindexing items
        return true;
    }
    return false;
}

int BookmarkItemModel::rowCount(const QModelIndex & parent) const {
    if(parent.isValid()) return 0;
    return m_items.count();
}

QVariant BookmarkItemModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_items.count())
        return QVariant();

    BookmarkItem *item = m_items[index.row()];
    return item->data(role - Qt::UserRole - 1);
}

bool BookmarkItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
        return false;

    BookmarkItem *item = m_items[index.row()];
    item->setData(role - Qt::UserRole - 1, value);

    m_changedRow = index.row();

    emit dataChanged(index, index, { role } );

    return true ;
}

QVariantMap BookmarkItemModel::get(int row) const {
    QVariantMap map;
    if(row < 0 || row >= m_items.count())
        return map;

    for(int i = 0; i < BookmarkItem::fields().count(); i++) {
        map[BookmarkItem::fields()[i]] = m_items[row]->data(i);
    }

    return map;
}

void BookmarkItemModel::save() const
{
    BookmarkDb::update(*m_items[m_changedRow]);
}
