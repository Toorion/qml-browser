#ifndef BOOKMARKITEMMODEL_H
#define BOOKMARKITEMMODEL_H

#include <QAbstractListModel>
#include "bookmarkitem.h"
#include "Api_global.h"

class API_EXPORT BookmarkItemModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit BookmarkItemModel(QObject *parent = nullptr);
    ~BookmarkItemModel();
    static BookmarkItemModel& instance(QObject *parent = nullptr);

    enum ItemRoles {
        IdxRole = Qt::UserRole + 1,
        IdRole,
        ParentIdRole,
        NameRole,
        ColorRole,
        IconRole,
        XRole,
        YRole,
        WidthRole,
        HeightRole
    };

    void fill(QList<BookmarkItem*> items);

    Q_INVOKABLE BookmarkItem* create() {
        return new BookmarkItem();
    }

    Q_INVOKABLE void addItem(BookmarkItem *item);

    void updateItem(BookmarkItem *item);

    Q_INVOKABLE bool removeItem(qint64 const index);

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Q_INVOKABLE QVariantMap get(int row) const;

    Q_INVOKABLE void save() const;

protected:
    QHash<int,QByteArray> roleNames() const override;
private:
    BookmarkItemModel(const BookmarkItemModel&other);
    BookmarkItemModel& operator=(const BookmarkItemModel&other);

    QList<BookmarkItem*> m_items;

    int m_changedRow = 0;
};

#endif // BOOKMARKITEMMODEL_H
