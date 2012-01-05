/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtLocation/qtlocation.h>

QT_BEGIN_NAMESPACE

namespace QtLocation {

/*!
    \namespace QtLocation
    \inmodule QtLocation
    \target QtLocation Namespace

    \brief The QtLocation namespace contains miscellaneous identifiers used throughout the
           QtLocation module.
*/

/*!
    \enum QtLocation::Visibility

    Defines the visibility of a QPlace or QPlaceCategory.

    \value UnspecifiedVisibility    No explicit visibility has been defined.
    \value DeviceVisibility         Places and categories with DeviceVisibility are only stored on
                                    the local device.
    \value PrivateVisibility        Places and categories with PrivateVisibility are only visible
                                    to the current user.  The data may be stored either locally or
                                    on a remote service or both.
    \value PublicVisibility         Places and categories with PublicVisibility are visible to
                                    everyone.

    A particular manager may support one or more visibility scopes.  For example a manager from one provider may only provide places
    that are public to everyone, whilst another may provide both public and private places.

    \note The meaning of unspecified visibility depends on the context it is used.

    When \e saving a place or category, the
    default visibility is unspecified meaning that the manager chooses an appropriate visibility scope for the item.

    When \e searching for places, unspecified means that places of any scope is returned.
*/
}

QT_END_NAMESPACE
