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

    //Valid move for checks
    Piece* originalDestPiece = pieceMap[destCol][destRow];
    pieceMap[destCol][destRow] = this;
    pieceMap[currentCol][currentRow] = nullptr;
    setPos(destCol * squareSize, destRow * squareSize);
    bool kingInCheck = isKinginCheck();
    pieceMap[currentCol][currentRow] = this;
    pieceMap[destCol][destRow] = originalDestPiece;
    setPos(lastPosition);
    if (kingInCheck)
        return false;

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
