/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtLocation module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qdeclarativeplacecontentmodel.h"
#include "qdeclarativeplace_p.h"
#include "qdeclarativegeoserviceprovider_p.h"
#include "qdeclarativeplaceuser_p.h"
#include "error_messages.h"

#include <QtQml/QQmlInfo>
#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/QPlaceManager>
#include <QtLocation/QPlaceContentRequest>

QT_BEGIN_NAMESPACE

QDeclarativePlaceContentModel::QDeclarativePlaceContentModel(QPlaceContent::Type type,
                                                             QObject *parent)
:   QAbstractListModel(parent), m_place(0), m_type(type), m_batchSize(1), m_contentCount(-1),
    m_reply(0), m_complete(false)
{
}

QDeclarativePlaceContentModel::~QDeclarativePlaceContentModel()
{
}

/*!
    \internal
*/
QDeclarativePlace *QDeclarativePlaceContentModel::place() const
{
    return m_place;
}

/*!
    \internal
*/
void QDeclarativePlaceContentModel::setPlace(QDeclarativePlace *place)
{
    if (m_place != place) {
        beginResetModel();

        int initialCount = m_contentCount;
        clearData();
        m_place = place;
        endResetModel();

        emit placeChanged();
        if (initialCount != -1)
            emit totalCountChanged();

        fetchMore(QModelIndex());
    }
}

/*!
    \internal
*/
int QDeclarativePlaceContentModel::batchSize() const
{
    return m_batchSize;
}

/*!
    \internal
*/
void QDeclarativePlaceContentModel::setBatchSize(int batchSize)
{
    if (m_batchSize != batchSize) {
        m_batchSize = batchSize;
        emit batchSizeChanged();
    }
}

/*!
    \internal
*/
int QDeclarativePlaceContentModel::totalCount() const
{
    return m_contentCount;
}

/*!
    \internal
*/
static QPair<int, int> findMissingKey(const QMap<int, QPlaceContent> &map)
{
    int start = 0;
    while (map.contains(start))
        ++start;

    QMap<int, QPlaceContent>::const_iterator it = map.lowerBound(start);
    if (it == map.end())
        return qMakePair(start, -1);

    int end = start;
    while (!map.contains(end))
        ++end;

    return qMakePair(start, end - 1);
}

/*!
    \internal
    Clears the model data but does not reset it.
*/
void QDeclarativePlaceContentModel::clearData()
{
    qDeleteAll(m_users);
    m_users.clear();

    qDeleteAll(m_suppliers);
    m_suppliers.clear();

    m_content.clear();

    m_contentCount = -1;

    if (m_reply) {
        m_reply->abort();
        m_reply->deleteLater();
        m_reply = 0;
    }
}

/*!
    \internal
*/
void QDeclarativePlaceContentModel::initializeCollection(int totalCount, const QPlaceContent::Collection &collection)
{
    beginResetModel();

    int initialCount = m_contentCount;
    clearData();

    QMapIterator<int, QPlaceContent> i(collection);
    while (i.hasNext()) {
        i.next();

        const QPlaceContent &content = i.value();
        if (content.type() != m_type)
            continue;

        m_content.insert(i.key(), content);
        if (!m_suppliers.contains(content.supplier().supplierId())) {
            m_suppliers.insert(content.supplier().supplierId(),
                               new QDeclarativeSupplier(content.supplier(), m_place->plugin(), this));
        }
        if (!m_users.contains(content.user().userId())) {
            m_users.insert(content.user().userId(),
                               new QDeclarativePlaceUser(content.user(), this));
        }
    }

    m_contentCount = totalCount;

    if (initialCount != totalCount)
        emit totalCountChanged();

    endResetModel();
}

/*!
    \internal
*/
int QDeclarativePlaceContentModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_content.count();
}

/*!
    \internal
*/
QVariant QDeclarativePlaceContentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= rowCount(index.parent()) || index.row() < 0)
        return QVariant();

    const QPlaceContent &content = m_content.value(index.row());

    switch (role) {
    case SupplierRole:
        return QVariant::fromValue(static_cast<QObject *>(m_suppliers.value(content.supplier().supplierId())));
    case PlaceUserRole:
        return QVariant::fromValue(static_cast<QObject *>(m_users.value(content.user().userId())));
    case AttributionRole:
        return content.attribution();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> QDeclarativePlaceContentModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles.insert(SupplierRole, "supplier");
    roles.insert(PlaceUserRole, "user");
    roles.insert(AttributionRole, "attribution");
    return roles;
}

/*!
    \internal
*/
bool QDeclarativePlaceContentModel::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid())
        return false;

    if (!m_place)
        return false;

    if (m_contentCount == -1)
        return true;

    return m_content.count() != m_contentCount;
}

/*!
    \internal
*/
void QDeclarativePlaceContentModel::fetchMore(const QModelIndex &parent)
{
    if (parent.isValid())
        return;

    if (!m_place)
        return;

    if (m_reply)
        return;

    if (!m_place->plugin())
        return;

    QDeclarativeGeoServiceProvider *plugin = m_place->plugin();

    QGeoServiceProvider *serviceProvider = plugin->sharedGeoServiceProvider();
    if (!serviceProvider)
        return;

    QPlaceManager *placeManager = serviceProvider->placeManager();
    if (!placeManager)
        return;

    QPlaceContentRequest request;
    request.setContentType(m_type);

    if (m_contentCount == -1) {
        request.setOffset(0);
        request.setLimit(m_batchSize);
    } else {
        QPair<int, int> missing = findMissingKey(m_content);
        request.setOffset(missing.first);
        if (missing.second == -1)
            request.setLimit(m_batchSize);
        else
            request.setLimit(qMin(m_batchSize, missing.second - missing.first + 1));
    }

    m_reply = placeManager->getPlaceContent(m_place->place().placeId(), request);
    connect(m_reply, SIGNAL(finished()), this, SLOT(fetchFinished()), Qt::QueuedConnection);
}

/*!
    \internal
*/
void QDeclarativePlaceContentModel::classBegin()
{
}

/*!
    \internal
*/
void QDeclarativePlaceContentModel::componentComplete()
{
    m_complete = true;
    fetchMore(QModelIndex());
}

/*!
    \internal
*/
void QDeclarativePlaceContentModel::fetchFinished()
{
    if (!m_reply)
        return;

    QPlaceContentReply *reply = m_reply;
    m_reply = 0;

    if (m_contentCount != reply->totalCount()) {
        m_contentCount = reply->totalCount();
        emit totalCountChanged();
    }

    if (!reply->content().isEmpty()) {
        QPlaceContent::Collection contents = reply->content();

        //find out which indexes are new and which ones have changed.
        QMapIterator<int, QPlaceContent> it(contents);
        QList<int> changedIndexes;
        QList<int> newIndexes;
        while (it.hasNext()) {
            it.next();
            if (!m_content.contains(it.key()))
                newIndexes.append(it.key());
            else if (it.value() != m_content.value(it.key()))
                changedIndexes.append(it.key());
        }

        //insert new indexes in blocks where within each
        //block, the indexes are consecutive.
        QListIterator<int> newIndexesIter(newIndexes);
        int startIndex = -1;
        while (newIndexesIter.hasNext()) {
            int currentIndex = newIndexesIter.next();
            if (startIndex == -1)
                startIndex = currentIndex;

            if (!newIndexesIter.hasNext() || (newIndexesIter.hasNext() && (newIndexesIter.peekNext() > (currentIndex + 1)))) {
                beginInsertRows(QModelIndex(),startIndex,currentIndex);
                for (int i = startIndex; i <= currentIndex; ++i) {
                    const QPlaceContent &content = contents.value(i);

                    m_content.insert(i, content);
                    if (!m_suppliers.contains(content.supplier().supplierId())) {
                        m_suppliers.insert(content.supplier().supplierId(),
                                           new QDeclarativeSupplier(content.supplier(), m_place->plugin(), this));
                    }
                    if (!m_users.contains(content.user().userId())) {
                        m_users.insert(content.user().userId(),
                                           new QDeclarativePlaceUser(content.user(), this));
                    }
                }
                endInsertRows();
                startIndex = -1;
            }
        }

        //modify changed indexes in blocks where within each
        //block, the indexes are consecutive.
        startIndex = -1;
        QListIterator<int> changedIndexesIter(changedIndexes);
        while (changedIndexesIter.hasNext()) {
            int currentIndex = changedIndexesIter.next();
            if (startIndex == -1)
                startIndex = currentIndex;

            if (!changedIndexesIter.hasNext() || (changedIndexesIter.hasNext() && changedIndexesIter.peekNext() > (currentIndex + 1))) {
                for (int i = startIndex; i <= currentIndex; ++i) {
                    const QPlaceContent &content = contents.value(i);
                    m_content.insert(i, content);
                    if (!m_suppliers.contains(content.supplier().supplierId())) {
                        m_suppliers.insert(content.supplier().supplierId(),
                                           new QDeclarativeSupplier(content.supplier(), m_place->plugin(), this));
                    }
                    if (!m_users.contains(content.user().userId())) {
                        m_users.insert(content.user().userId(),
                                           new QDeclarativePlaceUser(content.user(), this));
                    }
                }
                emit dataChanged(index(startIndex),index(currentIndex));
                startIndex = -1;
            }
        }
    }

    reply->deleteLater();
}

QT_END_NAMESPACE
