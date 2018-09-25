#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //new style
    connect(ui->btnSum, &QPushButton::clicked, this, &MainWindow::sumNumbers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sumNumbers()
{
    int r = ui->lineN1->text().toInt() + ui->lineN2->text().toInt();
    ui->labelResult->setText(QString("Result : %1").arg(r));
    repaint();
}
