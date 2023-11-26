#pragma once
#include "Piece.h"
class Rook : public Piece
{
public:
	Rook(QString color, QGraphicsItem* parent = nullptr);
	QString imagePath(QString color);
	bool isValidMove(int destCol, int destRow);
	bool hasMoved;
};

