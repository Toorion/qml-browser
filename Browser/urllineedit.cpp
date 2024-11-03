#include "urllineedit.h"
#include "qabstractitemview.h"
#include "qevent.h"
#include <QMenu>
#include <QAbstractProxyModel>

UrlLineEdit::UrlLineEdit(QWidget *parent) : QLineEdit(parent)
{
    installAction = new QAction(this);
    installAction->setIcon(QIcon(QStringLiteral("icons:download.svg")));
    installAction->setText("Insatll DAPP");
/*
    m_completer = new QCompleter(this);
    m_completerModel = new CompleterModel(this);
    m_completer->setModel(m_completerModel);
    m_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);

    m_completerPopup = new QTableView(this);

    m_completerHeader = new CompleterHeader(m_completerPopup);
    m_completerHeader->setStretchLastSection(true);
    m_completerHeader->setMinimumHeight(m_completerHeaderHeight);
    m_completerPopup->setHorizontalHeader(m_completerHeader);
    m_completerPopup->setMouseTracking(true);

    m_completerRow = new CompleterRow(m_completerPopup);

    m_completer->setPopup(m_completerPopup);
    setCompleter(m_completer);

    m_completerPopup->setItemDelegate(m_completerRow);
    m_completerPopup->installEventFilter(m_completerRow);

    connect(this, &UrlLineEdit::textEdited, this, [=](const QString text) {
        if (m_completerPopup->isHidden()) {
            m_completerHeader->btnAllClicked(true);
        }
        m_completerModel->applyFilter(text, m_completerHeader->filterType());
        updateCompleter();
    });

    connect(m_completerHeader, &CompleterHeader::typeChanged, this, [=](const CompleterItem::Type &type) {
        m_completerModel->applyFilter(this->text(), m_completerHeader->filterType());
        updateCompleter();
    });

    connect(m_completer, static_cast<void (QCompleter::*)(const QModelIndex&)>(&QCompleter::activated), [=](const QModelIndex& index) {
        emit returnPressed();
    });
*/
}

void UrlLineEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();

    menu->insertAction(menu->actions().at(0), installAction);
    //...
    menu->exec(event->globalPos());
    delete menu;
}
/*
void UrlLineEdit::focusOutEvent(QFocusEvent *event)
{
    m_completer->popup()->hide();
    QLineEdit::focusOutEvent(event);
}
*/
void UrlLineEdit::updateCompleter()
{
    int height = m_completerHeaderHeight;

    int rowCount = m_completerPopup->verticalHeader()->count();
    for (int i = 0; i < rowCount; ++i) {
        if (!m_completerPopup->verticalHeader()->isSectionHidden(i)) {
            height += m_completerPopup->verticalHeader()->sectionSize(i);
        }
    }

    height += m_completerPopup->frameWidth() * 2;
    height = qMin(height, int(window()->height() * 0.666));
    m_completerPopup->setMinimumHeight(height);
    m_completerPopup->setMaximumHeight(height);
}
