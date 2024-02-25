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
         int max_seed_production
         );
    int id;
    std::vector<int> x_y_cor;
    char species;
    int max_seed_production;
    int dispersal_factor;
    update_species_params();


private:
             // species of tree, "b" for birch, "o" for oak
};

#endif // TREE_H
