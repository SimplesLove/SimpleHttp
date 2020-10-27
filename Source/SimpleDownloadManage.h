/*********************************************************************************
*FileName:				SimpleDownloadManage.h
*
*Author:				simple
*
*Date:					2020-05-27
*
*Description:			���ع�����;
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
	//�������ȼ�;
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
	//����������ز�������;
	void setMaxDownloadNum(int num);
	//�����������,��������ID;
	QString addDownloadTask(SimpleHttp::stDownloadInfo info, Priority_Type type);
	//ͬ����������;
	bool syncDownloadTask(SimpleHttp::stDownloadInfo info);
public:
	SimpleDownloadManage(QObject *parent=0);
	~SimpleDownloadManage();

private slots:
	void onTimeOut();
	void onDownloadFinished(QString id);
signals:
	//���ؽ���;
	void downloadProgress(QString id, qint64 recv_total, qint64 all_total);
	//�������;
	void downloadFinished(QString id);

private:
	QTimer* m_timer;
	int m_maxNum;
	QList<SimpleHttp::stDownloadInfo> m_highDownloadTask;
	QList<SimpleHttp::stDownloadInfo> m_normalDownloadTask;
	QMap<QString, SimpleDownloadTask*> m_downloadingMap;
};
#endif
