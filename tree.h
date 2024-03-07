#ifndef TREE_H
#define TREE_H

#include <vector>        // package to use vectors
#include <QImage>

class tree
{
public:
    tree();
    tree(int id,
         std::vector<int> x_y_cor,
         char species,
         int dispersal_factor,
         int max_seed_production
         );
    int id;
    std::vector<int> x_y_cor;
    char species;
    int max_seed_production;
    int dispersal_factor;
    QRgb color;                 // color of tree species (green for birch or brown for oak)
    update_species_params();    // update the parameters based on species assigned in setup procedure
    set_burnt();


private:
             // species of tree, "b" for birch, "o" for oak
    bool burnt = false;         // tree is not burnt by default
};

#endif // TREE_H
