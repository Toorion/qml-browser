/****************************************************************************
*
* QmlBrowser - Web browser with QML page support
* Copyright (C) 2022 Denis Solomatin <toorion@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#include "qi.h"
#include "apicommon.h"
#include "downloaddb.h"
#include "historyitemmodel.h"
#include "dynamicobject.h"

QSettings *Qi::settings = nullptr;

Qi::Qi(QObject *parent) : QObject(parent)
{

}


DownloadItemModel *Qi::downloadHistoryModel()
{
    ApiCommon *apiCommon = qobject_cast<ApiCommon*>(parent());

    if(!apiCommon->accessRights()->allowDownloadsHistory) {
        apiCommon->console()->error("Access to download history deny");
        return new DownloadItemModel();
    }
    return m_downloadManagerWidget->downloadItemModel();
}

HistoryItemModel *Qi::visitHistoryModel()
{
    ApiCommon *apiCommon = qobject_cast<ApiCommon*>(parent());

    if(!apiCommon->accessRights()->allowVisitHitory) {
        apiCommon->console()->error("Access to visit history deny");
        return new HistoryItemModel(this);
    }
    return &HistoryItemModel::instance();
}

QQmlPropertyMap *Qi::settingsModel()
{
    ApiCommon *apiCommon = qobject_cast<ApiCommon*>(parent());

    if(!apiCommon->accessRights()->allowSetings) {
        apiCommon->console()->error("Access to settings deny");
        return new DynamicObject(new QObject(), this);
    }
    return new DynamicObject(Qi::settings, this);
}
