#include <QApplication>
#include <QFile>
#include <QTimer>
#include "Game.h"

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);
    Game game;
    game.show();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        QFile file("fen.txt");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                timer.start(2000);
                QString fenNotation = in.readLine();
                game.displayBoardState(fenNotation);
                QCoreApplication::processEvents();
            }
                file.close();
        }
        });

    // Start the timer for the first time, update every 5 seconds
    timer.start(2000);

    return app.exec();
}

/* Things to Implement
* Check for checks (forcing king to move)
* Pinning of pieces
* Checkmate/Stalemate
* Pawn Promotion
* King and Queen side castling
* Changing turns
* Display dead piece/ count material
* En passant
*/