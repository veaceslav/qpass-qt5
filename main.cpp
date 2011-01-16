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

#include "MainWindow.h"
#include "NewDatabaseDialog.h"

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
   
   QCoreApplication::setApplicationName("QPass");
   QCoreApplication::setOrganizationName("Mateusz Piekos");
   
   MainWindow mainWindow;
   
   return app.exec();
}