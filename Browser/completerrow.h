#ifndef COMPLETERROW_H
#define COMPLETERROW_H

#include <QStyledItemDelegate>

class CompleterRow : public QStyledItemDelegate
{
public:
    explicit CompleterRow(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    bool eventFilter(QObject *obj, QEvent *ev) override;
};

#endif // COMPLETERROW_H
