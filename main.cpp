/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include "QPass.h"
#include <QMessageBox>


int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	
	QString locale = QLocale::system().name();
	
	QTranslator translator;
	QString appDir(QCoreApplication::applicationDirPath());
	appDir.append("/");
	QString prefix;
#ifdef Q_WS_X11
	prefix = "../share/qpass/";
#else
	prefix = "./";
#endif
	translator.load(appDir+prefix+"translations/qpass_"+locale);
	app.installTranslator(&translator);
	
	QPass foo;
	foo.show();
	return app.exec();
}
