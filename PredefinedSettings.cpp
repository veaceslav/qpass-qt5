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

#include <QCoreApplication>
#include <QDir>

#include "PredefinedSettings.h"

QString PredefinedSettings::dataPath()
{
   QString path;
   if(path.isEmpty())
   {
#ifdef Q_WS_X11
      path = QCoreApplication::applicationDirPath()+"/../share/qpass";
#endif
#ifdef Q_WS_WIN
      path = QCoreApplication::applicationDirPath();
#endif
   }
   QDir p(path);
   return p.absolutePath();
}

QString PredefinedSettings::databasePath()
{
   QDir dir;
#ifdef Q_WS_X11
   return dir.homePath()+"/.qpassdb";
#endif
#ifdef Q_WS_WIN
   return dir.homePath()+"/qpassdb";
#endif
}