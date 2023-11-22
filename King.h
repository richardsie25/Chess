#pragma once
#include "Piece.h"
class King : public Piece
{
public:
	King(QString color, QGraphicsItem* parent = nullptr);
	QString imagePath(QString color);
	bool isValidMove(int destCol, int destRow);
};

