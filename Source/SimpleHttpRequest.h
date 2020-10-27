/*********************************************************************************
*FileName:				SimpleHttpRequest.h
*
*Author:				simple
*
*Date:					2020-05-27
*
*Description:			Http请求;
**********************************************************************************/
#ifndef SimpleHttpRequest_H
#define SimpleHttpRequest_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QMutex>
#include <QByteArray>
#include <functional>
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>

typedef std::function<QByteArray(QByteArray data)> HttpRequest_Callback;

class SimpleHttpRequest: public QObject
{
	Q_OBJECT

public:
	enum RequestType
	{
		Get_Type,
		Post_Type,
		Delete_Type,
	};

	struct stRequsetContent
	{
		stRequsetContent(QString name, QString value, QString path = "")
		{
			this->name = name;
			this->value = value;
			this->path = path;
		}
		//参数名字;
		QString name;
		//参数值;
		QString value;
		//文件路径，不为空则取文件数据上传;
		QString path;
	};

public:
	SimpleHttpRequest(QObject *parent=0);
	~SimpleHttpRequest();
	
	bool request(RequestType type, QString url,QString token="",QString jsonParam="");
	//带文件的数据请求;
	void requestFormData(QString url,QString token,QList<stRequsetContent> paramList);
	//设置超时时间;
	void setTimeOut(int ms);
	//设置同步方式;
	void setSynchronization(bool v);
	//设置回调函数;
	void setCallBackFunc(HttpRequest_Callback func);
	//获取结果;
	QByteArray result();
signals:
	void requestResult(QByteArray data);
	void requestFinish();

private slots:
	void onRequestFinished(QNetworkReply *reply);
private:
	QNetworkAccessManager*	m_networkAccessManager;

	int						m_timeout;
	QMutex					m_requsetMutex;
	bool					m_bSynchronization;

	HttpRequest_Callback	m_callBackFunc=NULL;
	QByteArray				m_data;
	QTimer*					m_timer;
	QNetworkRequest			m_request;
	QNetworkReply*          m_reply;

	bool m_bResult;
};

#endif