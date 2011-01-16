/***************************************************************************
 *   Copyright (c) 2011 Mateusz Piękos <mateuszpiekos@gmail.com>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVector>
#include <QString>

#include "DataAccess.h"

/*!
 * Model which represents data from encrypted database.
 */
class DataModel : public QAbstractTableModel
{
   Q_OBJECT
   public:
      DataModel(QString &path, QString &password, QObject *parent = 0);
      ~DataModel();
      QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
      Qt::ItemFlags flags(const QModelIndex &index) const;
      QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
      int rowCount(const QModelIndex &parent = QModelIndex()) const;
      int columnCount(const QModelIndex &parent = QModelIndex()) const;
      bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
      bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
      bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
   private:
      QList< QVector< QString > > dataList;
      DataAccess *database;
};

#endif //DATAMODEL_H