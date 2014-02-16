#include "mainwindow.h"
#include <QApplication>

#include "hardware.h"

int main(int argc, char *argv[])
{
#if 0
    Hardware hardware;
    hardware.Initialize();

    NoteId id1 = hardware.NoteOn(Hardware::Clarinet, 440.0f, 0.25f);
    NoteId id2 = hardware.NoteOn(Hardware::Clarinet, 600.0f, 0.25f);
    NoteId id3 = hardware.NoteOn(Hardware::Flute, 800.0f, 0.25f);

    Sleep(1000);
    hardware.NoteOff(id1);
    Sleep(1000);
    hardware.NoteOff(id2);
    Sleep(1000);
    hardware.NoteOff(id3);

    Sleep(1000);
    id1 = hardware.NoteOn(Hardware::Clarinet, 440.0f, 0.25f);
    Sleep(1000);
    hardware.NoteOff(id1);
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
