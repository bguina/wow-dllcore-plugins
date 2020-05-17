#pragma once

#include "ServerSDK.h"
#include "MessageManager.h"
#include <QtWidgets/QMainWindow>
#include "ui_windowQT.h"
#include "recordWaypointsWindow.h"


class windowQT : public QMainWindow
{
	Q_OBJECT

public:
	windowQT(QWidget* parent = Q_NULLPTR);

private:
	Ui::windowQTClass ui;
	void tickNext()
	{
		// Trigger the tick() invokation when the event loop runs next time
		QMetaObject::invokeMethod(this, "tick", Qt::QueuedConnection);
	}
	void windowQT::closeEvent(QCloseEvent* event);
	ServerSDK serverSDK;
	MessageManager messageManager;
	RecordWaypointsWindow* recordWaypointsWindow = NULL;
private slots:
	void exit();
	void inject();
	void deinject();
	void recordPath();
	void deleteRecordWindow();
	void tick();
};
