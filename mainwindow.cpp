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
    N_birch_pop_chart = new QChart();
    ui->N_birch_pop_chart->setChart(N_birch_pop_chart);
    ui->N_birch_pop_chart->setRenderHint(QPainter::Antialiasing); // looks prettier
    N_oak_pop_chart = new QChart();
    ui->N_oak_pop_chart->setChart(N_oak_pop_chart);
    ui->N_oak_pop_chart->setRenderHint(QPainter::Antialiasing); // looks prettier
}

// destructor
MainWindow::~MainWindow()
{
    delete ui;
    delete N_birch_pop_chart;
    delete N_oak_pop_chart;
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
    int number_of_simulation_years = ui->N_years_spinBox->value();
    for(int i = 0; i < number_of_simulation_years; i++){
        perform_dispersal();
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
    int N_birch_trees = N_trees * (1 - species_ratio);

    for (int i = 0; i < N_trees; ++i) { // loop over the number of trees
        trees.emplace_back(i, std::vector<int>{}, 'b', 1, 10);  // create dummy tree object
        trees[i].x_y_cor = {dist_x(gen), dist_y(gen)};          // assign random x and y coordinates
        if (i < N_birch_trees){                                 // assign birch based on species_ratio
            trees[i].species = 'b';                             // birch
            trees[i].update_species_params();                   // update species parameters
            image.setPixel(trees[i].x_y_cor[0], trees[i].x_y_cor[1], trees[i].color); // set pixel color
        } else {                                                // rest is oak
            trees[i].species = 'o';
            trees[i].update_species_params();
            image.setPixel(trees[i].x_y_cor[0], trees[i].x_y_cor[1], trees[i].color);
        }
        trees[i].id = i;                                        // assign tree id
        tree_ids.push_back(i);                                  // add tree id to tree_ids vector
    }
    scene->addPixmap(QPixmap::fromImage(image));                // update the map
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
    //    for (auto& p : patches) {
    //        p.reset_N_seeds();
    //    }
    //    std::cout << patches[1].N_seeds[1] << std::endl;
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
    if (trees.empty()) {                                       // check if there are any trees to compute distance to
        std::cerr << "Error: No trees to compute distance to" << std::endl;
        return;
    } else {                                                   // if there are trees, compute the distance to each patch
        for (auto& p : patches) {
            std::vector<float> distances(trees.size(), 0.0f);  // initialize distances with 0 for each patch

            for (unsigned int i = 0; i < trees.size(); ++i) {  // loop over the trees
                double distance = sqrt(pow(trees[i].x_y_cor[0] - p.x_y_cor[0], 2) + pow(trees[i].x_y_cor[1] - p.x_y_cor[1], 2));
                distances[i] = distance;
            }

            auto min_distance_iter = std::min_element(distances.begin(), distances.end());
            p.distance_to_tree = *min_distance_iter;                    // store the minimum distance

            const QColor color(0, 255 * p.distance_to_tree / 425.0, 0); // color based on distance to tree
            image.setPixelColor(p.x_y_cor[0], p.x_y_cor[1], color);     // set pixel color
        }
        cout << "Debug: Min distance to tree from patch 0_0 is " << patches[0].distance_to_tree << endl;
    }
    scene->addPixmap(QPixmap::fromImage(image)); // update the map
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
    for (auto& t : trees) {
        int real_seed_production = t.max_seed_production * dist_float(gen);     // real seed production as random number * max seed production
        for (int i = 1; i <= real_seed_production; i++) {
            double direction = 2 * M_PI * i / real_seed_production;             // direction of seed dispersal

            double distance_decay = std::pow(2, -3 * dist_float(gen));          // distance decay of seed dispersal

            int offset_x = static_cast<int>(t.dispersal_factor * distance_decay * cos(direction));
            int offset_y = static_cast<int>(t.dispersal_factor * distance_decay * sin(direction));

            int new_x = t.x_y_cor[0] + offset_x;
            int new_y = t.x_y_cor[1] + offset_y;

            // check if seed landed in the map extent
            if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size) {

                image.setPixel(new_x, new_y, color_seeds);

                for (auto& p : patches) { //loop over patches to find the patch where the seed landed
                    if (p.x_y_cor[0] == new_x && p.x_y_cor[1] == new_y) {
                        p.update_N_seeds(1, t.species);
                        //                        cout << p.get_N_oak_seeds() << endl;
                    }
                }
            }
        }
    }
    scene->addPixmap(QPixmap::fromImage(image));
}

//void MainWindow::perform_pop_dynamics() {
//    for(unsigned int i = 0; i < patches.size(); i++){
//        //        patches[i].update_N_seeds(100, 'o');
//        //        cout << "N oak seeds " << patches[i].patch_id << " " << patches[i].N_seeds[1] << endl; // Use index 1 for oak seeds
//        for (int j = 0; j < 2; j++) {
//            if(patches[i].N_seeds[j] > 0){
//                for (int k = 0; k < patches[i].N_seeds[j]; k++) {
//                    // Mortality
//                    if (dist_float(gen) < patches[i].mortality_rate) {
//                        patches[i].N_seeds[j] -= 1;
//                        patches[i].N_height_class_1[j] -= 1;
//                        patches[i].N_height_class_2[j] -= 1;
//                        patches[i].N_height_class_3[j] -= 1;
//                        patches[i].N_height_class_4[j] -= 1;

//                        // Growth into next height class
//                    } else if (dist_float(gen) < patches[i].growth_rate) {
//                        patches[i].N_height_class_1[j] += 1;
//                        patches[i].N_seeds[j] -= 1;
//                    }
//                }
//                cout << "N oak seeds after mort and growth in patch" <<
//                    patches[i].patch_id << " " << patches[i].N_height_class_1[1] << endl; // Use index 1 for oak seeds
//            }
//        }
//        //        cout << "N oak seeds after mort and growth in patch" <<
//        //            patches[i].patch_id << " " << patches[i].N_height_class_1[1] << endl; // Use index 1 for oak seeds
//    }
//}



/*
 Function to perform population dynamics on the patches
 working like a matrix model with 5 stages (seeds -> germination -> height class 1 to 4)
 probabilistic mortality and growth into next stages, rest is survival
 first: advancement of height class 3 into 4 to not have saplings from height class 2
        advancing and dying at the same time
 next:  continue with height class 3 down to the seeds

 possible extension:
 - implement growth rate dependent on height class,
    i.e. higher growth rate for lower height classes but lower if the taller saplings create too much shade
 */

void MainWindow::perform_pop_dynamics() {
    for(unsigned int i = 0; i < patches.size(); i++){ // loop over all patches
        for (int j = 0; j < 2; j++) {       // loop over both species => birch 0 and oak 1
            // first height class 4 mortality as no further growth is implemented
            if(patches[i].N_height_class_4[j] > 0){   // only continue if there is at least 1 sapling in height class 4
                for(int k = 0; k < patches[i].N_height_class_4[j]; k++){ // loop over all saplings in height class 4
                    if(dist_float(gen) < patches[i].mortality_rate){     // probability check for mortality
                        patches[i].N_height_class_4[j] -= 1;             // if passed, sapling dies
                    }
                }
            }
            // same for height class 3 and so on
            if(patches[i].N_height_class_3[j] > 0){   // only continue if there is at least 1 sapling in height class 3
                for(int k = 0; k < patches[i].N_height_class_3[j]; k++){ // loop over all saplings in height class 3
                    if(dist_float(gen) < patches[i].mortality_rate){     // probability check for mortality
                        patches[i].N_height_class_3[j] -= 1;             // if passed, sapling dies
                    }
                    if(dist_float(gen) < patches[i].growth_rate){        // probability check for growth
                        patches[i].N_height_class_4[j] += 1;             // if passed, sapling advances to height class 4
                        patches[i].N_height_class_3[j] -= 1;             // and is removed from height class 3
                    }
                }
            }
            if(patches[i].N_height_class_2[j] > 0){
                for(int k = 0; k < patches[i].N_height_class_2[j]; k++){
                    if(dist_float(gen) < patches[i].mortality_rate){
                        patches[i].N_height_class_2[j] -= 1;
                    }
                    if(dist_float(gen) < patches[i].growth_rate){
                        patches[i].N_height_class_3[j] += 1;
                        patches[i].N_height_class_2[j] -= 1;
                    }
                }
            }
            if(patches[i].N_height_class_1[j] > 0){
                for(int k = 0; k < patches[i].N_height_class_1[j]; k++){
                    if(dist_float(gen) < patches[i].mortality_rate){
                        patches[i].N_height_class_1[j] -= 1;
                    }
                    if(dist_float(gen) < patches[i].growth_rate){
                        patches[i].N_height_class_2[j] += 1;
                        patches[i].N_height_class_1[j] -= 1;
                    }
                }
            }
            if(patches[i].N_seeds[j] > 0){
                for (int k = 0; k < patches[i].N_seeds[j]; k++) {
                    if (dist_float(gen) < patches[i].mortality_rate) {
                        patches[i].N_seeds[j] -= 1;
                    }
                    if (dist_float(gen) < patches[i].growth_rate) {
                        patches[i].N_height_class_1[j] += 1;
                        patches[i].N_seeds[j] -= 1;
                    }
                }
            }
        }
    }
}


// Vectors to store the population counts as sum of all patches at each time step
std::vector<std::vector<int>> N_seeds_total;    // Seeds
std::vector<std::vector<int>> N_hc1_total;      // Height class 1

std::vector<std::vector<int>> birch_pop_total; // Birch population
std::vector<std::vector<int>> oak_pop_total;   // Oak population

void MainWindow::count_populations() {
    std::vector<int> birch_pop = {0, 0, 0, 0, 0};
    std::vector<int> oak_pop = {0, 0, 0, 0, 0};

    for (const auto& p : patches) {
        birch_pop[0] += p.N_seeds[0];
        birch_pop[1] += p.N_height_class_1[0];
        birch_pop[2] += p.N_height_class_2[0];
        birch_pop[3] += p.N_height_class_3[0];
        birch_pop[4] += p.N_height_class_4[0];

        oak_pop[0] += p.N_seeds[1];
        oak_pop[1] += p.N_height_class_1[1];
        oak_pop[2] += p.N_height_class_2[1];
        oak_pop[3] += p.N_height_class_3[1];
        oak_pop[4] += p.N_height_class_4[1];
    }
    birch_pop_total.push_back(birch_pop);
    oak_pop_total.push_back(oak_pop);

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

    // clear charts for setup
    //    N_seeds_chart->removeAllSeries();
    //    N_height_class_chart->removeAllSeries();
    N_birch_pop_chart->removeAllSeries();
    N_oak_pop_chart->removeAllSeries();

}

void MainWindow::draw_charts(){
    QLineSeries *N_birch_seeds_series = new QLineSeries();
    N_birch_seeds_series->setColor(Qt::black); // default color: blue
    N_birch_seeds_series->setName("Seeds");
    QLineSeries *N_birch_hc1_series = new QLineSeries();
    N_birch_hc1_series->setColor(Qt::black); // default color: blue
    N_birch_hc1_series->setName("Height class 1");
    QLineSeries *N_birch_hc2_series = new QLineSeries();
    N_birch_hc2_series->setColor(Qt::black); // default color: blue
    N_birch_hc2_series->setName("Height class 2");
    QLineSeries *N_birch_hc3_series = new QLineSeries();
    N_birch_hc3_series->setColor(Qt::black); // default color: blue
    N_birch_hc3_series->setName("Height class 3");
    QLineSeries *N_birch_hc4_series = new QLineSeries();
    N_birch_hc4_series->setColor(Qt::black); // default color: blue
    N_birch_hc4_series->setName("Height class 4");

    QLineSeries *N_oak_seeds_series = new QLineSeries();
    N_oak_seeds_series->setColor(Qt::black); // default color: blue
    N_oak_seeds_series->setName("Seeds");
    QLineSeries *N_oak_hc1_series = new QLineSeries();
    N_oak_hc1_series->setColor(Qt::black); // default color: blue
    N_oak_hc1_series->setName("Height class 1");
    QLineSeries *N_oak_hc2_series = new QLineSeries();
    N_oak_hc2_series->setColor(Qt::black); // default color: blue
    N_oak_hc2_series->setName("Height class 2");
    QLineSeries *N_oak_hc3_series = new QLineSeries();
    N_oak_hc3_series->setColor(Qt::black); // default color: blue
    N_oak_hc3_series->setName("Height class 3");
    QLineSeries *N_oak_hc4_series = new QLineSeries();
    N_oak_hc4_series->setColor(Qt::black); // default color: blue
    N_oak_hc4_series->setName("Height class 4");

    int number_of_simulation_years = ui->N_years_spinBox->value();
    for (int time = 0; time < number_of_simulation_years; time++) {
        N_birch_seeds_series->append(time, birch_pop_total[time][0]);
        N_birch_hc1_series->append(time, birch_pop_total[time][1]);
        N_birch_hc2_series->append(time, birch_pop_total[time][2]);
        N_birch_hc3_series->append(time, birch_pop_total[time][3]);
        N_birch_hc4_series->append(time, birch_pop_total[time][4]);

        N_oak_seeds_series->append(time, oak_pop_total[time][0]);
        N_oak_hc1_series->append(time, oak_pop_total[time][1]);
        N_oak_hc2_series->append(time, oak_pop_total[time][2]);
        N_oak_hc3_series->append(time, oak_pop_total[time][3]);
        N_oak_hc4_series->append(time, oak_pop_total[time][4]);
    }


    N_birch_pop_chart->addSeries(N_birch_seeds_series);
    N_birch_pop_chart->addSeries(N_birch_hc1_series);
    N_birch_pop_chart->addSeries(N_birch_hc2_series);
    N_birch_pop_chart->addSeries(N_birch_hc3_series);
    N_birch_pop_chart->addSeries(N_birch_hc4_series);

    N_birch_pop_chart->createDefaultAxes();
    N_birch_pop_chart->axisX()->setTitleText("Time [steps]");
    N_birch_pop_chart->axisY()->setTitleText("Birch seed and sapling population [N/ha]");

    N_oak_pop_chart->addSeries(N_oak_seeds_series);
    N_oak_pop_chart->addSeries(N_oak_hc1_series);
    N_oak_pop_chart->addSeries(N_oak_hc2_series);
    N_oak_pop_chart->addSeries(N_oak_hc3_series);
    N_oak_pop_chart->addSeries(N_oak_hc4_series);

    N_oak_pop_chart->createDefaultAxes();
    N_oak_pop_chart->axisX()->setTitleText("Time [steps]");
    N_oak_pop_chart->axisY()->setTitleText("Oak seed and sapling population [N/ha]");
}


