#include "mainwindow.h"
#include <QApplication>

#include "controller.h"

int main(int argc, char *argv[])
{
    Controller controller;

    MidiReceiver::PortInfo info;
    controller.GetMIDIPortInfo(info);

    if (info.empty()) {
        printf("no MIDI devices available\n");
        return 0;
    }

    for (auto &b : info) {
        printf("(%d, %s)\n", b.first, b.second.c_str());
    }

    controller.Initialize(0);

    controller.SetVoice(Hardware::Flute);
    //controller.SetSustain(true);
    //controller.SetStandardPitch(430.0f);
    float volumes[12] =
    {
        .2, .05, .2, .05, .2, .2, .05, .2, .05, .2, .05, .2
    };
    //controller.SetVolumes(volumes);

    //controller.SetTransposeAmount(-19);

    float centDeltas[12] =
    {
        .0, -29.3, 3.9, 15.6, -13.7, -2.0, -31.3, 2.0, -27.4, -15.6, 17.6, -11.7
    };

    controller.SetIntonation(centDeltas);

    while (1) {
        try {
            controller.RunIteration();
        }
        catch (...) {
            break;
        }
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
