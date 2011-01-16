/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "about.h"
#include "qpass-config.h"

#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QScrollArea>

AboutWindow::AboutWindow()
{
	setMinimumWidth(300);
	setWindowTitle(tr("About"));	
	
	QVBoxLayout *layout = new QVBoxLayout(this);
	QWidget *top = new QWidget();
	QLabel *icon = new QLabel();
	icon->setPixmap(QPixmap(":/qpass.png"));
	QLabel *description = new QLabel();
	description->setText("QPass "+QString(VERSION));
	QWidget *blank = new QWidget;
	blank->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum));
	QHBoxLayout *topLayout = new QHBoxLayout();
	topLayout->addWidget(icon);
	topLayout->addWidget(description);
	topLayout->addWidget(blank);
	top->setLayout(topLayout);
	layout->addWidget(top);
	
	QWidget *center = new QWidget();
	QVBoxLayout *centerLayout = new QVBoxLayout(center);
	center->setLayout(centerLayout);
	
	QTabWidget *tab = new QTabWidget(this);
	
	QPalette palette = this->palette();
	palette.setColor(QPalette::Base, Qt::transparent);
	
	AboutEdit *tb_authors = new AboutEdit(tab);
	tb_authors->setText("Mateusz Piekos\nmateuszpiekos@gmail.com");
	tab->addTab(tb_authors, tr("Author"));
	
	AboutEdit *tb_licence = new AboutEdit(tab);
	tb_licence->setText("GNU General Public License Version 3");
	tab->addTab(tb_licence, tr("Licence"));
	
	centerLayout->addWidget(tab);
	layout->addWidget(center);
	
	QWidget *bottom = new QWidget();
	QHBoxLayout *bottomLayout = new QHBoxLayout(bottom);
	QPushButton *close = new QPushButton( tr("Close") );
	connect(close, SIGNAL(clicked()), this, SLOT(close()));
	QWidget *blank3 = new QWidget();
	blank3->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum));
	bottomLayout->addWidget(blank3);
	bottomLayout->addWidget(close);
	layout->addWidget(bottom);
}

AboutEdit::AboutEdit(QWidget* parent): QTextEdit(parent)
{
	QPalette palette = this->palette();
	palette.setColor(QPalette::Base, Qt::transparent);
	setPalette(palette);
	setReadOnly(true);
	setFrameStyle(QFrame::NoFrame|QFrame::Plain);
	//setTextInteractionFlags(Qt::NoTextInteraction);
	//setWordWrapMode(QTextOption::NoWrap);
}


#include "about.moc"