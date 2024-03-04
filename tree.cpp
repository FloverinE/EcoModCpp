#include "tree.h"
#include <QImage>

QRgb color_birch = qRgb(165,42,42); // brown color
QRgb color_oak = qRgb(0, 128, 0); // green color

tree::tree()
{

}

tree::tree(int id,
           std::vector<int> x_y_cor,
           char species,
           int dispersal_factor,
           int max_seed_production)
{
    this -> id;
    this -> x_y_cor;
    this -> species;
    this -> dispersal_factor;
    this -> max_seed_production;
    this -> color;
}

tree::update_species_params() {
    if (species == 'b') {
        dispersal_factor = 20;
        max_seed_production = 100;
        color = color_birch;

    } else if (species == 'o') {
        dispersal_factor = 40;
        max_seed_production = 200;
        color = color_oak;
    }
}

