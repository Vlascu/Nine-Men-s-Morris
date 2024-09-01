#include "Move.h"
#include "Logger.h"
#include "InvalidStateException.h"

#include "Game.h"

static log4cpp::Category& LOG = Logger::getInstance().getRoot();

Move::Move(Game* game, int from, int to)
	: Command(game)
	, m_from(from)
	, m_to(to)
{}

void Move::Execute()
{
	if (m_game->m_state != EGameState::Moving)
	{
		LOG.notice("Not in move state");
		throw InvalidStateException("Can't move now");
	}

	m_board->MovePiece(m_from, m_to, m_game->m_players[m_game->m_activePlayer]->GetType());
	LOG.info("Moved piece from {%d} to {%d}", m_from, m_to);

	m_game->NotifyAll(m_game->GetNotifyMovedPiece(m_from, m_to, m_game->m_players[m_game->m_activePlayer]->GetType()));

	m_game->CheckWiningPlayer();
	if (m_game->m_winner != EPlayerType::None)// stop if we have a winner
		return;

	if (m_board->GetBoardState() == EBoardState::Full_line)
	{
		m_game->m_state = EGameState::Removing;
		m_game->NotifyAll(m_game->GetNotifyGameStateChanged(m_game->m_state));
		LOG.info("Game state changed to Removing");

		if (m_game->m_players[m_game->m_activePlayer]->IsComputer() == false)
			return;
		m_game->LetComputerPlay();
		return;
	}
	else if (m_board->GetBoardState() == EBoardState::Windmill)
	{
		m_game->m_state = EGameState::Removing;
		m_game->NotifyAll(m_game->GetNotifyWindmill());
		LOG.info("Board state is now in windmill after moving.");

		if (m_game->m_players[m_game->m_activePlayer]->IsComputer() == false)
			return;
		m_game->LetComputerPlay();
		return;
	}

	m_game->NextPlayer();
}

void Move::Undo()
{
	if (m_board->GetNodeType(m_to) != m_game->m_players[m_game->m_activePlayer]->GetType())
		m_game->PrevPlayer();

	m_board->UndoMovePiece(m_from, m_to);
	m_game->NotifyAll(m_game->GetNotifyMovedPiece(m_to, m_from, m_game->m_players[m_game->m_activePlayer]->GetType()));

	m_game->m_state = EGameState::Moving;
	m_game->NotifyAll(m_game->GetNotifyGameStateChanged(m_game->m_state));
}

void Move::Print(std::ostream& os) const
{
	os << static_cast<int>(CommandType::Move) << ' ';
	os << m_from << m_to << ' ';
	os << '\n';
}

void Move::Read(std::istream& is)
{
	is >> m_from >> m_to;
}
