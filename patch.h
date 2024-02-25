#ifndef PATCH_H
#define PATCH_H
#include <vector>        // package to use vectors


class patch
{
public:
    patch();

    patch(std::vector<int> x_y_cor,
          int N_seedlings);
    get_seedlings();
    update_N_seedlings(int count);
    get_coords();
    std::vector<int> x_y_cor;

private:
    int N_seedlings = 0;
    int dist_to_tree;

};

#endif // PATCH_H
