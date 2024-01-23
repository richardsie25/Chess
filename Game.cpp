#include "Game.h"
#include "Board.h"

Game::Game(QWidget *parent) : QGraphicsView(parent) {
    setFixedSize(squareSize * 16, squareSize * 10);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet("background-color: rgb(48, 46, 43);");

    QGraphicsScene* scene = new QGraphicsScene();
    setScene(scene);
    scene->setSceneRect(0, 0, boardSize * squareSize, boardSize * squareSize);

    Board* board = new Board(scene);
    board->drawBoard();
    board->resetDefaultBoard();
}

Game::~Game() {
    
}
