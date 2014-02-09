#include "mainwindow.h"
#include <QApplication>

#include "WvOut.h"
#include "Instrmnt.h"
#include "JCRev.h"
#include "Voicer.h"
#include "Skini.h"
#include "RtAudio.h"

using namespace stk;

int main(int argc, char *argv[])
{
    Stk::setSampleRate( 44100.0 );

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
