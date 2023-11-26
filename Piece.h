#pragma once
#include "Game.h"

class Piece : public QGraphicsPixmapItem
{
public:
    Piece(QString color, const QPixmap& pixmap, QGraphicsItem* parent = nullptr);
    void setScene(QGraphicsScene* scene);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void setOriginalPosition(const QPointF& pos);
    bool isOccupied(int destCol, int destRow);
    bool isEnemy(int destCol, int destRow);
    bool isValidPos(int destCol, int destRow);
    bool isKinginCheck(int destCol, int destRow);
    bool isStalemate();
    bool isCheckmate();
    void captures(int destCol, int destRow);
    void highlightSquares(int destCol, int destRow);
    void resetPieceMap();
    virtual bool isValidMove(int destCol, int destRow);
    virtual QString imagePath(QString color);

protected:
    QPointF lastPosition;
    QString color;
    static Piece* pieceMap[boardSize][boardSize];
    static QGraphicsScene* scene;
    static QGraphicsRectItem* currentHighlight;
    static QGraphicsRectItem* destHighlight;
};

