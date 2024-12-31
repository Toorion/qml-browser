#include "bookmarklinkmodel.h"
#include "bookmarkdb.h"
#include <QUrl>
#include <QDateTime>
#include <QTextDocumentFragment>

BookmarkLinkModel::BookmarkLinkModel(QObject *parent)
    : QAbstractListModel{parent}
{}

BookmarkLinkModel::~BookmarkLinkModel()
{

}

BookmarkLinkModel &BookmarkLinkModel::instance(QObject *parent)
{
    static BookmarkLinkModel instance(parent);
    if (instance.m_items.count() == 0) {
        instance.populate();
    }
    return instance;
}

void BookmarkLinkModel::populate()
{
    m_items.clear();
    auto rows = BookmarkDb::listLinks();
    for (int i = 0; i < rows->count(); i++) {
        m_items.append(new BookmarkLinkItem(rows->at(i)));
    }
}

void BookmarkLinkModel::addItem(BookmarkLinkItem *item)
{
    int id = BookmarkDb::insertLink(*item);
    item->setId(id);
    QModelIndex idx = QModelIndex();
    beginInsertRows(idx, 0, 0);
    m_items.prepend(item);
    endInsertRows();
}

bool BookmarkLinkModel::addNewItem(const QString title, const QUrl link, const QUrl icon, const QString html, const QString description)
{
    auto item = BookmarkDb::findLinkByLink(link);
    if (item != nullptr && item->id() > 0) {
        item->setValue("updated", QDateTime::currentDateTime());
        BookmarkDb::updateLink(*item);
        return true;
    }
    auto map = new QVariantMap();
    map->insert("title", title);
    map->insert("link", link);
    map->insert("icon", icon);
    map->insert("description", description);
    map->insert("html", html);
    map->insert("text", QTextDocumentFragment::fromHtml(html).toPlainText());
    map->insert("added", QDateTime::currentDateTime());
    map->insert("updated", QDateTime::currentDateTime());
    BookmarkDb::insertLink(*(new BookmarkLinkItem(*map)));
    return true;
}

void BookmarkLinkModel::updateItem(BookmarkLinkItem *item)
{
    BookmarkDb::updateLink(*item);
    for( int i=0; i<m_items.count(); ++i) {
        if(item->id() == m_items.at(i)->id()) {
            QModelIndex topLeft = createIndex(0,0);
            emit dataChanged(topLeft, topLeft);
            break;
        }
    }
}

bool BookmarkLinkModel::removeItem(const qint64 index)
{
    if(index >= 0 && index < m_items.count()) {
        beginRemoveRows(QModelIndex(), index, index);
        BookmarkDb::removeLink(m_items.at(index)->id());
        m_items.removeAt(index);
        endRemoveRows();
        emit layoutChanged(); // required for reindexing items
        return true;
    }
    return false;
}

int BookmarkLinkModel::count(const QString queryType, const QVariantMap args)
{
    return BookmarkDb::count(queryType, args);
}

int BookmarkLinkModel::updateLinksBookmark(const int oldBookmarkId, const int newBookmarkId)
{
    int rows = BookmarkDb::update("move_links", QVariantMap{{"bookmarkId", oldBookmarkId}}, QVariantMap{{"bookmarkId", newBookmarkId}});
    if (rows > 0) {
        beginResetModel();
        for( int i=0; i<m_items.count(); i++) {
            auto item = m_items.at(i);
            if (item->value("bookmarkId") == oldBookmarkId) {
                item->setValue("bookmarkId", newBookmarkId);
                m_items.replace(i, item);
            }
        }
        endResetModel();
    }
}


int BookmarkLinkModel::rowCount(const QModelIndex &parent) const
{
    return m_items.count();
}

QVariant BookmarkLinkModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_items.count())
        return QVariant();

    BookmarkLinkItem *item = m_items[index.row()];
    return item->data(role - Qt::UserRole - 1);
}

bool BookmarkLinkModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!hasIndex(index.row(), index.column(), index.parent()) || !value.isValid())
        return false;

    BookmarkLinkItem *item = m_items[index.row()];
    item->setData(role - Qt::UserRole - 1, value);

    m_changedRow = index.row();

    emit dataChanged(index, index, { role } );

    return true ;
}

QVariantMap BookmarkLinkModel::get(int row) const
{
    QVariantMap map;
    if(row < 0 || row >= m_items.count())
        return map;

    for(int i = 0; i < BookmarkLinkItem::fields().count(); i++) {
        map[BookmarkLinkItem::fields()[i]] = m_items[row]->data(i);
    }

    return map;
}

void BookmarkLinkModel::save(const QVariantMap &data)
{
    int id = data.value("id").toInt();
    BookmarkLinkItem *item = new BookmarkLinkItem(data);
    if (id > 0) {
        for (int i = 0; i < m_items.count(); i++) {
            if (m_items.at(i)->id() == id) {
                m_items.replace(i, item);
                BookmarkDb::updateLink(*item);
                return;
            }
        }
    }

    BookmarkDb::insertLink(*item);

}

QHash<int, QByteArray> BookmarkLinkModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    for (int i = 0; i < BookmarkLinkItem::fields().count(); i++) {
        roles[Qt::UserRole+i+1] = BookmarkLinkItem::fields().at(i);
    }
    return roles;
}








