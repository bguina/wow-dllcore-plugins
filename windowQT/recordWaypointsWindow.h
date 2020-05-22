#pragma once

#include <QDialog>
#include <QStringListModel>
#include <QFileDialog>
#include <QMessageBox>
#include "Client.h"

namespace Ui {
	class RecordWaypointsWindow;
}

class RecordWaypointsWindow : public QDialog
{
	Q_OBJECT

public:
	explicit RecordWaypointsWindow(QWidget* parent, Client* client);
	~RecordWaypointsWindow();
	void addNewRecordedWaypoints(std::string waypoint);

signals:
	//void exitPressed();


private:
	Ui::RecordWaypointsWindow* ui;
	Client* mClient;
	QStringListModel* model;
	void closeEvent(QCloseEvent* event);

private slots:
	void startRecord();
	void stopRecord();
	void saveFile();
};
