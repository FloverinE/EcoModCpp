#ifndef PATCH_H
#define PATCH_H

#include <vector>
#include <string>

class patch {
public:
    // Constructors
    patch();
    patch(std::string patch_id, std::vector<int> x_y_cor);

    // Member functions
    get_N_seedlings(); // Added return type
    update_N_seedlings(int count); // Added return type
    get_coords(); // Added return type
    reset_N_seedlings(); // Added return type

    // Member variables
    std::string patch_id;
    std::vector<int> x_y_cor;
    int N_seedlings = 0;

private:
    int dist_to_tree;
};

#endif // PATCH_H

