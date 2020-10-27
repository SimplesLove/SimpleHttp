#include "SimpleDownloadManage.h"
#include "SimpleDownloadTask.h"
#include <QUuid>
SimpleDownloadManage::SimpleDownloadManage(QObject *parent)
	: QObject(parent)
	, m_maxNum(3)
{
}


SimpleDownloadManage::~SimpleDownloadManage()
{
}

void SimpleDownloadManage::onTimeOut()
{
	if (m_downloadingMap.size() < m_maxNum)
	{
		if (!m_highDownloadTask.isEmpty())
		{
			SimpleHttp::stDownloadInfo info = m_highDownloadTask.takeFirst();
			SimpleDownloadTask* task = new SimpleDownloadTask(info.id,this);
			QString id = task->taskId();
			connect(task, &SimpleDownloadTask::downloadFinished, this, &SimpleDownloadManage::onDownloadFinished);
			connect(task, &SimpleDownloadTask::downloadProgress, this, &SimpleDownloadManage::downloadProgress);
			task->startDownload(info.url, info.fileName);
			m_downloadingMap.insert(info.id, task);
			return;
		}
		if (!m_normalDownloadTask.isEmpty())
		{
			SimpleHttp::stDownloadInfo info = m_normalDownloadTask.takeFirst();
			SimpleDownloadTask* task = new SimpleDownloadTask(info.id,this);
			connect(task, &SimpleDownloadTask::downloadFinished, this, &SimpleDownloadManage::onDownloadFinished);
			connect(task, &SimpleDownloadTask::downloadProgress, this, &SimpleDownloadManage::downloadProgress);
			
			task->startDownload(info.url, info.fileName);
			m_downloadingMap.insert(info.id, task);
			return;
		}
	}
}

void SimpleDownloadManage::onDownloadFinished(QString id)
{
	SimpleDownloadTask* task=m_downloadingMap.take(id);
	delete task;
	emit downloadFinished(id);
}

void SimpleDownloadManage::init()
{
    m_timer = new QTimer(this);
    m_timer->start(500);
    connect(m_timer, &QTimer::timeout, this, &SimpleDownloadManage::onTimeOut);
}

void SimpleDownloadManage::setMaxDownloadNum(int num)
{
	m_maxNum = num;
}

QString SimpleDownloadManage::addDownloadTask(SimpleHttp::stDownloadInfo info, Priority_Type type)
{
	QString id = QUuid::createUuid().toString().remove("{").remove("}");
	info.id = id;
	switch (type)
	{
	case SimpleDownloadManage::High:
	{
		m_highDownloadTask.append(info);
		return id;
	}
		break;
	case SimpleDownloadManage::Normal:
	{
		m_normalDownloadTask.append(info);
		return id;
	}
		break;
	default:
		break;
	}
	return "";
}

bool SimpleDownloadManage::syncDownloadTask(SimpleHttp::stDownloadInfo info)
{
	SimpleDownloadTask* task = new SimpleDownloadTask(info.id, this);
	return task->startDownload(info.url, info.fileName,true);
}
