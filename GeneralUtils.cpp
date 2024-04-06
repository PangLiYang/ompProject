#include "GeneralUtils.h"

vector< vector<int> >* init_graph_matrix(const int graph_size, const double p, const unsigned long seed) {

    static uniform_real_distribution<double> flip(0, 1);
    static uniform_int_distribution<int> choose_weight(1, 30);

    mt19937_64 rand_engine(seed);

    auto* out = new vector< vector<int> >(graph_size, vector<int>(graph_size, INT_MAX / 2));

    for (int i = 0; i < graph_size; i++) {
        for (int j = 0; j < graph_size; j++) {
            if (i == j) {
                out->at(i).at(j) = 0;
            } else if (flip(rand_engine) < p) {
                out->at(i).at(j) = choose_weight(rand_engine);
            }
        }
    }

    return out;
}