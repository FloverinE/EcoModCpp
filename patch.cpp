#include "patch.h"
#include <vector>
#include <string>

patch::patch(){}

//patch::patch(std::string patch_id,
//             std::vector<int> x_y_cor,
//             int N_seedlings)
//{
//    this -> patch_id;
//    this -> x_y_cor;
//    this -> N_seedlings;
//}

patch::patch(std::string patch_id,
             std::vector<int> x_y_cor)
    : patch_id(patch_id), x_y_cor(x_y_cor){}

patch::update_N_seedlings(int count) {
    N_seedlings += count;
}

patch::reset_N_seedlings() {
    N_seedlings = 0;
}

patch::get_N_seedlings(){
    return N_seedlings;
}


patch::get_coords(){
    return x_y_cor[0];
    //    return x_y_cor[1];
}


