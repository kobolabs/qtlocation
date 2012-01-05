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
***************************************************************************/

#include <qnumeric.h>
#include "qdeclarativegeoboundingbox_p.h"

QT_BEGIN_NAMESPACE

/*!
    \qmlclass BoundingBox QDeclarativeGeoBoundingBox
    \inqmlmodule QtLocation 5
    \ingroup qml-QtLocation5-positioning
    \since QtLocation 5.0

    \brief The BoundingBox element represents a rectangular geographic area.

    For behavioral details, please see \l QGeoBoundingBox.
*/

QDeclarativeGeoBoundingBox::QDeclarativeGeoBoundingBox(QObject* parent) :
        QDeclarativeGeoBoundingArea(parent), m_height(qQNaN()), m_width(qQNaN())
{
}

QDeclarativeGeoBoundingBox::QDeclarativeGeoBoundingBox(const QGeoBoundingBox& box, QObject* parent) :
    QDeclarativeGeoBoundingArea(parent),
    m_declarativeBottomLeft(box.bottomLeft()),
    m_declarativeBottomRight(box.bottomRight()),
    m_declarativeTopLeft(box.topLeft()),
    m_declarativeTopRight(box.topRight()),
    m_declarativeCenter(box.center()),
    m_box(box),
    m_height(box.height()),
    m_width(box.width())
{
}

/*!
    \qmlproperty QGeoBoundingBox BoundingBox::box

    This property is used to provide an interface between C++ and QML code.  First a pointer to a
    BoundingBox object must be obtained from C++, then use the \l {QObject::property()}{property()}
    and \l {QObject::setProperty()}{setProperty()} functions to get and set the \c box property.

    The following gets the QGeoBoundingBox representing this object from C++:

    \snippet snippets/cpp/cppqml.cpp BoundingBox get

    The following sets the properties of this object based on a QGeoBoundingBox object from C++:

    \snippet snippets/cpp/cppqml.cpp BoundingBox set
*/
void QDeclarativeGeoBoundingBox::setBox(const QGeoBoundingBox& box)
{
    m_box = box;
    synchronizeDeclarative();
}

QGeoBoundingBox QDeclarativeGeoBoundingBox::box()
{
    return m_box;
}

QGeoBoundingArea *QDeclarativeGeoBoundingBox::area() const
{
    return m_box.clone();
}

QDeclarativeCoordinate* QDeclarativeGeoBoundingBox::bottomLeft()
{
    return &m_declarativeBottomLeft;
}

/*!
  \qmlproperty Coordinate BoundingBox::bottomLeft

  This property holds the bottom left coordinate of this bounding box.

  Note: this property's changed() signal is currently emitted only if the
  whole element changes, not if only the contents of the element change.

  \sa {QGeoBoundingBox}
  */

void QDeclarativeGeoBoundingBox::setBottomLeft(QDeclarativeCoordinate *coordinate)
{
    if (m_box.bottomLeft() == coordinate->coordinate())
        return;
    m_box.setBottomLeft(coordinate->coordinate());
    synchronizeDeclarative();
    emit bottomLeftChanged();
}

Q_INVOKABLE bool QDeclarativeGeoBoundingBox::contains(QDeclarativeCoordinate* coordinate)
{
    if (!coordinate)
        return false;
    return m_box.contains(coordinate->coordinate());
}

QDeclarativeCoordinate* QDeclarativeGeoBoundingBox::bottomRight()
{
    return &m_declarativeBottomRight;
}

/*!
  \qmlproperty Coordinate BoundingBox::bottomRight

  This property holds the bottom right coordinate of this bounding box.

  Note: this property's changed() signal is currently emitted only if the
  whole element changes, not if only the contents of the element change.

  \sa {QGeoBoundingBox}
  */

void QDeclarativeGeoBoundingBox::setBottomRight(QDeclarativeCoordinate *coordinate)
{
    if (m_box.bottomRight() == coordinate->coordinate())
        return;
    m_box.setBottomRight(coordinate->coordinate());
    synchronizeDeclarative();
    emit bottomRightChanged();
}

QDeclarativeCoordinate* QDeclarativeGeoBoundingBox::topLeft()
{
    return &m_declarativeTopLeft;
}

/*!
  \qmlproperty Coordinate BoundingBox::topLeft

  This property holds the top left coordinate of this bounding box.

  Note: this property's changed() signal is currently emitted only if the
  whole element changes, not if only the contents of the element change.

  \sa {QGeoBoundingBox}
  */

void QDeclarativeGeoBoundingBox::setTopLeft(QDeclarativeCoordinate *coordinate)
{
    if (m_box.topLeft() == coordinate->coordinate())
        return;
    m_box.setTopLeft(coordinate->coordinate());
    synchronizeDeclarative();
    emit topLeftChanged();
}

QDeclarativeCoordinate* QDeclarativeGeoBoundingBox::topRight()
{
    return &m_declarativeTopRight;
}

/*!
  \qmlproperty Coordinate BoundingBox::topRight

  This property holds the top right coordinate of this bounding box.

  Note: this property's changed() signal is currently emitted only if the
  whole element changes, not if only the contents of the element change.

  \sa {QGeoBoundingBox}
  */

void QDeclarativeGeoBoundingBox::setTopRight(QDeclarativeCoordinate *coordinate)
{
    if (m_box.topRight() == coordinate->coordinate())
        return;
    m_box.setTopRight(coordinate->coordinate());
    synchronizeDeclarative();
    emit topRightChanged();
}

QDeclarativeCoordinate* QDeclarativeGeoBoundingBox::center()
{
    return &m_declarativeCenter;
}

/*!
  \qmlproperty Coordinate BoundingBox::center

  This property holds the center coordinate of this bounding box.

  Note: this property's changed() signal is currently emitted only if the
  whole element changes, not if only the contents of the element change.

  \sa {QGeoBoundingBox}

  */

void QDeclarativeGeoBoundingBox::setCenter(QDeclarativeCoordinate *coordinate)
{
    if (m_box.center() == coordinate->coordinate())
        return;
    m_box.setCenter(coordinate->coordinate());
    synchronizeDeclarative();
    emit centerChanged();
}

double QDeclarativeGeoBoundingBox::height()
{
    return m_height;
}

/*!
  \qmlproperty double BoundingBox::height

  This property holds the height of this bounding box (in degrees).

  \sa {QGeoBoundingBox}
  */

void QDeclarativeGeoBoundingBox::setHeight(const double height)
{
    m_box.setHeight(height);
    synchronizeDeclarative();
}

double QDeclarativeGeoBoundingBox::width()
{
    return m_width;
}

/*!
  \qmlproperty double BoundingBox::width

  This property holds the width of this bounding box (in degrees).

  \sa {QGeoBoundingBox}

  */

void QDeclarativeGeoBoundingBox::setWidth(const double width)
{
    m_box.setWidth(width);
    synchronizeDeclarative();
}

void QDeclarativeGeoBoundingBox::synchronizeDeclarative()
{
    m_declarativeBottomLeft.setCoordinate(m_box.bottomLeft());
    m_declarativeBottomRight.setCoordinate(m_box.bottomRight());
    m_declarativeTopLeft.setCoordinate(m_box.topLeft());
    m_declarativeTopRight.setCoordinate(m_box.topRight());
    m_declarativeCenter.setCoordinate(m_box.center());
    // Check not to compare two Not a Numbers, which by definition is 'false'.
    if ((!qIsNaN(m_width) || !qIsNaN(m_box.width())) && m_width != m_box.width()) {
        m_width = m_box.width();
        emit widthChanged();
    }
    if ((!qIsNaN(m_height) || !qIsNaN(m_box.height())) && m_height != m_box.height()) {
        m_height = m_box.height();
        emit heightChanged();
    }
}

#include "moc_qdeclarativegeoboundingbox_p.cpp"

QT_END_NAMESPACE
