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

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &Game::readData);
    connect(socket, &QTcpSocket::errorOccurred, this, &Game::handleSocketError);
    socket->connectToHost("127.0.0.1", PORT);

}

Game::~Game() {
    socket->disconnectFromHost();
    socket->deleteLater();
}

void Game::readData() {
    QByteArray data = socket->readAll();
    std::string dataString = data.toStdString();
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
import threading
import keyboard
import time

HOST = '127.0.0.1'
PORT = 8080


def handle_client_connection(conn, addr):
    print(f"Connected by {addr}")

    while True:
        data = conn.recv(1024)
        if not data:
            break

    print(f"Connection to {addr} closed")


def listen_for_keypress(conn):
    key_pressed = {'up': False, 'down': False, 'left': False, 'right': False}

    while True:
        if keyboard.is_pressed('up') and not key_pressed['up']:
            conn.sendall(b'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR')
            key_pressed['up'] = True
        elif not keyboard.is_pressed('up'):
            key_pressed['up'] = False

        if keyboard.is_pressed('down') and not key_pressed['down']:
            conn.sendall(b'r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1')
            key_pressed['down'] = True
        elif not keyboard.is_pressed('down'):
            key_pressed['down'] = False

        if keyboard.is_pressed('left') and not key_pressed['left']:
            conn.sendall(b'8/8/8/4p1K1/2k1P3/8/8/8')
            key_pressed['left'] = True
        elif not keyboard.is_pressed('left'):
            key_pressed['left'] = False

        if keyboard.is_pressed('right') and not key_pressed['right']:
            conn.sendall(b'4k2r/6r1/8/8/8/8/3R4/R3K3')
            key_pressed['right'] = True
        elif not keyboard.is_pressed('right'):
            key_pressed['right'] = False

        time.sleep(0.1)


def start_server(host, port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()
        print(f"Server listening on {host}:{port}")

        while True:
            conn, addr = s.accept()
            client_thread = threading.Thread(target=handle_client_connection, args=(conn, addr))
            client_thread.start()
            keyboard_thread = threading.Thread(target=listen_for_keypress, args=(conn,))
            keyboard_thread.start()

start_server(HOST, PORT)
*/