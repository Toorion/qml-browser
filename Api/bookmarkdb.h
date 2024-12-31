#ifndef BOOKMARKDB_H
#define BOOKMARKDB_H

#include <appdb.h>
#include "Api_global.h"
#include "bookmarkitem.h"
#include "bookmarklinkitem.h"
#include <QSqlQuery>

class API_EXPORT BookmarkDb : public AppDb
{
public:
    BookmarkDb();

    bool create() override;

    static int insert(const BookmarkItem &item);
    static int insertLink(const BookmarkLinkItem &item);

    static bool update(const BookmarkItem &item);
    static bool updateLink(const BookmarkLinkItem &item);

    static bool remove(const int id);
    static bool removeLink(const int id);

    static int count(QString const queryType, QVariantMap args);

    static int update(QString const queryType, QVariantMap const args, QVariantMap const values);

    static QList<QVariantMap>* list();
    static QList<QVariantMap>* listLinks();

    static BookmarkLinkItem *findLinkByLink(const QUrl &link);

    static QList<BookmarkLinkItem *> *findLinkByText(const QString &text);
};

#endif // BOOKMARKDB_H
