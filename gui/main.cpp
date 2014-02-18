#include "mainwindow.h"
#include <QApplication>

#include "hardware.h"
#include "controller.h"
#include "midireceiver.h"

int main(int argc, char *argv[])
{
    Controller controller;
    controller.Initialize();

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
