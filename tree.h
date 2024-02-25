#ifndef TREE_H
#define TREE_H

#include <vector>        // package to use vectors


class tree
{
public:
    tree();
    tree(std::vector<int> x_y_cor,
         char species,
         int dispersal_factor,
         int seed_production
         );
    std::vector<int> x_y_cor;
    update_species_params();


private:
    int seed_production;
    int dispersal_factor;
    char species;               // species of tree, "b" for birch, "o" for oak
};

#endif // TREE_H
