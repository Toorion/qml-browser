#include "bookmarkdb.h"
#include <QSqlQuery>
#include <QSqlError>


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
    if(!query.exec( "CREATE TABLE " + m_tableName + " ("
                "id INTEGER PRIMARY KEY,"
                "parent_id int NOT NULL,"
                "name VARCHAR(1024) NOT NULL,"
                "color VARCHAR(20),"
                "icon VARCHAR(2048),"
                "x INTEGER,"
                "y INTEGER,"
                "width INTEGER,"
                "height INTEGER"
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

    query.prepare("INSERT INTO " + p->m_tableName + " ( parent_id, name, color, icon, x, y, width, height ) "
                "VALUES (:ParentId, :Name, :Color, :Icon, :X, :Y, :Width, :Height)");

    p->bind(&query, item);

    if(!query.exec()){
        return p->queryError("insert", query.lastError().text());
    }

    QVariant id = query.lastInsertId();
    Q_ASSERT(id.isValid() && !id.isNull());

    return id.toInt();

}

bool BookmarkDb::update(const BookmarkItem &item)
{
    BookmarkDb *p = gs_bookmark_db();

    QSqlQuery query(p->m_db);

    query.prepare("UPDATE " + p->m_tableName + " SET name=:Name, parent_id=:ParentId, color=:Color, icon=:Icon, x=:X, y=:Y, width=:Width, height=:Height WHERE id=:Id");
    query.bindValue(":Id",   item.id());
    p->bind(&query, item);

    if(!query.exec()){
        return p->queryError("update", query.lastError().text());
    }

    return true;
}

bool BookmarkDb::remove(const int id)
{
    BookmarkDb *p = gs_bookmark_db();
    return p->removeRecord(id);
}

QList<BookmarkItem *> BookmarkDb::list()
{
    BookmarkDb *p = gs_bookmark_db();

    QList <BookmarkItem*> list;
    QSqlQuery query("SELECT id, parent_id, name, color, icon, x, y, width, height FROM " + p->m_tableName + " ORDER BY id", p->m_db);
    while (query.next()) {
        auto item = new BookmarkItem();
        item->setId(query.value(0).toString());
        item->setParentId(query.value(1).toString());
        item->setName(query.value(2).toString());
        item->setColor(query.value(3).toString());
        item->setIcon(query.value(4).toString());
        item->setX(query.value(5).toInt());
        item->setY(query.value(6).toInt());
        item->setWidth(query.value(7).toInt());
        item->setHeight(query.value(8).toInt());
        list << item;
    }
    return list;
}

bool BookmarkDb::bind(QSqlQuery *query, const BookmarkItem &item)
{
    query->bindValue(":ParentId", item.parentId());
    query->bindValue(":Name", item.name());
    query->bindValue(":Color", item.color());
    query->bindValue(":Icon", item.icon());
    query->bindValue(":X", item.x());
    query->bindValue(":Y", item.y());
    query->bindValue(":Width", item.width());
    query->bindValue(":Height", item.height());
    return true;
}
