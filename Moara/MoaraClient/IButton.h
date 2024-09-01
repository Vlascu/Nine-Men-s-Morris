#pragma once
#include "Position.h"
#include <QPushButton>
#include "EPieceType.h"
#include <QPainter>
#include <QWidget>

class IButton : QPushButton
{
public:
	virtual void SetNodeIndex(uint8_t id) = 0;
	virtual uint8_t GetNodeIndex() const = 0;
	virtual void SetType(EPieceType type) = 0;
	virtual EPieceType GetType() const = 0;
	virtual void SetPosition(Position pos) = 0;
	virtual Position GetPosition() const = 0;
	virtual void paintEvent(QPaintEvent*) = 0;
	virtual void mousePressEvent(QMouseEvent*) = 0;
};