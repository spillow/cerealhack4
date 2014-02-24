#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_TransposeAmount(0),
    m_OctaveShift(0)
{
    m_Controller.Initialize(0);

    ui->setupUi(this);
    QObject::connect(&m_ControllerTimer,
                     SIGNAL(timeout()), this,
                     SLOT(InvokeController()));

    ui->radio_clarinet_select->setChecked(true);
    ui->radio_transpose_c->setChecked(true);
    ui->checkbox_hold_notes->setChecked(false);

    m_ControllerTimer.start(1);
}

void MainWindow::InvokeController()
{
    m_Controller.RunIteration();
}

MainWindow::~MainWindow()
{
    m_ControllerTimer.stop();
    delete ui;
}

void MainWindow::SetClarinetVoice()
{
    m_Controller.SetVoice(Hardware::Clarinet);
}

void MainWindow::SetFluteVoice()
{
    m_Controller.SetVoice(Hardware::Flute);
}

void MainWindow::SetBrassVoice()
{
    m_Controller.SetVoice(Hardware::Brass);
}

void MainWindow::SetSaxVoice()
{
    m_Controller.SetVoice(Hardware::Saxofony);
}

void MainWindow::SetBowedVoice()
{
    m_Controller.SetVoice(Hardware::Bowed);
}

void MainWindow::SetTransposeC()
{
    m_TransposeAmount = +0;
    int amount = m_OctaveShift*12 + m_TransposeAmount;
    m_Controller.SetTransposeAmount(amount);
}

void MainWindow::SetTransposeBFlat()
{
    m_TransposeAmount = -2;
    int amount = m_OctaveShift*12 + m_TransposeAmount;
    m_Controller.SetTransposeAmount(amount);
}

void MainWindow::SetTransposeEFlat()
{
    m_TransposeAmount = +3;
    int amount = m_OctaveShift*12 + m_TransposeAmount;
    m_Controller.SetTransposeAmount(amount);
}

void MainWindow::SetTransposeF()
{
    m_TransposeAmount = +5;
    int amount = m_OctaveShift*12 + m_TransposeAmount;
    m_Controller.SetTransposeAmount(amount);
}

void MainWindow::SetHoldNotes(int value)
{
    m_Controller.SetSustain(value);
}

void MainWindow::SetOctaveShift(int value)
{
    m_OctaveShift = value;
    int amount = m_OctaveShift*12 + m_TransposeAmount;
    m_Controller.SetTransposeAmount(amount);
}

void MainWindow::SetReferencePitch()
{
    // TODO: sanitize the input.
    QString freqStr = ui->lineEdit_reference_pitch->displayText();
    bool ok;
    const float freq = freqStr.toFloat(&ok);
    if (ok) {
        m_Controller.SetStandardPitch(freq);
    }
}
