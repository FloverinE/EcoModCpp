#ifndef TREE_H
#define TREE_H

#include <vector>        // package to use vectors


class tree
{
public:
    tree();
    tree(int id,
         std::vector<int> x_y_cor,
         char species,
         int dispersal_factor,
         int seed_production
         );
    int id;
    std::vector<int> x_y_cor;
    char species;
    update_species_params();


private:
    int seed_production;
    int dispersal_factor;
             // species of tree, "b" for birch, "o" for oak
};

#endif // TREE_H
