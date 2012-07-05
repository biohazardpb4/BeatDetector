#ifndef QTMAIN_H
#define QTMAIN_H

#include <QMainWindow>
#include <QGraphicsView>
#include <vector>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setAlgorithmResults(std::vector<std::vector<float>*>*);
    void updateAlgorithmRendering(float);
    
private:
    Ui::MainWindow *ui;

    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;
    QTimer *timer;

    std::vector<std::vector<float>*>* algorithmResults;

private slots:
    void updateGraphics();
};

#endif // QTMAIN_H
