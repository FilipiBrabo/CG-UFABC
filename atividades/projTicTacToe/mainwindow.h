#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    int checkWin();

    void changePlayer();

    void startGame();

    bool gameOver(int i);

    void lockBtns();

    void on_newGameBtn_clicked();

    void resetBoard();

    void makeMove(QPushButton* btn);

    void connectBtns();

private:
    Ui::MainWindow *ui;

    /* This is used to control the states of the board
     * 0 represents an empty cell, 1 is a cell that has a 'X'
     * 2 is a cell that has a 'O' */
    int board[3][3] = {
        {0,0,0},
        {0,0,0},
        {0,0,0}
    };

    /* Variable that indicates whose turn it is
     * 1  represents player 'X'
     * -1 represents player 'O' */
    int player = 1;

    //Vector with all buttons of the game board
    QList<QPushButton*> btns;

};

#endif // MAINWINDOW_H
