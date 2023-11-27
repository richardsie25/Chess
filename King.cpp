#include "King.h"
#include "Rook.h"

King::King(QString color, QGraphicsItem* parent) : Piece(color, QPixmap(imagePath(color)).scaled(squareSize, squareSize), parent) {
    hasMoved = false;
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
    
    if (qAbs(destCol - currentCol) <= 1 && qAbs(destRow - currentRow) <= 1 && qAbs(destCol - currentCol) + qAbs(destRow - currentRow) >= 1) {
        return true;
    }

    //King Side Castling
    if (!hasMoved && destRow == currentRow && destCol - currentCol == 2) {
        if (isOccupied(currentCol + 1, currentRow) || isOccupied(currentCol + 2, currentRow))
            return false;
        if (isKinginCheck(currentCol, currentRow, color) || isKinginCheck(currentCol + 1, currentRow, color) || isKinginCheck(currentCol + 2, currentRow, color))
            return false;
        Piece* piece = pieceMap[destCol + 1][destRow];
        if (piece && dynamic_cast<Rook*>(piece) && dynamic_cast<Rook*>(piece)->hasMoved == false) {
            pieceMap[destCol - 1][destRow] = pieceMap[destCol + 1][destRow];
            pieceMap[destCol + 1][destRow] = nullptr;
            piece->setOriginalPosition(QPointF((destCol - 1) * squareSize, destRow * squareSize));
            return true;
        }
    }

    //Queen Side Castling
    if (!hasMoved && destRow == currentRow && destCol - currentCol == -2) {
        if (isOccupied(currentCol - 1, currentRow) || isOccupied(currentCol - 2, currentRow) || isOccupied(currentCol - 3, currentRow))
            return false;
        if (isKinginCheck(currentCol, currentRow, color) || isKinginCheck(currentCol - 1, currentRow, color) || isKinginCheck(currentCol - 2, currentRow, color))
            return false;
        Piece* piece = pieceMap[destCol - 2][destRow];
        if (piece && dynamic_cast<Rook*>(piece) && dynamic_cast<Rook*>(piece)->hasMoved == false) {
            pieceMap[destCol + 1][destRow] = pieceMap[destCol - 2][destRow];
            pieceMap[destCol - 2][destRow] = nullptr;
            piece->setOriginalPosition(QPointF((destCol + 1) * squareSize, destRow * squareSize));
            return true;
        }
    }

    return false;
}