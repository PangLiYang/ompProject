#include "FloydWarshallPar.h"

FloydWarshallPar::FloydWarshallPar(int num_thread, int graph_size) {
    this->num_thread = num_thread;
    this->graph_size = graph_size;
    this->parallel_factor = 32;
}

vector< vector<int> >* FloydWarshallPar::forward(vector< vector<int> >* graph) {

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
            for (int j = 0; j < graph_size; j += 1) {
                output->at(i).at(j) = local_graph->at(i).at(n - graph_size + j);
            }
        }
    }

    return output;
}

vector< vector<int> >*  FloydWarshallPar::forward_optimized(vector< vector<int> >* graph) {
    int n = graph->size();
    int ext = n;
    auto* output = new vector< vector<int> >(graph_size, vector<int>(graph_size));

    // Extend the original graph vertices to a multiple of parallel_factor

    int residual = n % parallel_factor;
    if (residual != 0) {
        residual = parallel_factor - residual;
        ext += residual;
    }

    auto* local_graph = new vector< vector<int> >(ext, vector<int>(ext, INT_MAX / 2));

    for (int i = 0; i < ext; i += 1) {
        for (int j = 0; j < ext; j += 1) {
            if (i == j) {
                local_graph->at(i).at(j) = 0;
            } else if (i < n && j < n) {
                local_graph->at(i).at(j) = graph->at(i).at(j);
            }
        }
    }

    // Run FloydWarshall

    for (int k = 0; k < ext; k += 1) {
        for (int i = 0; i < ext; i += 1) {
            for (int j = 0; j < ext; j += 1) {
                if (local_graph->at(i).at(j) > local_graph->at(i).at(k) + local_graph->at(k).at(j)) {
                    local_graph->at(i).at(j) = local_graph->at(i).at(k) + local_graph->at(k).at(j);
                }
            }
        }
    }

    // Map the targeted output

    for (int i = 0; i < graph_size; i += 1) {
        for (int j = 0; j < graph_size; j += 1) {
            output->at(i).at(j) = local_graph->at(i).at(ext - graph_size - residual + j);
        }
    }

    return output;
}