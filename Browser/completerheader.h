#ifndef COMPLETERHEADER_H
#define COMPLETERHEADER_H

#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include "completermodel.h"

class CompleterHeader : public QHeaderView
{
    Q_OBJECT
public:
    explicit CompleterHeader(QWidget *parent = nullptr);

public slots:
    void btnAllClicked(bool checked = false);
    void btnBookmarkClicked(bool checked = false);
    void btnHistoryClicked(bool checked = false);
    CompleterItem::Type filterType() {
        return m_type;
    };

signals:
    void typeChanged(CompleterItem::Type &activeType);


protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;

private:
    mutable QWidget *m_widget;
    QHBoxLayout *m_layout;
    QPushButton *m_btnAll;
    QPushButton *m_btnBookmark;
    QPushButton *m_btnHistory;
    CompleterItem::Type m_type;

};

#endif // COMPLETERHEADER_H
