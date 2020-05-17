#include "recordwaypointswindow.h"
#include "ui_recordwaypointswindow.h"

RecordWaypointsWindow::RecordWaypointsWindow(QWidget* parent, ServerSDK* _serverSDK) : QDialog(parent), ui(new Ui::RecordWaypointsWindow)
{
	serverSDK = _serverSDK;
	ui->setupUi(this);
	connect(ui->startRecordButton, SIGNAL(clicked()), this, SLOT(startRecord()));
	connect(ui->stopRecordButton, SIGNAL(clicked()), this, SLOT(stopRecord()));

	// Create model
	model = new QStringListModel(this);

	// Make data

	QStringList List;

	// Populate our model
	//model->setStringList(List);

	// Glue model and view together
	ui->listWaypoints->setModel(model);
}

RecordWaypointsWindow::~RecordWaypointsWindow()
{
	delete ui;
}

void RecordWaypointsWindow::startRecord()
{
	std::cout << "startRecord Clicked" << std::endl;
	std::list<std::string> toSubscribe;
	toSubscribe.push_back("position");
	serverSDK->sendMessage(serverSDK->getMessageManager().builRequestStartSubcribe(toSubscribe));
}

void RecordWaypointsWindow::stopRecord()
{
	std::cout << "stopRecord Clicked" << std::endl;
	std::list<std::string> stopSubscribe;
	stopSubscribe.push_back("position");
	serverSDK->sendMessage(serverSDK->getMessageManager().builRequestStopSubcribe(stopSubscribe));
}

void RecordWaypointsWindow::addNewRecordedWaypoints(std::string waypoint) {
	model->insertRow(model->rowCount());
	QModelIndex index = model->index(model->rowCount() - 1);
	model->setData(index, QVariant(QString::fromStdString(waypoint)));
}

void RecordWaypointsWindow::closeEvent(QCloseEvent* event)
{
	//This is used to send event
	//emit exitPressed();

}