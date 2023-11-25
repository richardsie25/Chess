#include "Pawn.h"
#include "Queen.h"
#include <QTimer>

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
    bool validMove = false;

    if (color == "white") {
        //Captures
        if (qAbs(destCol - currentCol) == 1 && destRow == currentRow - 1) {
            validMove = isEnemy(destCol, destRow);
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
        //Normal Pawn Move
        else {
            if (destCol == currentCol && destRow == currentRow - 1) {
                validMove = !isOccupied(destCol, destRow);
            }
        }

        //Promotion
        if (destRow == 0 && validMove) {
            pieceMap[currentCol][currentRow] = nullptr;
            captures(destCol, destRow);
            Queen* promotedQueen = new Queen(color);
            promotedQueen->setOriginalPosition(QPointF(destCol * squareSize, destRow * squareSize));
            scene->addItem(promotedQueen);
            QTimer::singleShot(0, [this]() {
                scene->removeItem(this);
            });
        }

        return validMove;
    }
    else {
        //Captures
        if (qAbs(destCol - currentCol) == 1 && destRow == currentRow + 1) {
            validMove = isEnemy(destCol, destRow);
        }

        //First Move
        if (currentRow == 1) {
            if (destCol == currentCol && destRow == currentRow + 1) {
                return !isOccupied(destCol, destRow);
            }
            else if (destCol == currentCol && destRow == currentRow + 2) {
                return !isOccupied(destCol, destRow) && !isOccupied(destCol, destRow - 1);
            }
        }

        //Normal Pawn Move
        else {
            if (destCol == currentCol && destRow == currentRow + 1) {
                validMove = !isOccupied(destCol, destRow);
            }
        }

        //Promotion
        if (destRow == 7 && validMove) {
            scene->removeItem(this);
            pieceMap[currentCol][currentRow] = nullptr;
            captures(destCol, destRow);
            Queen* promotedQueen = new Queen(color);
            promotedQueen->setOriginalPosition(QPointF(destCol * squareSize, destRow * squareSize));
            scene->addItem(promotedQueen);
            QTimer::singleShot(0, [this]() {
                scene->removeItem(this);
            });
        }

        return validMove;
    }
}