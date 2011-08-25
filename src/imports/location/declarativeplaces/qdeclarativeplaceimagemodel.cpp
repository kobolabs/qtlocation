/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
** Public License version 3.0 as published by tOhe Free Software Foundation
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

#include "qdeclarativeplaceimagemodel_p.h"
#include "qdeclarativesupplier_p.h"

#include <QtCore/QUrl>
#include <QtLocation/QPlaceImage>

QT_BEGIN_NAMESPACE

/*!
    \qmlclass PlaceImageModel QDeclarativePlaceImage

    \brief The PlaceImageModel element provides access to place images.
    \ingroup qml-places
    \since 5.0

    The PlaceImageModel is a read-only model used to fetch images related to a Place.  The model
    performs fetches incrmentally.  The number of images which are fetched at a time
    is specified by the batchSize property.  The total number of images available can be
    accessed via the totalCount property and the number of fetched images via the count
    property.

    The model returns data for the following roles:

    \table
        \header
            \o Role
            \o Type
            \o Description
        \row
            \o url
            \o url
            \o The URL of the image.
        \row
            \o thumbnailUrl
            \o url
            \o The URL of the image thumbnail.
        \row
            \o imageId
            \o string
            \o The id of the image.
        \row
            \o metaInfo
            \o string
            \o Meta information about the image.
        \row
            \o mimeType
            \o string
            \o The MIME type of the image.
        \row
            \o supplier
            \o Supplier
            \o The source of the image.
    \endtable
*/

/*!
    \qmlproperty Place PlaceImageModel::place

    This property holds the Place that the images are for.
*/

/*!
    \qmlproperty int PlaceImageModel::batchSize

    This property holds the batch size to use when fetching more image items.
*/

/*!
    \qmlproperty int PlaceImageModel::totalCount

    This property holds the total number of image items for the place.
*/

QDeclarativePlaceImageModel::QDeclarativePlaceImageModel(QObject* parent)
:   QDeclarativePlaceContentModel(QPlaceContent::ImageType, parent)
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(UrlRole, "url");
    roleNames.insert(ThumbnailUrlRole, "thumbnailUrl");
    roleNames.insert(ImageIdRole, "imageId");
    roleNames.insert(MetaInfoRole, "metaInfo");
    roleNames.insert(MimeTypeRole, "mimeType");
    roleNames.insert(SupplierRole, "supplier");
    setRoleNames(roleNames);
}

QDeclarativePlaceImageModel::~QDeclarativePlaceImageModel()
{
    qDeleteAll(m_suppliers);
}

void QDeclarativePlaceImageModel::clearData()
{
    qDeleteAll(m_suppliers);
    m_suppliers.clear();
    QDeclarativePlaceContentModel::clearData();
}

void QDeclarativePlaceImageModel::processContent(const QPlaceContent &content, int index)
{
    Q_UNUSED(index);

    QPlaceImage image(content);

    if (!m_suppliers.contains(image.supplier().supplierId())) {
        m_suppliers.insert(image.supplier().supplierId(),
                           new QDeclarativeSupplier(image.supplier(), this));
    }
}

QVariant QDeclarativePlaceImageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= rowCount(index.parent()) || index.row() < 0)
        return QVariant();

    const QPlaceImage &image = m_content.value(index.row());

    switch (role) {
    case UrlRole:
        return image.url();
    case ThumbnailUrlRole:
        return image.thumbnailUrl();
    case ImageIdRole:
        return image.id();
    case MetaInfoRole:
        return image.metaInfo();
    case MimeTypeRole:
        return image.mimeType();
    case SupplierRole:
        return QVariant::fromValue(static_cast<QObject *>(m_suppliers.value(image.supplier().supplierId())));
    default:
        return QVariant();
    }
}

QT_END_NAMESPACE