/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QPass_H
#define QPass_H

#include <QtGui/QMainWindow>
#include <QtGui/QAction>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>
#include <QtGui/QListWidget>
#include <QtGui/QFormLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QTextEdit>
#include <QSystemTrayIcon>
#include <QMenu>
class QPass : public QMainWindow
{
	Q_OBJECT
	private:
		bool passwordVisible;
		//char key[33];
		QString key;
		bool dataChanged;
		
		QPushButton *addButton;
		QPushButton *deleteButton;
		QAction *hideOnCloseAction;
		
		QListWidget *list;
		QFormLayout *form;
		QWidget *formWidget;
		QLineEdit *name;
		QLineEdit *url;
		QLineEdit *username;
		QLineEdit *password;
		QTextEdit *notes;
		QPushButton *copyUrlButton;
		QPushButton *copyUsernameButton;
		QPushButton *copyPasswordButton;
		QPushButton *showPasswordButton;
		
		QSystemTrayIcon *trayIcon;
		QMenu *trayMenu;
		enum role
		{
			nameRole = 0,
			urlRole = 32,
			usernameRole,
			passwordRole,
			notesRole
		};
		
	public:
		int readData(QString password = QString::null, QString dbpath = QString::null);
		void readSettings();
		void writeSettings();
		void createGUI();
		void closeEvent(QCloseEvent * event);
		QPass();
		virtual ~QPass();
		
	private slots:
		void changedList();
		void nameChanged(QString text);
		void urlChanged(QString text);
		void usernameChanged(QString text);
		void passwordChanged(QString text);
		void notesChanged();
		void copyUrl();
		void copyUsername();
		void copyPassword();
		void showPassword();
		void deleteCurrentItem();
		void addItem();
		int saveData(QString path = QString::null);
		void changePassword();
		void trayClicked(QSystemTrayIcon::ActivationReason reason);
		void about();
		void exportDatabase();
		void importDatabase();
		void newFromImport();
};

#endif // QPass_H
