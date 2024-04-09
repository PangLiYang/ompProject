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

vector<vector<int> >* add_graph_layers(vector<vector<int> > *graph, int num_layers) {

    int n = graph->size();
    int ext = n * num_layers;

    auto* layered_graph = new vector< vector<int> >(ext, vector<int>(ext, INT_MAX / 2));

    for (int i = 0; i < n; i += 1) {
        for (int j = 0; j < ext; j += 1) {
            if (i == j) {
                layered_graph->at(i).at(j) = 0;
            } else if (j >= ((i / n + 1) * n) && j < ((i / n + 2) * n)) {
                layered_graph->at(i).at(j) = graph->at(i).at(j - n);
            }
        }
    }

    for (int i = n; i < ext; i += 1) {
        for (int j = 0; j < ext; j += 1) {
            if (i == j) {
                layered_graph->at(i).at(j) = 0;
            } else if (j >= ((i / n + 1) * n) && j < ((i / n + 2) * n)) {
                layered_graph->at(i).at(j) = layered_graph->at(i-n).at(j - n);
            }
        }
    }

    delete(graph);

    return layered_graph;
}

void testCorrectness(vector< vector<int> >* output, vector< vector<int> >* graph_matrix,
                     int graph_size) {
    Solver* solver = new FloydWarshallSeq(graph_size);

    auto fit = solver->forward(graph_matrix);
    int n = fit->size();

    for (int i = 0; i < n; i += 1) {
        for (int j = 0; j < n; j += 1) {
            if (fit->at(i).at(j) != output->at(i).at(j)) {
                cout << "Not Correct!" << endl;
                return;
            }
        }
    }

    cout << "Correct!" << endl;
}

void print_matrix(vector< vector<int> >* graph) {
    int n = graph->size();

    for (int i = 0; i < n; i += 1) {
        for (int j = 0; j < n; j += 1) {
            cout << graph->at(i).at(j);

            if (j != n - 1) {
                cout << ", ";
            } else {
                cout << endl;
            }
        }
    }
}