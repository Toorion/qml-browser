#ifndef URLLINEEDIT_H
#define URLLINEEDIT_H

#include <QLineEdit>

class UrlLineEdit : public QLineEdit
{
public:
    explicit UrlLineEdit(QWidget *parent = nullptr);

    QAction *installAction;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // URLLINEEDIT_H
