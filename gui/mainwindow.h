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

private slots:
  void turn_on(void);
  void turn_off(void);
  void InvokeController(void);
};

#endif // MAINWINDOW_H
