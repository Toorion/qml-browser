#include "completerheader.h"
#include <QPainter>
#include <QPushButton>
#include <QBoxLayout>

CompleterHeader::CompleterHeader(QWidget *parent) : QHeaderView (Qt::Orientation::Horizontal, parent) {
    m_widget = new QWidget(qobject_cast<QWidget*>(parent));
    m_widget->setStyleSheet("QPushButton:checked { background-color : red; color : blue; }");

    m_layout = new QHBoxLayout(m_widget);
    m_layout->setContentsMargins(5, 5, 5, 5);
    m_layout->setSpacing(10);
    m_widget->setLayout(m_layout);

    m_btnAll = new QPushButton("All", m_widget);
    m_btnAll->setCursor(Qt::PointingHandCursor);
    m_btnAll->setCheckable(true);
    m_btnAll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_btnAll, &QPushButton::clicked, this, &CompleterHeader::btnAllClicked);
    m_layout->addWidget(m_btnAll);

    m_btnBookmark = new QPushButton("Bookmark", m_widget);
    m_btnBookmark->setCursor(Qt::PointingHandCursor);
    m_btnBookmark->setCheckable(true);
    m_btnBookmark->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_btnBookmark, &QPushButton::clicked, this, &CompleterHeader::btnBookmarkClicked);
    m_layout->addWidget(m_btnBookmark);

    m_btnHistory = new QPushButton("History", m_widget);
    m_btnHistory->setCursor(Qt::PointingHandCursor);
    m_btnHistory->setCheckable(true);
    m_btnHistory->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(m_btnHistory, &QPushButton::clicked, this, &CompleterHeader::btnHistoryClicked);
    m_layout->addWidget(m_btnHistory);

    m_btnAll->setChecked(true);
    m_type = CompleterItem::Type::AllType;
}

void CompleterHeader::btnAllClicked(bool checked)
{
    Q_UNUSED(checked);
    m_btnAll->setChecked(true);
    m_btnBookmark->setChecked(false);
    m_btnHistory->setChecked(false);
    m_type = CompleterItem::Type::AllType;
    emit typeChanged(m_type);
}

void CompleterHeader::btnBookmarkClicked(bool checked)
{
    Q_UNUSED(checked);
    m_btnBookmark->setChecked(true);
    m_btnAll->setChecked(false);
    m_btnHistory->setChecked(false);
    m_type = CompleterItem::Type::BookmarkType;
    emit typeChanged(m_type);
}

void CompleterHeader::btnHistoryClicked(bool checked)
{
    Q_UNUSED(checked);
    m_btnHistory->setChecked(true);
    m_btnAll->setChecked(false);
    m_btnBookmark->setChecked(false);
    m_type = CompleterItem::Type::HistoryType;
    emit typeChanged(m_type);
}


void CompleterHeader::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    QRect widgetRect = rect.adjusted(5, 5, -5, -5);
    m_widget->setGeometry(widgetRect);
    m_widget->show();
}

