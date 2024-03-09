#include "Queen.h"
Queen::Queen(QString color, QGraphicsItem* parent) : Piece(color, QPixmap(imagePath(color)).scaled(squareSize, squareSize), parent) {
    wasPawn = false;
}

QString Queen::imagePath(QString color){
    if (color == "white")
        return ":/Game/whiteQueen.png";
    else
        return ":/Game/blackQueen.png";
}

bool Queen::isValidMove(int destCol, int destRow) {
    int currentCol = lastPosition.x() / squareSize;
    int currentRow = lastPosition.y() / squareSize;
    return (destCol != currentCol && destRow == currentRow) || (destCol == currentCol && destRow != currentRow) || (qAbs(destCol - currentCol) == qAbs(destRow - currentRow));
}
