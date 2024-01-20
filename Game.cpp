#include "Game.h"
#include "Board.h"

Board* board;
Game::Game(QWidget *parent) : QGraphicsView(parent) {
    setFixedSize(squareSize * 14, squareSize * 9);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet("background-color: rgb(48, 46, 43);");

    scene = new QGraphicsScene();
    setScene(scene);
    scene->setSceneRect(0, -squareSize/2, boardSize * squareSize, boardSize * squareSize);

    board = new Board(scene);
    board->drawBoard();
    board->resetDefaultBoard();
}

Game::~Game() {
    
}
