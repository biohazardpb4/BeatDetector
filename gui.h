#ifndef QTMAIN_H
#define QTMAIN_H

#include <QMainWindow>
#include <QGraphicsView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setAlgorithmResults(float **);
    void setAlgorithmResultLengths(int *);
    void setNumAlgorithms(int);
    void updateAlgorithmRendering(float);
    
private:
    Ui::MainWindow *ui;

    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;

    float ** algorithmResults;
    int * algorithmResultLengths;
    int numAlgorithms;
};

#endif // QTMAIN_H
