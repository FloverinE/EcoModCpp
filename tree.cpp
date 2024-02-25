#include "tree.h"

tree::tree()
{

}

tree::tree(
    int id,
    std::vector<int> x_y_cor,
    char species,
    int dispersal_factor,
    int max_seed_production){
    this -> id;
    this -> x_y_cor;
    this -> species;
    this -> dispersal_factor;
    this -> max_seed_production;
}

tree::update_species_params() {
    if (species == 'b') {
        dispersal_factor = 20;
        max_seed_production = 100;
    } else if (species == 'o') {
        dispersal_factor = 40;
        max_seed_production = 200;
    }
}

