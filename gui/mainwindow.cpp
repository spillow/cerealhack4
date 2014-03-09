#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_TransposeAmount(0),
    m_OctaveShift(0),
    m_CentDeltas(std::vector<float>(12, 0.f)),
    m_Volumes(std::vector<float>(12, 1.f))
{
    m_Controller.Initialize(0);

    ui->setupUi(this);
    QObject::connect(&m_ControllerTimer,
                     SIGNAL(timeout()), this,
                     SLOT(InvokeController()));

    ui->radio_clarinet_select->setChecked(true);
    ui->radio_transpose_c->setChecked(true);
    ui->checkbox_hold_notes->setChecked(false);
    ui->radioButton_Equal->setChecked(true);
    ui->lineEdit_reference_pitch->setText(QString::number(440));
    ui->radioButton_Key_C->setChecked(true);

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

#define ArraySize(x) (sizeof(x) / sizeof(x[0]))

void MainWindow::SetAllCentsFromTemperament()
{
    QRadioButton *intonationButtons[] = {
        ui->radioButton_Equal,
        ui->radioButton_Pure_Major,
        ui->radioButton_Pure_Minor
    };

    // cent deltas in the order of the above buttons
    float centDeltas[][12] = {
        { 0.f,      0.f,   0.f,   0.f,    0.f,   0.f,    0.f,   0.f,    0.f,    0.f,   0.f,    0.f },
        { 0.f,   -29.3f,  3.9f, 15.6f, -13.7f, -2.0f, -31.3f,  2.0f, -27.4f, -15.6f, 17.6f, -11.7f },
        //{ 15.6f, -13.7f, -2.0f, 31.3f,   2.0f, 13.7f, -15.6f, 17.6f, -11.7f,   0.0f, 33.2f,   3.9f }
        { 0.f,    33.2f,  3.9f, 15.6f, -13.7f, -2.0f,  31.3f,  2.0f,  13.7f, -15.6f, 17.6f, -11.7f }
    };

    QRadioButton *keyButtons[] = {
        ui->radioButton_Key_C,
        ui->radioButton_Key_Cs,
        ui->radioButton_Key_D,
        ui->radioButton_Key_Ds,
        ui->radioButton_Key_E,
        ui->radioButton_Key_F,
        ui->radioButton_Key_Fs,
        ui->radioButton_Key_G,
        ui->radioButton_Key_Gs,
        ui->radioButton_Key_A,
        ui->radioButton_Key_As,
        ui->radioButton_Key_B
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

    unsigned temperamentMode = 0;
    for (unsigned i=0; i < ArraySize(intonationButtons); i++) {
        if (intonationButtons[i]->isChecked()) {
            temperamentMode = i;
            break;
        }
    }

    unsigned key = 0;
    for (unsigned i=0; i < ArraySize(keyButtons); i++) {
        if (keyButtons[i]->isChecked()) {
            key = i;
            break;
        }
    }

    std::vector<float> deltas(centDeltas[temperamentMode], centDeltas[temperamentMode] + 12);

    if (key != 0) {
        std::rotate(deltas.begin(), deltas.begin() + 12 - key, deltas.end());
    }

    for (unsigned i=0; i < ArraySize(verticalSliders); i++) {
        verticalSliders[i]->setValue(int(deltas[i] * 10.0f)); // scale it; sliders only take ints
    }

    SetCentDeltaText(0);
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

    for (unsigned i=0; i < ArraySize(pitchBoxes); i++) {
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

void MainWindow::SetVolumeSlider(int /*value*/)
{
    QLineEdit* pitchBoxes[] = {
        ui->lineEdit_Volume_C,
        ui->lineEdit_Volume_Cs,
        ui->lineEdit_Volume_D,
        ui->lineEdit_Volume_Ds,
        ui->lineEdit_Volume_E,
        ui->lineEdit_Volume_F,
        ui->lineEdit_Volume_Fs,
        ui->lineEdit_Volume_G,
        ui->lineEdit_Volume_Gs,
        ui->lineEdit_Volume_A,
        ui->lineEdit_Volume_As,
        ui->lineEdit_Volume_B
    };

    QSlider* verticalSliders[] = {
        ui->verticalSlider_Volume_C,
        ui->verticalSlider_Volume_Cs,
        ui->verticalSlider_Volume_D,
        ui->verticalSlider_Volume_Ds,
        ui->verticalSlider_Volume_E,
        ui->verticalSlider_Volume_F,
        ui->verticalSlider_Volume_Fs,
        ui->verticalSlider_Volume_G,
        ui->verticalSlider_Volume_Gs,
        ui->verticalSlider_Volume_A,
        ui->verticalSlider_Volume_As,
        ui->verticalSlider_Volume_B
    };

    for (unsigned i=0; i < ArraySize(pitchBoxes); i++) {
        const float value = float(verticalSliders[i]->value());
        pitchBoxes[i]->setText(QString::number(value));
    }

    SetVolumeLineEdit();
}

void MainWindow::SetVolumeLineEdit()
{
    QLineEdit* pitchBoxes[] = {
        ui->lineEdit_Volume_C,
        ui->lineEdit_Volume_Cs,
        ui->lineEdit_Volume_D,
        ui->lineEdit_Volume_Ds,
        ui->lineEdit_Volume_E,
        ui->lineEdit_Volume_F,
        ui->lineEdit_Volume_Fs,
        ui->lineEdit_Volume_G,
        ui->lineEdit_Volume_Gs,
        ui->lineEdit_Volume_A,
        ui->lineEdit_Volume_As,
        ui->lineEdit_Volume_B
    };

    QSlider* verticalSliders[] = {
        ui->verticalSlider_Volume_C,
        ui->verticalSlider_Volume_Cs,
        ui->verticalSlider_Volume_D,
        ui->verticalSlider_Volume_Ds,
        ui->verticalSlider_Volume_E,
        ui->verticalSlider_Volume_F,
        ui->verticalSlider_Volume_Fs,
        ui->verticalSlider_Volume_G,
        ui->verticalSlider_Volume_Gs,
        ui->verticalSlider_Volume_A,
        ui->verticalSlider_Volume_As,
        ui->verticalSlider_Volume_B
    };

    for (unsigned i=0; i < ArraySize(pitchBoxes); i++) {
        QString deltaStr = pitchBoxes[i]->displayText();
        bool ok;
        const float delta = deltaStr.toFloat(&ok);
        if (ok) {
            const int maxValue = verticalSliders[i]->maximum();
            const int minValue = verticalSliders[i]->minimum();
            const float clampedDelta = clamp(delta, float(maxValue), float(minValue));
            m_Volumes[i] = clampedDelta;
            verticalSliders[i]->setValue(int(clampedDelta));
        }
        // TODO: format this so it always has one decimal place.
        pitchBoxes[i]->setText(QString::number(m_Volumes[i]));
    }

    m_Controller.SetVolumes(&m_Volumes[0]);
}
