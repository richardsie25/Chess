#include "Piece.h"
#include "King.h"

Piece* Piece::pieceMap[boardSize][boardSize];
QGraphicsScene* Piece::scene;
QGraphicsRectItem* Piece::currentHighlight = nullptr;
QGraphicsRectItem* Piece::destHighlight = nullptr;

Piece::Piece(QString color, const QPixmap& pixmap, QGraphicsItem* parent): QGraphicsPixmapItem(pixmap, parent) {
    this->color = color;
}

void Piece::setScene(QGraphicsScene* scene) {
    this->scene = scene;
}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        setFlag(QGraphicsItem::ItemIsMovable);
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}

void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        setFlag(QGraphicsItem::ItemIsMovable, false);

        QPointF releasePos = event->scenePos();
        int destCol = releasePos.x() / squareSize;
        int destRow = releasePos.y() / squareSize;

        bool flag = true;

        if (isValidMove(destCol, destRow) && isValidPos(releasePos.x(), releasePos.y()) && !isKinginCheck(destCol,destRow)) {

            if (isOccupied(destCol, destRow)) {
                if (isEnemy(destCol, destRow))
                    captures(destCol, destRow);
                else {
                    flag = false;
                    setPos(lastPosition);
                }
            }

            if (flag) {
                //Highlight squares
                highlightSquares(destCol, destRow);

                //Change PieceMap of each square
                int currentCol = lastPosition.x() / squareSize;
                int currentRow = lastPosition.y() / squareSize;
                pieceMap[destCol][destRow] = this;
                pieceMap[currentCol][currentRow] = nullptr;

                //Change Position of piece
                setPos(destCol * squareSize, destRow * squareSize);
                lastPosition = QPointF(destCol * squareSize, destRow * squareSize);
            }
        }
        else {
            //Place piece back to last position in case of invalid move
            setPos(lastPosition);
        }
    }
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void Piece::setOriginalPosition(const QPointF& pos) {
    lastPosition = pos;
    setPos(pos);
    int col = pos.x() / squareSize;
    int row = pos.y() / squareSize;
    pieceMap[col][row] = this;
}

bool Piece::isOccupied(int destCol, int destRow) {
    return !(pieceMap[destCol][destRow] == nullptr);
}

bool Piece::isEnemy(int destCol, int destRow){
    return isOccupied(destCol, destRow) && pieceMap[destCol][destRow]->color != color;
}

bool Piece::isValidPos(int destCol, int destRow) {
    return destCol >= 0 && destCol < boardSize * squareSize && destRow >= 0 && destRow < boardSize * squareSize;
}

bool Piece::isKinginCheck(int destCol, int destRow) {
    int currentCol = lastPosition.x() / squareSize;
    int currentRow = lastPosition.y() / squareSize;

    Piece* originalDestPiece = pieceMap[destCol][destRow];
    pieceMap[currentCol][currentRow] = nullptr;
    pieceMap[destCol][destRow] = this;
    setPos(destCol * squareSize, destRow * squareSize);

    for (int col = 0; col < boardSize; col++) {
        for (int row = 0; row < boardSize; row++) {
            Piece* piece = pieceMap[col][row];
            if (piece && piece->color == color && dynamic_cast<King*>(piece)) {
                int kingCol = col;
                int kingRow = row;

                for (int i = 0; i < boardSize; i++) {
                    for (int j = 0; j < boardSize; j++) {
                        Piece* opponentPiece = pieceMap[i][j];
                        if (opponentPiece && opponentPiece->color != color) {
                            if (opponentPiece->isValidMove(kingCol, kingRow)) {
                                pieceMap[currentCol][currentRow] = this;
                                pieceMap[destCol][destRow] = originalDestPiece;
                                setPos(lastPosition);
                                return true;
                            }
                        }
                    }
                }
                pieceMap[currentCol][currentRow] = this;
                pieceMap[destCol][destRow] = originalDestPiece;
                setPos(lastPosition);
                return false;
            }
        }
    }
    return false;
}

void Piece::captures(int destCol, int destRow) {
    for (QGraphicsItem* item : scene->items()) {
        if (dynamic_cast<Piece*>(item) && item->scenePos() == QPointF(destCol * squareSize, destRow * squareSize)) {
            scene->removeItem(item);
            delete item;
        }
    }

}

void Piece::highlightSquares(int destCol, int destRow) {
    scene->removeItem(currentHighlight);
    scene->removeItem(destHighlight);
    currentHighlight = scene->addRect(lastPosition.x(), lastPosition.y(), squareSize, squareSize,
        QPen(Qt::transparent), QBrush(QColor(255, 255, 0, 50)));
    destHighlight = scene->addRect(destCol * squareSize, destRow * squareSize, squareSize, squareSize,
        QPen(Qt::transparent), QBrush(QColor(255, 255, 0, 50)));
}

void Piece::resetPieceMap() {
    for (int col = 0; col < boardSize; col++) {
        for (int row = 0; row < boardSize; row++) {
            pieceMap[col][row] = nullptr;
        }
    }
}

bool Piece::isValidMove(int destCol, int destRow) {
    return true;
}

QString Piece::imagePath(QString color){
    return "";
}




