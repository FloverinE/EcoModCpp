#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "patch.h"
#include "landscape.h"
#include "tree.h"
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

QRgb color_tree_base = qRgb(165,42,42); // brown color
QRgb color_birch = qRgb(255, 255, 255); // white color
QRgb color_oak = qRgb(0, 128, 0); // green color

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
    setup_trees();
}

std::vector<int> tree_ids;
void MainWindow::setup_trees() {
    int N_trees = ui->N_trees_spinBox->value();
    double species_ratio = ui->species_ratio_spinBox->value();
    int N_birch_trees = N_trees * species_ratio;
//    int N_oak_trees = N_trees - N_birch_trees; // not used since if_else
    tree trees[N_trees];
    for (int i = 0; i < N_trees; ++i) {
        trees[i].x_y_cor = {rand() % xSize, rand() % ySize};
        if (i <= N_birch_trees){
            trees[i].species = 'b';
            image.setPixel(trees[i].x_y_cor[0], trees[i].x_y_cor[1], color_birch);
        } else {
            trees[i].species = 'o';
            image.setPixel(trees[i].x_y_cor[0], trees[i].x_y_cor[1], color_oak);
        }
        trees[i].id = i;
        tree_ids.push_back(i);
        trees[i].update_species_params();
    }

    cout << N_trees << endl;
    scene->addPixmap(QPixmap::fromImage(image));

}

void MainWindow::perform_dispersal() {
    int N_trees = ui->N_trees_spinBox->value();

    for (int i = 0; i < N_trees; ++i) {

        double angle = 2 * M_PI * i / numPoints;
        offset_x = trees[i].dispersal_factor * cos(direction);
        offset_y = trees[i].dispersal_factor * sin(direction);

    }
}

void MainWindow::on_go_button_clicked()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

