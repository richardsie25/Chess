#include "Game.h"
#include "Board.h"
Board* board;

Game::Game(QWidget *parent) : QGraphicsView(parent) {
    setFixedSize(squareSize * 16, squareSize * 10);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setStyleSheet("background-color: rgb(48, 46, 43);");

    QGraphicsScene* scene = new QGraphicsScene();
    setScene(scene);
    scene->setSceneRect(0, 0, boardSize * squareSize, boardSize * squareSize);

    board = new Board(scene);
    board->drawBoard();
    board->resetDefaultBoard();

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &Game::readData);
    connect(socket, &QTcpSocket::errorOccurred, this, &Game::handleSocketError);
    socket->connectToHost("192.168.1.168", 8000);

}

Game::~Game() {
    socket->disconnectFromHost();
    socket->deleteLater();
}

void Game::readData() {
    QByteArray data = socket->readAll();
    std::string dataString = data.toStdString();
    qDebug() << dataString;
    board->displayBoard(dataString);
}

void Game::handleSocketError(QAbstractSocket::SocketError socketError) {
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << "Remote host closed the connection.";
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "The host was not found. Please check the host and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "The connection was refused by the peer. Make sure the server is running,"
            << "and check that the host address and port settings are correct.";
        break;
    default:
        qDebug() << "The following socket error occurred:" << socket->errorString();
    }
}

/*
import socket

HOST = '127.0.0.1'  # Standard loopback interface (localhost)
PORT = 8080        # Port to listen on (non-privileged ports are > 1023)

def send_line_to_client(conn, file):
    with open(file, 'r') as f:
        for line in f:
            input("Press Enter to send the next line...")
            conn.sendall(line.encode())

def start_server(host, port, file):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()
        print(f"Server listening on {host}:{port}")

        conn, addr = s.accept()
        with conn:
            print(f"Connected by {addr}")
            send_line_to_client(conn, file)

start_server(HOST, PORT, 'fen.txt')

C:\Qt\6.5.2\msvc2019_64\bin\windeployqt.exe C:\Users\richa\Documents\School\Irvine\ChessGUI\x64\Debug\ChessGUI.exe

rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR
rnbqkbnr/p1pppppp/1p6/8/3P4/8/PPP1PPPP/RNBQKBNR
rnbqkbnr/p1pppppp/1p6/8/2PP4/8/PP2PPPP/RNBQKBNR
rn1qkbnr/pbpppppp/1p6/8/2PP4/8/PP2PPPP/RNBQKBNR
rn1qkbnr/pbpppppp/1p6/8/2PP4/2N5/PP2PPPP/R1BQKBNR
rn1qkbnr/pbpppp1p/1p4p1/8/2PP4/2N5/PP2PPPP/R1BQKBNR
rn1qkbnr/pbpppp1p/1p4p1/8/2PP4/2N2N2/PP2PPPP/R1BQKB1R
rn1qk1nr/pbppppbp/1p4p1/8/2PP4/2N2N2/PP2PPPP/R1BQKB1R
rn1qk1nr/pbppppbp/1p4p1/8/2PP1B2/2N2N2/PP2PPPP/R2QKB1R
rn1qk1nr/pbppppbp/6p1/1p6/2PP1B2/2N2N2/PP2PPPP/R2QKB1R
rn1qk1nr/pbppppbp/6p1/1P6/3P1B2/2N2N2/PP2PPPP/R2QKB1R
rn1qk1nr/pbppp1bp/5pp1/1P6/3P1B2/2N2N2/PP2PPPP/R2QKB1R
rn1qk1nr/pbppp1bp/5pp1/1P6/3P1B2/2N1PN2/PP3PPP/R2QKB1R
rn1qk1nr/pbpp2bp/5pp1/1P2p3/3P1B2/2N1PN2/PP3PPP/R2QKB1R
rn1qk1nr/pbpp2bp/5pp1/1P2P3/5B2/2N1PN2/PP3PPP/R2QKB1R
rn1qk1nr/pbpp2bp/6p1/1P2p3/5B2/2N1PN2/PP3PPP/R2QKB1R
rn1qk1nr/pbpp2bp/6p1/1P2B3/8/2N1PN2/PP3PPP/R2QKB1R
rn1qk1nr/pbpp3p/6p1/1P2b3/8/2N1PN2/PP3PPP/R2QKB1R
rn1qk1nr/pbpp3p/6p1/1P2N3/8/2N1P3/PP3PPP/R2QKB1R
rn2k1nr/pbpp3p/5qp1/1P2N3/8/2N1P3/PP3PPP/R2QKB1R
rn2k1nr/pbpp3p/5qp1/1P6/8/2N1PN2/PP3PPP/R2QKB1R
rn2k1nr/p1pp3p/5qp1/1P6/8/2N1Pb2/PP3PPP/R2QKB1R
rn2k1nr/p1pp3p/5qp1/1P6/8/2N1PQ2/PP3PPP/R3KB1R
rn2k1nr/p1pp3p/6p1/1P6/8/2N1Pq2/PP3PPP/R3KB1R
rn2k1nr/p1pp3p/6p1/1P6/8/2N1PP2/PP3P1P/R3KB1R
rn2k1nr/p1pp4/6pp/1P6/8/2N1PP2/PP3P1P/R3KB1R
rn2k1nr/p1pp4/6pp/1P6/8/2N1PP2/PP3PBP/R3K2R
rn2k1nr/2pp4/p5pp/1P6/8/2N1PP2/PP3PBP/R3K2R
rn2k1nr/2pp4/p5pp/1P6/8/2N1PP2/PP3PBP/R4RK1
rn2k2r/2ppn3/p5pp/1P6/8/2N1PP2/PP3PBP/R4RK1
rn2k2r/2ppn3/P5pp/8/8/2N1PP2/PP3PBP/R4RK1
rn2k2r/2p1n3/P5pp/3p4/8/2N1PP2/PP3PBP/R4RK1
rn2k2r/2p1n3/P5pp/3p4/5P2/2N1P3/PP3PBP/R4RK1
rn2k1r1/2p1n3/P5pp/3p4/5P2/2N1P3/PP3PBP/R4RK1
rn2k1r1/2p1n3/P5pp/3N4/5P2/4P3/PP3PBP/R4RK1
r3k1r1/2p1n3/n5pp/3N4/5P2/4P3/PP3PBP/R4RK1
r3k1r1/2N1n3/n5pp/8/5P2/4P3/PP3PBP/R4RK1
r3k1r1/2n1n3/6pp/8/5P2/4P3/PP3PBP/R4RK1
B3k1r1/2n1n3/6pp/8/5P2/4P3/PP3P1P/R4RK1
n3k1r1/4n3/6pp/8/5P2/4P3/PP3P1P/R4RK1
n3k1r1/4n3/6pp/8/P4P2/4P3/1P3P1P/R4RK1
n3k1r1/4n3/6p1/7p/P4P2/4P3/1P3P1P/R4RK1
n3k1r1/4n3/6p1/P6p/5P2/4P3/1P3P1P/R4RK1
n3k1r1/4n3/8/P5pp/5P2/4P3/1P3P1P/R4RK1
n3k1r1/4n3/8/P5Pp/8/4P3/1P3P1P/R4RK1
n3k3/4n3/8/P5rp/8/4P3/1P3P1P/R4RK1
n3k3/4n3/8/P5rp/8/4P3/1P3P1P/R4R1K
n3k3/4n3/8/Pr5p/8/4P3/1P3P1P/R4R1K
n3k3/4n3/8/Pr5p/8/4P3/1P3P1P/RR5K
n3k1n1/8/8/Pr5p/8/4P3/1P3P1P/RR5K
n3k1n1/8/P7/1r5p/8/4P3/1P3P1P/RR5K
n3k3/8/P4n2/1r5p/8/4P3/1P3P1P/RR5K
n3k3/8/P4n2/1r5p/8/4P3/RP3P1P/1R5K
n7/3k4/P4n2/1r5p/8/4P3/RP3P1P/1R5K
n7/3k4/P4n2/1r5p/8/4P3/RP3P1P/R6K
n7/8/P2k1n2/1r5p/8/4P3/RP3P1P/R6K
n7/8/P2k1n2/Rr5p/8/4P3/1P3P1P/R6K
n7/8/P2k1n2/r6p/8/4P3/1P3P1P/R6K
n7/8/P2k1n2/R6p/8/4P3/1P3P1P/7K
n7/8/P2k4/R6p/6n1/4P3/1P3P1P/7K
n7/8/P2k4/R6p/6n1/4P3/1P3PKP/8
n7/8/P2k4/R6p/8/4n3/1P3PKP/8
n7/8/P2k4/R6p/8/4P3/1P4KP/8
n7/8/P2k4/R7/7p/4P3/1P4KP/8
n7/8/P2k4/7R/7p/4P3/1P4KP/8
n7/8/P1k5/7R/7p/4P3/1P4KP/8
n7/8/P1k5/8/7R/4P3/1P4KP/8
8/2n5/P1k5/8/7R/4P3/1P4KP/8
8/2n5/P1k4R/8/8/4P3/1P4KP/8
8/2n5/P6R/1k6/8/4P3/1P4KP/8
8/2n5/P6R/1k6/7P/4P3/1P4K1/8
8/8/n6R/1k6/7P/4P3/1P4K1/8
8/8/R7/1k6/7P/4P3/1P4K1/8
8/8/k7/8/7P/4P3/1P4K1/8
8/8/k7/7P/8/4P3/1P4K1/8
8/1k6/8/7P/8/4P3/1P4K1/8
8/1k6/7P/8/8/4P3/1P4K1/8
8/8/2k4P/8/8/4P3/1P4K1/8
8/7P/2k5/8/8/4P3/1P4K1/8
8/7P/8/3k4/8/4P3/1P4K1/8
7Q/8/8/3k4/8/4P3/1P4K1/8
7Q/8/8/8/2k5/4P3/1P4K1/8
8/8/8/8/2kQ4/4P3/1P4K1/8
8/8/8/1k6/3Q4/4P3/1P4K1/8
8/8/8/1k6/3Q4/1P2P3/6K1/8
8/8/k7/8/3Q4/1P2P3/6K1/8
8/8/k7/8/1Q6/1P2P3/6K1/8
8/k7/8/8/1Q6/1P2P3/6K1/8
8/k7/8/1Q6/8/1P2P3/6K1/8
k7/8/8/1Q6/8/1P2P3/6K1/8
k7/8/8/1Q6/8/1P2PK2/8/8
8/k7/8/1Q6/8/1P2PK2/8/8
8/k7/8/1Q6/4K3/1P2P3/8/8
k7/8/8/1Q6/4K3/1P2P3/8/8
k7/8/8/1Q1K4/8/1P2P3/8/8
8/k7/8/1Q1K4/8/1P2P3/8/8
8/k7/2K5/1Q6/8/1P2P3/8/8
k7/8/2K5/1Q6/8/1P2P3/8/8
k7/1Q6/2K5/8/8/1P2P3/8/8
*/