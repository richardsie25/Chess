#include "Knight.h"
Knight::Knight(QString color, QGraphicsItem* parent) : Piece(color, QPixmap(imagePath(color)).scaled(squareSize, squareSize), parent) {

}

QString Knight::imagePath(QString color) {
    if (color == "white")
        return "whiteKnight.png";
    else
        return "blackKnight.png";
}

bool Knight::isValidMove(int destCol, int destRow) {
    int currentCol = lastPosition.x() / squareSize;
    int currentRow = lastPosition.y() / squareSize;
    int colDiff = qAbs(destCol - currentCol);
    int rowDiff = qAbs(destRow - currentRow);
    return (colDiff == 2 && rowDiff == 1) || (colDiff == 1 && rowDiff == 2);
}