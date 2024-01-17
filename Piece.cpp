#include "Piece.h"
#include "King.h"
#include "Rook.h"   
#include <QDebug>
#include <QMessageBox>

Piece* Piece::pieceMap[boardSize][boardSize];
QGraphicsScene* Piece::scene;
QGraphicsRectItem* Piece::currentHighlight = nullptr;
QGraphicsRectItem* Piece::destHighlight = nullptr;
QString Piece::playerTurn = "white";
QString Piece::gameState = "";
QGraphicsTextItem* Piece::text = nullptr;

Piece::Piece(QString color, const QPixmap& pixmap, QGraphicsItem* parent): QGraphicsPixmapItem(pixmap, parent) {
    this->color = color;
}

void Piece::setScene(QGraphicsScene* scene) {
    this->scene = scene;

    text = new QGraphicsTextItem("White Turn!");
    QFont font("Times", squareSize / 2, QFont::Bold);
    text->setFont(font);
    text->setPos(squareSize * 2, -squareSize);

    QColor brushColor = QColor(118, 150, 86, 255);
    QBrush brush(brushColor);
    text->setDefaultTextColor(brush.color());
    scene->addItem(text);
}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton && gameState == "") {
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

        if (color == playerTurn && !isKinginCheck(destCol, destRow, color) && isValidMove(destCol, destRow) && isValidPos(releasePos.x(), releasePos.y())) {

            if (isOccupied(destCol, destRow)) {
                if (isEnemy(destCol, destRow))
                    captures(destCol, destRow);
                else {
                    flag = false;
                    setPos(lastPosition);
                }
            }

            if (flag) {
                //Flag for castling
                if (dynamic_cast<King*>(this))
                    dynamic_cast<King*>(this)->hasMoved = true;
                if (dynamic_cast<Rook*>(this))
                    dynamic_cast<Rook*>(this)->hasMoved = true;

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

                //Switch Player Turn for valid moves
                if (playerTurn == "white") {
                    playerTurn = "black";
                }
                else {
                    playerTurn = "white";
                }
            }
        }
        else {
            //Place piece back to last position in case of invalid move
            setPos(lastPosition);
        }
    }

    //Process Events accordingly
    processEvents();

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

QPointF Piece::getKingLocation(QString kingColor){
    int kingCol = -1;
    int kingRow = -1;
    for (int col = 0; col < boardSize; col++) {
        for (int row = 0; row < boardSize; row++) {
            Piece* piece = pieceMap[col][row];
            if (piece && piece->color == kingColor && dynamic_cast<King*>(piece)) {
                kingCol = col;
                kingRow = row;
                break;
            }
        }
    }
    return QPointF(kingCol * squareSize, kingRow * squareSize);
}

bool Piece::isKinginCheck(int destCol, int destRow, QString kingColor) {
    int currentCol = lastPosition.x() / squareSize;
    int currentRow = lastPosition.y() / squareSize;

    //Move piece temporarily
    Piece* originalDestPiece = pieceMap[destCol][destRow];
    pieceMap[currentCol][currentRow] = nullptr;
    pieceMap[destCol][destRow] = this;
    setPos(destCol * squareSize, destRow * squareSize);

    //Get king location
    QPointF kingLoc = getKingLocation(kingColor);
    int kingCol = kingLoc.x() / squareSize;
    int kingRow = kingLoc.y() / squareSize;

    //Check if king is checked
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            Piece* opponentPiece = pieceMap[i][j];
            if (opponentPiece && opponentPiece->color != kingColor) {
                if (opponentPiece->isValidMove(kingCol, kingRow)) {
                    //Revert pieces to original location
                    pieceMap[currentCol][currentRow] = this;
                    pieceMap[destCol][destRow] = originalDestPiece;
                    setPos(lastPosition);
                    return true;
                }
            }
        }
    }
    //Revert pieces to original location
    pieceMap[currentCol][currentRow] = this;
    pieceMap[destCol][destRow] = originalDestPiece;
    setPos(lastPosition);
    return false;
}

bool Piece::isStalemate(QString kingColor) {
    QPointF kingLoc = getKingLocation(kingColor);
    int kingCol = kingLoc.x() / squareSize;
    int kingRow = kingLoc.y() / squareSize;

    //Check if king is in check
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            Piece* opponentPiece = pieceMap[i][j];
            if (opponentPiece && opponentPiece->color != kingColor) {
                if (opponentPiece->isValidMove(kingCol, kingRow)) {
                    return false;
                }
            }
        }
    }

    //Check for any valid moves
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            Piece* teamPiece = pieceMap[i][j];
            if (teamPiece && teamPiece->color == kingColor) {
                for (int k = 0; k < boardSize; k++) {
                    for (int l = 0; l < boardSize; l++) {
                        if (teamPiece->isValidMove(k, l) && (pieceMap[k][l] == nullptr || pieceMap[k][l]->color != kingColor)) {
                            if (!teamPiece->isKinginCheck(k, l, kingColor))
                                return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool Piece::isCheckmate(QString kingColor) {
    QPointF kingLoc = getKingLocation(kingColor);
    int kingCol = kingLoc.x() / squareSize;
    int kingRow = kingLoc.y() / squareSize;

    //Check for any valid moves
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            Piece* teamPiece = pieceMap[i][j];
            if (teamPiece && teamPiece->color == kingColor) {
                for (int k = 0; k < boardSize; k++) {
                    for (int l = 0; l < boardSize; l++) {
                        if (teamPiece->isValidMove(k, l) && (pieceMap[k][l] == nullptr || pieceMap[k][l]->color != kingColor)) {
                            if (!teamPiece->isKinginCheck(k, l, kingColor))
                                return false;
                        }
                    }
                }
            }
        }
    }

    //Check if king is in check
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            Piece* opponentPiece = pieceMap[i][j];
            if (opponentPiece && opponentPiece->color != kingColor) {
                if (opponentPiece->isValidMove(kingCol, kingRow)) {
                    return true;
                }
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

void Piece::processEvents() {
    if (playerTurn == "black") {
        if (isCheckmate("black")) {
            gameState = "White Wins!";
        }
        if (isStalemate("black")) {
            gameState = "Draw!";
        }
    }
    else if (playerTurn == "white") {
        if (isCheckmate("white")) {
            gameState = "Black Wins!";
        }
        if (isStalemate("white")) {
            gameState = "Draw!";
        }
    }
    scene->removeItem(text);
    QString message;
    if (playerTurn == "white")
        message = "White Turn!";
    if (playerTurn == "black")
        message = "Black Turn!";
    if (gameState != "")
        message = gameState;
    text = new QGraphicsTextItem(message);
    QFont font("Times", squareSize / 2, QFont::Bold);
    text->setFont(font);
    text->setPos(squareSize * 2, -squareSize);

    QColor brushColor = QColor(118, 150, 86, 255);
    QBrush brush(brushColor);
    text->setDefaultTextColor(brush.color());
    scene->addItem(text);

    if (!gameState.isEmpty()) {
        int reply = QMessageBox::question(nullptr, "Game Over!", gameState , QMessageBox::Ok);
    }
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




