/***************************************************************************
 *   Copyright (c) 2010-2012 Mateusz Piękos <mateuszpiekos@gmail.com>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "DataModel.h"

#define COLUMNCOUNT 5 //There are columns: "Name", "URL", "Username", "Password", "Notes"

DataModel::DataModel(QObject *parent) : QAbstractTableModel(parent)
{
	database = NULL;
}

DataModel::~DataModel()
{
	if(database != NULL)
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
		//if( !database->write(dataList) )
		//	return false;
		
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
	//if( !database->write(dataList) )
		//return false;
	
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
	//if( !database->write(dataList) )
		//return false;
	
	return true;
}

bool lessThanAscending(const QVector< QString > &v1, const QVector< QString > &v2)
{
	return v1[0].toLower() < v2[0].toLower();
}

bool lessThanDescending(const QVector< QString > &v1, const QVector< QString > &v2)
{
	return v1[0].toLower() > v2[0].toLower();
}

void DataModel::sort(int column, Qt::SortOrder order)
{
	layoutAboutToBeChanged();
	if(order == Qt::AscendingOrder)
		qSort(dataList.begin(), dataList.end(), lessThanAscending);
	else
		qSort(dataList.begin(), dataList.end(), lessThanDescending);
	layoutChanged();
}

errorCode DataModel::exportDatabase(const QString &path,const QString &password, int format)
{
	if(format == Native)
	{
		DataAccess databaseToExport(path, password);
		databaseToExport.setNumberOfIterations(database->getNumberOfIterations());
		return databaseToExport.write(dataList);
	}
	else if(format == Csv)
	{
		CsvFormat csv(path);
		if(csv.write(dataList))
			return SUCCESS;
		else
			return FILE_ERROR;
	}
	else
		return FILE_ERROR;
}

int DataModel::importDatabase(const QString &path,const QString &password, bool replaceExisting, int format)
{
	QList< QVector< QString > > data;
	if(format == Native)
	{
		DataAccess databaseToImport(path, password);
		int res = databaseToImport.read(data);
		if(res < 0)
			return res;
	}
	else if(format == Csv)
	{
		CsvFormat csv(path);
		data = csv.read();
		if(data.isEmpty())
			return -2;
	}
	else
		return -4;//undefined format

	if(data.count() > 0)
	{
		if(replaceExisting)
		{
			if(data.count() > rowCount())
				insertRows(rowCount(), data.count()-rowCount());
			else if(data.count() < rowCount())
				removeRows(data.count(), rowCount()-data.count());
			dataList = data;
			emit dataChanged( index(0, 0), index( data.count()-1, COLUMNCOUNT-1));
		}
		else
		{
			beginInsertRows(QModelIndex(), rowCount(), rowCount()+data.count()-1);
			dataList += data;
			endInsertRows();
		}
	}
	if( database->write(dataList) != SUCCESS)
		return -3;
	
	return 0;
}

QString DataModel::getPassword()
{
	return database->getPassword();
}

errorCode DataModel::changePassword(const QString &newPassword)
{
	database->setPassword(newPassword);
	return database->write(dataList);
}

void DataModel::swapEntries(int firstIndex, int secondIndex)
{
	if( firstIndex < 0 || firstIndex >= dataList.count() )
		return;
	if( secondIndex < 0 || secondIndex >= dataList.count() )
		return;
	dataList.swap( firstIndex, secondIndex );
	if( firstIndex > secondIndex )
	{
		int temp = firstIndex;
		firstIndex = secondIndex;
		secondIndex = temp;
	}
	emit dataChanged( index(firstIndex, 0), index(secondIndex, COLUMNCOUNT - 1) );
	database->write(dataList);
}

errorCode DataModel::saveDatabase()
{
	return database->write(dataList);
}

errorCode DataModel::openDatabase(const QString &path,const QString &password,int iterations, bool openExisting)
{
	if(database != NULL)
		delete database;

	database = new DataAccess(path, password);
	database->setNumberOfIterations(iterations);
	errorCode err;
	if(openExisting)
		err = database->read(dataList);
	else
		err = database->write( QList< QVector< QString> >() );
	
	return err;
}

int DataModel::getNumberOfIterations()
{
	return database->getNumberOfIterations();
}

void DataModel::setNumberOfIterations(int iterations)
{
	database->setNumberOfIterations(iterations);
	database->write(dataList);
}
