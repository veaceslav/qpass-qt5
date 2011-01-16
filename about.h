/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <qtextedit.h>

class AboutWindow : public QDialog
{
	Q_OBJECT
	public:
		AboutWindow();
};

class AboutEdit : public QTextEdit
{
	public:
		AboutEdit(QWidget* parent = 0);
};

#endif //ABOUT_H