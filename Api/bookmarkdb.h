#ifndef BOOKMARKDB_H
#define BOOKMARKDB_H

#include <appdb.h>
#include "Api_global.h"
#include "bookmarkitem.h"
#include <QSqlQuery>

class API_EXPORT BookmarkDb : public AppDb
{
public:
    BookmarkDb();

    bool create() override;

    int static insert(const BookmarkItem &item);

    bool static update(const BookmarkItem &item);

    bool static remove(const int id);

    QList<BookmarkItem*> static list();

private:
    bool bind(QSqlQuery *query, const BookmarkItem &item);
};

#endif // BOOKMARKDB_H
