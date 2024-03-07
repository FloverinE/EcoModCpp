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
    std::vector<int> get_coords(); //
    float set_distance_to_tree(int x, int y); //
    void set_burnt();

    // Member variables
    std::string patch_id;
    std::vector<int> x_y_cor;
    float mortality_rate = 0.2;
    float growth_rate = 0.2;
    std::vector<int> N_seeds = {0, 0};
    std::vector<int> N_height_class_1 = {0, 0};
    std::vector<int> N_height_class_2 = {0, 0};
    std::vector<int> N_height_class_3 = {0, 0};
    std::vector<int> N_height_class_4 = {0, 0};
    float distance_to_tree;
    bool burnt = false;

private:

//    int dist_to_tree;

//    float mortality_rate = 0.2;
//    float growth_rate = 0.2;
};

#endif // PATCH_H
