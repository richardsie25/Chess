#include "King.h"
#include "Rook.h"

King::King(QString color, QGraphicsItem* parent) : Piece(color, QPixmap(imagePath(color)).scaled(squareSize, squareSize), parent) {
    hasMoved = false;
}

QString King::imagePath(QString color) {
    if (color == "white")
        return ":/Game/whiteKing.png";
    else
        return ":/Game/blackKing.png";
}

bool King::isValidMove(int destCol, int destRow) {
    int currentCol = lastPosition.x() / squareSize;
    int currentRow = lastPosition.y() / squareSize;
    
    if (qAbs(destCol - currentCol) <= 1 && qAbs(destRow - currentRow) <= 1 && qAbs(destCol - currentCol) + qAbs(destRow - currentRow) >= 1) {
        return true;
    }

    if (!hasMoved && destRow == currentRow && qAbs(destCol - currentCol) == 2) {
        return true;
    }

    return false;
}