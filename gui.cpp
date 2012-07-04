#include "gui.h"
#include "ui_qtmain.h"
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(800, 600);
    this->setWindowTitle("Beat Detector v0.000001");
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
    this->graphicsScene = new QGraphicsScene();

    this->graphicsView = new QGraphicsView(this->graphicsScene, this);
    this->graphicsView->resize(760, 540);
    this->graphicsView->move(20, 40);

    QRect rect = QRect(100,20,100,100);
    this->graphicsScene->addRect(rect, QPen(Qt::black), QBrush(Qt::green));

    this->graphicsView->update();
    this->graphicsView->show();
}
