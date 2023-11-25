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

    int colDiff = qAbs(destCol - currentCol);
    int rowDiff = qAbs(destRow - currentRow);
    return (colDiff == 2 && rowDiff == 1) || (colDiff == 1 && rowDiff == 2);
}