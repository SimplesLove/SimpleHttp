#include "SimpleWebSocket.h"
#include <QDebug>
SimpleWebSocket::SimpleWebSocket(QObject *parent)
	: QObject(parent)
{
	m_websocket = new QWebSocket();
	connect(m_websocket, &QWebSocket::connected, this, &SimpleWebSocket::onConnected);
	connect(m_websocket, &QWebSocket::disconnected, this, &SimpleWebSocket::onCloseConnection);
	connect(m_websocket, &QWebSocket::textMessageReceived, this, &SimpleWebSocket::onMessageReceived);

	m_heartBeatTimer = new QTimer(this);
	connect(m_heartBeatTimer, &QTimer::timeout, this, &SimpleWebSocket::onHeartBeatTimeOut);
}

SimpleWebSocket::~SimpleWebSocket()
{
	closeConnect();
	delete m_websocket;
}

void SimpleWebSocket::connectServer(QString url)
{
	m_websocket->open(QUrl(url));
	m_heartBeatTimer->start(30 * 1000);
	m_url = url;
}

void SimpleWebSocket::closeConnect()
{
	m_heartBeatTimer->stop();
	m_websocket->close();
}

void SimpleWebSocket::sendMsg(QString msg)
{
	m_websocket->sendTextMessage(msg);
}

void SimpleWebSocket::onConnected()
{
    qDebug() << "WebSocket 连接成功";
}

void SimpleWebSocket::onCloseConnection()
{
	qDebug() << "关闭连接";
}

void SimpleWebSocket::onMessageReceived(QString content)
{
	qDebug() << "收到消息：" << content;
	emit receivedMessage(content);
}

void SimpleWebSocket::onHeartBeatTimeOut()
{
	//qDebug() << "HeartBeat";
	m_websocket->sendTextMessage("HeartBeat");
}
