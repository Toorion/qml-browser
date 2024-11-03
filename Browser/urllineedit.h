#ifndef URLLINEEDIT_H
#define URLLINEEDIT_H

#include <QLineEdit>
#include <QCompleter>
#include "completermodel.h"
#include "completerheader.h"
#include <QTableView>
#include "completerrow.h"

class UrlLineEdit : public QLineEdit
{
public:
    explicit UrlLineEdit(QWidget *parent = nullptr);

    QAction *installAction;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    //void focusOutEvent(QFocusEvent* event) override;
    void updateCompleter();

private:
    QCompleter *m_completer;
    CompleterModel *m_completerModel;
    CompleterHeader *m_completerHeader;
    QTableView *m_completerPopup;
    CompleterRow *m_completerRow;

    int m_completerHeaderHeight = 60;
};

#endif // URLLINEEDIT_H
