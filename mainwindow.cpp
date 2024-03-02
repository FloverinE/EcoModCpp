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
#include <tuple>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}
// define the size of the map
const int x_size = 300;
const int y_size = 300;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dist_x(0, x_size - 1);
std::uniform_int_distribution<int> dist_y(0, y_size - 1);
std::uniform_real_distribution<float> dist_float(0.0f, 1.0f);

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
std::unordered_map<std::string, patch*> patch_map;  // Map to efficiently look up patches by coordinates


void MainWindow::setup_patches() {
    patches.clear();
    patch_map.clear();  // Clear the map before repopulating

    for (int i = 0; i < x_size; i++) {
        for (int j = 0; j < y_size; j++) {
            std::string patch_id = std::to_string(i) + "_" + std::to_string(j);
            patches.emplace_back(patch_id, std::vector<int>{i, j}, std::vector<int>{0, 0});

            // Populate the patch_map for efficient lookup
            patch_map[patch_id] = &patches.back();
        }
    }
   // use the reset_N_seedlings() function to reset the number of seedlings in all patches
    for (auto& p : patches) {
        p.reset_N_seeds();
    }
    std::cout << patches[1].N_seeds[1] << std::endl;
//    std::cout << "Number of patches: " << patches.size() << std::endl; // works
//    std::cout << "Number of patches in patch_map: " << patch_map.size() << std::endl; // works

}

void MainWindow::perform_dispersal() {
    std::string patch_coord;
    for (auto& t : trees) {
        int real_seed_production = t.max_seed_production * dist_float(gen);
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

                std::string patch_coord = "30_42";
                patch_coord = std::to_string(new_x) + '_' + std::to_string(new_y);
                std::cout << "Debug: Coordinates of seed: " << patch_coord << std::endl;
                //check if patch_coord is in patch_map
                if (patch_map.find(patch_coord) == patch_map.end()) {
                    std::cout << "Debug: Patch " << patch_coord << " not found in patch_map" << std::endl;
                    std::cout << "Debug: Coordinates of seed: " << patch_coord << std::endl;
                } else {
                    // Efficiently look up the patch by coordinates using the unordered_map
//                    patch_map[patch_coord]->update_N_seeds(1, t.species);
//                    patch_map[patch_coord]->update_N_seeds(1, 'b');
//                    cout << patches[1].get_N_oak_seeds() << endl;
                }
                for (auto& p : patches) {
                    if (p.x_y_cor[0] == new_x && p.x_y_cor[1] == new_y) {
                        p.update_N_seeds(100, t.species);
                        cout << p.get_N_oak_seeds() << endl;
                    }
                }
            }
        }
    }
    scene->addPixmap(QPixmap::fromImage(image));
}

void MainWindow::perform_pop_dynamics() {

    for(int i = 0; i < patches.size(); i++){
//        patches[i].update_N_seeds(100, 'o');
//        cout << "N oak seeds " << patches[i].patch_id << " " << patches[i].N_seeds[1] << endl; // Use index 1 for oak seeds
        for (int j = 0; j < 2; j++) {  // Use a different loop variable 'j'
            if(patches[i].N_seeds[j] > 0){
                for (int k = 0; k < patches[i].N_seeds[j]; k++) {
                    // Mortality
                    if (dist_float(gen) < patches[i].mortality_rate) {
                        patches[i].N_seeds[j] -= 1;
                        // Growth into next height class
                    } else if (dist_float(gen) < patches[i].growth_rate) {
                        patches[i].N_height_class_1[j] += 1;
                        patches[i].N_seeds[j] -= 1;
                    }
                }
                cout << "N oak seeds after mort and growth in patch" <<
                    patches[i].patch_id << " " << patches[i].N_height_class_1[1] << endl; // Use index 1 for oak seeds
            }
        }
        //        cout << "N oak seeds after mort and growth in patch" <<
        //            patches[i].patch_id << " " << patches[i].N_height_class_1[1] << endl; // Use index 1 for oak seeds
    }
}



void MainWindow::on_go_button_clicked()
{
    perform_dispersal();
    perform_pop_dynamics();
}

MainWindow::~MainWindow()
{
    delete ui;
}

