#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeSymbol(int turn)
{
    //if it's player 1, write a 'X'
    if (turn == 1) {
        ui->cell1->setText(QString("X"));

    //if it's player 2, write a 'O'
    } else {
        ui->cell1->setText(QString("O"));
    }
}
