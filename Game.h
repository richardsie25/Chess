#pragma once
#include <QGraphicsView>
#include <QGuiApplication>
#include <QScreen>
#include <QTcpSocket>
#define PORT 8000
#include "ui_Game.h"

const int boardSize = 8;
const int squareSize = 70;

class Game : public QGraphicsView
{
    Q_OBJECT

public:
    Game(QWidget* parent = nullptr);
    ~Game();

private:
    QTcpSocket* socket;
    Ui::GameClass ui;

private slots:
    void readData();
    void handleSocketError(QAbstractSocket::SocketError socketError);
};
