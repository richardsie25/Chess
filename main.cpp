#include <QApplication>
#include <QFile>
#include <QTimer>
#include "Game.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Game game;
    game.show();
    return app.exec();
}

/* Things to Implement
* En passant
* Draw conditions
* Flip Board
* Timer
*/