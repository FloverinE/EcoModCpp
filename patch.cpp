#include "patch.h"
#include <vector>
#include <string>
#include <random>

patch::patch() {}

patch::patch(std::string patch_id, std::vector<int> x_y_cor, std::vector<int> N_seeds)
    : patch_id(patch_id), x_y_cor(x_y_cor), N_seeds(N_seeds) {}

void patch::update_N_seeds(int count, char species) {
    if (species == 'b') {
        N_seeds[0] += count;
    } else if (species == 'o') {
        N_seeds[1] += count;
    }
}

void patch::reset_N_seeds() {
    N_seeds[0] = 0;
    N_seeds[1] = 0;
}


int patch::get_N_birch_seeds() {
    // Assuming N_seeds is initialized somewhere
    return N_seeds[0];
}

int patch::get_N_oak_seeds() {
    // Assuming N_seeds is initialized somewhere
    return N_seeds[1];
}


std::vector<int> patch::get_coords() {
    return x_y_cor;
}


//void patch::perform_pop_dynamics(){
//    for (int i = 0; i < 2; i++) {
//        if(N_seeds[i]>0){
//            for (int j = 0; j < N_seeds[i]; j++) {
//                // Mortality
//                if (dist_float(gen) < mortality_rate) {
//                    N_seeds[i] -= 1;
//                // Growth into next height class
//                } else if (dist_float(gen) < growth_rate) {
//                    N_height_class_1[i] += 1;
//                    N_seeds[i] -= 1;
//                }
//            }
//        }
//    }
//}


//N_seeds[i] = static_cast<int>(N_seeds[i] * mortality_rate);
//}
//// Growth
//for (int i = 0; i < 2; i++) {
//int N_advance = static_cast<int>(N_seeds[i] * growth_rate);
//N_height_class_1[i] = N_height_class_1[i] + N_advance;
//N_seeds[i] = N_seeds[i] - N_advance;
//}

