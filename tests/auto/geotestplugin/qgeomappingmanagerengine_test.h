/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef QGEOMAPPINGMANAGERENGINE_TEST_H
#define QGEOMAPPINGMANAGERENGINE_TEST_H

#include <qgeoserviceprovider.h>
#include <qgeomappingmanagerengine.h>
#include <QLocale>
#include <QPixmap>
#include <QByteArray>
#include <QBuffer>
#include <qgeotiledmapreply.h>
#include "maptype.h"
#include "tilespec.h"

#include <QTimer>
#include <QDebug>
#include <QTimerEvent>

QT_USE_NAMESPACE


class TiledMapReplyTest :public QGeoTiledMapReply
{
    Q_OBJECT
public:
    TiledMapReplyTest(const TileSpec &spec, QObject *parent=0): QGeoTiledMapReply (spec, parent) {}
    void callSetError ( Error error, const QString & errorString ) {setError(error, errorString);}
    void callSetFinished ( bool finished ) { setFinished(finished);}
    void callSetCached(bool cached) { setFinished(cached);}
    void callSetMapImageData(const QByteArray &data) { setMapImageData(data); }
    void callSetMapImageFormat(const QString &format) { setMapImageFormat(format); }
    void abort() { emit aborted(); }

Q_SIGNALS:
    void aborted();
};

class QGeoMappingManagerEngineTest: public QGeoMappingManagerEngine

{
Q_OBJECT
public:
    QGeoMappingManagerEngineTest(const QMap<QString, QVariant> &parameters,
        QGeoServiceProvider::Error *error, QString *errorString) :
        QGeoMappingManagerEngine(parameters),
        finishRequestImmediately_(true),
        mappingReply_(0),
        timerId_(0),
        errorCode_(QGeoTiledMapReply::NoError)
    {
        Q_UNUSED(error)
        Q_UNUSED(errorString)
        if (parameters.contains("finishRequestImmediately"))
            finishRequestImmediately_ = qvariant_cast<bool>(parameters.value("finishRequestImmediately"));
        setLocale(QLocale (QLocale::German, QLocale::Germany));
        setMinimumZoomLevel(0.0);
        setMaximumZoomLevel(20.0);
    }

    void init()
    {
        setTileSize(QSize(256, 256));
        QList<MapType> types;
        types << MapType(MapType::StreetMap,tr("Street Map"),tr("Test Street Map"), false, 1);
        setSupportedMapTypes(types);
        QGeoMappingManagerEngine::init();
    }

    QGeoTiledMapReply* getTileImage(const TileSpec &spec)
    {
        //qDebug() << __FUNCTION__ << "in test plugin for spec: " << spec.x() << spec.y() << spec.zoom();
        mappingReply_ = new TiledMapReplyTest(spec, this);

        QImage im(256, 256, QImage::Format_RGB888);
        im.fill(0);

        for (int y = 0; y < im.height(); y++) {
            for (int x = 0; x < im.width(); x++) {
                im.setPixel(x, y, (x + y) % 255);
            }
        }
        QPixmap pm = QPixmap::fromImage(im);

        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        pm.save(&buffer, "PNG");

        mappingReply_->callSetMapImageData(bytes);
        mappingReply_->callSetMapImageFormat("PNG");
        mappingReply_->callSetFinished(true);

        return static_cast<QGeoTiledMapReply*>(mappingReply_);
    }

public Q_SLOTS:
    void requestAborted()
    {
        if (timerId_) {
            killTimer(timerId_);
            timerId_ = 0;
        }
        errorString_ = "";
        errorCode_ = QGeoTiledMapReply::NoError;
    }

protected:
     void timerEvent(QTimerEvent *event)
     {
         Q_ASSERT(timerId_ == event->timerId());
         Q_ASSERT(mappingReply_);
         killTimer(timerId_);
         timerId_ = 0;
         if (errorCode_) {
             mappingReply_->callSetError(errorCode_, errorString_);
             emit tileError(mappingReply_->tileSpec(), errorString_);
        } else {
             mappingReply_->callSetError(QGeoTiledMapReply::NoError, "no error");
             mappingReply_->callSetFinished(true);
         }
         // emit finished(mappingReply_); todo tileFinished
     }

private:
    bool finishRequestImmediately_;
    TiledMapReplyTest* mappingReply_;
    int timerId_;
    QGeoTiledMapReply::Error errorCode_;
    QString errorString_;
};

#endif