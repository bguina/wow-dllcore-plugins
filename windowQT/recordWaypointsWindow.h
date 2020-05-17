#ifndef RECORDWAYPOINTSWINDOW_H
#define RECORDWAYPOINTSWINDOW_H

#include <QDialog>
#include <QStringListModel>
#include "ServerSDK.h"

namespace Ui {
	class RecordWaypointsWindow;
}

class RecordWaypointsWindow : public QDialog
{
	Q_OBJECT

public:
	explicit RecordWaypointsWindow(QWidget* parent, ServerSDK* serverSDK);
	~RecordWaypointsWindow();
	void addNewRecordedWaypoints(std::string waypoint);

signals:
	//void exitPressed();


private:
	Ui::RecordWaypointsWindow* ui;
	ServerSDK* serverSDK;
	QStringListModel* model;
	void closeEvent(QCloseEvent* event);
	
private slots:
	void startRecord();
	void stopRecord();
};

#endif // RECORDWAYPOINTSWINDOW_H
