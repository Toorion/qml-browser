#include "completermodel.h"
#include "bookmarkdb.h"
#include "bookmarklinkitem.h"
#include "historydb.h"
#include "historyitem.h"

CompleterModel::CompleterModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

int CompleterModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

int CompleterModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant CompleterModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_items.count())
        return QVariant();

    auto *item = m_items[index.row()];

    if (role == 0)
        return QVariantMap{
            {"title", item->title},
            {"id", item->id},
            {"typeId", item->typeId},
            {"url", item->url},
            {"description", item->description}
        };
    if (role == 2) {
        return QVariant(item->url);
    }

    return QVariant();
}

QVariant CompleterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant("");
}

bool CompleterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

bool CompleterModel::applyFilter(const QString &text, const CompleterItem::Type &type)
{
    beginResetModel();
    m_items.clear();

    if (text.length() > 2) {
        if (type == CompleterItem::Type::AllType || type == CompleterItem::Type::HistoryType) {
            auto hrows = HistoryDb::findByText(text);
            HistoryItem* hrow;
            foreach (hrow, *hrows) {
                m_items.append(new CompleterItem{hrow->id(), CompleterItem::Type::HistoryType, hrow->title(), hrow->url()});
            }
        }

        if (type == CompleterItem::Type::AllType || type == CompleterItem::Type::BookmarkType) {
            auto rows = BookmarkDb::findLinkByText(text);
            BookmarkLinkItem* row;
            foreach (row, *rows) {
                m_items.append(new CompleterItem{row->id(), CompleterItem::Type::BookmarkType, row->value("title").toString(), row->value("link").toString()});
            }
        }
    }
    endResetModel();
}

QString CompleterModel::typeName(const int &typeId)
{
    if (typeId == CompleterItem::Type::BookmarkType) {
        return "Bookmark";
    } else if (typeId == CompleterItem::Type::HistoryType) {
        return "History";
    }
}
