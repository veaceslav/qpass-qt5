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

#include <QApplication>
#include <QMessageBox>

#include "DataModel.h"

#define COLUMNCOUNT 5 //There are columns: "Name", "URL", "Username", "Password", "Notes"

DataModel::DataModel(const QString &path,const QString &password, bool openExisting, QObject *parent) : QAbstractTableModel(parent)
{
   database = new DataAccess(path, password);
   if(openExisting)
      dataList = database->read();
}

DataModel::~DataModel()
{
   delete database;
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{
   if(!index.isValid())
      return QVariant();
   if(index.row() > dataList.count() || index.column() > COLUMNCOUNT)
      return QVariant();
   if(role == Qt::DisplayRole)
   {
      return dataList.at( index.row() ).at( index.column() );
   }
   else
      return QVariant();
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
   return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
   return QVariant();
}

int DataModel::rowCount(const QModelIndex &parent) const
{
   return dataList.count();
}

int DataModel::columnCount(const QModelIndex &parent) const
{
   return COLUMNCOUNT; 
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
   if(!index.isValid())
      return false;
   if(index.row() > dataList.count() || index.column() > COLUMNCOUNT)
      return false;
   if(role == Qt::EditRole)
   {
      dataList[ index.row()][ index.column() ] = value.toString();
      emit dataChanged(index, index);
      if( !database->write(dataList) )
	 return false;
      
      return true;
   }
   else
      return false;
}

bool DataModel::insertRows(int row, int count, const QModelIndex &parent)
{
   if(row > dataList.count())
      return false;
   beginInsertRows(QModelIndex(), row, row+count-1);
   for(int i = 0; i < count; i++)
      dataList.insert(row, QVector<QString>(COLUMNCOUNT));
   endInsertRows();
   if( !database->write(dataList) )
      return false;
   
   return true;
}

bool DataModel::removeRows(int row, int count, const QModelIndex &parent)
{
   if(row+count-1 > dataList.count()-1)
      return false;
   beginRemoveRows(QModelIndex(), row, row+count-1);
   for(int i = 0; i < count; i++)
   {
      dataList.removeAt(row);
   }
   endRemoveRows();
   if( !database->write(dataList) )
      return false;
   
   return true;
}

int DataModel::checkDatabase(const QString &path,const QString &password)
{
   DataAccess database(path, password);
   return database.checkDatabase();
}

bool DataModel::exportDatabase(const QString &path,const QString &password)
{
   DataAccess databaseToExport(path, password);
   return databaseToExport.write(dataList);
}

int DataModel::importDatabase(const QString &path,const QString &password, bool replaceExisting)
{
   DataAccess databaseToImport(path, password);
   int res = databaseToImport.checkDatabase();
   if(res != 0)
      return res;
   QList< QVector< QString > > data;
   data = databaseToImport.read();
   if(data.count() > 0)
   {
      if(replaceExisting)
      {
	 if(data.count() > rowCount())
	    insertRows(rowCount(), data.count()-rowCount());
	 else if(data.count() < rowCount())
	    removeRows(data.count(), rowCount()-data.count());
	 dataList = data;
	 dataChanged( index(0, 0), index( data.count()-1, COLUMNCOUNT));
      }
      else
      {
	 beginInsertRows(QModelIndex(), rowCount(), rowCount()+data.count()-1);
	 dataList += data;
	 endInsertRows();
      }
   }
   if( !database->write(dataList) )
      return -3;
   
   return 0;
}

QString DataModel::getPassword()
{
   return database->getPassword();
}

bool DataModel::changePassword(const QString &newPassword)
{
   database->setPassword(newPassword);
   return database->write(dataList);
}
