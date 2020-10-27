#include "SimpleHttpRequest.h"
#include <QNetworkReply>
#include <QEventLoop>
#include <QHttpMultiPart>
#include <QFileInfo>

SimpleHttpRequest::SimpleHttpRequest(QObject *parent)
	: QObject(parent)
	, m_timeout(0)
	, m_bSynchronization(false)
	, m_bResult(true)
{
	m_networkAccessManager = new QNetworkAccessManager(this);
	connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onRequestFinished(QNetworkReply *)));

    m_timer = new QTimer(this);
}

SimpleHttpRequest::~SimpleHttpRequest()
{
    delete m_reply;
}

bool SimpleHttpRequest::request(RequestType type, QString url, QString token, QString jsonParam)
{
    m_bResult = true;
	if (!token.isEmpty())
	{
		m_request.setRawHeader("token", token.toStdString().c_str());
	}
	m_request.setUrl(url);
	if (jsonParam.isEmpty())
	{
		m_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");
	}
	else
	{
		m_request.setRawHeader("Content-Type", "application/json");
	}
	switch (type)
	{
	case SimpleHttpRequest::Get_Type:
	{
		m_reply=m_networkAccessManager->get(m_request);
	}
		break;
	case SimpleHttpRequest::Post_Type:
	{
		m_reply = m_networkAccessManager->post(m_request, jsonParam.toUtf8());
	}
		break;
	case SimpleHttpRequest::Delete_Type:
	{
		m_reply = m_networkAccessManager->deleteResource(m_request);
	}
		break;
	default:
		break;
	}
	if (m_timeout > 0)
	{
		m_timer->start(m_timeout);
		connect(m_timer, &QTimer::timeout, [=]()
		{
			m_reply->abort();
			m_timer->stop();
			emit requestFinish();
		});
	}
	if (m_bSynchronization)
	{
		QEventLoop loop;
		connect(this, &SimpleHttpRequest::requestFinish, &loop, &QEventLoop::quit);
		loop.exec();
	}
	return m_bResult;
}

void SimpleHttpRequest::requestFormData(QString url, QString token, QList<stRequsetContent> paramList)
{
	if (!token.isEmpty())
	{
		m_request.setRawHeader("token", token.toStdString().c_str());
	}
	m_request.setUrl(url);
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	for (int i = 0; i < paramList.size(); i++)
	{
		if (!paramList[i].path.isEmpty())
		{
			QHttpPart httpPart;
            QFile* file=new QFile(paramList[i].path);
			QFileInfo fileInfo(paramList[i].path);
			QString fileName = fileInfo.fileName();
			//httpPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"image\"; filename=\"" + fileInfo.fileName().toUtf8() + "\"");
			httpPart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%0\";filename=\"%1\"").arg(paramList[i].name).arg(fileInfo.fileName()));
			if (file->open(QIODevice::ReadOnly))
			{
				httpPart.setBodyDevice(file);
				file->setParent(multiPart);
				multiPart->append(httpPart);
			}
		}
		else
		{
			QHttpPart httpPart;
			httpPart.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%0\"").arg(paramList[i].name));
			httpPart.setBody(paramList[i].value.toUtf8());
			multiPart->append(httpPart);
		}
	}
	m_reply = m_networkAccessManager->post(m_request, multiPart);
    multiPart->setParent(m_reply);
	if (m_timeout > 0)
	{
		m_timer->start(m_timeout);
		connect(m_timer, &QTimer::timeout, [=]()
		{
			m_reply->abort();
			m_timer->stop();
			emit requestFinish();
		});
	}
	if (m_bSynchronization)
	{
		QEventLoop loop;
		connect(this, &SimpleHttpRequest::requestFinish, &loop, &QEventLoop::quit);
		loop.exec();
	}
}

void SimpleHttpRequest::setTimeOut(int ms)
{
	m_timeout = ms;
}

void SimpleHttpRequest::setSynchronization(bool v)
{
	m_bSynchronization = v;
}

void SimpleHttpRequest::setCallBackFunc(HttpRequest_Callback func)
{

}

QByteArray SimpleHttpRequest::result()
{
	return m_data;
}

void SimpleHttpRequest::onRequestFinished(QNetworkReply *reply)
{
	if (reply->error() == QNetworkReply::NoError)
	{
		 m_data = reply->readAll();
		 if (m_callBackFunc)
		 {
			 m_callBackFunc(m_data);
		 }
		 else {
			 emit requestResult(m_data);
		 }
	}
	else
	{
		m_bResult = false;
		qDebug() << "CONNECT FAILED:" << reply->error();
	}
	emit requestFinish();
}
