/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
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

//TESTED_COMPONENT=src/location

#include "tst_qgeoroutingmanager.h"

QT_USE_NAMESPACE


void tst_QGeoRoutingManager::initTestCase()
{
    tst_QGeoRoutingManager::loadRoutingManager();
}

void tst_QGeoRoutingManager::cleanupTestCase()
{
    //delete qgeoroutingmanager;
    delete qgeoserviceprovider;
}

void tst_QGeoRoutingManager::init()
{
}

void tst_QGeoRoutingManager::cleanup()
{
}

void tst_QGeoRoutingManager::loadRoutingManager()
{
    QStringList providers = QGeoServiceProvider::availableServiceProviders();
    QVERIFY(providers.contains("static.georoute.test.plugin"));

    qgeoserviceprovider = new QGeoServiceProvider("static.georoute.test.plugin");
    QVERIFY(qgeoserviceprovider);
    QCOMPARE(qgeoserviceprovider->error(), QGeoServiceProvider::NoError);

    qgeoroutingmanager = qgeoserviceprovider->routingManager();
    QVERIFY(qgeoroutingmanager);

}

void tst_QGeoRoutingManager::supports()
{
    QVERIFY(qgeoroutingmanager->supportsAlternativeRoutes());
    QVERIFY(qgeoroutingmanager->supportsRouteUpdates());
    QVERIFY(qgeoroutingmanager->supportsExcludeAreas());
    QCOMPARE(qgeoroutingmanager->supportedTravelModes(),QGeoRouteRequest::PedestrianTravel);
    QCOMPARE(qgeoroutingmanager->supportedFeatureTypes(),QGeoRouteRequest::TollFeature);
    QCOMPARE(qgeoroutingmanager->supportedFeatureWeights(),QGeoRouteRequest::PreferFeatureWeight);
    QCOMPARE(qgeoroutingmanager->supportedRouteOptimizations(),QGeoRouteRequest::FastestRoute);
    QCOMPARE(qgeoroutingmanager->supportedSegmentDetails(),QGeoRouteRequest::BasicSegmentData);
    QCOMPARE(qgeoroutingmanager->supportedManeuverDetails(),QGeoRouteRequest::BasicManeuvers);
}

void tst_QGeoRoutingManager::locale()
{
    QLocale *german = new QLocale(QLocale::German, QLocale::Germany);
    QLocale *english = new QLocale(QLocale::C, QLocale::AnyCountry);

    qgeoroutingmanager->setLocale(*german);

    QCOMPARE(qgeoroutingmanager->locale(), *german);

    QVERIFY(qgeoroutingmanager->locale() != *english);

    delete german;
    delete english;
}

void tst_QGeoRoutingManager::name()
{
    QString name = "static.georoute.test.plugin";
    QCOMPARE(qgeoroutingmanager->managerName(), name);
}

void tst_QGeoRoutingManager::version()
{
    QCOMPARE(qgeoroutingmanager->managerVersion(), 2);
}

void tst_QGeoRoutingManager::calculate()
{
    QString error = "no error";
    origin = new QGeoCoordinate(12.12 , 23.23);
    destination = new QGeoCoordinate(34.34 , 89.32);
    request = new QGeoRouteRequest(*origin, *destination);

    reply = qgeoroutingmanager->calculateRoute(*request);

    QCOMPARE(reply->error(), QGeoRouteReply::NoError);
    QCOMPARE(reply->errorString(), error);

    delete origin;
    delete destination;
    delete request;
    delete reply;
}


void tst_QGeoRoutingManager::update()
{
    QString error = "no error";
    position = new QGeoCoordinate(34.34, 89.32);
    route = new QGeoRoute();

    reply = qgeoroutingmanager->updateRoute(*route, *position);

    QCOMPARE(reply->error(), QGeoRouteReply::CommunicationError);
    QCOMPARE(reply->errorString(), error);

    delete position;
    delete route;
    delete reply;
}

QTEST_APPLESS_MAIN(tst_QGeoRoutingManager)

