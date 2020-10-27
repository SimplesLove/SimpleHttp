#include "SimpleDownloadTask.h"
#include <QEventLoop>
SimpleDownloadTask::SimpleDownloadTask(QString id,QObject *parent)
	: QObject(parent)
	, m_id(id)
{
	m_networkAccessManager = new QNetworkAccessManager(this);
	m_file = new QFile(this);
}

SimpleDownloadTask::~SimpleDownloadTask()
{
}


bool SimpleDownloadTask::startDownload(QString url, QString fileName, bool sync)
{
	m_url = url;
	m_fileName = fileName;
	m_request.setUrl(m_url);
	m_reply = m_networkAccessManager->get(m_request);

	connect(m_reply, &QNetworkReply::readyRead, this, &SimpleDownloadTask::onDownloadReadyRead);
	connect(m_reply, &QNetworkReply::finished, this, &SimpleDownloadTask::onDownloadFinished);
	connect(m_reply, &QNetworkReply::downloadProgress, this, &SimpleDownloadTask::onDownloadProgress);

	m_file->setFileName(fileName);
	bool ret = m_file->open(QIODevice::WriteOnly | QIODevice::Truncate);    //创建文件
	if (!ret)
	{
		qDebug() << "文件创建失败";
		return false;
	}

	if (sync)
	{
		QEventLoop loop;
		connect(m_reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
		loop.exec();
	}
	return true;
}

QString SimpleDownloadTask::taskId()
{
	return m_id;
}

void SimpleDownloadTask::onDownloadFinished()
{
	if (m_reply->error() != QNetworkReply::NoError)
	{
		qDebug() << "DOWNLOAD FAILED";
	}
	m_file->close();
	emit downloadFinished(m_id);
}

void SimpleDownloadTask::onDownloadReadyRead()
{
	m_file->write(m_reply->readAll());
}

void SimpleDownloadTask::onDownloadProgress(qint64 recv_total, qint64 all_total)
{
	emit downloadProgress(m_id, recv_total, all_total);
}

