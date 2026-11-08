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
