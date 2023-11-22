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
        //Captures
        if (qAbs(destCol - currentCol) == 1 && destRow == currentRow - 1) {
            return isEnemy(destCol, destRow);
        }
        //First move
        if (currentRow == 6) {
            if (destCol == currentCol && destRow == currentRow - 1) {
                return !isOccupied(destCol, destRow);
            }
            else if (destCol == currentCol && destRow == currentRow - 2) {
                return !isOccupied(destCol, destRow) && !isOccupied(destCol, destRow + 1);
            }
        }
        //Normal pawn move
        else {
            if (destCol == currentCol && destRow == currentRow - 1) {
                return !isOccupied(destCol, destRow);
            }
        }
        return false;
    }
    else {
        if (qAbs(destCol - currentCol) == 1 && destRow == currentRow + 1) {
            return isEnemy(destCol, destRow);
        }
        if (currentRow == 1) {
            if (destCol == currentCol && destRow == currentRow + 1) {
                return !isOccupied(destCol, destRow);
            }
            else if (destCol == currentCol && destRow == currentRow + 2) {
                return !isOccupied(destCol, destRow) && !isOccupied(destCol, destRow - 1);
            }
        }
        else {
            if (destCol == currentCol && destRow == currentRow + 1) {
                return !isOccupied(destCol, destRow);
            }
        }
        return false;
    }
}