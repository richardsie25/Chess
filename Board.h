#include <QGraphicsScene>
#include <QTimer>
#include <QMessageBox>
#include "Piece.h"

class Board : public QObject
{
    Q_OBJECT;

public:
    Board(QGraphicsScene* scene);
    ~Board();
    bool isValidPos(int destCol, int destRow);
    bool collisionCheck(int destCol, int destRow, Piece* piece);
    bool isEnemy(int destCol, int destRow, Piece* piece);
    void captures(int destCol, int destRow);
    QPointF getKingLocation(QString kingColor);
    bool isKinginCheck(int destCol, int destRow, Piece* piece);
    bool isStalemate(QString kingColor);
    bool isCheckmate(QString kingColor);
    void processEvents();
    void highlightSquares(int destCol, int destRow, Piece* piece);
    void drawBoard();
    void resetDefaultBoard();
    void placeWhite();
    void placeBlack();
    void clearBoard();

public slots:
    bool onPiecePressed(Piece* piece);
    void onPieceReleased(Piece* piece, QPointF releasePosition);

private:
    QGraphicsScene* scene;
    Piece* pieceMap[boardSize][boardSize];
    QGraphicsRectItem* currentHighlight = nullptr;
    QGraphicsRectItem* destHighlight = nullptr;
    QString playerTurn = "white";
    QString gameState = "";
    QGraphicsTextItem* text = nullptr;
};