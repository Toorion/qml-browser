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
#include "apiweb.h"

ApiWeb::ApiWeb(NetworkAccessManagerFactory *networkManagerFactory, QObject *parent) : ApiCommon(networkManagerFactory, parent)
{
    m_window = new Window(this);

    m_document = new Document(this);

    m_locationUrl = new LocationUrl();

    m_window->location.setLocationUrl(m_locationUrl);

    m_document->location.setLocationUrl(m_locationUrl);
}
