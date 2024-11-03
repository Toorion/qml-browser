#include "completerrow.h"
#include <QPainter>
#include "completermodel.h"
#include <QApplication>

CompleterRow::CompleterRow(QObject *parent) : QStyledItemDelegate(parent)
{
}

void CompleterRow::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariantMap data = qvariant_cast<QVariantMap>(index.data());

    auto typeRect = QRect(option.rect);
    typeRect.setY(option.rect.y()+5);
    typeRect.setX(5);

    painter->save();
    if(option.state & QStyle::State_MouseOver) {
        painter->fillRect(option.rect, option.palette.highlight());
        QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
    } else {
        QApplication::restoreOverrideCursor();
    }

    QPen typePen(QColor("#0e5a77"));
    painter->setPen(typePen);
    painter->drawText(typeRect, CompleterModel::typeName(data["typeId"].toInt()) + ": ");
    painter->restore();

    auto titleRect = QRect(option.rect);
    titleRect.setY(option.rect.y()+5);
    titleRect.setX(70);

    painter->drawText(titleRect, data["title"].toString());
}

bool CompleterRow::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::Leave || ev->type() == QEvent::Hide) {
        QApplication::restoreOverrideCursor();
    }
    return obj->eventFilter(obj, ev);
}

