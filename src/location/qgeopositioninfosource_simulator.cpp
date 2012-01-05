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

#include "qgeopositioninfosource_simulator_p.h"
#include "qlocationdata_simulator_p.h"
#include "qlocationconnection_simulator_p.h"

#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtCore/QDataStream>

#include <QtNetwork/QLocalSocket>

QT_BEGIN_NAMESPACE

namespace Simulator
{
    QGeoPositionInfo toPositionInfo(const QGeoPositionInfoData &data)
    {
        QDateTime timestamp;
        if (data.dateTime.isValid())
            timestamp = data.dateTime;
        else
            timestamp = QDateTime::currentDateTime();
        QGeoCoordinate coord(data.latitude, data.longitude, data.altitude);
        QGeoPositionInfo info(coord, timestamp);
        info.setAttribute(QGeoPositionInfo::Direction, data.direction);
        info.setAttribute(QGeoPositionInfo::GroundSpeed, data.groundSpeed);
        info.setAttribute(QGeoPositionInfo::VerticalSpeed, data.verticalSpeed);
        info.setAttribute(QGeoPositionInfo::MagneticVariation, data.magneticVariation);
        info.setAttribute(QGeoPositionInfo::HorizontalAccuracy, data.horizontalAccuracy);
        info.setAttribute(QGeoPositionInfo::VerticalAccuracy, data.verticalAccuracy);
        return info;
    }
} //namespace

// Location API

QGeoPositionInfoSourceSimulator::QGeoPositionInfoSourceSimulator(QObject *parent)
    : QGeoPositionInfoSource(parent)
    , timer(new QTimer(this))
    , requestTimer(new QTimer(this))
{
    Simulator::LocationConnection::ensureSimulatorConnection();

    connect(timer, SIGNAL(timeout()), this, SLOT(updatePosition()));
    requestTimer->setSingleShot(true);
    connect(requestTimer, SIGNAL(timeout()), this, SLOT(updatePosition()));
}

QGeoPositionInfoSourceSimulator::~QGeoPositionInfoSourceSimulator()
{
}

QGeoPositionInfo QGeoPositionInfoSourceSimulator::lastKnownPosition(bool /*fromSatellitePositioningMethodsOnly*/) const
{
    return lastPosition;
}

QGeoPositionInfoSource::PositioningMethods QGeoPositionInfoSourceSimulator::supportedPositioningMethods() const
{
    // Is GPS now Satelite or not? Guessing so...
    return QGeoPositionInfoSource::SatellitePositioningMethods;
}

void QGeoPositionInfoSourceSimulator::setUpdateInterval(int msec)
{
    // If msec is 0 we send updates as data becomes available, otherwise we force msec to be equal
    // to or larger than the minimum update interval.
    if (msec != 0 && msec < minimumUpdateInterval())
        msec = minimumUpdateInterval();

    QGeoPositionInfoSource::setUpdateInterval(msec);
    if (timer->isActive()) {
        timer->setInterval(msec);
        timer->start();
    }
}

int QGeoPositionInfoSourceSimulator::minimumUpdateInterval() const
{
    return qtPositionInfo()->minimumInterval;
}

void QGeoPositionInfoSourceSimulator::startUpdates()
{
    int interval = updateInterval();
    if (interval < minimumUpdateInterval())
        interval = minimumUpdateInterval();
    timer->setInterval(interval);
    timer->start();
}

void QGeoPositionInfoSourceSimulator::stopUpdates()
{
    timer->stop();
}

void QGeoPositionInfoSourceSimulator::requestUpdate(int timeout)
{
    if (!requestTimer->isActive()) {
        // Get a single update within timeframe
        if (timeout < minimumUpdateInterval() && timeout != 0)
            emit updateTimeout();
        else {
            requestTimer->start(timeout * qreal(0.75));
        }
    }
}

void QGeoPositionInfoSourceSimulator::updatePosition()
{
    if (qtPositionInfo()->enabled) {
        lastPosition = Simulator::toPositionInfo(*qtPositionInfo());
        emit positionUpdated(lastPosition);
    } else {
        emit updateTimeout();
    }
}

#include "moc_qgeopositioninfosource_simulator_p.cpp"

QT_END_NAMESPACE
