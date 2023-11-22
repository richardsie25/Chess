#pragma once
#include "Piece.h"
class Pawn : public Piece
{
public:
	Pawn(QString color, QGraphicsItem* parent = nullptr);
	QString imagePath(QString color);
	bool isValidMove(int destCol, int destRow);
};

