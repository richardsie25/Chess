#pragma once
#include "Piece.h"
class Queen : public Piece
{
public:
	Queen(QString color, QGraphicsItem* parent = nullptr);
	QString imagePath(QString color);
	bool isValidMove(int destCol, int destRow);
	bool wasPawn;
};

