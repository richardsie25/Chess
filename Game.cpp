#include "Game.h"
#include "Board.h"
Board* board;

Game::Game(QWidget *parent) : QGraphicsView(parent) {
    setFixedSize(squareSize * 16, squareSize * 10);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet("background-color: rgb(48, 46, 43);");

    QGraphicsScene* scene = new QGraphicsScene();
    setScene(scene);
    scene->setSceneRect(0, 0, boardSize * squareSize, boardSize * squareSize);

    board = new Board(scene);
    board->drawBoard();
    //board->resetDefaultBoard();
}

Game::~Game() {
    
}

void Game::keyPressEvent(QKeyEvent* event) {
    
    char boardState[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    switch (event->key()) {
    case Qt::Key_Left:
        board->displayBoard(boardState);
        break;
    case Qt::Key_Right:
        boardState[1][0] = ' ';
        boardState[3][0] = 'p';
        board->displayBoard(boardState);
        break;
    case Qt::Key_Up:
        break;
    case Qt::Key_Down:
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}
