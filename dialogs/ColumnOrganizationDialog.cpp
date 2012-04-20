/***************************************************************************
 *   Copyright (c) 2010-2012 Mateusz Piękos <mateuszpiekos@gmail.com>      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "ColumnOrganizationDialog.h"

#include <QMessageBox>

ColumnOrganizationDialog::ColumnOrganizationDialog(QWidget *parent) : QDialog(parent)
{
	setupUi(this);
}

QVector<DataModel::Columns> ColumnOrganizationDialog::getColumnOrganization() const
{
	QVector<DataModel::Columns> organization(5);
	organization[0] = (DataModel::Columns)c1Box->currentIndex();
	organization[1] = (DataModel::Columns)c2Box->currentIndex();
	organization[2] = (DataModel::Columns)c3Box->currentIndex();
	organization[3] = (DataModel::Columns)c4Box->currentIndex();
	organization[4] = (DataModel::Columns)c5Box->currentIndex();

	return organization;
}

void ColumnOrganizationDialog::accept()
{
	int occurenceCount[5] = {0,0,0,0,0};
	occurenceCount[c1Box->currentIndex()]++;
	occurenceCount[c2Box->currentIndex()]++;
	occurenceCount[c3Box->currentIndex()]++;
	occurenceCount[c4Box->currentIndex()]++;
	occurenceCount[c5Box->currentIndex()]++;

	bool ok = true;
	for(int i = 0; i < 5; i++)
	{
		if(occurenceCount[i] > 1)
		{
			ok = false;
			QMessageBox box(this);
			box.setText( tr("There are duplicate selections!") );
			box.setWindowTitle( tr("QPass - warning") );
			box.setIcon( QMessageBox::Warning );
			box.exec();
		}
	}
	if(ok)
		done(QDialog::Accepted);
}
