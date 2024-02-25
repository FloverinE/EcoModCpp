#ifndef TREE_H
#define TREE_H

#include <vector>        // package to use vectors


class tree
{
public:
    tree();
    tree(std::vector<int> x_y_cor,
        int dispersal_factor
         );
    std::vector<int> x_y_cor;


private:
    int dispersal_factor;
};

#endif // TREE_H
