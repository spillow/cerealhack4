#include "mainwindow.h"
#include <QApplication>

#include "hardware.h"
#include "controller.h"
#include "midireceiver.h"

int main(int argc, char *argv[])
{
    Hardware hardware;
    hardware.Initialize();

    Controller controller(hardware);

    MidiReceiver receiver(controller);
    receiver.Initialize();

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
