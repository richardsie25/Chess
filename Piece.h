#pragma once
#include "Game.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

class Piece : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    Piece(QString color, const QPixmap& pixmap, QGraphicsItem* parent = nullptr);
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void setPosition(const QPointF& pos);
    QPointF getPosition();
    QString getColor();
    virtual bool isValidMove(int destCol, int destRow);
    virtual QString imagePath(QString color);

signals:
    bool piecePressed(Piece* piece);
    void pieceReleased(Piece* piece, QPointF releasePosition);

protected:
    QPointF lastPosition;
    QString color;
};

