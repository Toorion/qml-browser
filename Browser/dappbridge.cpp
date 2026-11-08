#include "dappbridge.h"

DappBridge::DappBridge(QObject *parent)
    : QObject{parent}, m_dappsDir(AppPaths::dappsPath().toLocalFile())
{
    m_dappsDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    refreshDapps();
}

void DappBridge::refreshDapps() {
    m_dapps = m_dappsDir.entryList();
    emit dappsChanged();
}

void DappBridge::launchDapp(const QString& dappId) {
    QDir dappDir(m_dappsDir.filePath(dappId));
    if (dappDir.exists()) {
        // TODO
    }
}

