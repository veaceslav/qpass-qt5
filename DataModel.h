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

#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVector>
#include <QString>

#include "DataAccess.h"
#include "CsvFormat.h"

/*! Model which represents data from encrypted database.
 * 
 * This model uses DataAccess class to have access to database files.
 * Some functions are implementation of QAbstractTableModel class and they
 * are undocumented here.
 */
class DataModel : public QAbstractTableModel
{
	Q_OBJECT;
public:
	enum FileFormat
	{
		Native,
		Csv
	};
	enum Columns {Name, URL, UserName, Password, Notes};

	DataModel(QObject *parent = 0);
	~DataModel();
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
	bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
	void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
	errorCode exportDatabase(const QString &path, const QString &password, int format = Native, QVector<Columns> organization = QVector<Columns>());
	int importDatabase(const QString &path,const QString &password, bool replaceExisting = false, int format = Native, QVector<Columns> organization = QVector<Columns>());
	QString getPassword();
	errorCode changePassword(const QString &newPassword);
	void swapEntries(int firstIndex, int secondIndex);
	errorCode saveDatabase();
	/*! Opens database
	 *
	 * @param path Path to database.
	 * @param password Password to database.
	 * @param openExisting If true model will use existing database, if false model will create new.
	 * @param parent Parent.
	 * @return status code.
	 */
	errorCode openDatabase(const QString &path,const QString &password, int iterations = 10000, bool openExisting = true);
	int getNumberOfIterations();
	void setNumberOfIterations(int iterations);
private:
	QList< QVector< QString > > dataList;
	DataAccess *database;
};

#endif //DATAMODEL_H
