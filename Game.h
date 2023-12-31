#pragma once
#include <QtWidgets/QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include "ui_Game.h"

const int boardSize = 8;
const int squareSize = 100;

class Game : public QGraphicsView
{
    Q_OBJECT

public:
    Game(QWidget* parent = nullptr);
    ~Game();
    void displayBoardState(QString fen);

private:
    Ui::GameClass ui;
    QGraphicsScene* scene;
};
