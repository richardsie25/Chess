#pragma once
#include <QGraphicsView>
#include <QGuiApplication>
#include <QScreen>
#include "ui_Game.h"

const int boardSize = 8;
const int squareSize = 100;

class Game : public QGraphicsView
{
    Q_OBJECT

public:
    Game(QWidget* parent = nullptr);
    ~Game();

private:
    Ui::GameClass ui;
};
