#include "windowQT.h"

windowQT::windowQT(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.exit, SIGNAL(clicked()), this, SLOT(exit()));
	connect(ui.injectButton, SIGNAL(clicked()), this, SLOT(inject()));
	connect(ui.deinjectButton, SIGNAL(clicked()), this, SLOT(deinject()));

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

void windowQT::exit() {
	//QApplication::exit();
	std::cout << "Click !" << std::endl;
}

void windowQT::deinject() {
	std::cout << "Click deinject !" << std::endl;
	serverSDK.sendMessage(messageManager.builRequestdDeinjecteMessage());
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