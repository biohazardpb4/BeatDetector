#include "gui.h"
#include "ui_gui.h"
#include <QGraphicsScene>
#include <stdio.h>
#include <iostream>
#include <QImage>
#include <QPixmap>

const float MainWindow::TRACK_WIDTH_PIXELS = 20000.0f;
const float MainWindow::END_BAR_POSITION = 20.0f;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(800, 600);
    this->setWindowTitle("Beat Detector v0.1A");
    this->timer = new QTimer(this);
    this->connect(timer, SIGNAL(timeout()), this, SLOT(updateGraphics()));
    timer->setInterval( 10 );
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
    // calculate height for each algo row based on graphics view height
    int algoRowHeight = (this->graphicsView->height() - 40)/results->size();

    for(int i = 0; i < results->size(); i++) {
        std::vector<float>* currentFlags = (*results)[i];
        std::vector<QRect*> * newRects = new std::vector<QRect*>();
        for(int j = 0; j < currentFlags->size(); j++) {
            float leftPixelPos = (*currentFlags)[j] * TRACK_WIDTH_PIXELS + END_BAR_POSITION;
            newRects->push_back(new QRect(leftPixelPos, algoRowHeight*i, 10, algoRowHeight-10));
            this->graphicsScene->addRect(*(*newRects)[j], QPen(Qt::black), QBrush(Qt::green));
        }
        this->beatFlags->push_back(newRects);
    }
}

void MainWindow::setAlgorithmSamples(unsigned char * samples) {
    this->samples = samples;
}

void MainWindow::setAlgorithmSampleSize(long numSamples) {
    this->numSamples = numSamples;
}

void MainWindow::drawSamples() {

    short sampleOne, sampleTwo;
    long sampleIndex = 0;
    double sampleWidth = (double)this->numSamples / MainWindow::TRACK_WIDTH_PIXELS / 2;
    QImage sampleImage((int)MainWindow::TRACK_WIDTH_PIXELS, 500, QImage::Format_ARGB32);
    QPainter painter;
    QPen pen(Qt::blue);
    pen.setWidth(2);
    painter.setPen(pen);

    painter.begin(&sampleImage);
    for(long i = 2; i < TRACK_WIDTH_PIXELS; i++) {
        sampleOne = ((short*)this->samples)[sampleIndex];
        sampleIndex = (((long)(sampleWidth*i) >> 1) << 1);
        sampleTwo = ((short*)this->samples)[sampleIndex];

        painter.drawLine(i-1 + END_BAR_POSITION, (sampleOne >> 8)+250, i + END_BAR_POSITION, (sampleTwo >> 8)+250);
    }
    painter.end();

    this->graphicsScene->addPixmap(QPixmap::fromImage(sampleImage));
}

void MainWindow::setMusicPlayer(Phonon::MediaObject* music) {
    this->music = music;
}

void MainWindow::updateGraphics() {
    float r = 0;
    if(this->music != NULL)
        r = float(this->music->currentTime())/this->music->totalTime();
    updateAlgorithmRendering(r);
    //printf("moving to: %f\n", r);
    //std::cout.flush();
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
