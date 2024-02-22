#include <QGraphicsScene>
#include <QTimer>
#include <QMessageBox>
#include <QGridLayout>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "Piece.h"

class Board : public QObject
{
    Q_OBJECT;

public:
    Board(QGraphicsScene* scene);
    ~Board();
    bool isValidPos(int destCol, int destRow);
    bool collisionCheck(int destCol, int destRow, Piece* piece);
    bool isTeam(int destCol, int destRow, Piece* piece);
    bool isEnemy(int destCol, int destRow, Piece* piece);
    void captures(int destCol, int destRow);
    void copyPieceMap();
    QPointF getKingLocation(QString kingColor);
    bool isKinginCheck(int destCol, int destRow, Piece* piece);
    bool isStalemate(QString kingColor);
    bool isCheckmate(QString kingColor);
    void handlePromotions(int destCol, int destRow, Piece *piece);
    void addPiecePromotion(int destCol, int destRow, Piece* piece, QString type);
    void processEvents();
    void highlightSquares();
    int materialCounter();
    void displayDeadMaterial();
    void drawBoard();
    void resetDefaultBoard();
    void placeWhite();
    void placeBlack();
    void clearBoard();
    void displayBoard(std::string fen);

public slots:
    bool onPiecePressed(Piece* piece);
    void onPieceReleased(Piece* piece, QPointF releasePosition);

private:
    QGraphicsScene* scene;
    Piece* pieceMap[boardSize][boardSize];
    Piece* previousPieceMap[boardSize][boardSize];
    QList<QPair<int, QGraphicsPixmapItem*>> whiteDeadPieces;
    QList<QPair<int, QGraphicsPixmapItem*>> blackDeadPieces;
    QGraphicsRectItem* currentHighlight = nullptr;
    QGraphicsRectItem* destHighlight = nullptr;
    QString playerTurn = "white";
    QString gameState = "";
    QGraphicsTextItem* turn = nullptr;
    QGraphicsTextItem* materialScore = nullptr;
};