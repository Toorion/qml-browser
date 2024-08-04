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
#include "historyitemmodel.h"
#include "dynamicobject.h"
#include <QQmlEngine>

QSettings *Qi::settings = nullptr;

Qi::Qi(QObject *parent) : QObject(parent)
{
    m_progressInfo = new QQmlPropertyMap();
}

DownloadItemModel *Qi::downloadHistoryModel()
{
    ApiCommon *apiCommon = qobject_cast<ApiCommon*>(parent());

    DownloadItemModel *model;
    if(apiCommon->accessRights()->allowDownloadsHistory) {
        model = m_downloadManagerWidget->downloadItemModel();
    } else {
        apiCommon->log()->error("Access to download history deny");
        model = new DownloadItemModel();
    }
    QQmlEngine::setObjectOwnership(model, QQmlEngine::CppOwnership);
    return model;
}

HistoryItemModel *Qi::visitHistoryModel()
{
    ApiCommon *apiCommon = qobject_cast<ApiCommon*>(parent());

    HistoryItemModel *model;
    if(apiCommon->accessRights()->allowVisitHitory) {
        model = &HistoryItemModel::instance();
    } else {
        apiCommon->log()->error("Access to visit history deny");
        model = new HistoryItemModel(this);
    }
    QQmlEngine::setObjectOwnership(model, QQmlEngine::CppOwnership);
    return model;
}

BookmarkItemModel *Qi::bookmarkModel()
{
    ApiCommon *apiCommon = qobject_cast<ApiCommon*>(parent());

    BookmarkItemModel *model;
    if(apiCommon->accessRights()->allowBookmark) {
        model = &BookmarkItemModel::instance();
    } else {
        apiCommon->log()->error("Access to bookmark deny");
        model = new BookmarkItemModel(this);
    }
    QQmlEngine::setObjectOwnership(model, QQmlEngine::CppOwnership);
    return model;

}

QQmlPropertyMap *Qi::settingsModel()
{
    ApiCommon *apiCommon = qobject_cast<ApiCommon*>(parent());

    DynamicObject *model;
    if(apiCommon->accessRights()->allowSetings) {
        model = new DynamicObject(Qi::settings, this);
    } else {
        apiCommon->log()->error("Access to settings deny");
        model = new DynamicObject(new QObject(), this);
    }
    QQmlEngine::setObjectOwnership(model, QQmlEngine::CppOwnership);
    return model;
}

