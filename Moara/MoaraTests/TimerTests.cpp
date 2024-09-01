#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "Timer.h"

using::testing::Return;

class MockClock : public IClock
{
public:
	Timestamp GetCurrentTime() override
	{
		return currentTime;
	}
	Miliseconds TimeBetween(Timestamp from, Timestamp to) override
	{
		if (from == to)
			return Miliseconds(0);

		return std::chrono::duration_cast<Miliseconds>(to - from);
	}

	Timestamp currentTime;
};

class TimerTests : public testing::Test
{
protected:
	static void SetUpTestSuite()
	{
		Timer::SetClock(m_clock = std::make_shared<MockClock>());

		m_timer = std::make_shared<Timer>(Miliseconds(5000));

		Timer::s_running = false;
		if (Timer::s_thread.joinable())
			Timer::s_thread.join();
	}

	static void TearDownTestSuite()
	{
		m_timer.reset();
	}

	void SetUp()
	{
		SetTime(Miliseconds(0));
		m_timer->SetDuration(Miliseconds(5000));
	}

	void TearDown()
	{
		m_timer->Stop();
	}

public:
	void SetTime(Miliseconds miliseconds)
	{
		m_clock->currentTime = Timestamp(miliseconds);
		Timer::Run();
	}

	static TimerPtr m_timer;
	static std::shared_ptr<MockClock> m_clock;
};

TimerPtr TimerTests::m_timer;
std::shared_ptr<MockClock> TimerTests::m_clock;

TEST_F(TimerTests, TimerInitialization)
{
	EXPECT_EQ(m_timer->GetTotalDuration(), Miliseconds(5000));
	EXPECT_EQ(m_timer->GetElapsedTime(), Miliseconds(0));
	EXPECT_EQ(m_timer->GetRemainingDuration(), Miliseconds(5000));
}

TEST_F(TimerTests, TimerGettersSetters)
{
	m_timer->SetDuration(Miliseconds(10000));
	EXPECT_EQ(m_timer->GetTotalDuration(), Miliseconds(10000));
	EXPECT_EQ(m_timer->GetElapsedTime(), Miliseconds(0));
	EXPECT_EQ(m_timer->GetRemainingDuration(), Miliseconds(10000));

	m_timer->AddDuration(Miliseconds(1000));
	EXPECT_EQ(m_timer->GetTotalDuration(), Miliseconds(11000));
	EXPECT_EQ(m_timer->GetElapsedTime(), Miliseconds(0));
	EXPECT_EQ(m_timer->GetRemainingDuration(), Miliseconds(11000));

	m_timer->SubtractDuration(Miliseconds(500));
	EXPECT_EQ(m_timer->GetTotalDuration(), Miliseconds(10500));
	EXPECT_EQ(m_timer->GetElapsedTime(), Miliseconds(0));
	EXPECT_EQ(m_timer->GetRemainingDuration(), Miliseconds(10500));
}

TEST_F(TimerTests, TimerStates)
{
	EXPECT_EQ(m_timer->GetTimerState(), Timer::TimerState::Stopped);

	m_timer->Pause();//Stop->Paused should remain Stop
	EXPECT_EQ(m_timer->GetTimerState(), Timer::TimerState::Stopped);
	
	m_timer->Start();//Stop->Running
	EXPECT_EQ(m_timer->GetTimerState(), Timer::TimerState::Running);

	m_timer->Pause();//Running->Paused
	EXPECT_EQ(m_timer->GetTimerState(), Timer::TimerState::Paused);

	m_timer->Stop();//Paused->Stop
	EXPECT_EQ(m_timer->GetTimerState(), Timer::TimerState::Stopped);

	m_timer->Start(); m_timer->Pause(); m_timer->Start();//Paused->Running
	EXPECT_EQ(m_timer->GetTimerState(), Timer::TimerState::Running);

	m_timer->Stop();//Running->Stop
	EXPECT_EQ(m_timer->GetTimerState(), Timer::TimerState::Stopped);
}

TEST_F(TimerTests, TimerRunning)
{
	//stopped
	SetTime(Miliseconds(1000));
	EXPECT_EQ(m_timer->GetElapsedTime(), Miliseconds(0));

	//running
	SetTime(Miliseconds(0));
	m_timer->Start();
	SetTime(Miliseconds(1000));
	EXPECT_EQ(m_timer->GetElapsedTime(), Miliseconds(1000));
	EXPECT_EQ(m_timer->GetRemainingDuration(), Miliseconds(4000));
	SetTime(Miliseconds(5000));
	EXPECT_EQ(m_timer->GetElapsedTime(), Miliseconds(5000));
	EXPECT_EQ(m_timer->GetRemainingDuration(), Miliseconds(0));
	SetTime(Miliseconds(8000));
	EXPECT_EQ(m_timer->GetElapsedTime(), Miliseconds(5000));
	EXPECT_EQ(m_timer->GetRemainingDuration(), Miliseconds(0));
}