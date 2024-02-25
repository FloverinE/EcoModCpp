#include "tree.h"

tree::tree()
{

}

tree::tree(
    std::vector<int> x_y_cor,
    int dispersal_factor){
    this -> x_y_cor;
    this -> dispersal_factor;
}
