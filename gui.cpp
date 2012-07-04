#include "gui.h"
#include "ui_qtmain.h"
#include <QGraphicsScene>

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


void MainWindow::setAlgorithmResults(float ** results) {
    this->algorithmResults = results;
}

void MainWindow::setAlgorithmResultLengths(int * resultLengths) {
    this->algorithmResultLengths = resultLengths;
}

void MainWindow::setNumAlgorithms(int numAlgorithms) {
    this->numAlgorithms = numAlgorithms;
}

void MainWindow::updateAlgorithmRendering(float seekPosition) {
    QGraphicsScene scene;
    this->graphicsView = new QGraphicsView(&scene, this);

    QGraphicsRectItem *rect = scene.addRect(QRectF(0, 0, 100, 100));
    QGraphicsItem *item = scene.itemAt(50, 50);

    scene.addItem(item);

    this->graphicsView->show();
}
