#include "mainwindow.h"
#include "qmediaplayer.h"
#include "ui_mainwindow.h"
#include "music.h"
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
