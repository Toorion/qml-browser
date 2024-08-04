#ifndef BOOKMARKITEM_H
#define BOOKMARKITEM_H

#include <QObject>

class BookmarkItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString parentId READ parentId WRITE setParentId)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString color READ icon WRITE setColor)
    Q_PROPERTY(QString icon READ icon WRITE setIcon)
    Q_PROPERTY(int x READ x WRITE setX)
    Q_PROPERTY(int y READ y WRITE setY)
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
public:
    explicit BookmarkItem(QObject *parent = nullptr);

    void setId(const QString id) {
        m_id = id;
    };

    QString id() const {
        return m_id;
    }

    void setParentId(const QString parent_id) {
        m_parent_id = parent_id;
    };

    QString parentId() const {
        return m_parent_id;
    }

    void setName(const QString name) {
        m_name = name;
    };

    QString name() const {
        return m_name;
    }

    void setColor(const QString color) {
        m_color = color;
    };

    QString color() const {
        return m_color;
    }

    void setIcon(const QString icon) {
        m_icon = icon;
    };

    QString icon() const {
        return m_icon;
    }

    void setX(const int x) {
        m_x = x;
    };

    int x() const {
        return m_x;
    }

    void setY(const int y) {
        m_y = y;
    };

    int y() const {
        return m_y;
    }

    void setWidth(const int width) {
        m_width = width;
    };

    int width() const {
        return m_width;
    }

    void setHeight(const int height) {
        m_height = height;
    };

    int height() const {
        return m_height;
    }

    void appendChild(std::unique_ptr<BookmarkItem> &&child);
    BookmarkItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    BookmarkItem *parentItem();

signals:

private:
    QString m_id;
    QString m_parent_id = nullptr;
    QString m_name;
    QString m_color;
    QString m_icon;
    int m_x = 0;
    int m_y = 0;
    int m_width = 0;
    int m_height = 0;

    BookmarkItem *m_parentItem;
    std::vector<std::unique_ptr<BookmarkItem>> m_childItems;
};

#endif // BOOKMARKITEM_H
