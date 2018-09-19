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

    //if it's player 2, write a 'O'
    } else {

    }
}
