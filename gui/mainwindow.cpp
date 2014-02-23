#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_Controller.Initialize(0);

    ui->setupUi(this);
    QObject::connect(&m_ControllerTimer,
                     SIGNAL(timeout()), this,
                     SLOT(InvokeController()));

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

void MainWindow::turn_on(void)
{
  ui->button_on->setDown(true);
  ui->button_off->setDown(false);
}

void MainWindow::turn_off(void)
{
  ui->button_on->setDown(false);
  ui->button_off->setDown(true);
}
