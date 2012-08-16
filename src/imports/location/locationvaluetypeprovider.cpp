/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "locationvaluetypeprovider.h"
#include "qdeclarativecoordinate_p.h"
#include "qdeclarativegeoshape.h"
#include "qdeclarativegeorectangle.h"
#include "qdeclarativegeocircle.h"
#include <QtLocation/QGeoRectangle>
#include <QtLocation/QGeoCircle>

QT_BEGIN_NAMESPACE

QGeoCoordinate parseCoordinate(const QJSValue &value, bool *ok)
{
    QGeoCoordinate c;

    if (value.isObject()) {
        if (value.hasProperty(QStringLiteral("latitude")))
            c.setLatitude(value.property(QStringLiteral("latitude")).toNumber());
        if (value.hasProperty(QStringLiteral("longitude")))
            c.setLongitude(value.property(QStringLiteral("longitude")).toNumber());
        if (value.hasProperty(QStringLiteral("altitude")))
            c.setAltitude(value.property(QStringLiteral("altitude")).toNumber());

        if (ok)
            *ok = true;
    }

    return c;
}

QGeoRectangle parseRectangle(const QJSValue &value, bool *ok)
{
    QGeoRectangle r;

    *ok = false;

    if (value.isObject()) {
        if (value.hasProperty(QStringLiteral("bottomLeft"))) {
            QGeoCoordinate c = parseCoordinate(value.property(QStringLiteral("bottomLeft")), ok);
            if (*ok)
                r.setBottomLeft(c);
        }
        if (value.hasProperty(QStringLiteral("bottomRight"))) {
            QGeoCoordinate c = parseCoordinate(value.property(QStringLiteral("bottomRight")), ok);
            if (*ok)
                r.setBottomRight(c);
        }
        if (value.hasProperty(QStringLiteral("topLeft"))) {
            QGeoCoordinate c = parseCoordinate(value.property(QStringLiteral("topLeft")), ok);
            if (*ok)
                r.setTopLeft(c);
        }
        if (value.hasProperty(QStringLiteral("topRight"))) {
            QGeoCoordinate c = parseCoordinate(value.property(QStringLiteral("topRight")), ok);
            if (*ok)
                r.setTopRight(c);
        }
        if (value.hasProperty(QStringLiteral("center"))) {
            QGeoCoordinate c = parseCoordinate(value.property(QStringLiteral("center")), ok);
            if (*ok)
                r.setCenter(c);
        }
        if (value.hasProperty(QStringLiteral("height")))
            r.setHeight(value.property(QStringLiteral("height")).toNumber());
        if (value.hasProperty(QStringLiteral("width")))
            r.setWidth(value.property(QStringLiteral("width")).toNumber());
    }

    return r;
}

QGeoCircle parseCircle(const QJSValue &value, bool *ok)
{
    QGeoCircle c;

    *ok = false;

    if (value.isObject()) {
        if (value.hasProperty(QStringLiteral("center"))) {
            QGeoCoordinate coord = parseCoordinate(value.property(QStringLiteral("center")), ok);
            if (*ok)
                c.setCenter(coord);
        }
        if (value.hasProperty(QStringLiteral("radius")))
            c.setRadius(value.property(QStringLiteral("radius")).toNumber());
    }

    return c;
}

LocationValueTypeProvider::LocationValueTypeProvider()
:   QQmlValueTypeProvider()
{
}

bool LocationValueTypeProvider::create(int type, QQmlValueType *&v)
{
    if (type == qMetaTypeId<QGeoCoordinate>())
        return typedCreate<CoordinateValueType>(v);
    else if (type == qMetaTypeId<QGeoShape>())
        return typedCreate<GeoShapeValueType>(v);
    else if (type == qMetaTypeId<QGeoRectangle>())
        return typedCreate<GeoRectangleValueType>(v);
    else if (type == qMetaTypeId<QGeoCircle>())
        return typedCreate<GeoCircleValueType>(v);

    return false;
}

bool LocationValueTypeProvider::init(int type, void *data, size_t dataSize)
{
    if (type == qMetaTypeId<QGeoCoordinate>())
        return typedInit<QGeoCoordinate>(data, dataSize);
    else if (type == qMetaTypeId<QGeoShape>())
        return typedInit<QGeoShape>(data, dataSize);
    else if (type == qMetaTypeId<QGeoRectangle>())
        return typedInit<QGeoRectangle>(data, dataSize);
    else if (type == qMetaTypeId<QGeoCircle>())
        return typedInit<QGeoCircle>(data, dataSize);

    return false;
}

bool LocationValueTypeProvider::destroy(int type, void *data, size_t dataSize)
{
    if (type == qMetaTypeId<QGeoCoordinate>())
        return typedDestroy<QGeoCoordinate>(data, dataSize);
    else if (type == qMetaTypeId<QGeoShape>())
        return typedDestroy<QGeoShape>(data, dataSize);
    else if (type == qMetaTypeId<QGeoRectangle>())
        return typedDestroy<QGeoRectangle>(data, dataSize);
    else if (type == qMetaTypeId<QGeoCircle>())
        return typedDestroy<QGeoCircle>(data, dataSize);

    return false;
}

bool LocationValueTypeProvider::copy(int type, const void *src, void *dst, size_t dstSize)
{
    if (type == qMetaTypeId<QGeoCoordinate>())
        return typedCopyConstruct<QGeoCoordinate>(src, dst, dstSize);
    else if (type == qMetaTypeId<QGeoShape>())
        return typedCopyConstruct<QGeoShape>(src, dst, dstSize);
    else if (type == qMetaTypeId<QGeoRectangle>())
        return typedCopyConstruct<QGeoRectangle>(src, dst, dstSize);
    else if (type == qMetaTypeId<QGeoCircle>())
        return typedCopyConstruct<QGeoCircle>(src, dst, dstSize);

    return false;
}

bool LocationValueTypeProvider::create(int type, int argc, const void *argv[], QVariant *v)
{
    if (type == qMetaTypeId<QGeoCoordinate>()) {
        if (argc == 2) {
            const float *a = reinterpret_cast<const float *>(argv[0]);
            const float *b = reinterpret_cast<const float *>(argv[1]);
            *v = QVariant::fromValue(QGeoCoordinate(*a, *b));
            return true;
        } else if (argc == 3) {
            const float *a = reinterpret_cast<const float *>(argv[0]);
            const float *b = reinterpret_cast<const float *>(argv[1]);
            const float *c = reinterpret_cast<const float *>(argv[2]);
            *v = QVariant::fromValue(QGeoCoordinate(*a, *b, *c));
            return true;
        }
    }

    return false;
}

bool LocationValueTypeProvider::createFromString(int type, const QString &s, void *data, size_t dataSize)
{
    Q_UNUSED(data)
    Q_UNUSED(dataSize)

    if (type == qMetaTypeId<QGeoCoordinate>() || type == qMetaTypeId<QGeoShape>() ||
        type == qMetaTypeId<QGeoRectangle>() || type == qMetaTypeId<QGeoCircle>()) {
        qWarning("Cannot create value type %d from string '%s'", type, qPrintable(s));
    }

    return false;
}

bool LocationValueTypeProvider::createStringFrom(int type, const void *data, QString *s)
{
    Q_UNUSED(data)
    Q_UNUSED(s)

    if (type == qMetaTypeId<QGeoCoordinate>() || type == qMetaTypeId<QGeoShape>() ||
        type == qMetaTypeId<QGeoRectangle>() || type == qMetaTypeId<QGeoCircle>()) {
        qWarning("Cannot create string from value type %d", type);
    }

    return false;
}

bool LocationValueTypeProvider::variantFromJsObject(int type, QQmlV8Handle h, QV8Engine *e, QVariant *v)
{
    Q_UNUSED(h)
    Q_UNUSED(e)
    Q_UNUSED(v)

    if (type == qMetaTypeId<QGeoCoordinate>() || type == qMetaTypeId<QGeoShape>() ||
        type == qMetaTypeId<QGeoRectangle>() || type == qMetaTypeId<QGeoCircle>()) {
        qWarning("Cannot create variant from js object for type %d", type);
    }

    return false;
}

bool LocationValueTypeProvider::equal(int type, const void *lhs, const void *rhs, size_t rhsSize)
{
    Q_UNUSED(rhsSize)

    if (type == qMetaTypeId<QGeoCoordinate>())
        return typedEqual<QGeoCoordinate>(lhs, rhs);
    else if (type == qMetaTypeId<QGeoShape>())
        return typedEqual<QGeoShape>(lhs, rhs);
    else if (type == qMetaTypeId<QGeoRectangle>())
        return typedEqual<QGeoRectangle>(lhs, rhs);
    else if (type == qMetaTypeId<QGeoCircle>())
        return typedEqual<QGeoCircle>(lhs, rhs);

    return false;
}

bool LocationValueTypeProvider::store(int type, const void *src, void *dst, size_t dstSize)
{
    if (type == qMetaTypeId<QGeoCoordinate>())
        return typedStore<QGeoCoordinate>(src, dst, dstSize);
    else if (type == qMetaTypeId<QGeoShape>())
        return typedStore<QGeoShape>(src, dst, dstSize);
    else if (type == qMetaTypeId<QGeoRectangle>())
        return typedStore<QGeoRectangle>(src, dst, dstSize);
    else if (type == qMetaTypeId<QGeoCircle>())
        return typedStore<QGeoCircle>(src, dst, dstSize);

    return false;
}

bool LocationValueTypeProvider::read(int srcType, const void *src, size_t srcSize, int dstType, void *dst)
{
    if (srcType == qMetaTypeId<QGeoCoordinate>())
        return typedRead<QGeoCoordinate>(srcType, src, srcSize, dstType, dst);
    else if (srcType == qMetaTypeId<QGeoShape>())
        return typedRead<QGeoShape>(srcType, src, srcSize, dstType, dst);
    else if (srcType == qMetaTypeId<QGeoRectangle>())
        return typedRead<QGeoRectangle>(srcType, src, srcSize, dstType, dst);
    else if (srcType == qMetaTypeId<QGeoCircle>())
        return typedRead<QGeoCircle>(srcType, src, srcSize, dstType, dst);

    return false;
}

bool LocationValueTypeProvider::write(int type, const void *src, void *dst, size_t dstSize)
{
    if (type == qMetaTypeId<QGeoCoordinate>())
        return typedWrite<QGeoCoordinate>(src, dst, dstSize);
    else if (type == qMetaTypeId<QGeoShape>())
        return typedWrite<QGeoShape>(src, dst, dstSize);
    else if (type == qMetaTypeId<QGeoRectangle>())
        return typedWrite<QGeoRectangle>(src, dst, dstSize);
    else if (type == qMetaTypeId<QGeoCircle>())
        return typedWrite<QGeoCircle>(src, dst, dstSize);

    return false;
}

QT_END_NAMESPACE
