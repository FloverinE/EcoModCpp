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
#include <unordered_map>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}
// define the size of the map
const int x_size = 50;
const int y_size = 50;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dist_x(0, x_size - 1);
std::uniform_int_distribution<int> dist_y(0, y_size - 1);

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
        trees[i].x_y_cor = {dist_x(gen), dist_y(gen)};
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
std::unordered_map<std::string, patch*> patchMap;  // Map to efficiently look up patches by coordinates


void MainWindow::setup_patches() {
    patches.clear();
    patchMap.clear();  // Clear the map before repopulating

    for (int i = 0; i < x_size; i++) {
        for (int j = 0; j < y_size; j++) {
            std::string patch_id = std::to_string(i) + "_" + std::to_string(j);
            patches.emplace_back(patch_id, std::vector<int>{i, j});

            // Populate the patchMap for efficient lookup
            patchMap[patch_id] = &patches.back();
        }
    }
   // use the reset_N_seedlings() function to reset the number of seedlings in all patches
    for (auto& p : patches) {
        p.reset_N_seedlings();
    }
    std::cout << "Number of patches: " << patches.size() << std::endl;
    std::cout << "Number of patches in patchMap: " << patchMap.size() << std::endl;

}

void MainWindow::perform_dispersal() {
    std::string patch_coord;
    for (const auto& t : trees) {
        int real_seed_production = t.max_seed_production / 50;
        for (int i = 1; i <= real_seed_production; i++) {
            cout << "Debug: Seed " << i << " of tree " << t.id << endl;
            double direction = 2 * M_PI * i / real_seed_production;
            int offset_x = static_cast<int>(t.dispersal_factor * cos(direction));
            int offset_y = static_cast<int>(t.dispersal_factor * sin(direction));

            int new_x = t.x_y_cor[0] + offset_x;
            int new_y = t.x_y_cor[1] + offset_y;
            cout << "Debug: New coordinates: " << new_x << ", " << new_y << endl;
            if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size) {

                image.setPixel(new_x, new_y, color_saplings);

                patch_coord = "30_42";
//                patch_coord = std::to_string(new_x) + "_" + std::to_string(new_y);
                std::cout << "Debug: Coordinates of seedling: " << patch_coord << std::endl;
                //check if patch_coord is in patchMap
                if (patchMap.find(patch_coord) == patchMap.end()) {
                    std::cout << "Debug: Patch " << patch_coord << " not found in patchMap" << std::endl;
                    std::cout << "Debug: Coordinates of seedling: " << patch_coord << std::endl;
                } else {
                    // Efficiently look up the patch by coordinates using the unordered_map
                    std::cout << "Debug: old number of seedlings in patch " <<
                        patch_coord << ": " << patchMap[patch_coord]->get_N_seedlings() << std::endl;
                    patchMap[patch_coord]->update_N_seedlings(1);
                    std::cout << "Debug: Number of seedlings in patch " <<
                        patch_coord << ": " << patchMap[patch_coord]->get_N_seedlings() << std::endl;
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

