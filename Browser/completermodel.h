#ifndef COMPLETERMODEL_H
#define COMPLETERMODEL_H

#include <QAbstractTableModel>
#include <QUrl>

struct CompleterItem {
    int id;

    enum Type {
        AllType = 0,
        BookmarkType,
        HistoryType
    };

    Type typeId;

    QString title;

    QUrl url;

    QString description;
};


class CompleterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CompleterModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &) const override;
    int columnCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool applyFilter(const QString &text, const CompleterItem::Type &type);

    static QString typeName(const int &typeId);

private:
    QList<CompleterItem*> m_items;
};

#endif // COMPLETERMODEL_H
