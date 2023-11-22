#include <QGraphicsScene>
#include "Piece.h"

class Board 
{
public:
    Board(QGraphicsScene* scene);
    ~Board();
    void drawBoard();
    void resetDefaultBoard();
    void placeWhite();
    void placeBlack();
    void displayBoardState(QString string);
    void clearBoard();

private:
    QGraphicsScene* scene;
};