#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "controller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer m_ControllerTimer;
    Controller m_Controller;

    int m_TransposeAmount;
    int m_OctaveShift;

    std::vector<float> m_CentDeltas;

private slots:
  void InvokeController(void);
  void SetClarinetVoice();
  void SetFluteVoice();
  void SetBrassVoice();
  void SetSaxVoice();
  void SetBowedVoice();
  void SetTransposeC();
  void SetTransposeBFlat();
  void SetTransposeEFlat();
  void SetTransposeF();
  void SetHoldNotes(int value);
  void SetOctaveShift(int value);
  void SetReferencePitch();
  void SetCentDeltas();
  void SetCentDeltaText(int value);
};

#endif // MAINWINDOW_H
