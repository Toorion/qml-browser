#include "bookmarkdb.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QUrl>

Q_GLOBAL_STATIC(BookmarkDb, gs_bookmark_db)


BookmarkDb::BookmarkDb() : AppDb()
{
    m_dbName = QLatin1String("bookmark_v1.db");
    m_tableName = QLatin1String("bookmark");
    connect();
}

bool BookmarkDb::create()
{
    QSqlQuery query(m_db);
    // Linux path 4095 max
    if(!query.exec( "CREATE TABLE bookmark ("
                "id INTEGER PRIMARY KEY,"
                "parentId int NOT NULL,"
                "name VARCHAR(1024) NOT NULL,"
                "color VARCHAR(20),"
                "icon VARCHAR(2048),"
                "idx INTEGER,"
                "pin INTEGER,"
                "expanded INTEGER"
                " )"
            )) {
        return queryError("create", query.lastError().text());
    }
    if(!query.exec( "INSERT INTO bookmark (id, parentId, name)"
                "VALUES (1, 0, \"Unsorted\")"
            )) {
        return queryError("insert", query.lastError().text());
    }

    if(!query.exec( "CREATE TABLE link ("
                "id INTEGER PRIMARY KEY,"
                "bookmarkId int NOT NULL,"
                "title VARCHAR(1024) NOT NULL,"
                "link VARCHAR(2048),"
                "icon VARCHAR(2048),"
                "description VARCHAR(2048),"
                "idx INTEGER,"
                "added TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                "updated TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                "pin INTEGER,"
                "shortcut TEXT,"
                "hotkey TEXT,"
                "html TEXT,"
                "text TEXT"
                " )"
            )) {
        return queryError("create", query.lastError().text());
    }
    return true;
}

int BookmarkDb::insert(const BookmarkItem &item)
{
    BookmarkDb *p = gs_bookmark_db();

    QSqlQuery query(p->m_db);

    query.prepare("INSERT INTO " + p->m_tableName + " ( parentId, name, color, icon, idx, pin, expanded ) "
                "VALUES (:parentId, :name, :color, :icon, :idx, :pin, :expanded)");
    for(int i = 0; i < BookmarkItem::fields().count(); i++) {
        query.bindValue(":" + BookmarkItem::fields()[i], item.data(i));
    }

    if(!query.exec()){
        return p->queryError("insert", query.lastError().text());
    }

    QVariant id = query.lastInsertId();
    Q_ASSERT(id.isValid() && !id.isNull());

    return id.toInt();

}

int BookmarkDb::insertLink(const BookmarkLinkItem &item)
{
    auto *p = gs_bookmark_db();

    QSqlQuery query(p->m_db);

    query.prepare("INSERT INTO link ( bookmarkId, title, link, icon, idx, added, updated, pin, shortcut, hotkey, html, text ) "
                                                    "VALUES (:bookmarkId, :title, :link, :icon, :idx, :added, :updated, :pin, :shortcut, :hotkey, :html, :text)");
    for(int i = 0; i < BookmarkLinkItem::fields().count(); i++) {
        query.bindValue(":" + BookmarkLinkItem::fields()[i], item.data(i));
    }

    if(!query.exec()){
        return p->queryError(QLatin1String("link"), "insert", query.lastError().text());
    }

    QVariant id = query.lastInsertId();
    Q_ASSERT(id.isValid() && !id.isNull());

    return id.toInt();
}

bool BookmarkDb::update(const BookmarkItem &item)
{
    BookmarkDb *p = gs_bookmark_db();

    QSqlQuery query(p->m_db);

    query.prepare("UPDATE " + p->m_tableName + " SET name=:name, parentId=:parentId, color=:color, icon=:icon, idx=:idx, pin=:pin, expanded=:expanded WHERE id=:id");
    for(int i = 0; i < BookmarkItem::fields().count(); i++) {
        query.bindValue(":" + BookmarkItem::fields()[i], item.data(i));
    }

    if(!query.exec()){
        return p->queryError("update", query.lastError().text());
    }

    return true;
}

bool BookmarkDb::updateLink(const BookmarkLinkItem &item)
{
    auto *p = gs_bookmark_db();

    QSqlQuery query(p->m_db);

    query.prepare("UPDATE link SET bookmarkId=:bookmarkId, title=:title, link=:link, icon=:icon, idx=:idx, added=:added, updated=:updated, pin=:pin, shortcut=:shortcut, hotkey=:hotkey, html=:html, text=:text WHERE id=:id");
    for(int i = 0; i < BookmarkLinkItem::fields().count(); i++) {
        query.bindValue(":" + BookmarkLinkItem::fields()[i], item.data(i));
    }

    if(!query.exec()){
        return p->queryError(QLatin1String("link"), "update", query.lastError().text());
    }

    return true;
}

bool BookmarkDb::remove(const int id)
{
    BookmarkDb *p = gs_bookmark_db();
    return p->removeRecord(id);
}

bool BookmarkDb::removeLink(const int id)
{
    BookmarkDb *p = gs_bookmark_db();
    return p->removeRecord(QLatin1String("link"), id);
}

QList<QVariantMap>* BookmarkDb::list()
{
    BookmarkDb *p = gs_bookmark_db();

    QSqlQuery query("SELECT id, parentId, name, color, icon, idx, pin, expanded FROM " + p->m_tableName + " ORDER BY parentId, id", p->m_db);
    auto list = new QList<QVariantMap>();
    while (query.next()) {
        list->append(queryToMap(&query));
    }
    return list;
}

QList<QVariantMap> *BookmarkDb::listLinks()
{
    auto *p = gs_bookmark_db();

    QSqlQuery query("SELECT * FROM link ORDER BY idx", p->m_db);
    if (!query.isActive()) {
        p->queryError(QLatin1String("link"), "select", query.lastError().text());
    }
    auto list = new QList<QVariantMap>();
    while (query.next()) {
        list->append(queryToMap(&query));
    }
    return list;
}

BookmarkLinkItem *BookmarkDb::findLinkByLink(const QUrl &link)
{
    auto *p = gs_bookmark_db();

    QSqlQuery query("SELECT * FROM link WHERE link=?", p->m_db);
    query.addBindValue(link);
    if (!query.exec()) {
        p->queryError(QLatin1String("link"), "select(find)", query.lastError().text());
    }
    if(!query.first()) {
        return nullptr;
    }
    return new BookmarkLinkItem(queryToMap(&query));
}

QList<BookmarkLinkItem*> *BookmarkDb::findLinkByText(const QString &text)
{
    auto *p = gs_bookmark_db();

    QSqlQuery query("SELECT * FROM link WHERE link LIKE ?", p->m_db);
    query.addBindValue(QString("%%1%").arg(text));
    if (!query.exec()) {
        p->queryError(QLatin1String("link"), "select(find_text)", query.lastError().text());
    }
    auto list = new QList<BookmarkLinkItem*>();
    while (query.next()) {
        list->append(new BookmarkLinkItem(queryToMap(&query)));
    }
    return list;
}

