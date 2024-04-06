#include "FloydWarshallPar.h"

FloydWarshallPar::FloydWarshallPar(int num_thread) {
    this->num_thread = num_thread;

}

vector< vector<int> >* FloydWarshallPar::forward(vector< vector<int> >* graph, int graph_size) {

//    omp_set_num_threads(num_thread);

    // init output matrix and working graph local copy
    int n = graph->size();
    auto* output = new vector< vector<int> >(graph_size, vector<int>(graph_size));
    auto* local_graph = new vector< vector<int> >(n, vector<int>(n));

    #pragma omp parallel
    {
        #pragma omp for collapse(2)
        for (int i = 0; i < n; i += 1) {
            for (int j = 0; j < n; j += 1) {
                local_graph->at(i).at(j) = graph->at(i).at(j);
            }
        }

        for (int k = 0; k < n; k += 1) {
            #pragma omp for collapse(2)
            for (int i = 0; i < n; i += 1) {
                for (int j = 0; j < n; j += 1) {
                    if (local_graph->at(i).at(j) > local_graph->at(i).at(k) + local_graph->at(k).at(j)) {
                        local_graph->at(i).at(j) = local_graph->at(i).at(k) + local_graph->at(k).at(j);
                    }
                }
            }
        }

        #pragma omp for collapse(2)
        for (int i = 0; i < graph_size; i += 1) {
            for (int j = n - graph_size; j < n; j += 1) {
                output->at(i).at(j) = local_graph->at(i).at(j);
            }
        }
    }

    return output;
}

