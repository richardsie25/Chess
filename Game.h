#pragma once
#include <QGraphicsView>
#include <QGuiApplication>
#include <QScreen>
#include <QKeyEvent>
#include "ui_Game.h"

const int boardSize = 8;
const int squareSize = 70;

class Game : public QGraphicsView
{
    Q_OBJECT

public:
    Game(QWidget* parent = nullptr);
    ~Game();
    void keyPressEvent(QKeyEvent* event);

private:
    Ui::GameClass ui;
};
