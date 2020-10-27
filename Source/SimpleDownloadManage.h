/*********************************************************************************
*FileName:				SimpleDownloadManage.h
*
*Author:				simple
*
*Date:					2020-05-27
*
*Description:			下载管理器;
**********************************************************************************/
#ifndef SimpleDownloadManage_H
#define SimpleDownloadManage_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include "SimpleHttpDefine.h"
class SimpleDownloadTask;
class SimpleDownloadManage : public QObject
{
	Q_OBJECT
public:
	//下载优先级;
	enum Priority_Type
	{
		High,
		Normal
	};

public:
	static SimpleDownloadManage* getInstance()
	{
		static SimpleDownloadManage instance;
		return &instance;
	}
    void init();
	//设置最大下载并发数量;
	void setMaxDownloadNum(int num);
	//添加下载任务,返回任务ID;
	QString addDownloadTask(SimpleHttp::stDownloadInfo info, Priority_Type type);
	//同步下载任务;
	bool syncDownloadTask(SimpleHttp::stDownloadInfo info);
public:
	SimpleDownloadManage(QObject *parent=0);
	~SimpleDownloadManage();

private slots:
	void onTimeOut();
	void onDownloadFinished(QString id);
signals:
	//下载进度;
	void downloadProgress(QString id, qint64 recv_total, qint64 all_total);
	//下载完成;
	void downloadFinished(QString id);

private:
	QTimer* m_timer;
	int m_maxNum;
	QList<SimpleHttp::stDownloadInfo> m_highDownloadTask;
	QList<SimpleHttp::stDownloadInfo> m_normalDownloadTask;
	QMap<QString, SimpleDownloadTask*> m_downloadingMap;
};
#endif
