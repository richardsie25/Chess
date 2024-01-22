#pragma once
#include "Piece.h"
class Bishop : public Piece
{
public:
	Bishop(QString color, QGraphicsItem* parent = nullptr);
	QString imagePath(QString color);
	bool isValidMove(int destCol, int destRow);
	bool wasPawn;
};

