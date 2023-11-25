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
    
    return qAbs(destCol - currentCol) <= 1 && qAbs(destRow - currentRow) <= 1;
}