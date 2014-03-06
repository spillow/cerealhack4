#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_TransposeAmount(0),
    m_OctaveShift(0),
    m_CentDeltas(std::vector<float>(12, 0.f))
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

static float clamp(float val, float max, float min)
{
    if (val > max) {
        return max;
    }
    else if (val < min) {
        return min;
    }

    return val;
}

void MainWindow::SetCentDeltaText(int /*value*/)
{
    QLineEdit* pitchBoxes[] = {
        ui->lineEdit_Pitch_C,
        ui->lineEdit_Pitch_Cs,
        ui->lineEdit_Pitch_D,
        ui->lineEdit_Pitch_Ds,
        ui->lineEdit_Pitch_E,
        ui->lineEdit_Pitch_F,
        ui->lineEdit_Pitch_Fs,
        ui->lineEdit_Pitch_G,
        ui->lineEdit_Pitch_Gs,
        ui->lineEdit_Pitch_A,
        ui->lineEdit_Pitch_As,
        ui->lineEdit_Pitch_B
    };

    QSlider* verticalSliders[] = {
        ui->verticalSlider_Pitch_C,
        ui->verticalSlider_Pitch_Cs,
        ui->verticalSlider_Pitch_D,
        ui->verticalSlider_Pitch_Ds,
        ui->verticalSlider_Pitch_E,
        ui->verticalSlider_Pitch_F,
        ui->verticalSlider_Pitch_Fs,
        ui->verticalSlider_Pitch_G,
        ui->verticalSlider_Pitch_Gs,
        ui->verticalSlider_Pitch_A,
        ui->verticalSlider_Pitch_As,
        ui->verticalSlider_Pitch_B
    };

    for (unsigned i=0; i < sizeof(pitchBoxes) / sizeof(pitchBoxes[0]); i++) {
        const float value = float(verticalSliders[i]->value()) / 10.0f;
        pitchBoxes[i]->setText(QString::number(value));
    }

    SetCentDeltas();
}

void MainWindow::SetCentDeltas()
{
    QLineEdit* pitchBoxes[] = {
        ui->lineEdit_Pitch_C,
        ui->lineEdit_Pitch_Cs,
        ui->lineEdit_Pitch_D,
        ui->lineEdit_Pitch_Ds,
        ui->lineEdit_Pitch_E,
        ui->lineEdit_Pitch_F,
        ui->lineEdit_Pitch_Fs,
        ui->lineEdit_Pitch_G,
        ui->lineEdit_Pitch_Gs,
        ui->lineEdit_Pitch_A,
        ui->lineEdit_Pitch_As,
        ui->lineEdit_Pitch_B
    };

    QSlider* verticalSliders[] = {
        ui->verticalSlider_Pitch_C,
        ui->verticalSlider_Pitch_Cs,
        ui->verticalSlider_Pitch_D,
        ui->verticalSlider_Pitch_Ds,
        ui->verticalSlider_Pitch_E,
        ui->verticalSlider_Pitch_F,
        ui->verticalSlider_Pitch_Fs,
        ui->verticalSlider_Pitch_G,
        ui->verticalSlider_Pitch_Gs,
        ui->verticalSlider_Pitch_A,
        ui->verticalSlider_Pitch_As,
        ui->verticalSlider_Pitch_B
    };

    for (unsigned i=0; i < sizeof(pitchBoxes) / sizeof(pitchBoxes[0]); i++) {
        QString deltaStr = pitchBoxes[i]->displayText();
        bool ok;
        const float delta = deltaStr.toFloat(&ok);
        if (ok) {
            const int maxValue = verticalSliders[i]->maximum();
            const int minValue = verticalSliders[i]->minimum();
            const float clampedDelta = clamp(delta, float(maxValue) / 10.0f, float(minValue) / 10.0f);
            m_CentDeltas[i] = clampedDelta;
            verticalSliders[i]->setValue(int(clampedDelta * 10.0f));
        }
        // TODO: format this so it always has one decimal place.
        pitchBoxes[i]->setText(QString::number(m_CentDeltas[i]));
    }

    m_Controller.SetIntonation(&m_CentDeltas[0]);
}

