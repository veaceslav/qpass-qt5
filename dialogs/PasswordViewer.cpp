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

#include "PasswordViewer.h"

#include <QTableWidgetItem>

PasswordViewer::PasswordViewer(QWidget *parent, QString password) : QDialog(parent)
{
	setupUi(this);

	table->clear();
	table->setColumnCount(password.size());
	table->setMaximumWidth(password.size()*25);
	table->setMinimumWidth(password.size()*25);

	for(int i = 0; i < password.size(); i++)
	{
		QTableWidgetItem *headerItem = new QTableWidgetItem();
		headerItem->setText(QString::number(i+1));
		table->setHorizontalHeaderItem(i, headerItem);

		QTableWidgetItem *item = new QTableWidgetItem();
		item->setText(password.at(i));
		table->setItem(0, i, item);
	}
}
