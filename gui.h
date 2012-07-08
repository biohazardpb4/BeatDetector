#ifndef QTMAIN_H
#define QTMAIN_H

#include <QMainWindow>
#include <QGraphicsView>
#include <vector>
#include <QTimer>
#include <phonon/mediaobject.h>

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
    void setMusicPlayer(Phonon::MediaObject*);
    void updateAlgorithmRendering(float);
    
private:
    static const float TRACK_WIDTH_PIXELS;

    Ui::MainWindow *ui;

    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;
    QTimer *timer;
    Phonon::MediaObject* music;

    std::vector<std::vector<float>*>* algorithmResults;
    std::vector<std::vector<QRect*>*>* beatFlags;

private slots:
    void updateGraphics();
};

#endif // QTMAIN_H
