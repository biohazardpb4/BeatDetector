#include <QtGui/QApplication>
#include "gui.h"
#include "Decoder/MP3Decoder.h"
#include "Algorithm/Algorithm.h"
#include <phonon/mediaobject.h>

static const int NUM_ALGOS = 1;

int main(int argc, char **argv)
{
        // decode the mp3
        MP3Decoder d = MP3Decoder();
        d.loadFile(argv[1]);
        unsigned char * samples = d.getSampleBuffer();
        int numSamples = d.getSampleBufferSize();

        // make an array of algos
        Algorithm* algos[NUM_ALGOS];

        // set different algos
        algos[0] = new SimplePowerHistory();

        /*for(int i = 0; i < NUM_ALGOS; i++) {
                Algorithm::setSampleBuffer(samples, numSamples);
                algos[i]->process();
        }*/

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
