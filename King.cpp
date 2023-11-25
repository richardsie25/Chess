#include "King.h"
King::King(QString color, QGraphicsItem* parent) : Piece(color, QPixmap(imagePath(color)).scaled(squareSize, squareSize), parent) {

}

QString King::imagePath(QString color) {
    if (color == "white")
        return "whiteKing.png";
    else
        return "blackKing.png";
}

bool King::isValidMove(int destCol, int destRow) {
    int currentCol = lastPosition.x() / squareSize;
    int currentRow = lastPosition.y() / squareSize;
    
    return qAbs(destCol - currentCol) <= 1 && qAbs(destRow - currentRow) <= 1;
}