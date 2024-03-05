// todo
// more height classes per species


/*
 population density graphs
 + first graph for N_seeds
 - all height classes
*/


// light availability changing with distance to tree
// - not yet

// water availability changing with percent of deadwood remaining
// - not yet

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

// constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    N_seeds_chart = new QChart();
    ui->N_seeds_chart->setChart(N_seeds_chart);
    ui->N_seeds_chart->setRenderHint(QPainter::Antialiasing); // looks prettier
    N_height_class_chart = new QChart();
    ui->N_height_class_chart->setChart(N_height_class_chart);
    ui->N_height_class_chart->setRenderHint(QPainter::Antialiasing); // looks prettier
}

// destructor
MainWindow::~MainWindow()
{
    delete ui;
    delete N_seeds_chart;
    delete N_height_class_chart;
}

// define the size of the map
const int x_size = 300; //
const int y_size = 300;

std::random_device rd;  // used to obtain a seed for the random number engine
std::mt19937 gen(rd()); // standard mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<int> dist_x(0, x_size - 1);   //
std::uniform_int_distribution<int> dist_y(0, y_size - 1);
std::uniform_real_distribution<float> dist_float(0.0f, 1.0f);


QRgb color_seeds = qRgb(255, 255, 255); // white color


//
void MainWindow::on_setup_button_clicked()
{
    setup_map();
    setup_patches();
    setup_trees();
    setup_min_distance_to_tree();
    update_map();
    count_populations();
    clear_charts();
}


void MainWindow::on_go_button_clicked()
{
    perform_dispersal();
    for(int i = 0; i < 10; i++){
            perform_pop_dynamics();
            count_populations();
    }
    draw_charts();
}

void MainWindow::setup_map() {
    scene = new QGraphicsScene;
    // and hook the scene to main_map
    ui->main_map->setScene(scene);

    // declare and initialize an image
    ui->main_map->resize(x_size, y_size);
    image = QImage(x_size, y_size, QImage::Format_RGB32);
    image.fill(Qt::white);
    scene->addPixmap(QPixmap::fromImage(image));
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
            trees[i].update_species_params();
            image.setPixel(trees[i].x_y_cor[0], trees[i].x_y_cor[1], trees[i].color);
        } else {
            trees[i].species = 'o';
            trees[i].update_species_params();
            image.setPixel(trees[i].x_y_cor[0], trees[i].x_y_cor[1], trees[i].color);
        }
        trees[i].id = i;
        tree_ids.push_back(i);
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

    //    // compute distance to tree
    //    for (auto& t : trees) {
    //        for (auto& p : patches) {
    ////            float distance = sqrt(pow(t.x_y_cor[0] - p.x_y_cor[0], 2) + pow(t.x_y_cor[1] - p.x_y_cor[1], 2));
    ////            p.distance_to_tree[t.id] = distance;
    //            std::vector<float> distances = {0};
    //            // push back distances per patch to tree relation
    //            distances.push_back(p.set_distance_to_tree(t.x_y_cor[0], t.x_y_cor[1]));
    //            auto min_distance = std::min_element(distances.begin(), distances.end());

    //            cout << "Debug: Distance to tree " << t.id << " from patch " << p.patch_id << " is " << distances[1] << endl;
    ////            p.distance_to_tree = p.set_distance_to_tree(t.x_y_cor[0], t.x_y_cor[1]);

    //        }
    //    }
}

//// compute distance to tree
//void MainWindow::setup_distance_to_tree() {
//    for (auto& t : trees) {
//        for (auto& p : patches) {
//            std::vector<float> distances = {0};
//            float distance = sqrt(pow(t.x_y_cor[0] - p.x_y_cor[0], 2) + pow(t.x_y_cor[1] - p.x_y_cor[1], 2));
//            p.distance_to_tree[t.id] = distance;
//        }
//    }
//}


#include <vector>
#include <limits>  // for std::numeric_limits

// distance map of min distance to trees for mapping colors
std::vector<std::vector<double>> distance_map(y_size, std::vector<double>(x_size, 0.0));

void MainWindow::setup_min_distance_to_tree() {
    if (trees.empty()) {
        std::cerr << "Error: No trees to compute distance to" << std::endl;
        return;
    } else {
        for (auto& p : patches) {
            std::vector<float> distances(trees.size(), 0.0f);  // Initialize distances with 0 for each patch

            for (unsigned int i = 0; i < trees.size(); ++i) {
                double distance = sqrt(pow(trees[i].x_y_cor[0] - p.x_y_cor[0], 2) + pow(trees[i].x_y_cor[1] - p.x_y_cor[1], 2));
                distances[i] = distance;
            }
            auto min_distance_iter = std::min_element(distances.begin(), distances.end());
            // Store the minimum distance
            p.distance_to_tree = *min_distance_iter;
            const QColor color(0, 255 * p.distance_to_tree / 425.0, 0); // (distance / 1000.0)
            image.setPixelColor(p.x_y_cor[0], p.x_y_cor[1], color);
        }
        cout << "Debug: Min distance to tree from patch 0_0 is " << patches[0].distance_to_tree << endl;
        //    double distance = patch_map[patch_coord]->distance_to_tree;

        //    double min_distance = 0.0;
        //    double max_distance = sqrt(pow(x_size, 2) + pow(y_size, 2));
        //    // Normalize the distance to be between 0 and 1
        //    double normalized_distance = (14.0 - min_distance) / (max_distance - min_distance);
        //    cout << normalized_distance << endl;


        //        // color the patches on a gradient based on distance to tree
        //        for (unsigned x = 0; x < x_size; x++) {
        //            for (unsigned y = 0; y < x_size; y++) {
        //                string patch_coord = std::to_string(x) + "_" + std::to_string(y);
        //                double distance = patch_map[patch_coord]->distance_to_tree;

        ////                double min_distance = 0.0;
        ////                //            double max_distance = sqrt(pow(x_size, 2) + pow(y_size, 2));
        ////                double max_distance = 1000.0;

        ////                // Normalize the distance to be between 0 and 1
        ////                double normalized_distance = (distance - min_distance) / (max_distance - min_distance);
        //                cout << distance << endl;
        //                distance_map[x][y] = distance;
        //                const QColor color(0, 255 * dist_float(gen), 0); // (distance / 1000.0)
        //                image.setPixelColor(x, y, color);
        //            }
        //        }
    }
    //    cout << dist_float(gen) * 255 << endl;
    scene->addPixmap(QPixmap::fromImage(image));
}


void MainWindow::update_map(){ // not used as of now
    for(int x = 0; x < x_size; x++){
        for(int y = 0; y < y_size; y++){
            const QColor color(0, 128 * dist_float(gen), 0);
            //            image.setPixelColor(x, y, color);
        }
    }

    for (const auto& t : trees) {
        image.setPixel(t.x_y_cor[0], t.x_y_cor[1], t.color);
    }
    //    scene->addPixmap(QPixmap::fromImage(image));
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

                image.setPixel(new_x, new_y, color_seeds);

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
    for(unsigned int i = 0; i < patches.size(); i++){
        //        patches[i].update_N_seeds(100, 'o');
        //        cout << "N oak seeds " << patches[i].patch_id << " " << patches[i].N_seeds[1] << endl; // Use index 1 for oak seeds
        for (int j = 0; j < 2; j++) {
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

// Vectors to store the population counts as sum of all patches at each time step
std::vector<std::vector<int>> N_seeds_total;    // Seeds
std::vector<std::vector<int>> N_hc1_total;      // Height class 1

void MainWindow::count_populations() {
    // Create a vector to store the counts for the current time step
    std::vector<int> seed_counts = {0, 0};
    std::vector<int> hc1_counts  = {0, 0};

    // Iterate through patches and update counts
    for (const auto& p : patches) {
        seed_counts[0] += p.N_seeds[0];
        seed_counts[1] += p.N_seeds[1];

        hc1_counts[0] += p.N_height_class_1[0];
        hc1_counts[1] += p.N_height_class_1[1];
    }
    // Push back the counts for the current time step
    N_seeds_total.push_back(seed_counts);
    N_hc1_total.push_back(hc1_counts);
}


void MainWindow::clear_charts()
{
    //clear all output vectors
    //    N_seeds_vector.clear();

    // clear N_seeds chart
    N_seeds_chart->removeAllSeries(); // also calls delete
    N_height_class_chart->removeAllSeries(); // also calls delete
}

void MainWindow::draw_charts(){
    // draw N_seeds chart
    QLineSeries *N_birch_seeds_series = new QLineSeries();
    N_birch_seeds_series->setColor(Qt::black); // default color: blue
    N_birch_seeds_series->setName("Number of birch seeds per hectare");

    QLineSeries *N_oak_seeds_series = new QLineSeries();
    N_oak_seeds_series->setColor(Qt::black); // default color: blue
    N_oak_seeds_series->setName("Number of oak seeds per hectare");

    for (int time = 0; time < 10; time++) {
        N_birch_seeds_series->append(time, N_seeds_total[time][0]);
        cout << "N seeds total: " << N_seeds_total[time][0] << endl;
        N_oak_seeds_series->append(time, N_seeds_total[time][1]);
        cout << "N seeds total: " << N_seeds_total[time][1] << endl;
    }
    N_seeds_chart->addSeries(N_birch_seeds_series);
    N_seeds_chart->addSeries(N_oak_seeds_series);

    N_seeds_chart->createDefaultAxes();
    N_seeds_chart->axisX()->setTitleText("Time [steps]");
    N_seeds_chart->axisY()->setTitleText("N seeds [ha]");

    // draw height class 1 chart
    QLineSeries *N_birch_hc1_series = new QLineSeries();
    N_birch_hc1_series->setColor(Qt::black); // default color: blue
    N_birch_hc1_series->setName("Number of birch saplings height class 1 per hectare");

    QLineSeries *N_oak_hc1_series = new QLineSeries();
    N_oak_hc1_series->setColor(Qt::black); // default color: blue
    N_oak_hc1_series->setName("Number of oak saplings height class 1 per hectare");

    for (int time = 0; time < 10; time++) {
        N_birch_hc1_series->append(time, N_hc1_total[time][0]);
        cout << "N hc1 total: " << N_hc1_total[time][0] << endl;
        N_oak_hc1_series->append(time, N_hc1_total[time][1]);
        cout << "N hc1 total: " << N_hc1_total[time][1] << endl;
    }

    N_height_class_chart->addSeries(N_birch_hc1_series);
    N_height_class_chart->addSeries(N_oak_hc1_series);

    N_height_class_chart->createDefaultAxes();
    N_height_class_chart->axisX()->setTitleText("Time [steps]");
    N_height_class_chart->axisY()->setTitleText("N saplings height class 1 [ha]");
}


