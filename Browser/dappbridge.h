#ifndef DAPPBRIDGE_H
#define DAPPBRIDGE_H

#include <QObject>
#include <QStringList>
#include <QString>
#include <QUrl>
#include <QDir>

#include "apppaths.h"

class DappBridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList dapps READ dapps NOTIFY dappsChanged)
public:
    explicit DappBridge(QObject *parent = nullptr);

    QStringList dapps() const { return m_dapps; }

    Q_INVOKABLE void refreshDapps();

signals:
    void dappsChanged();
private:
    QDir m_dappsDir;
    QStringList m_dapps;
};

#endif // DAPPBRIDGE_H
