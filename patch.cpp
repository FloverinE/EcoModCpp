#include "patch.h"
#include <vector>
#include <string>
#include <random>

patch::patch() {}

patch::patch(std::string patch_id, std::vector<int> x_y_cor, std::vector<int> N_seeds)
    : patch_id(patch_id), x_y_cor(x_y_cor), N_seeds(N_seeds) {}

// function to add (dispersal) and subtract (mortality) seeds to patch
void patch::update_N_seeds(int count, char species) {
    if (species == 'b') {
        N_seeds[0] += count;
    } else if (species == 'o') {
        N_seeds[1] += count;
    }
}

//void patch::reset_N_seeds() {
//    N_seeds[0] = 0;
//    N_seeds[1] = 0;
//}


//int patch::get_N_birch_seeds() {
//    // Assuming N_seeds is initialized somewhere
//    return N_seeds[0];
//}

//int patch::get_N_oak_seeds() {
//    // Assuming N_seeds is initialized somewhere
//    return N_seeds[1];
//}


//std::vector<int> patch::get_coords() {
//    return x_y_cor;
//}

float patch::set_distance_to_tree(int x, int y) {
    float dist_to_tree = sqrt(pow(x - x_y_cor[0], 2) + pow(y - x_y_cor[1], 2));
    return dist_to_tree;
}

void patch::set_burnt() {
    burnt = true;
}


// write function to return the total number of seeds and saplings per patch
int patch::get_all_N_seeds_saplings(){
    int N_seeds_saplings = 0;
    for (int i = 0; i < 2; ++i) {
        N_seeds_saplings += N_seeds[i];
        N_seeds_saplings += N_height_class_1[i];
        N_seeds_saplings += N_height_class_2[i];
        N_seeds_saplings += N_height_class_3[i];
        N_seeds_saplings += N_height_class_4[i];
    }
    return N_seeds_saplings;
}
