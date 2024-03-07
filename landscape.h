#ifndef LANDSCAPE_H
#define LANDSCAPE_H
#include "patch.h"
#include <vector>

class landscape {
public:
    landscape(const unsigned x_size,
              const unsigned y_size,
              const unsigned N_trees);
    landscape();

    std::vector<std::vector<float>> distance_map;


private:

    unsigned x_size = 0;
    unsigned y_size = 0;

};

#endif // LANDSCAPE_H
