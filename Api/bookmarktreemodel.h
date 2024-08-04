#ifndef BOOKMARKTREEMODEL_H
#define BOOKMARKTREEMODEL_H

#include <QAbstractItemModel>
#include "bookmarkitem.h"

class BookmarkTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    Q_DISABLE_COPY_MOVE(BookmarkTreeModel)

    explicit BookmarkTreeModel(QObject *parent = nullptr);
    ~BookmarkTreeModel() override;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = {}) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = {}) const override;
    int columnCount(const QModelIndex &parent = {}) const override;

private:
    static void setupModelData(const QList<QStringView> &lines, BookmarkItem *parent);

    std::unique_ptr<BookmarkItem> rootItem;
};

#endif // BOOKMARKTREEMODEL_H
