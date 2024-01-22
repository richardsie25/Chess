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
* Count material
* En passant
* Pawn Promotion choose pieces
* Draw conditions
* Flip Board
* Timer
* 
* QTimer timer;
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
    timer.start(1000);
*/