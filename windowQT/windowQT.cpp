#include "windowQT.h"

windowQT::windowQT(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.exit, SIGNAL(clicked()), this, SLOT(exit()));
	connect(ui.injectButton, SIGNAL(clicked()), this, SLOT(inject()));
	connect(ui.deinjectButton, SIGNAL(clicked()), this, SLOT(deinject()));
	connect(ui.recordPathButton, SIGNAL(clicked()), this, SLOT(recordPath()));
	connect(ui.loadWaypointsFile, SIGNAL(clicked()), this, SLOT(loadWaypointsFile()));
	connect(ui.startBotButton, SIGNAL(clicked()), this, SLOT(startBot()));
	connect(ui.stopBotButton, SIGNAL(clicked()), this, SLOT(stopBot()));

	ui.dllStatusIndicator->setStyleSheet("background-color: rgb(255,0,0)");

	if (serverSDK.connectToServer())
	{
		std::cout << "Connected ! = " << std::endl;
		ui.serverStatusIndicator->setStyleSheet("background-color: rgb(50,205,50)");
		ui.containerInjector->setVisible(false);
		ui.containerDeinject->setVisible(false);
		serverSDK.sendMessage(messageManager.builRequestdAvailableConfigationMessage());
	}
	else {
		std::cout << "Failed to connect !" << std::endl;
		//SHOW ERROR NO SERVER...
	}
	tickNext();

	return;
}

void windowQT::tick() {
	if (serverSDK.getConnectionStatus()) {
		std::list<std::string> messages = serverSDK.getMessageAvailable();
		for (std::list<std::string>::iterator it = messages.begin(); it != messages.end(); it++)
		{
			std::cout << "Message available : " << (*it).c_str() << std::endl;
			if (messageManager.getMessageType((*it)) == MessageType::AVAILABLE_CONFIGURATION) {
				std::cout << "TYPE == AVAILABLE_CONFIGURATION " << std::endl;
				AvailableConfigurationObject* object = messageManager.getAvailableConfigurationObject(*it);

				if (object)
				{
					std::list<std::string> listPIDS = object->getListPID();
					for (std::list<std::string>::iterator itList = listPIDS.begin(); itList != listPIDS.end(); itList++) {
						ui.processPicker->addItem(QString::fromStdString(*itList), QVariant(QString::fromStdString(*itList)));
					}

					std::list<std::string> listModules = object->getListModule();
					for (std::list<std::string>::iterator itList = listModules.begin(); itList != listModules.end(); itList++) {
						ui.modulePicker->addItem(QString::fromStdString(*itList), QVariant(QString::fromStdString(*itList)));
					}

					ui.containerInjector->setVisible(true);
					ui.containerDeinject->setVisible(false);

					free(object);
				}
			}
			else if (messageManager.getMessageType((*it)) == MessageType::DLL_INJECTED)
			{
				std::cout << "TYPE == DLL_INJECTED ! " << std::endl;
				ui.dllStatusIndicator->setStyleSheet("background-color: rgb(50,205,50)");
				ui.containerInjector->setVisible(false);
				ui.containerDeinject->setVisible(true);
			}
			else if (messageManager.getMessageType((*it)) == MessageType::DEINJECT)
			{
				std::cout << "TYPE == DEINJECT ! " << std::endl;
				ui.dllStatusIndicator->setStyleSheet("background-color: rgb(255,0,0)");
				ui.containerInjector->setVisible(true);
				ui.containerDeinject->setVisible(false);
			}
			else if (messageManager.getMessageType((*it)) == MessageType::INFO) {
				std::cout << "TYPE == INFO ! " << std::endl;
				std::pair<std::string, std::string> infoObject = messageManager.getInfoObject(*it);

				if (infoObject.first == "position" && recordWaypointsWindow)
				{
					recordWaypointsWindow->addNewRecordedWaypoints(infoObject.second);
				}
			}
			else {
				std::cout << "TYPE == UNKNOWN TYPE ! " << std::endl;
			}
		}
	}
	else {
		//NOT CONNECTED OR SERVER SHUTDOWN -> DO SOMETHING...
		ui.serverStatusIndicator->setStyleSheet("background-color: rgb(255,0,0)");
	}

	tickNext();
}

void windowQT::closeEvent(QCloseEvent* event)
{
	//OVERIDE the close (using the top right corner)
	serverSDK.disconnect();
}

#include "recordwaypointswindow.h"

void windowQT::exit() {
	//QApplication::exit();
	std::cout << "Click exit !" << std::endl;
}

void windowQT::deinject() {
	std::cout << "Click deinject !" << std::endl;
	serverSDK.sendMessage(messageManager.builRequestdDeinjecteMessage());
}
void windowQT::startBot() {
	std::cout << "Click startBot !" << std::endl;
	serverSDK.sendMessage(messageManager.builRequestdStartBotMessage());
}

void windowQT::stopBot() {
	std::cout << "Click stopBot !" << std::endl;
	serverSDK.sendMessage(messageManager.builRequestdStopBotMessage());
}


void  windowQT::recordPath() {
	std::cout << "Click recordPath !" << std::endl;

	recordWaypointsWindow = new RecordWaypointsWindow(nullptr, &serverSDK);
	//this if used if need to receive event from the created window
	//QObject::connect(recordWaypointsWindow, SIGNAL(exitPressed()), this, SLOT(deleteRecordWindow()));
	recordWaypointsWindow->exec();
	std::cout << "CLOSED RecordWaypointsWindow" << std::endl;
	delete recordWaypointsWindow;
	recordWaypointsWindow = NULL;
}

void windowQT::loadWaypointsFile() {
	std::cout << "Click loadWaypointsFile !" << std::endl;
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load Path File"), "", tr("JSON (*.json)"));
	if (fileName.isEmpty())
		return;
	else {

		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::information(this, tr("Unable to open file"), file.errorString());
			return;
		}
		QDataStream in(&file);
		QString fileContentQstring = file.readAll();
		std::string filecontent = fileContentQstring.toStdString();
		std::cout << "content is =  " << filecontent << std::endl;

		std::list<std::string> listWaypoint = messageManager.getWaypoinsObject(filecontent);
		std::cout << "listWaypoint is =  " << listWaypoint.size() << std::endl;
		if (listWaypoint.size() > 0)
		{
			serverSDK.sendMessage(filecontent);
		}
		else {
			QMessageBox::information(this, tr("Unable to use this file"), "Please save this file wit record path function");
		}
	}
}

void windowQT::deleteRecordWindow() {
	std::cout << "deleteRecordWindow !" << std::endl;
	//delete recordWaypointsWindow;
}

void windowQT::inject() {
	std::cout << "Click inject !" << std::endl;
	int pid = ui.processPicker->itemData(ui.processPicker->currentIndex()).toInt();
	std::string module = ui.modulePicker->itemData(ui.modulePicker->currentIndex()).toString().toStdString();
	std::cout << "PID == " << pid << std::endl;
	std::cout << "Module == " << module << std::endl;
	if (pid > 0 && !module.empty())
	{
		std::cout << "SEND INJECT == " << pid << std::endl;
		serverSDK.sendMessage(messageManager.builRequestdInjectMessage(pid, module));
	}
	else {
		std::cout << "[ERROR] SELECT PID / MODULE == " << pid << std::endl;
	}
}