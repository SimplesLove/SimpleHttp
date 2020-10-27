#include "SimpleHttpDemo.h"
#include <QJsonDocument>
#include "SimpleDownloadManage.h"
SimpleHttpDemo::SimpleHttpDemo(QObject *parent)
	: QObject(parent)
{
	connect(SimpleDownloadManage::getInstance(), &SimpleDownloadManage::downloadFinished, this, &SimpleHttpDemo::onDownloadFinished);
	connect(SimpleDownloadManage::getInstance(), &SimpleDownloadManage::downloadProgress, this, &SimpleHttpDemo::onDownloadProgress);
}

SimpleHttpDemo::~SimpleHttpDemo()
{
}

void SimpleHttpDemo::webSocketTest(QString url)
{
	m_webSocket = new SimpleWebSocket(this);
	connect(m_webSocket, &SimpleWebSocket::receivedMessage, this, &SimpleHttpDemo::onMessageReceived);
	m_webSocket->connectServer(url);
}

void SimpleHttpDemo::postHttpRequestTest(QString url,QVariantMap info)
{
	QVariantMap info;
	SimpleHttpRequest httpRequest;
	//同步调用方式;
	httpRequest.setSynchronization(true);
	if (!httpRequest.request(SimpleHttpRequest::Post_Type, url, "", serializeJson(info)))
	{
		qDebug() << "ALARM FAIL NETWORKERROR!";
		return;
	}
	QByteArray result = httpRequest.result();
}

void SimpleHttpDemo::getHttpRequestTest(QString url)
{
	SimpleHttpRequest httpRequest;
	//同步调用方式;
	httpRequest.setSynchronization(true);
	if (!httpRequest.request(SimpleHttpRequest::Get_Type, url))
	{
		qDebug() << "ALARM FAIL NETWORKERROR!";
		return;
	}
	QByteArray result = httpRequest.result();
}

void SimpleHttpDemo::uploadHttpRequestTest(QString url, QString filename)
{
	SimpleHttpRequest httpRequest;
	QList<SimpleHttpRequest::stRequsetContent> contentList;
	contentList << SimpleHttpRequest::stRequsetContent("filename", "1.jpg");
	contentList << SimpleHttpRequest::stRequsetContent("file", "", filename);
	httpRequest.setSynchronization(true);
	httpRequest.requestFormData(url, "", contentList);
	QString res = QString::fromUtf8(httpRequest.result());
}

void SimpleHttpDemo::downloadFileTest(QString url)
{
	SimpleHttp::stDownloadInfo info;
	info.url = url;
	info.fileName = "d://1.jpg";
	SimpleDownloadManage::getInstance()->syncDownloadTask(info);
}

void SimpleHttpDemo::downloadFileManageTest(QString url)
{
	SimpleDownloadManage::getInstance()->init();
	SimpleHttp::stDownloadInfo downloadVideoInfo;
	downloadVideoInfo.url = url;
	QString fileName = "d://1.jpg";
	//返回值ID为下载ID,onDownloadFinished和onDownloadProgress参数ID保持一致
	QString downloadId = SimpleDownloadManage::getInstance()->addDownloadTask(downloadVideoInfo, SimpleDownloadManage::High);
}

void SimpleHttpDemo::onDownloadFinished(QString id)
{

}

void SimpleHttpDemo::onDownloadProgress(QString id, qint64 recv_total, qint64 all_total)
{

}

QString SimpleHttpDemo::serializeJson(const QVariantMap data)
{
	QJsonDocument jsonDocument = QJsonDocument::fromVariant(data);
	return jsonDocument.toJson();
}
