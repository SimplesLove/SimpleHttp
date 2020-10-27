/*********************************************************************************
*FileName:				SimpleWebSocket.h
*
*Author:				simple
*
*Date:					2020-08-03
*
*Description:			WebSocketͨ��;
**********************************************************************************/

#ifndef SimpleWebSocket_H
#define SimpleWebSocket_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>
class SimpleWebSocket : public QObject
{
	Q_OBJECT

public:
	SimpleWebSocket(QObject *parent);
	~SimpleWebSocket();
	//���ӷ���;
	void connectServer(QString url);
	//�رշ���;
	void closeConnect();
	//������Ϣ;
	void sendMsg(QString msg);

signals:
	void receivedMessage(QString content);

private slots:
	void onConnected();
	void onCloseConnection();
	void onMessageReceived(QString content);

	void onHeartBeatTimeOut();
private:
	QWebSocket* m_websocket;
	QString  m_url;

	QTimer* m_heartBeatTimer;
};
#endif
