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
      /*! Initializes model and database.
       *
       * @param path Path to database.
       * @param password Password to database.
       * @param openExisting If true model will use existing database, if false model will create new.
       * @param parent Parent.
       */
      DataModel(QString &path, QString &password, bool openExisting = true, QObject *parent = 0);
      ~DataModel();
      QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
      Qt::ItemFlags flags(const QModelIndex &index) const;
      QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
      int rowCount(const QModelIndex &parent = QModelIndex()) const;
      int columnCount(const QModelIndex &parent = QModelIndex()) const;
      bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
      bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
      bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
      /*! Static function to check if given file is database and if password is correct.
       *
       * This function uses DataAccess::checkDatabase function to do it.
       *
       * @param path Path to database.
       * @param password Password to database.
       * @return 0 if header of database is ok and password is correct, -1 if password is incorrect, -2 if file is corrupted.
       */
      static int checkDatabase(QString &path, QString &password);
      bool exportDatabase(QString &path, QString &password);
   private:
      QList< QVector< QString > > dataList;
      DataAccess *database;
};

#endif //DATAMODEL_H
