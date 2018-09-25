#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    btns.append(ui->pushButton_1);
    btns.append(ui->pushButton_2);
    btns.append(ui->pushButton_3);
    btns.append(ui->pushButton_4);
    btns.append(ui->pushButton_5);
    btns.append(ui->pushButton_6);
    btns.append(ui->pushButton_7);
    btns.append(ui->pushButton_8);
    btns.append(ui->pushButton_9);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeMove(int i, int j, int pos) {
    if (board[i][j] == 0) {
        if (player == 1) {
            board[i][j] = 1;
            this->btns.at(pos)->setText("X");
        } else {
            board[i][j] = -1;
            this->btns.at(pos)->setText("O");
        }

        if (gameOver(checkWin()))
            return;
        changePlayer();
    }
}

void MainWindow::on_pushButton_1_clicked()
{
    makeMove(0,0,0);
}

void MainWindow::on_pushButton_2_clicked()
{
    makeMove(0,1,1);
}

void MainWindow::on_pushButton_3_clicked()
{
    makeMove(0,2,2);
}

void MainWindow::on_pushButton_4_clicked()
{
    makeMove(1,0,3);
}

void MainWindow::on_pushButton_5_clicked()
{
    makeMove(1,1,4);
}

void MainWindow::on_pushButton_6_clicked()
{
   makeMove(1,2,5);
}

void MainWindow::on_pushButton_7_clicked()
{
    makeMove(2,0,6);
}

void MainWindow::on_pushButton_8_clicked()
{
   makeMove(2,1,7);
}

void MainWindow::on_pushButton_9_clicked()
{
    makeMove(2,2,8);
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

void MainWindow::restartGame() {
    for (QPushButton* btn : this->btns) {
        btn->setText("");
        btn->setEnabled(true);
    }
    ui->mainLabel->setText("X turn");
    resetBoard();
}

void MainWindow::on_newGameBtn_clicked()
{

    restartGame();
}

void MainWindow::resetBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j ++) {
            board[i][j] = 0;
        }
    }
}
