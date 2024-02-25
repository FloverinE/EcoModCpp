#include "tree.h"

tree::tree()
{

}

tree::tree(
    std::vector<int> x_y_cor,
    char species,
    int dispersal_factor,
    int seed_production){
    this -> x_y_cor;
    this -> species;
    this -> dispersal_factor;
    this -> seed_production;
}

tree::update_species_params() {
    if (species == 'b') {
        dispersal_factor = 1;
        seed_production = 100;
    } else if (species == 'o') {
        dispersal_factor = 2;
        seed_production = 200;
    }
}

