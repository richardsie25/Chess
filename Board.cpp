#include "Board.h"
#include "Piece.h"
#include "Pawn.h"
#include "Bishop.h"
#include "Knight.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

Board::Board(QGraphicsScene* scene){
    this->scene = scene;
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            pieceMap[row][col] = nullptr;
            previousPieceMap[row][col] = nullptr;
        }
    }
}

Board::~Board() {
    
}

bool Board::onPiecePressed(Piece* piece) {
    return gameState == "";
}

void Board::onPieceReleased(Piece* piece, QPointF releasePos) {
    int destCol = releasePos.x() / squareSize;
    int destRow = releasePos.y() / squareSize;
    int currentCol = piece->getPosition().x() / squareSize;
    int currentRow = piece->getPosition().y() / squareSize;
    
    if (piece->getColor() == playerTurn && !isKinginCheck(destCol, destRow, piece) && isValidPos(releasePos.x(), releasePos.y()) && piece->isValidMove(destCol, destRow) && !collisionCheck(destCol, destRow, piece) && !isTeam(destCol,destRow, piece)) {

        //Copy PieceMap
        copyPieceMap();

        if (isEnemy(destCol,destRow, piece))
            captures(destCol,destRow);

        //Update position and pieceMap
        piece->setPosition(QPointF(destCol * squareSize, destRow * squareSize));
        pieceMap[currentRow][currentCol] = nullptr;
        pieceMap[destRow][destCol] = piece;

        //Highlight squares
        highlightSquares();

        //Castling Flag
        if (dynamic_cast<King*>(piece))
            dynamic_cast<King*>(piece)->hasMoved = true;
        if (dynamic_cast<Rook*>(piece))
            dynamic_cast<Rook*>(piece)->hasMoved = true;

        //Pawn Promotion Check
        if (dynamic_cast<Pawn*>(piece)) {
            if ((piece->getColor() == "white" && destRow == 0) || (piece->getColor() == "black" && destRow == 7)) {
                Queen* promotedQueen = new Queen(piece->getColor());
                promotedQueen->setPosition(QPointF(destCol * squareSize, destRow * squareSize));
                scene->addItem(promotedQueen);
                pieceMap[destRow][destCol] = promotedQueen;
                connect(promotedQueen, &Piece::piecePressed, this, &Board::onPiecePressed);
                connect(promotedQueen, &Piece::pieceReleased, this, &Board::onPieceReleased);
                QTimer::singleShot(0, [this, piece]() {
                    scene->removeItem(piece);
                    });
            }
        }

        //Switch Player Turn
        if (playerTurn == "white") {
            playerTurn = "black";
        }
        else {
            playerTurn = "white";
        }
    }
    else {
        piece->setPosition(piece->getPosition());
    }
    processEvents();
}

bool Board::isValidPos(int destCol, int destRow) {
    return destCol >= 0 && destCol < boardSize * squareSize && destRow >= 0 && destRow < boardSize * squareSize;
}

bool Board::collisionCheck(int destCol, int destRow, Piece* piece) {
    int currentCol = piece->getPosition().x() / squareSize;
    int currentRow = piece->getPosition().y() / squareSize;

    //Rook Collision Check
    if (dynamic_cast<Rook*>(piece)) {
        //Horizontal Collision Check
        int startCol = std::min(currentCol, destCol);
        int endCol = std::max(currentCol, destCol);
        for (int col = startCol; col < endCol; col++) {
            if (col != currentCol && col != destCol && pieceMap[destRow][col] != nullptr) {
                return true;
            }
        }
        //Vertical Collision Check
        int startRow = std::min(currentRow, destRow);
        int endRow = std::max(currentRow, destRow);
        for (int row = startRow; row < endRow; row++) {
            if (row != currentRow && row != destRow && pieceMap[row][destCol] != nullptr) {
                return true;
            }
        }
    }

    //Bishop Collision Check
    if (dynamic_cast<Bishop*>(piece)) {
        //Diagonal Collision Check
        int colStep = (destCol > currentCol) ? 1 : -1;
        int rowStep = (destRow > currentRow) ? 1 : -1;
        for (int col = currentCol + colStep, row = currentRow + rowStep; col != destCol; col += colStep, row += rowStep) {
            if (pieceMap[row][col] != nullptr) {
                return true;
            }
        }
    }

    //Queen Collision Check
    if (dynamic_cast<Queen*>(piece)) {
        //Horizonal Movement Collision Check
        if (destCol != currentCol && destRow == currentRow) {
            int startCol = std::min(currentCol, destCol);
            int endCol = std::max(currentCol, destCol);
            for (int col = startCol; col < endCol; col++) {
                if (col != currentCol && col != destCol && pieceMap[destRow][col] != nullptr) {
                    return true;
                }
            }
        }
        //Vertical Movement Collision Check
        if (destCol == currentCol && destRow != currentRow) {
            int startRow = std::min(currentRow, destRow);
            int endRow = std::max(currentRow, destRow);
            for (int row = startRow; row < endRow; row++) {
                if (row != currentRow && row != destRow && pieceMap[row][destCol] != nullptr) {
                    return true;
                }
            }
        }
        //Diagonal Movement Collision Check
        if (qAbs(destCol - currentCol) == qAbs(destRow - currentRow)) {
            int colStep = (destCol > currentCol) ? 1 : -1;
            int rowStep = (destRow > currentRow) ? 1 : -1;
            for (int col = currentCol + colStep, row = currentRow + rowStep; col != destCol; col += colStep, row += rowStep) {
                if (pieceMap[row][col] != nullptr) {
                    return true;
                }
            }
        }
    }

    //Pawn Collision Check
    if (dynamic_cast<Pawn*>(piece)) {
        if (piece->getColor() == "white") {
            if (destCol == currentCol) {
                if (destRow == currentRow - 2) {
                    return pieceMap[destRow][destCol] != nullptr || pieceMap[destRow + 1][destCol] != nullptr;
                }
                else if (destRow == currentRow - 1) {
                    return pieceMap[destRow][destCol] != nullptr;
                }
            }
            if (qAbs(destCol - currentCol) == 1 && destRow == currentRow - 1) {
                return pieceMap[destRow][destCol] == nullptr;
            }
        }

        else {
            if (destCol == currentCol) {
                if (destRow == currentRow + 2) {
                    return pieceMap[destRow][destCol] != nullptr || pieceMap[destRow - 1][destCol] != nullptr;
                }
                else if (destRow == currentRow + 1) {
                    return pieceMap[destRow][destCol] != nullptr;
                }
            }
            if (qAbs(destCol - currentCol) == 1 && destRow == currentRow + 1) {
                return pieceMap[destRow][destCol] == nullptr;
            }
        }
    }

    //Castling Check
    if (dynamic_cast<King*>(piece)) {
        //King Side Castling
        if (destRow == currentRow && destCol - currentCol == 2) {
            if (pieceMap[currentRow][currentCol + 1] != nullptr || pieceMap[currentRow][currentCol + 2] != nullptr)
                return true;
            if (isKinginCheck(currentCol, currentRow, piece) || isKinginCheck(currentCol + 1, currentRow, piece) || isKinginCheck(currentCol + 2, currentRow, piece))
                return true;
            Piece* rook = pieceMap[destRow][destCol + 1];
            if (rook && dynamic_cast<Rook*>(rook) && dynamic_cast<Rook*>(rook)->hasMoved == false) {
                pieceMap[destRow][destCol - 1] = rook;
                pieceMap[destRow][destCol + 1] = nullptr;
                rook->setPosition(QPointF((destCol - 1) * squareSize, destRow * squareSize));
                return false;
            }
            return true;
        }

        //Queen Side Castling
        if (destRow == currentRow && destCol - currentCol == -2) {
            if (pieceMap[currentRow][currentCol - 1] != nullptr || pieceMap[currentRow][currentCol - 2] != nullptr || pieceMap[currentRow][currentCol - 3] != nullptr)
                return true;
            if (isKinginCheck(currentCol, currentRow, piece) || isKinginCheck(currentCol - 1, currentRow, piece) || isKinginCheck(currentCol - 2, currentRow, piece))
                return true;
            Piece* rook = pieceMap[destRow][destCol - 2];
            if (rook && dynamic_cast<Rook*>(rook) && dynamic_cast<Rook*>(rook)->hasMoved == false) {
                pieceMap[destRow][destCol + 1] = rook;
                pieceMap[destRow][destCol - 2] = nullptr;
                rook->setPosition(QPointF((destCol + 1) * squareSize, destRow * squareSize));
                return false;
            }
            return true;
        }
    }

    return false;
}

bool Board::isTeam(int destCol, int destRow, Piece* piece) {
    return pieceMap[destRow][destCol] != nullptr && pieceMap[destRow][destCol]->getColor() == piece->getColor();
}

bool Board::isEnemy(int destCol, int destRow, Piece* piece) {
    return pieceMap[destRow][destCol] != nullptr && pieceMap[destRow][destCol]->getColor() != piece->getColor();
}

void Board::captures(int destCol, int destRow) {
    Piece* piece = pieceMap[destRow][destCol];
    scene->removeItem(piece);
}

void Board::copyPieceMap(){
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            previousPieceMap[row][col] = pieceMap[row][col];
        }
    }
}

QPointF Board::getKingLocation(QString kingColor) {
    int kingCol = -1;
    int kingRow = -1;
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            Piece* piece = pieceMap[row][col];
            if (piece && piece->getColor() == kingColor && dynamic_cast<King*>(piece)) {
                kingCol = col;
                kingRow = row;
                break;
            }
        }
    }
    return QPointF(kingCol * squareSize, kingRow * squareSize);
}

bool Board::isKinginCheck(int destCol, int destRow, Piece* piece) {
    int currentCol = piece->getPosition().x() / squareSize;
    int currentRow = piece->getPosition().y() / squareSize;

    //Move piece temporarily
    Piece* originalDestPiece = pieceMap[destRow][destCol];
    QPointF originalPosition = piece->getPosition();
    pieceMap[currentRow][currentCol] = nullptr;
    pieceMap[destRow][destCol] = piece;
    piece->setPosition(QPointF(destCol * squareSize, destRow * squareSize));

    //Get king location
    QPointF kingLoc = getKingLocation(piece->getColor());
    int kingCol = kingLoc.x() / squareSize;
    int kingRow = kingLoc.y() / squareSize;

    //Check if king is checked
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            Piece* opponentPiece = pieceMap[i][j];
            if (opponentPiece && opponentPiece->getColor() != piece->getColor()) {
                if (opponentPiece->isValidMove(kingCol, kingRow) && !collisionCheck(kingCol, kingRow, opponentPiece)) {
                    //Revert pieces to original location
                    pieceMap[currentRow][currentCol] = piece;
                    pieceMap[destRow][destCol] = originalDestPiece;
                    piece->setPosition(originalPosition);
                    return true;
                }
            }
        }
    }
    //Revert pieces to original location
    pieceMap[currentRow][currentCol] = piece;
    pieceMap[destRow][destCol] = originalDestPiece;
    piece->setPosition(originalPosition);
    return false;
}

bool Board::isStalemate(QString kingColor) {
    QPointF kingLoc = getKingLocation(kingColor);
    int kingCol = kingLoc.x() / squareSize;
    int kingRow = kingLoc.y() / squareSize;

    //Check if king is in check
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            Piece* opponentPiece = pieceMap[i][j];
            if (opponentPiece && opponentPiece->getColor() != kingColor) {
                if (opponentPiece->isValidMove(kingCol, kingRow) && !collisionCheck(kingCol, kingRow, opponentPiece)) {
                    return false;
                }
            }
        }
    }

    //Check for any valid moves
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            Piece* teamPiece = pieceMap[i][j];
            if (teamPiece && teamPiece->getColor() == kingColor) {
                for (int k = 0; k < boardSize; k++) {
                    for (int l = 0; l < boardSize; l++) {
                        if (teamPiece->isValidMove(k, l) && !collisionCheck(k, l, teamPiece) && (pieceMap[l][k] == nullptr || pieceMap[l][k]->getColor() != kingColor)) {
                            if (!isKinginCheck(k, l, teamPiece))
                                return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool Board::isCheckmate(QString kingColor) {
    QPointF kingLoc = getKingLocation(kingColor);
    int kingCol = kingLoc.x() / squareSize;
    int kingRow = kingLoc.y() / squareSize;

    //Check for any valid moves
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            Piece* teamPiece = pieceMap[i][j];
            if (teamPiece && teamPiece->getColor() == kingColor) {
                for (int k = 0; k < boardSize; k++) {
                    for (int l = 0; l < boardSize; l++) {
                        if (teamPiece->isValidMove(k, l) && !collisionCheck(k, l, teamPiece) && (pieceMap[l][k] == nullptr || pieceMap[l][k]->getColor() != kingColor)) {
                            if (!isKinginCheck(k, l, teamPiece))
                                return false;
                        }
                    }
                }
            }
        }
    }

    //Check if king is in check
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            Piece* opponentPiece = pieceMap[i][j];
            if (opponentPiece && opponentPiece->getColor() != kingColor) {
                if (opponentPiece->isValidMove(kingCol, kingRow) && !collisionCheck(kingCol, kingRow, opponentPiece)) {
                    return true;
                }
            }
        }
    }

    return false;
}

void Board::processEvents() {
    if (playerTurn == "black") {
        if (isCheckmate("black")) {
            gameState = "White Wins!";
        }
        if (isStalemate("black")) {
            gameState = "Draw!";
        }
    }
    else if (playerTurn == "white") {
        if (isCheckmate("white")) {
            gameState = "Black Wins!";
        }
        if (isStalemate("white")) {
            gameState = "Draw!";
        }
    }
    //Change Text
    scene->removeItem(text);

    QString message;
    if (playerTurn == "white")
        message = "White Turn!";
    if (playerTurn == "black")
        message = "Black Turn!";
    if (gameState != "")
        message = gameState;

    text = new QGraphicsTextItem(message);
    QFont font("Times", squareSize / 2, QFont::Bold);
    text->setFont(font);
    text->setPos(squareSize * 2, -squareSize);

    QColor brushColor = QColor(118, 150, 86, 255);
    QBrush brush(brushColor);
    text->setDefaultTextColor(brush.color());
    scene->addItem(text);

    //Display Dead Material
    displayDeadMaterial();

    if (!gameState.isEmpty()) {
        QMessageBox::StandardButton reply = QMessageBox::question(nullptr, "Game Over!", gameState + "\nWould you like to play again?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            scene->removeItem(text);
            resetDefaultBoard();
        }
    }
}

void Board::highlightSquares(){
    scene->removeItem(currentHighlight);
    scene->removeItem(destHighlight);
    QPointF pieceOriginalPos, pieceDestPos;
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (previousPieceMap[row][col] != pieceMap[row][col] && pieceMap[row][col] == nullptr) {
                pieceOriginalPos.setX(col * squareSize);
                pieceOriginalPos.setY(row * squareSize);
            }
            else if (previousPieceMap[row][col] != pieceMap[row][col]) {
                pieceDestPos.setX(col * squareSize);
                pieceDestPos.setY(row * squareSize);
            }
        }
    }
    currentHighlight = scene->addRect(pieceOriginalPos.x(), pieceOriginalPos.y(), squareSize, squareSize,
        QPen(Qt::transparent), QBrush(QColor(255, 255, 0, 50)));
    destHighlight = scene->addRect(pieceDestPos.x(), pieceDestPos.y(), squareSize, squareSize,
        QPen(Qt::transparent), QBrush(QColor(255, 255, 0, 50)));
}

int Board::materialCounter() {
    int whiteTotalScore = 0;
    int blackTotalScore = 0;
    int score;
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if(pieceMap[row][col] != nullptr){
                if (dynamic_cast<Queen*>(pieceMap[row][col]))
                    score = 9;
                else if (dynamic_cast<Rook*>(pieceMap[row][col]))
                    score = 5;
                else if (dynamic_cast<Knight*>(pieceMap[row][col]))
                    score = 3;
                else if (dynamic_cast<Bishop*>(pieceMap[row][col]))
                    score = 3;
                else if (dynamic_cast<Pawn*>(pieceMap[row][col]))
                    score = 1;

                if (pieceMap[row][col]->getColor() == "white")
                    whiteTotalScore += score;
                else
                    blackTotalScore += score;
            }
        }
    }
    return whiteTotalScore - blackTotalScore;
}

void Board::displayDeadMaterial() {
    int whiteRow = 0;
    int whiteCol = 0;
    int blackRow = 0;
    int blackCol = 0;

    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            if (previousPieceMap[row][col] != nullptr && pieceMap[row][col] != nullptr && previousPieceMap[row][col]->getColor() != pieceMap[row][col]->getColor()) {

                // A piece was captured
                Piece* capturedPiece = previousPieceMap[row][col];
                QString imagePath = capturedPiece->getColor();
                if (dynamic_cast<Queen*>(capturedPiece))
                    imagePath += "Queen.png";
                else if (dynamic_cast<Bishop*>(capturedPiece))
                    imagePath += "Bishop.png";
                else if (dynamic_cast<Knight*>(capturedPiece))
                    imagePath += "Knight.png";
                else if (dynamic_cast<Rook*>(capturedPiece))
                    imagePath += "Rook.png";
                else if (dynamic_cast<Pawn*>(capturedPiece))
                    imagePath += "Pawn.png";

                QGraphicsPixmapItem* capturedPieceItem = new QGraphicsPixmapItem(QPixmap(imagePath).scaled(squareSize/2, squareSize/2));
                capturedPieceItem->setFlag(QGraphicsItem::ItemIsMovable, false);
                scene->addItem(capturedPieceItem);
                if (capturedPiece->getColor() == "white")
                    whiteDeadPieces.append(capturedPieceItem);
                else
                    blackDeadPieces.append(capturedPieceItem);

                // Display captured white pieces on the top left and black pieces on the top right
                if (capturedPiece->getColor() == "white") {
                    capturedPieceItem->setPos(QPointF((whiteDeadPieces.length() % 4 - 4) * squareSize, (-1 + whiteDeadPieces.length() / 4) * squareSize));
                }
                else {
                    capturedPieceItem->setPos(QPointF((boardSize + blackDeadPieces.length() % 4) * squareSize, (-1 + blackDeadPieces.length() / 4) * squareSize));
                }
            }
        }
    }
}

void Board::drawBoard() {
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {

            //Squares
            QColor squareColor = ((row + col) % 2 == 0) ? QColor(238, 238, 210, 255) : QColor(118, 150, 86, 255);
            scene->addRect(col * squareSize, row * squareSize, squareSize, squareSize,
                QPen(Qt::transparent), QBrush(squareColor));

            if (col == 0) {
                //Font Text
                QGraphicsTextItem* textItem = new QGraphicsTextItem();
                QFont font("Times", squareSize / 7, QFont::Bold);
                textItem->setFont(font);

                //Font Color
                QColor brushColor = ((row + col) % 2 == 1) ? QColor(238, 238, 210, 255) : QColor(118, 150, 86, 255);
                QBrush brush(brushColor);
                textItem->setDefaultTextColor(brush.color());

                textItem->setPlainText(QString("%1").arg(boardSize - row));
                qreal x = col * squareSize - 3;
                qreal y = row * squareSize - 5;
                textItem->setPos(x, y);
                scene->addItem(textItem);
            }

            if (row == boardSize - 1) {
                //Font Text
                QGraphicsTextItem* textItem = new QGraphicsTextItem();
                QFont font("Times", squareSize / 7, QFont::Bold);
                textItem->setFont(font);

                //Font Color
                QColor brushColor = ((row + col) % 2 == 1) ? QColor(238, 238, 210, 255) : QColor(118, 150, 86, 255);
                QBrush brush(brushColor);
                textItem->setDefaultTextColor(brush.color());

                textItem->setPlainText(QString("%1").arg(QChar('a' + col)));
                qreal x = (col + 1) * squareSize - textItem->boundingRect().width() + 3;  
                qreal y = (row + 1) * squareSize - textItem->boundingRect().height() + 3;  
                textItem->setPos(x, y);
                scene->addItem(textItem);
            }
            
        }
    }
}

void Board::resetDefaultBoard() {
    clearBoard();
    placeWhite();
    placeBlack();
}

void Board::placeWhite() {
    Piece* piece;
    for (int i = 0; i < boardSize; i++) {
        piece = new Pawn("white");
        scene->addItem(piece);
        piece->setPosition(QPointF(i * squareSize, 6 * squareSize));
        pieceMap[6][i] = piece;
        connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
        connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);
    }
    piece = new Rook("white");
    scene->addItem(piece);
    piece->setPosition(QPointF(0, 7 * squareSize));
    pieceMap[7][0] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);

    piece = new Knight("white");
    scene->addItem(piece);
    piece->setPosition(QPointF(squareSize, 7 * squareSize));
    pieceMap[7][1] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);
    
    piece = new Bishop("white");
    scene->addItem(piece);
    piece->setPosition(QPointF(2 * squareSize, 7 * squareSize));
    pieceMap[7][2] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);
    
    piece = new Queen("white");
    scene->addItem(piece);
    piece->setPosition(QPointF(3 * squareSize, 7 * squareSize));
    pieceMap[7][3] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);

    piece = new King("white");
    scene->addItem(piece);
    piece->setPosition(QPointF(4 * squareSize, 7 * squareSize));
    pieceMap[7][4] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);

    piece = new Bishop("white");
    scene->addItem(piece);
    piece->setPosition(QPointF(5 * squareSize, 7 * squareSize));
    pieceMap[7][5] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);

    piece = new Knight("white");
    scene->addItem(piece);
    piece->setPosition(QPointF(6 * squareSize, 7 * squareSize));
    pieceMap[7][6] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);

    piece = new Rook("white");
    scene->addItem(piece);
    piece->setPosition(QPointF(7 * squareSize, 7 * squareSize));
    pieceMap[7][7] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);
}

void Board::placeBlack() {
    Piece* piece;
    for (int i = 0; i < boardSize; i++) {
        piece = new Pawn("black");
        scene->addItem(piece);
        piece->setPosition(QPointF(i * squareSize, squareSize));
        pieceMap[1][i] = piece;
        connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
        connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);
    }
    piece = new Rook("black");
    scene->addItem(piece);
    piece->setPosition(QPointF(0, 0));
    pieceMap[0][0] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);

    piece = new Knight("black");
    scene->addItem(piece);
    piece->setPosition(QPointF(squareSize, 0));
    pieceMap[0][1] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);

    piece = new Bishop("black");
    scene->addItem(piece);
    piece->setPosition(QPointF(2 * squareSize, 0));
    pieceMap[0][2] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);

    piece = new Queen("black");
    scene->addItem(piece);
    piece->setPosition(QPointF(3 * squareSize, 0));
    pieceMap[0][3] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);

    piece = new King("black");
    scene->addItem(piece);
    piece->setPosition(QPointF(4 * squareSize, 0));
    pieceMap[0][4] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);

    piece = new Bishop("black");
    scene->addItem(piece);
    piece->setPosition(QPointF(5 * squareSize, 0));
    pieceMap[0][5] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);

    piece = new Knight("black");
    scene->addItem(piece);
    piece->setPosition(QPointF(6 * squareSize, 0));
    pieceMap[0][6] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);

    piece = new Rook("black");
    scene->addItem(piece);
    piece->setPosition(QPointF(7 * squareSize, 0));
    pieceMap[0][7] = piece;
    connect(piece, &Piece::piecePressed, this, &Board::onPiecePressed);
    connect(piece, &Piece::pieceReleased, this, &Board::onPieceReleased);
}

void Board::clearBoard() {
    for (QGraphicsItem* item : scene->items()) {
        if (dynamic_cast<Piece*>(item)) {
            QTimer::singleShot(0, [this, item]() {
                scene->removeItem(item);
                delete item;
                });
        }
    }
    for (int row = 0; row < boardSize; row++) {
        for (int col = 0; col < boardSize; col++) {
            pieceMap[row][col] = nullptr;
            previousPieceMap[row][col] = nullptr;
        }
    }

    scene->removeItem(currentHighlight);
    scene->removeItem(destHighlight);
    currentHighlight = nullptr;
    destHighlight = nullptr;
    playerTurn = "white";
    gameState = "";
    text = new QGraphicsTextItem("White Turn!");
    QFont font("Times", squareSize / 2, QFont::Bold);
    text->setFont(font);
    text->setPos(squareSize * 2, -squareSize);

    QColor brushColor = QColor(118, 150, 86, 255);
    QBrush brush(brushColor);
    text->setDefaultTextColor(brush.color());
    scene->addItem(text);

    whiteDeadPieces.clear();
    blackDeadPieces.clear();
}



