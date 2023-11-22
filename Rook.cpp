#include "Rook.h"
Rook::Rook(QString color, QGraphicsItem* parent) : Piece(color, QPixmap(imagePath(color)).scaled(squareSize, squareSize), parent) {

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

    if (destCol != currentCol && destRow == currentRow) {
        int startCol = std::min(currentCol, destCol);
        int endCol = std::max(currentCol, destCol) + 1;
        for (int col = startCol; col < endCol; col++) {
            if (col != currentCol && col != destCol && isOccupied(col, currentRow)) {
                return false;
            }
        }
        return true; 
    }

    if (destCol == currentCol && destRow != currentRow) {
        int startRow = std::min(currentRow, destRow);
        int endRow = std::max(currentRow, destRow) + 1;
        for (int row = startRow; row < endRow; row++) {
            if (row != currentRow && row != destRow && isOccupied(currentCol, row)) {
                return false;
            }
        }
        return true;
    }
    return false;
}

