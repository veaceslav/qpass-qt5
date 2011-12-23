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
		QString line;
		QString temporary;
		bool first = true;
		printf("nowy\n");
		while(true)
		{
			ret = file.readLine(buffer, 1023);
			if(ret <= 0)
				break;
			line += QString::fromUtf8(buffer);
			temporary = line;
			temporary.remove("\"\"");
			temporary.remove("\n");
			temporary.remove("\r");

			if(temporary.lastIndexOf(",\"") == temporary.size()-2)
			{
				temporary.remove(temporary.size()-2, 2); 

				if(temporary.lastIndexOf("\"") == temporary.lastIndexOf(",\"")+1 && ret != -1)
					break;
				else
					continue;
			}
			printf("cyk: %d\n",!first);
			if(!first && (temporary.indexOf("\"") == -1))
			{
				printf("tu\n");
				continue;
			}
			first = false;

			if(temporary.lastIndexOf("\"") == temporary.lastIndexOf(",\"")+1)
				continue;

			break;
		}

		if(ret <= 0)
			break;
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
			line += '\"' + element.replace("\"", "\"\"") + '\"';
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
	QStringList result;
	QString string = str;
	while(true)
	{
		if(string.at(0) == '"')
		{
			int pos;
			int start = 1;
			while(true)
			{
				if((pos = string.indexOf("\"", start)) == string.indexOf("\"\"", start) && pos != -1)
				{
					start = pos+2;
				}
				else
					break;
			}
			QString value = string.mid(1, pos-1);
			value.replace("\"\"", "\"");
			result.append(value);
			string.remove(0, pos);
			if(string.indexOf(",") == -1)
				break;
			string.remove(0, string.indexOf(",")+1);
		}
		else
		{
			int comma = string.indexOf(",");
			if(comma == -1)
			{
				result.append(string);
				string.clear();
				break;
			}
			else
			{
				result.append(string.left(comma));
				string.remove(0, comma+1);
			}
		}
	}
	return result;
}
