#ifndef BOOKMARKLINKMODEL_H
#define BOOKMARKLINKMODEL_H

#include <QAbstractListModel>
#include "bookmarklinkitem.h"
#include "Api_global.h"

class API_EXPORT BookmarkLinkModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BookmarkLinkModel(QObject *parent = nullptr);
    ~BookmarkLinkModel();
    static BookmarkLinkModel& instance(QObject *parent = nullptr);

    void populate();

    Q_INVOKABLE void addItem(BookmarkLinkItem *item);

    static bool addNewItem(const QString title, const QUrl link,  const QUrl icon, const QString html, const QString description);

    Q_INVOKABLE void updateItem(BookmarkLinkItem *item);

    Q_INVOKABLE bool removeItem(qint64 const index);

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Q_INVOKABLE QVariantMap get(int row) const;

    Q_INVOKABLE void save() const;

protected:
    QHash<int,QByteArray> roleNames() const override;
private:
    BookmarkLinkModel(const BookmarkLinkModel&other);
    BookmarkLinkModel& operator=(const BookmarkLinkModel&other);

    QList<BookmarkLinkItem*> m_items;

    int m_changedRow = 0;
};

#endif // BOOKMARKLINKMODEL_H



