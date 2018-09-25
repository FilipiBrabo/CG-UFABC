#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;

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

    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    int checkWin();

    void changePlayer();

    void restartGame();

    bool gameOver(int i);

    void lockBtns();

    void on_newGameBtn_clicked();

    void resetBoard();

    void makeMove(int i, int j, int pos);

private:
    Ui::MainWindow *ui;

    /* This is used to control the states of the board
     * 0 represents an empty cell, 1 is a cell that has a 'X'
     * 2 is a cell that has a 'O'*/
    int board[3][3] = {
        {0,0,0},
        {0,0,0},
        {0,0,0}
    };

    int player = 1;
    QVector<QPushButton*> btns;

};

#endif // MAINWINDOW_H
