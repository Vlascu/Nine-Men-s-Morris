#include "TimerUpdater.h"

#include <QDateTime>

TimerUpdater::TimerUpdater(IClientSDKPtr sdk, QObject* parent)
	: m_sdk(sdk)
	, QThread(parent)
	, m_running(false)
{
}

TimerUpdater::~TimerUpdater()
{
	Stop();
	wait();
}

void TimerUpdater::Start()
{
	m_running = true;
}

void TimerUpdater::Stop()
{
	QMutexLocker locker(&m_mutex);
	m_running = false;
	m_waitCondition.wakeAll();
}

void TimerUpdater::run()
{
	m_running = true;

	while (m_running)
	{
		QMutexLocker locker(&m_mutex);
		m_sdk->GetTimer();
		m_waitCondition.wait(&m_mutex, 250);
	}
}
