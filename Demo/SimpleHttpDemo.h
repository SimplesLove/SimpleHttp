#pragma once

#include <QObject>
#include "SimpleWebSocket.h"
#include "SimpleHttpRequest.h"
#include <QVariantMap>
class SimpleHttpDemo : public QObject
{
	Q_OBJECT

public:
	SimpleHttpDemo(QObject *parent);
	~SimpleHttpDemo();

	void webSocketTest(QString url);
	void postHttpRequestTest(QString url,QVariantMap info);
	void getHttpRequestTest(QString url);
	void uploadHttpRequestTest(QString url,QString filename);
	void downloadFileTest(QString url);
	void downloadFileManageTest(QString url);

private slots:
	void onMessageReceived(QString content);
	void onDownloadFinished(QString id);
	void onDownloadProgress(QString id, qint64 recv_total, qint64 all_total);

protected:
	QString serializeJson(const QVariantMap data);

private:
	SimpleWebSocket* m_webSocket;
};
