#include "Queen.h"
Queen::Queen(QString color, QGraphicsItem* parent) : Piece(color, QPixmap(imagePath(color)).scaled(squareSize, squareSize), parent) {

}

QString Queen::imagePath(QString color){
    if (color == "white")
        return "whiteQueen.png";
    else
        return "blackQueen.png";
}

bool Queen::isValidMove(int destCol, int destRow) {
    int currentCol = lastPosition.x() / squareSize;
    int currentRow = lastPosition.y() / squareSize;

    //Horizonal Movement Collision Check
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

    //Vertical Movement Collision Check
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
    
    //Diagonal Movement Collision Check
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
