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

#include "qgeoboundingarea.h"

QT_BEGIN_NAMESPACE

/*!
    \class QGeoBoundingArea
    \inmodule QtLocation
    \ingroup QtLocation-positioning
    \since QtLocation 5.0

    \brief The QGeoBoundingArea class defines a geographic area.

    This class is the base class for classes which specify a geographic
    area.

    For the sake of consistency, subclasses should describe the specific
    details of the associated areas in terms of QGeoCoordinate instances
    and distances in meters.
*/

/*!
    \enum QGeoBoundingArea::AreaType

    Describes the type of a bounding area.

    \value BoxType A box shaped bounding area.

    \value CircleType A circular bounding area.
*/

/*!
    Destroys this bounding area.
*/
QGeoBoundingArea::~QGeoBoundingArea() {}

/*!
\fn virtual QGeoBoundingArea::AreaType QGeoBoundingArea::type() const

Returns the type of this area.
*/

/*!
\fn bool QGeoBoundingArea::isValid() const

Returns whether this bounding area is valid.

An area is considered to be invalid if some of the data that is required to
unambiguously describe the area has not been set or has been set to an
unsuitable value.
*/

/*!
\fn bool QGeoBoundingArea::isEmpty() const

Returns whether this bounding area is empty.

An empty area is a region which has a geometrical area of 0.
*/

/*!
\fn bool QGeoBoundingArea::contains(const QGeoCoordinate &coordinate) const

Returns whether the coordinate \a coordinate is contained within this
area.
*/

/*!
\fn QGeoBoundingArea* QGeoBoundingArea::clone() const

Creates a new QGeoBoundingArea that is a deep copy of this bounding area.
*/

/*!
\fn bool QGeoBoundingArea::operator==(const QGeoBoundingArea &other) const

  Returns true if the \a other bounding area is equivalent to this bounding area,
  otherwise returns false.
*/

/*!
    \fn bool QGeoBoundingArea::operator!=(const QGeoBoundingArea &other) const
    Returns true if \a other is not equivalent to this bounding area,
    otherwise returns false.
*/

QT_END_NAMESPACE
