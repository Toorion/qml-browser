#include "urllineedit.h"
#include "qevent.h"
#include <QMenu>


UrlLineEdit::UrlLineEdit(QWidget *parent) : QLineEdit(parent)
{
    installAction = new QAction(this);
    installAction->setIcon(QIcon(QStringLiteral("icons:download.svg")));
    installAction->setText("Insatll DAPP");

}

void UrlLineEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();

    menu->insertAction(menu->actions().at(0), installAction);
    //...
    menu->exec(event->globalPos());
    delete menu;
}
