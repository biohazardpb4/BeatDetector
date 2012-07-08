#include "gui.h"
#include "ui_gui.h"
#include <QGraphicsScene>
#include <stdio.h>
#include <iostream>

const float MainWindow::TRACK_WIDTH_PIXELS = 10000.0f;
const float MainWindow::END_BAR_POSITION = 20.0f;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(800, 600);
    this->setWindowTitle("Beat Detector v0.000001");
    this->timer = new QTimer(this);
    this->connect(timer, SIGNAL(timeout()), this, SLOT(updateGraphics()));
    timer->setInterval( 50 );
    timer->start();

    // set up graphics
    this->graphicsScene = new QGraphicsScene();
    this->graphicsView = new QGraphicsView(this->graphicsScene, this);
    this->graphicsView->resize(760, 540);
    this->graphicsView->move(20, 40);

    this->endBar = this->graphicsScene->addRect(
                QRectF(MainWindow::END_BAR_POSITION, 0, 10, 500),
                QPen(Qt::black), QBrush(Qt::red));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setAlgorithmResults(std::vector<std::vector<float>*>* results) {

    this->algorithmResults = results;
    this->beatFlags = new std::vector<std::vector<QRect*>*>();

    for(int i = 0; i < results->size(); i++) {
        std::vector<float>* currentFlags = (*results)[i];
        std::vector<QRect*> * newRects = new std::vector<QRect*>();
        for(int j = 0; j < currentFlags->size(); j++) {
            float leftPixelPos = (*currentFlags)[j] * TRACK_WIDTH_PIXELS;
            newRects->push_back(new QRect(leftPixelPos, 0, 10, 100));
            this->graphicsScene->addRect(*(*newRects)[j], QPen(Qt::black), QBrush(Qt::green));
        }
        this->beatFlags->push_back(newRects);
    }
}

void MainWindow::setMusicPlayer(Phonon::MediaObject* music) {
    this->music = music;
}

void MainWindow::updateGraphics() {
    float r = 0;
    if(this->music != NULL)
        r = float(this->music->currentTime())/this->music->totalTime();
    updateAlgorithmRendering(r);
    printf("moving to: %f\n", r);
    std::cout.flush();
}

/**
  * Draw the beat flags at the current location.
  *
  */
void MainWindow::updateAlgorithmRendering(float seekPosition) {
    QRectF rect = this->graphicsView->sceneRect();

    QPointF point = rect.topLeft();
    QPointF newPoint(seekPosition * rect.width(), point.y());
    QRectF newRect(rect);
    newRect.moveTo(newPoint);
    this->graphicsView->setSceneRect(newRect);

    // move the baseline rectangle
    float newX = seekPosition * rect.width() + MainWindow::END_BAR_POSITION;
    this->endBar->moveBy(newX - this->endBar->x() ,0);
}
