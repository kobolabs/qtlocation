/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef QGEOMAPDATA_P_H
#define QGEOMAPDATA_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qgeocameradata_p.h"
#include "qgeomaptype_p.h"
#include <QtPositioning/private/qdoublevector2d_p.h>
#include <QtCore/QObject>

QT_BEGIN_NAMESPACE

class QGeoMappingManagerEngine;
class QGeoMapDataPrivate;
class QGeoMapController;
class QGeoCameraCapabilities;
class QSGNode;
class QQuickWindow;

class Q_LOCATION_EXPORT QGeoMapData : public QObject
{
    Q_OBJECT
public:
    QGeoMapData(QGeoMappingManagerEngine *engine, QObject *parent = 0);
    virtual ~QGeoMapData();

    QGeoMapController *mapController();

    virtual QSGNode *updateSceneGraph(QSGNode *, QQuickWindow *window) = 0;

    void resize(int width, int height);
    int width() const;
    int height() const;

    void setCameraData(const QGeoCameraData &cameraData);
    QGeoCameraData cameraData() const;

    void setActiveMapType(const QGeoMapType mapType);
    const QGeoMapType activeMapType() const;

    virtual QGeoCoordinate itemPositionToCoordinate(const QDoubleVector2D &pos, bool clipToViewport = true) const = 0;
    virtual QDoubleVector2D coordinateToItemPosition(const QGeoCoordinate &coordinate, bool clipToViewport = true) const = 0;

    QString pluginString();
    QGeoCameraCapabilities cameraCapabilities();
    QGeoMappingManagerEngine *engine();
    virtual void prefetchData() {}

protected:
    virtual void mapResized(int width, int height) = 0;
    virtual void changeCameraData(const QGeoCameraData &oldCameraData) = 0;
    virtual void changeActiveMapType(const QGeoMapType mapType) = 0;

public Q_SLOTS:
    void update();

Q_SIGNALS:
    void cameraDataChanged(const QGeoCameraData &cameraData);
    void updateRequired();
    void activeMapTypeChanged();
    void copyrightsChanged(const QImage &copyrightsImage);
    void copyrightsChanged(const QString &copyrightsHtml);

private:
    QGeoMapDataPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QGeoMapData)
    Q_DISABLE_COPY(QGeoMapData)
};

QT_END_NAMESPACE

#endif // QGEOMAP_P_H
