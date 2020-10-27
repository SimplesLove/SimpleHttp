/*********************************************************************************
*FileName:				SimpleDownloadTask.h
*
*Author:				simple
*
*Date:					2020-05-27
*
*Description:			œ¬‘ÿ»ŒŒÒ;
**********************************************************************************/
#ifndef SimpleDownloadTask_H
#define SimpleDownloadTask_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
class SimpleDownloadTask : public QObject
{
	Q_OBJECT

public:
	SimpleDownloadTask(QString id,QObject*parent=0);
	~SimpleDownloadTask();

	bool startDownload(QString url, QString fileName,bool sync=false);
	QString taskId();

private slots:
	void onDownloadFinished();
	void onDownloadReadyRead();
	void onDownloadProgress(qint64 recv_total, qint64 all_total);

signals:
	void downloadFinished(QString m_id);
	void downloadProgress(QString m_id,qint64 recv_total, qint64 all_total);
private:
	QString m_id;
	QString m_url;
	QString m_fileName;

	QNetworkAccessManager*	m_networkAccessManager;
	QNetworkRequest			m_request;
	QNetworkReply*          m_reply;
	QFile*					m_file;
};

#endif
