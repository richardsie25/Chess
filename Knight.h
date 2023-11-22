#pragma once
#include "Piece.h"
class Knight : public Piece
{
public:
	Knight(QString color, QGraphicsItem* parent = nullptr);
	QString imagePath(QString color);
	bool isValidMove(int destCol, int destRow);
};

