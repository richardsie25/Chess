#include "Pawn.h"

Pawn::Pawn(QString color, QGraphicsItem* parent) : Piece(color, QPixmap(imagePath(color)).scaled(squareSize, squareSize), parent) {

}

QString Pawn::imagePath(QString color) {
    if (color == "white")
        return "whitePawn.png";
    else
        return "blackPawn.png";
}

bool Pawn::isValidMove(int destCol, int destRow) {
    int currentCol = lastPosition.x() / squareSize;
    int currentRow = lastPosition.y() / squareSize;

    if (color == "white") {
        if (qAbs(destCol - currentCol) == 1 && destRow == currentRow - 1) {
            return true;
        }
        if (currentRow == 6) {
            return (destCol == currentCol && destRow == currentRow - 1) || (destCol == currentCol && destRow == currentRow - 2);
        }
        else {
            return destCol == currentCol && destRow == currentRow - 1;
        }
    }

    else {
        if (qAbs(destCol - currentCol) == 1 && destRow == currentRow + 1) {
            return true;
        }
        if (currentRow == 1) {
            return (destCol == currentCol && destRow == currentRow + 1) || (destCol == currentCol && destRow == currentRow + 2);
        }
        else {
            return destCol == currentCol && destRow == currentRow + 1;
        }
    }
}