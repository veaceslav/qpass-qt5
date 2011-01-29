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
#include <QFile>
#include <QSharedMemory>
#include <QObject>
#include <QTranslator>

#include "MainWindow.h"
#include "PredefinedSettings.h"

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
   
   QCoreApplication::setApplicationName("QPass");
   QCoreApplication::setOrganizationName("QPass");
      
   QString locale = QLocale::system().name();
   QTranslator translator;
   translator.load(PredefinedSettings::dataPath()+"translations/"+locale);
   app.installTranslator(&translator);
   
   //Code to check if there is any running instance of QPass
   QSharedMemory memory("91628a41-6284-41a9-9b25-7b3cc365ced9"); 
   if(!memory.create(1, QSharedMemory::ReadWrite))
   {
      QMessageBox box;
      box.setText( QObject::tr("One instance of QPass is already running!") );
      box.setIcon( QMessageBox::Critical );
      box.exec();
      return 0;
   }
   
   MainWindow mainWindow;
   
   return app.exec();
}
