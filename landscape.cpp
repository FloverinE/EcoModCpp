#include "landscape.h"

landscape::landscape(const unsigned x_size,
                     const unsigned y_size,
                     const unsigned N_trees)
    : x_size(x_size), y_size(y_size) {
    distance_map =
        std::vector<std::vector<double>>(x_size, std::vector<double>(y_size, 0.0));
}
