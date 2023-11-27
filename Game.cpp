#include "Game.h"
#include "Board.h"

Board* board;
Game::Game(QWidget *parent) : QGraphicsView(parent) {
    setFixedSize(1400, 802);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet("background-color: rgb(48, 46, 43);");

    scene = new QGraphicsScene();
    setScene(scene);
    scene->setSceneRect(0, 0, boardSize * squareSize, boardSize * squareSize);

    board = new Board(scene);
    board->drawBoard();
    board->resetDefaultBoard();
    board->displayBoardState("8/8/8/8/8/1k6/1r6/K7");
}

Game::~Game() {
    
}

void Game::displayBoardState(QString fen) {
    board->displayBoardState(fen);
}
