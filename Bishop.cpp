#include "Bishop.h"
Bishop::Bishop(QString color, QGraphicsItem* parent) : Piece(color, QPixmap(imagePath(color)).scaled(squareSize, squareSize), parent) {

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

    if (qAbs(destCol - currentCol) == qAbs(destRow - currentRow)) {
        int colStep = (destCol > currentCol) ? 1 : -1;
        int rowStep = (destRow > currentRow) ? 1 : -1;

        for (int col = currentCol + colStep, row = currentRow + rowStep; col != destCol; col += colStep, row += rowStep) {
            if (isOccupied(col, row)) {
                return false;
            }
        }
        return true;
    }
    return false;
}
