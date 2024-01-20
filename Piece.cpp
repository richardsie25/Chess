#include "Piece.h"

Piece::Piece(QString color, const QPixmap& pixmap, QGraphicsItem* parent): QObject(), QGraphicsPixmapItem(pixmap, parent) {
    this->color = color;
}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        if(emit piecePressed(this))
            setFlag(QGraphicsItem::ItemIsMovable);
    }
    QGraphicsPixmapItem::mousePressEvent(event);
}

void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit pieceReleased(this, event->scenePos());
        setFlag(QGraphicsItem::ItemIsMovable, false);
    }
    QGraphicsPixmapItem::mouseReleaseEvent(event);
}

void Piece::setPosition(const QPointF& pos) {
    lastPosition = pos;
    setPos(pos);
}

QPointF Piece::getPosition() {
    return lastPosition;
}

QString Piece::getColor() {
    return color;
}

bool Piece::isValidMove(int destCol, int destRow) {
    return true;
}

QString Piece::imagePath(QString color){
    return "";
}




