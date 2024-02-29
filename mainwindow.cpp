#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "patch.h"
#include "landscape.h"
#include "tree.h"
#include <iostream>
#include <random>
#include <vector>
#include <QImage>
#include <string>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

// setup procedure
// define the size of the map
const int x_size = 500;
const int y_size = 500;

QRgb color_birch = qRgb(165,42,42); // brown color
QRgb color_oak = qRgb(0, 128, 0); // green color
QRgb color_saplings = qRgb(255, 255, 255); // white color

void MainWindow::setup_map() {
    scene = new QGraphicsScene;
    // and hook the scene to main_map
    ui->main_map->setScene(scene);

    // declare and initialize an image
    ui->main_map->resize(x_size, y_size);
    image = QImage(x_size, y_size, QImage::Format_RGB32);
    image.fill(Qt::black);
    scene->addPixmap(QPixmap::fromImage(image));
}



void MainWindow::on_setup_button_clicked()
{
    setup_map();
    setup_patches();
    setup_trees();
}

std::vector<int> tree_ids;

std::vector<tree> trees;

void MainWindow::setup_trees() {
    trees.clear();
    int N_trees = ui->N_trees_spinBox->value();
    double species_ratio = ui->species_ratio_spinBox->value();
    int N_birch_trees = N_trees * species_ratio;
//    int N_oak_trees = N_trees - N_birch_trees; // not used since if_else
//    tree trees[N_trees];
    for (int i = 0; i < N_trees; ++i) {
        trees.emplace_back(i, std::vector<int>{}, 'b', 1, 10);
        trees[i].x_y_cor = {rand() % x_size, rand() % y_size};
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

    for (const auto& t : trees) {
        std::cout << "Tree ID: " << t.id << ", Species: " << t.species << std::endl;
    }
    scene->addPixmap(QPixmap::fromImage(image));
}

std::vector<patch> patches;

void MainWindow::setup_patches() {
    patches.clear();
    for (int i = 0; i < x_size; i++) {
        for (int j = 0; j < y_size; j++) {
            patches.emplace_back(std::to_string(i) + "_" + std::to_string(j), std::vector<int>{i, j}, 0);
//            cout << std::to_string(i) + "_" + std::to_string(j) << endl;
        }
    }
}



void MainWindow::perform_dispersal() {
    for (const auto& t : trees) {
        int real_seed_production = t.max_seed_production / 50;
        std::cout << real_seed_production << std::endl;
        for (int i = 0; i < real_seed_production; ++i) {
            double direction = 2 * M_PI * i / real_seed_production;
            int offset_x = static_cast<int>(t.dispersal_factor * cos(direction));
            int offset_y = static_cast<int>(t.dispersal_factor * sin(direction));

            int new_x = t.x_y_cor[0] + offset_x;    // calculate the new x coordinate where the seedling will be placed
            int new_y = t.x_y_cor[1] + offset_y;    // and calculate the new y coordinate

            image.setPixel(new_x, new_y, color_saplings);   // set the pixel to white
            string patch_coord = to_string(new_x) + "_" + to_string(new_y);     // create a string with the coordinates of the patch
            cout << patches[10].get_N_seedlings() << endl;
            for (auto& p : patches) {
                if (p.patch_id == patch_coord) {        // find the patch with the same coordinates
                    cout << p.get_N_seedlings() << endl;
                    cout << "patchid " + p.patch_id << endl;
                    p.update_N_seedlings(1);            // update the number of seedlings in the patch
//                    cout << p.get_N_seedlings() << endl;
                }
            }
        }
    }
    scene->addPixmap(QPixmap::fromImage(image));
}


void MainWindow::on_go_button_clicked()
{
    perform_dispersal();
}

MainWindow::~MainWindow()
{
    delete ui;
}

