#include <QtGui/QApplication>
#include "gui.h"
#include "Decoder/MP3Decoder.h"
#include "Algorithm/Algorithm.h"
#include <phonon/mediaobject.h>
#include <vector>
#include <iostream>
#include <string>

void cleanup() {
    Algorithm::cleanup();
}

void decode(string fname) {
    // decode the mp3
    MP3Decoder d = MP3Decoder();
    d.loadFile(fname.c_str());
    unsigned char * samples = d.getSampleBuffer();
    int numSamples = d.getSampleBufferSize();
    Algorithm::setSampleBuffer(samples, numSamples);
}

vector<Algorithm*>* process() {
    vector<Algorithm*> * algos = new vector<Algorithm*>();

    // set different algos
    algos->push_back(new SimplePowerHistory());

    for(int i = 0; i < algos->size(); i++)
        (*algos)[i]->process();

    // look at algo output
    std::cout.flush();

    return algos;
}

int main(int argc, char **argv)
{
    decode(string(argv[1]));
    vector<Algorithm*> * algos = process();

    // phonon setup
    QApplication app(argc, argv);
    app.setApplicationName("Beat Detector v0.000001");
    app.setQuitOnLastWindowClosed(true);

    MainWindow window;
    window.show();
    window.updateAlgorithmRendering(0);

    Phonon::MediaObject *music =
            Phonon::createPlayer(Phonon::MusicCategory,
                    Phonon::MediaSource(argv[1]));
    //music->play();

    return app.exec();
}
