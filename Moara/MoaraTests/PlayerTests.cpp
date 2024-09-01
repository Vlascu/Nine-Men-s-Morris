#include <gtest/gtest.h>
#include <memory>

#include "Player.h"

class PlayerTests : public testing::Test
{
protected:
	void SetUp()
	{
		m_player = std::make_shared<Player>();
	}
public:
	PlayerPtr m_player;
};

TEST_F(PlayerTests, PlayerInitialization)
{
	EXPECT_EQ(m_player->GetType(), EPieceType::None);
}

TEST_F(PlayerTests, GettersSetters)
{
	m_player->SetType(EPieceType::Black);
	EXPECT_EQ(m_player->GetType(), EPieceType::Black);

	m_player->SetType(EPieceType::White);
	EXPECT_EQ(m_player->GetType(), EPieceType::White);

	m_player->SetType(EPieceType::None);
	EXPECT_EQ(m_player->GetType(), EPieceType::None);
}

TEST_F(PlayerTests, PlayerTimer)
{
	EXPECT_EQ(m_player->GetTimerDuration().count(), 60000);

	m_player->SetTimerDuration(Miliseconds(10000));
	EXPECT_EQ(m_player->GetTimerDuration().count(), 10000);

	auto duration = m_player->GetTimerDuration().count();
	auto elapsed = m_player->GetTimerElapsedTime().count();
	auto remaining = m_player->GetTimerRemainingDuration().count();

	EXPECT_EQ(duration - elapsed, remaining);

	m_player->SubtractTimerDuration(Miliseconds(5000));
	EXPECT_EQ(m_player->GetTimerDuration().count(), 5000);
}