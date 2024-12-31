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
    auto list =  BookmarkDb::list();
    for (int i = 0; i < list->size(); i++) {
        auto row = list->at(i);
        m_items.append(new BookmarkItem(row));
    }
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
    item->setValue("childsCount", 0);

    int idx = 0;
    int parentTo = item->value("parentId").toInt();
    if (parentTo > 0) {
        for (int i = 0; i< m_items.size(); i ++) {
            auto row = m_items.at(i);
            if (row->value("id").toInt() == parentTo) {
                idx = i;
                m_items.at(i)->setValue("childsCount", row->value("childsCount").toInt()+1);
                emit dataChanged(createIndex(0,0), createIndex(m_items.size() - 1, 0), QList{Qt::UserRole + 10});
            } else {
                if (idx > 0) {
                    idx = i;
                    if (row->value("parentId").toInt() != parentTo) {
                        break;
                    }
                }
            }
        }
        beginInsertRows(QModelIndex(), idx, idx);
        m_items.insert(idx, item);
    } else {
        beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
        m_items.append(item);
    }

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

void BookmarkItemModel::save(const QVariantMap &data)
{
    int id = data.value("id").toInt();
    BookmarkItem *item = new BookmarkItem(data);
    if (id > 0) {
        for (int i = 0; i < m_items.count(); i++) {
            if (m_items.at(i)->id() == id) {
                m_items.replace(i, item);
                BookmarkDb::update(*item);
                return;
            }
        }
    }

    BookmarkDb::insert(*item);
}

void BookmarkItemModel::remove(const int id)
{
    BookmarkDb::remove(id);
    for (int i = 0; i< m_items.size(); i ++) {
        if (m_items.at(i)->id() == id) {
            removeItem(i);
            return;
        }
    }
}

bool BookmarkItemModel::setProperty(const int idx, const QString name, const QVariant value)
{
    QHash<int, QByteArray> roles = roleNames();
    for (auto i = roles.cbegin(), end = roles.cend(); i != end; ++i) {
        if (name.compare(i.value()) == 0) {
            setData(createIndex(idx,0), value, i.key());
            return true;
        }
    }
    return false;
}
