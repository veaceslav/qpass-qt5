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

#include <QCoreApplication>
#include <QDir>

#include "PredefinedSettings.h"

#ifdef Q_WS_WIN
#include <windows.h>
#include <shlobj.h>
#endif

QString PredefinedSettings::dataPath()
{
	QString path;
	if(path.isEmpty())
	{
#ifdef Q_WS_WIN
		path = QCoreApplication::applicationDirPath();
#else 
		path = QCoreApplication::applicationDirPath()+"/../share/qpass";
#endif
	}
	QDir p(path);
	return p.absolutePath();
}

QString PredefinedSettings::databasePath()
{
	QDir dir;
#ifdef Q_WS_WIN
#ifdef PORTABLE
	return "database.qpa";
#else
	char szPath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, szPath);
	QString path = QString(szPath)+"\\QPass";
	if(!dir.exists(path))
	{
		dir.mkdir(path);
	}
	return path+"\\database";
#endif
#else 
	return dir.homePath()+"/.qpassdb";
#endif
}
