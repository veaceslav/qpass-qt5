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

#include "CsvFormat.h"

#include <QMessageBox>

CsvFormat::CsvFormat(const QString &filename):file(filename)
{
}

QList< QVector< QString > > CsvFormat::read()
{
	if(!file.open(QIODevice::ReadOnly))
	{
		error = OpeningError;
		return QList< QVector< QString> >();
	}
	QList< QVector< QString > > list;

	char buffer[1024];
	int ret;
	while(true)
	{
		ret = file.readLine(buffer, 1023);
		if(ret <= 0)
		{
			break;
		}

		QString line = QString::fromUtf8(buffer);
		printf("%d\n", line.size());
		if(line.size() <= 2)
			continue;
		QStringList slist = split(line);
		if(slist.size() != 5)
		{
			error = IncorrectStructure;
			return QList< QVector< QString> >();
		}
		list.append(slist.toVector());
	}
	file.close();
	return list;
}

bool CsvFormat::write(const QList< QVector< QString > > &data)
{
	if(!file.open(QIODevice::WriteOnly))
	{
		return OpeningError;
	}
	for(int i = 0; i < data.size(); i++)
	{
		QString line;
		for(int j = 0; j < 5; j++)
		{
			QString element = data[i][j];
			line += '\"' + element.replace("\\", "\\\\").replace("\"", "\\\"") + '\"';
			if(j < 4)
				line += ',';
		}
		line += "\r\n";
		file.write(line.toUtf8());
	}
	file.close();
	return true;
}

int CsvFormat::getErrorID()
{
	return error;
}

QStringList CsvFormat::split(QString &str)
{
	int len = str.length();
	int pos;
	pos = str.lastIndexOf('\"');
	str.chop(len-pos);
	len = pos;
	QStringList result;

	while((pos = str.lastIndexOf("\",\"")) > 0)
	{
		result.prepend( str.right(len-pos-3).replace("\\\\", "\\").replace("\\\"", "\"") );
		str.chop(len-pos);
		len = pos;
	}
	str.remove(0, 1);
	result.prepend(str.replace("\\\\", "\\").replace("\\\"", "\""));
	return result;
}
