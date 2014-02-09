#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
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
