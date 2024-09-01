#pragma once
#include "IClientSDK.h"

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class TimerUpdater : public QThread
{
	Q_OBJECT

public:
	explicit TimerUpdater(IClientSDKPtr sdk, QObject* parent = nullptr);
	~TimerUpdater();

	void Start();
	void Stop();

private:
	void run() override;

	bool m_running;

	QMutex m_mutex;
	QWaitCondition m_waitCondition;

	IClientSDKPtr m_sdk;
};
