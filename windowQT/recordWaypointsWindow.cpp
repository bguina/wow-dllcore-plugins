#include "recordwaypointswindow.h"
#include "ui_recordwaypointswindow.h"

RecordWaypointsWindow::RecordWaypointsWindow(QWidget* parent, Client* cl) :
	QDialog(parent),
	ui(new Ui::RecordWaypointsWindow),
	mClient(cl)
{
	ui->setupUi(this);
	connect(ui->startRecordButton, SIGNAL(clicked()), this, SLOT(startRecord()));
	connect(ui->stopRecordButton, SIGNAL(clicked()), this, SLOT(stopRecord()));
	connect(ui->savefileButton, SIGNAL(clicked()), this, SLOT(saveFile()));

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
	mClient->sendMessage(mClient->getMessageManager().builRequestStartSubcribe(toSubscribe));
}

void RecordWaypointsWindow::stopRecord()
{
	std::cout << "stopRecord Clicked" << std::endl;
	std::list<std::string> stopSubscribe;
	stopSubscribe.push_back("position");
	mClient->sendMessage(mClient->getMessageManager().builRequestStopSubcribe(stopSubscribe));
}

void RecordWaypointsWindow::saveFile() {
	std::cout << "saveFile Clicked" << std::endl;

	std::list<std::string> listWaypoint;

	for (int count = 0; count < model->rowCount(); count++)
	{
		std::string waypoint = model->data(model->index(count)).toString().toStdString();
		listWaypoint.push_back(waypoint);
	}

	MessageManager messageManager;
	std::string listWaypointJSON = messageManager.buildWaypoinsFile(listWaypoint);
	std::cout << "listWaypointJSON to save == " << listWaypointJSON << std::endl;

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Waypoints"), "", tr("All Files (*)"));
	if (fileName.isEmpty())
		return;
	else {
		QFile file(fileName + ".json");
		if (!file.open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("Unable to open file"), file.errorString());
			return;
		}
		QDataStream out(&file);
		out.writeRawData(listWaypointJSON.c_str(), listWaypointJSON.size());
	}
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