#pragma once

#include <QImage>

struct ImagesCache
{
public:
	ImagesCache()
		: m_blackPiece(QImage(QString::fromStdString("../MoaraClient/black.png")))
		, m_whitePiece(QImage(QString::fromStdString("../MoaraClient/white.png")))
		, m_redPiece(QImage(QString::fromStdString("../MoaraClient/red.png")))
	{
	}

	const QImage& GetImage(EPieceType type)
	{
		switch (type)
		{
		case EPieceType::Black:
			return m_blackPiece;
		case EPieceType::White:
			return m_whitePiece;
		case EPieceType::Red:
			return m_redPiece;
		case EPieceType::None:
			break;
		default:
			break;
		}

		return m_whitePiece;
	}

private:
	QImage m_blackPiece;
	QImage m_whitePiece;
	QImage m_redPiece;
};