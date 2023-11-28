#include "Board.h"
#include "Piece.h"
#include "Pawn.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

Board::Board(QGraphicsScene* scene){
    this->scene = scene;
}

Board::~Board() {
    
}

void Board::drawBoard() {
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {

            //Squares
            QColor squareColor = ((row + col) % 2 == 0) ? QColor(238, 238, 210, 255) : QColor(118, 150, 86, 255);
            scene->addRect(col * squareSize, row * squareSize, squareSize, squareSize,
                QPen(Qt::transparent), QBrush(squareColor));

            if (col == 0) {
                //Font Text
                QGraphicsTextItem* textItem = new QGraphicsTextItem();
                QFont font("Times", 14, QFont::Bold);
                textItem->setFont(font);

                //Font Color
                QColor brushColor = ((row + col) % 2 == 1) ? QColor(238, 238, 210, 255) : QColor(118, 150, 86, 255);
                QBrush brush(brushColor);
                textItem->setDefaultTextColor(brush.color());

                textItem->setPlainText(QString("%1").arg(boardSize - row));
                qreal x = col * squareSize - 3;
                qreal y = row * squareSize - 5;
                textItem->setPos(x, y);
                scene->addItem(textItem);
            }

            if (row == boardSize - 1) {
                //Font Text
                QGraphicsTextItem* textItem = new QGraphicsTextItem();
                QFont font("Times", 14, QFont::Bold);
                textItem->setFont(font);

                //Font Color
                QColor brushColor = ((row + col) % 2 == 1) ? QColor(238, 238, 210, 255) : QColor(118, 150, 86, 255);
                QBrush brush(brushColor);
                textItem->setDefaultTextColor(brush.color());

                textItem->setPlainText(QString("%1").arg(QChar('a' + col)));
                qreal x = (col + 1) * squareSize - textItem->boundingRect().width() + 3;  
                qreal y = (row + 1) * squareSize - textItem->boundingRect().height() + 3;  
                textItem->setPos(x, y);
                scene->addItem(textItem);
            }
            
        }
    }
}

void Board::resetDefaultBoard() {
    clearBoard();
    placeWhite();
    placeBlack();
}

void Board::placeWhite() {
    Piece* piece;
    for (int i = 0; i < boardSize; i++) {
        piece = new Pawn("white");
        scene->addItem(piece);
        piece->setOriginalPosition(QPointF(i * squareSize, 6 * squareSize));
    }
    piece->setScene(scene);
    piece = new Rook("white");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(0, 7 * squareSize));

    piece = new Knight("white");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(squareSize, 7 * squareSize));

    piece = new Bishop("white");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(2 * squareSize, 7 * squareSize));

    piece = new Queen("white");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(3 * squareSize, 7 * squareSize));

    piece = new King("white");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(4 * squareSize, 7 * squareSize));

    piece = new Bishop("white");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(5 * squareSize, 7 * squareSize));

    piece = new Knight("white");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(6 * squareSize, 7 * squareSize));

    piece = new Rook("white");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(7 * squareSize, 7 * squareSize));
}

void Board::placeBlack() {
    Piece* piece;
    for (int i = 0; i < boardSize; i++) {
        piece = new Pawn("black");
        scene->addItem(piece);
        piece->setOriginalPosition(QPointF(i * squareSize, squareSize));
    }
    piece = new Rook("black");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(0, 0));

    piece = new Knight("black");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(squareSize, 0));

    piece = new Bishop("black");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(2 * squareSize, 0));

    piece = new Queen("black");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(3 * squareSize, 0));

    piece = new King("black");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(4 * squareSize, 0));

    piece = new Bishop("black");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(5 * squareSize, 0));

    piece = new Knight("black");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(6 * squareSize, 0));

    piece = new Rook("black");
    scene->addItem(piece);
    piece->setOriginalPosition(QPointF(7 * squareSize, 0));
}

void Board::displayBoardState(QString string) {
    clearBoard();
    Piece* piece{};
    piece->setScene(scene);
    int row = 0, col = 0;
    for (QChar ch : string) {
        if (ch == '/') {
            row++;
            col = 0;
        }
        else if (ch.isDigit()) {
            col += ch.digitValue();
        }
        else {
            switch (ch.unicode()) {
            case 'p':
                piece = new Pawn("black");
                break;
            case 'P':
                piece = new Pawn("white");
                break;
            case 'r':
                piece = new Rook("black");
                break;
            case 'R':
                piece = new Rook("white");
                break;
            case 'n':
                piece = new Knight("black");
                break;
            case 'N':
                piece = new Knight("white");
                break;
            case 'b':
                piece = new Bishop("black");
                break;
            case 'B':
                piece = new Bishop("white");
                break;
            case 'q':
                piece = new Queen("black");
                break;
            case 'Q':
                piece = new Queen("white");
                break;
            case 'k':
                piece = new King("black");
                break;
            case 'K':
                piece = new King("white");
                break;
            }
            scene->addItem(piece);
            piece->setOriginalPosition(QPointF(col * squareSize, row * squareSize));
            col++;
        }
    }
}

void Board::clearBoard() {
    for (QGraphicsItem* item : scene->items()) {
        if (dynamic_cast<Piece*>(item)) {
            dynamic_cast<Piece*>(item)->resetPieceMap();
            scene->removeItem(item);
            delete item;
        }
    }
    
}



