#include "Bishop.h"
Bishop::Bishop(QString color, QGraphicsItem* parent) : Piece(color, QPixmap(imagePath(color)).scaled(squareSize, squareSize), parent) {
    wasPawn = false;
}

QString Bishop::imagePath(QString color) {
    if (color == "white")
        return "whiteBishop.png";
    else
        return "blackBishop.png";
}

bool Bishop::isValidMove(int destCol, int destRow) {
    int currentCol = lastPosition.x() / squareSize;
    int currentRow = lastPosition.y() / squareSize;
    return qAbs(destCol - currentCol) == qAbs(destRow - currentRow);
}
