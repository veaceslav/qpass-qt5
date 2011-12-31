#include <QApplication>

#include "MainWindow.h"

#ifndef APP_H
#define APP_H

class App : public QApplication
{
	public:
		App(int &argc, char **argv);
		void commitData(QSessionManager & manager);
		void setMainWindow(MainWindow *mainWindow);
	private:
		MainWindow *mainWindow;
};

#endif //APP_H
