#include "bookmarkitemmodel.h"
#include "bookmarkdb.h"

BookmarkItemModel::BookmarkItemModel(QObject *parent)
    : QAbstractListModel{parent}
{}

BookmarkItemModel &BookmarkItemModel::instance(QObject *parent)
{
    static BookmarkItemModel instance(parent);
    return instance;
}

BookmarkItemModel::~BookmarkItemModel()
{

}

void BookmarkItemModel::fill(QList<BookmarkItem *> items)
{
    m_items = items;
}


QHash<int, QByteArray> BookmarkItemModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdxRole] = "idx";
    roles[IdRole] = "id";
    roles[ParentIdRole] = "parentId";
    roles[NameRole] = "name";
    roles[ColorRole] = "color";
    roles[IconRole] = "icon";
    roles[XRole] = "x";
    roles[YRole] = "y";
    roles[WidthRole] = "width";
    roles[HeightRole] = "height";
    return roles;
}


void BookmarkItemModel::addItem(BookmarkItem *item)
{
    int id = BookmarkDb::insert(*item);
    item->setId(QString::number(id));
    QModelIndex idx = QModelIndex();
    beginInsertRows(idx, 0, 0);
    m_items.prepend(item);
    endInsertRows();
}

void BookmarkItemModel::updateItem(BookmarkItem *item)
{
    BookmarkDb::update(*item);
    for( int i=0; i<m_items.count(); ++i) {
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
        BookmarkDb::remove(m_items.at(index)->id().toInt());
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
    QString v("");

    if (index.row() < 0 || index.row() >= m_items.count())
        return QVariant();

    BookmarkItem *item = m_items[index.row()];
    if (role == IdxRole)
        return index.row();
    else if (role == IdRole)
        return item->id();
    else if (role == ParentIdRole)
        return item->parentId();
    else if (role == NameRole)
        return item->name();
    else if (role == ColorRole)
        return item->color();
    else if (role == IconRole)
        return item->icon();
    else if (role == XRole)
        return item->x();
    else if (role == YRole)
        return item->y();
    else if (role == WidthRole)
        return item->width();
    else if (role == HeightRole)
        return item->height();
    return QVariant();
}

bool BookmarkItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
        return false;

    BookmarkItem *item = m_items[index.row()];
    if (role == NameRole) item->setName(value.toString());
    else if (role == ColorRole) item->setColor(value.toString());
    else if (role == IconRole) item->setIcon(value.toString());
    else if (role == XRole) item->setX(value.toInt());
    else if (role == YRole) item->setY(value.toInt());
    else if (role == WidthRole) item->setWidth(value.toInt());
    else if (role == HeightRole) item->setHeight(value.toInt());
    else return false;

    m_changedRow = index.row();

    emit dataChanged(index, index, { role } );

    return true ;
}

QVariantMap BookmarkItemModel::get(int row) const {
    QVariantMap map;
    if(row < 0 || row >= m_items.count())
        return map;

    map["id"] = m_items[row]->id();
    map["parentId"] = m_items[row]->parentId();
    map["name"] = m_items[row]->name();
    map["color"] = m_items[row]->color();
    map["icon"] = m_items[row]->icon();
    map["x"] = m_items[row]->x();
    map["y"] = m_items[row]->y();
    map["width"] = m_items[row]->width();
    map["height"] = m_items[row]->height();
    return map;
}

void BookmarkItemModel::save() const
{
    BookmarkDb::update(*m_items[m_changedRow]);
}
