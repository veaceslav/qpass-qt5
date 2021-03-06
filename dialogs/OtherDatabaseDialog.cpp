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

#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

#include "OtherDatabaseDialog.h"

OtherDatabaseDialog::OtherDatabaseDialog(DataModel *model, QWidget *parent) : QDialog(parent)
{
	setupUi(this);
	passwordEdit2->setVisible(false);
	retypePasswordLabel->setVisible(false);
 	benchmark = new Benchmark(this);
	this->model = model;

	connect(browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	connect(nextButton1, SIGNAL(clicked()), this, SLOT(next()));
	connect(nextButton2, SIGNAL(clicked()), this, SLOT(next()));
	connect(backButton1, SIGNAL(clicked()), this, SLOT(previous()));
	connect(backButton2, SIGNAL(clicked()), this, SLOT(previous()));
	connect(benchmarkButton, SIGNAL(clicked()), this, SLOT(runBenchmark()));
	connect(benchmark, SIGNAL(finished()), this, SLOT(showResult()));
}

QString OtherDatabaseDialog::getPath()
{
	return pathEdit->text();
}

void OtherDatabaseDialog::setPath(const QString &path)
{
	pathEdit->setText(path);
}

bool OtherDatabaseDialog::isSetAsDefault()
{
	return setAsDefaultBox->isChecked();
}

void OtherDatabaseDialog::setAsDefault(bool isDefault)
{
	setAsDefaultBox->setChecked(isDefault);
}

void OtherDatabaseDialog::setMode(int mode) 
{
	if(mode == OpenExisting)
		openExistingButton->setChecked(true);
	else
		createNewButton->setChecked(true);
}

void OtherDatabaseDialog::browse()
{
	QString path;

	if(openExistingButton->isChecked())
	{
		path = QFileDialog::getOpenFileName(this, tr("Select database"), NULL, tr("QPass database files (*.qpa);;All Files (*)"));
	}
	else
	{
		path = QFileDialog::getSaveFileName(this, tr("Select database"), NULL, tr("QPass database files (*.qpa);;All Files (*)"));
	}

	pathEdit->setText(path);
}

void OtherDatabaseDialog::accept()
{
	if(openExistingButton->isChecked())
	{
		errorCode ret = model->openDatabase(pathEdit->text(), passwordEdit1->text());
		if(ret == INVALID_PASSWORD)
		{
			QMessageBox box(this);
			box.setWindowTitle("QPass");
			box.setText( tr("Password is incorrect") );
			box.setIcon( QMessageBox::Warning );
			box.exec();
			return;
		}
		else if(ret == FILE_ERROR)
		{
			QMessageBox box(this);
			box.setWindowTitle("QPass");
			box.setText( tr("Error reading database or database doesn't exist") );
			box.setIcon( QMessageBox::Critical );
			box.exec();
			return;
		}
		else if(ret == GCRYPT_ERROR)
		{
			QMessageBox box(this);
			box.setWindowTitle("QPass");
			box.setText( tr("libgcrypt library error") );
			box.setIcon( QMessageBox::Critical );
			box.exec();
			return;
		}
		else if(ret == SUCCESS_OLD_VERSION)
		{
			QMessageBox box(this);
			box.setWindowTitle( tr("QPass") );
			box.setText( tr("This version of QPass uses new version of database. Your database has been converted to new version. You can change default number of PBKDF2 iterations in settings.") );
			box.setIcon(QMessageBox::Information);
			box.exec();
		}
	}
	else
	{
		if(passwordEdit1->text() != passwordEdit2->text())
		{
			QMessageBox box(this);
			box.setWindowTitle("QPass");
			box.setText( tr("Passwords doesn't match") );
			box.setIcon( QMessageBox::Warning );
			box.exec();
			return;
		}
		errorCode ret = model->openDatabase(pathEdit->text(), passwordEdit1->text(), iterationsBox->value(), false);
		if(ret == FILE_ERROR)
		{
			QMessageBox box(this);
			box.setWindowTitle("QPass");
			box.setText( tr("Selected file is not writable") );
			box.setIcon( QMessageBox::Critical );
			box.exec();
			return;
		}
		else if(ret == GCRYPT_ERROR)
		{
			QMessageBox box(this);
			box.setWindowTitle("QPass");
			box.setText( tr("libgcrypt library error") );
			box.setIcon( QMessageBox::Critical );
			box.exec();
			return;
		}
	}
	done(QDialog::Accepted);
}

void OtherDatabaseDialog::next()
{
	if(stackedWidget->currentIndex() == 0)
	{
		if(openExistingButton->isChecked())
		{
			passwordEdit2->setVisible(false);
			retypePasswordLabel->setVisible(false);
			nextButton2->setText(tr("OK"));
		}
		else
		{
			passwordEdit2->setVisible(true);
			retypePasswordLabel->setVisible(true);
			nextButton2->setText(tr("Next"));
		}
		stackedWidget->setCurrentIndex(stackedWidget->currentIndex()+1);
	}
	else if(stackedWidget->currentIndex() == 1 && openExistingButton->isChecked())
	{
		emit accept();		
	}
	else
		stackedWidget->setCurrentIndex(stackedWidget->currentIndex()+1);
}

void OtherDatabaseDialog::previous()
{
	stackedWidget->setCurrentIndex(stackedWidget->currentIndex()-1);
}

void OtherDatabaseDialog::runBenchmark()
{
	resultLabel->setText(tr("Checking..."));
	benchmark->setNumberOfIterations(iterationsBox->value());
	benchmark->start();
}

void OtherDatabaseDialog::showResult()
{
	resultLabel->setText(QString::number(benchmark->getTime())+"s");
}
