#ifndef PATCH_H
#define PATCH_H

#include <vector>
#include <string>
#include <random>

class patch {
public:
    // Constructors
    patch();
    patch(std::string patch_id,
          std::vector<int> x_y_cor,
          std::vector<int> N_seeds);

    // Member functions
    void update_N_seeds(int count, char species);
    int get_N_birch_seeds(); // Added return type 'int'
    int get_N_oak_seeds();   // Added return type 'int'
    void reset_N_seeds();
//    void perform_pop_dynamics();
    std::vector<int> get_coords(); // Added return type 'std::vector<int>'

    // Member variables
    std::string patch_id;
    std::vector<int> x_y_cor;
    std::vector<int> N_seeds = {0, 0};
    std::vector<int> N_height_class_1 = {0, 0};
    float mortality_rate = 0.2;
    float growth_rate = 0.2;

private:
    int dist_to_tree;


//    float mortality_rate = 0.2;
//    float growth_rate = 0.2;
};

#endif // PATCH_H
