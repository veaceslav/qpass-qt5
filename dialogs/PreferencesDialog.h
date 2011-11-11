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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

#include "ui_PreferencesDialog.h"

class PreferencesDialog : public QDialog, private Ui::PreferencesDialog
{
	Q_OBJECT
	public:
		PreferencesDialog(QWidget *parent);
		void setVisibleElementsAmount(int amount);
		int getVisibleElementsAmount();
		void setClipboardTimeout(int timeout);
		int getClipboardTimeout();
};

#endif

