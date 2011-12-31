#include "App.h"
#include <QMessageBox>
#include <QSessionManager>

App::App(int &argc, char **argv) : QApplication(argc, argv)
{
}

void App::commitData(QSessionManager & manager)
{
	if(mainWindow->isUnsaved())
	{
		mainWindow->show();
		if(manager.allowsInteraction())
		{
			int ret = QMessageBox::warning(
				mainWindow,
				tr("Unsaved entry - QPass"),
				tr("Selected data entry has been modified\nDo you want to save your changes or discard them?"),
				QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

			switch (ret) {
			case QMessageBox::Save:
				manager.release();
				mainWindow->saveItem();
				break;
			case QMessageBox::Discard:
				break;
			case QMessageBox::Cancel:
			default:
				manager.cancel();
			}
		}
   	}
	manager.release();

}

void App::setMainWindow(MainWindow *mainWindow)
{
	this->mainWindow = mainWindow;
}

