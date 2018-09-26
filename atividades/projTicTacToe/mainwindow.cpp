#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectBtns();
    startGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectBtns() {
    connect(ui->newGameBtn, &QPushButton::clicked, this, &MainWindow::startGame);
    connect(ui->exitBtn, &QPushButton::clicked, this, &MainWindow::close);

    btns = ui->gridLayoutWidget->findChildren<QPushButton *>();

    for (QPushButton* btn : btns) {
        connect(btn, &QPushButton::clicked, this, [=](){
            makeMove(btn);
        });
    }
}

void MainWindow::makeMove(QPushButton* btn) {
    QString btnName = btn->objectName().toLatin1();

    int i = btnName.at(3).digitValue();
    int j = btnName.at(4).digitValue();

    if (board[i][j] == 0) {
            if (player == 1) {
                board[i][j] = 1;
                btn->setText("X");
            } else {
                board[i][j] = -1;
                btn->setText("O");
            }

            if (gameOver(checkWin()))
                return;
            changePlayer();
        }
}


void MainWindow::changePlayer() {
    if (player == 1) {
        ui->mainLabel->setText("O turn");
        player = -1;
    } else {
        ui->mainLabel->setText("X turn");
        player = 1;
    }
}

int MainWindow::checkWin() {

    //Checking lines and collums
    for (int i = 0; i < 3; i++) {
        int countHorizontal = 0;
        int countVertical = 0;

        for (int j = 0; j < 3; j++) {
            countHorizontal += board[i][j];
            countVertical += board[j][i];
        }

        if (countHorizontal == 3 || countVertical == 3) {
            return 1;
        } else if (countHorizontal == -3 || countVertical == -3) {
            return 2;
        }
    }

    //Checking diagonals
    int countDiagonal1 = 0;
    int countDiagonal2 = 0;

    for (int i = 0; i < 3; i++) {
        countDiagonal1 += board[i][i];
        countDiagonal2 += board[i][3-i-1];
    }

    if (countDiagonal1 == 3 || countDiagonal2 == 3) {
        return 1;
    } else if (countDiagonal1 == -3 || countDiagonal2 == -3) {
        return 2;
    }

    //if nobody won
    return 0;
}

bool MainWindow::gameOver(int nWinner) {
    for (int i = 1; i < 10; i++) {
    //    ui->pushButton_

    }

    //Check draw
    bool draw = true;
    for (int i = 0; i < 3 && draw; i++) {
        for (int j = 0; j < 3 && draw; j ++) {
            if (board[i][j] == 0)
                draw = false;
        }
    }

    if (draw) {
        ui->mainLabel->setText("DRAW!!!");
        lockBtns();
        return true;
    } else if (nWinner == 1) {
        ui->mainLabel->setText("X WON!!!!");
        lockBtns();
        return true;
    } else if (nWinner == 2){
        ui->mainLabel->setText("O WON!!!!");
        lockBtns();
        return true;
    }

    return false;
}

void MainWindow::lockBtns() {
    for (QPushButton* btn : this->btns ) {
        btn->setEnabled(false);
    }
}

void MainWindow::startGame() {
    for (QPushButton* btn : this->btns) {
        btn->setText("");
        btn->setEnabled(true);
    }
    ui->mainLabel->setText("X turn");
    resetBoard();
}

void MainWindow::on_newGameBtn_clicked()
{

    startGame();
}

void MainWindow::resetBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j ++) {
            board[i][j] = 0;
        }
    }
}
