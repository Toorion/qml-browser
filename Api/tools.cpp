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
#include "tools.h"

Tools::Tools()
{

}

QString Tools::bytesToText(double bytes)
{
    if(bytes) {
        if(bytes > 1024 * 1024 * 1024) {
            return QString::asprintf("%.2fGb", bytes / 1024 / 1024 / 1024);
        } else if(bytes > 1024 * 1024) {
            return QString::asprintf("%.2fMb", bytes / 1024 / 1024);
        } else if(bytes > 1024) {
            return QString::asprintf("%.2fKb", bytes / 1024);
        }

        return QString::asprintf("%d", (int) bytes);
    }
    return QString("");
}

QVariantMap Tools::QJSValueToMap(QJSValue obj)
{
    QVariantMap map;

    // JS Object
    if(obj.prototype().hasOwnProperty("__proto__")) {
        QJSValueIterator it(obj);
        while (it.hasNext()) {
            it.next();
            map.insert(it.name(), it.value().toVariant());
        }
    } else { // QAbstractItemModel
        QJSValueIterator it(obj.prototype());
        while (it.hasNext()) {
            it.next();
            map.insert(it.name(), obj.property(it.name()).toVariant());
        }
    }

    return map;
}
