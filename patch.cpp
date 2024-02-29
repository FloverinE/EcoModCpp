#include "patch.h"
#include <vector>
#include <string>

patch::patch(){}

patch::patch(std::string patch_id,
             std::vector<int> x_y_cor,
             int N_seedlings)
{
    this -> patch_id;
    this -> x_y_cor;
    this -> N_seedlings;
}

patch::update_N_seedlings(int count) {
    N_seedlings += count;
}

patch::get_seedlings(){
    return N_seedlings;
}
patch::get_coords(){
    return x_y_cor[0];
    //    return x_y_cor[1];
}
