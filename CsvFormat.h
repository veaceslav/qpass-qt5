/***************************************************************************
 *   Copyright (c) 2010-2011 Mateusz Piękos <mateuszpiekos@gmail.com>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#ifndef CSVFORMAT_H
#define CSVFORMAT_H

#include <QString>
#include <QList>
#include <QVector>
#include <QFile>
#include <QObject>
#include <QStringList>

class CsvFormat
{
public:
	enum ErrorID {
		OpeningError = -1,
		IncorrectStructure = -2
	};
	CsvFormat(const QString &filename);
	QList< QVector< QString > > read();
	bool write(const QList< QVector< QString > > &data);
	int getErrorID();
private:
	int error;
	QFile file;

	QStringList split(QString &str);
};

#endif //CSVFORMAT_H
