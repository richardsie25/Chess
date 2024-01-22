#include "Game.h"
#include "Board.h"

Board* board;
Game::Game(QWidget *parent) : QGraphicsView(parent) {
    setFixedSize(squareSize * 16, squareSize * 10);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet("background-color: rgb(48, 46, 43);");

    scene = new QGraphicsScene();
    setScene(scene);
    scene->setSceneRect(0, 0, boardSize * squareSize, boardSize * squareSize);

    board = new Board(scene);
    board->drawBoard();
    board->resetDefaultBoard();
}

Game::~Game() {
    
}
