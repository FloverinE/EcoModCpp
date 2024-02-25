#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "patch.h"
//#include "landscape.h"
//#include "tree.h"
#include <iostream>
#include <random>
#include <vector>
#include <QImage>  // Include the necessary header file
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

// setup procedure
// define the size of the map
const int xSize = 500;
const int ySize = 500;

void MainWindow::setup_map() {
    scene = new QGraphicsScene;
    // and hook the scene to main_map
    ui->main_map->setScene(scene);

    // declare and initialize an image
    ui->main_map->resize(xSize, ySize);
    image = QImage(xSize, ySize, QImage::Format_RGB32);
    image.fill(Qt::black);
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_setup_button_clicked()
{
    setup_map();
}


void MainWindow::on_go_button_clicked()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

