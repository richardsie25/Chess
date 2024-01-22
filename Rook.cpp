#include "Rook.h"
Rook::Rook(QString color, QGraphicsItem* parent) : Piece(color, QPixmap(imagePath(color)).scaled(squareSize, squareSize), parent) {
    hasMoved = false;
    wasPawn = false;
}

QString Rook::imagePath(QString color) {
    if (color == "white")
        return "whiteRook.png";
    else
        return "blackRook.png";
}

bool Rook::isValidMove(int destCol, int destRow) {
    int currentCol = lastPosition.x() / squareSize;
    int currentRow = lastPosition.y() / squareSize;
    return (destCol != currentCol && destRow == currentRow) || (destCol == currentCol && destRow != currentRow);
}

