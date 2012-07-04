#ifndef QTMAIN_H
#define QTMAIN_H

#include <QMainWindow>
#include <QGraphicsView>
#include <vector>

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

    std::vector<std::vector<float>*>* algorithmResults;
};

#endif // QTMAIN_H
