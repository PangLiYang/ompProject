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

    #pragma omp parallel shared(local_graph, output)
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

    #pragma omp parallel
    {

        #pragma omp for collapse(2)
        for (int i = 0; i < ext; i += 1) {
            for (int j = 0; j < ext; j += 1) {
                if (i == j) {
                    local_graph->at(i).at(j) = 0;
                } else if (i < n && j < n) {
                    local_graph->at(i).at(j) = graph->at(i).at(j);
                }
            }
        }

        // Run Blocked FloydWarshall

        int block_size = ext / parallel_factor;

        for (int k = 0; k < block_size; k += 1) {

            partial_forward(local_graph, k, k, k, k, k, k);

            // Using static schedule with small chunk size to balance threads workload
            // due to the duplicated term (i == k && j == k)

            #pragma omp for schedule(static,1)
            for (int i = 0; i < block_size; i += 1) {
                if (i == k) {
                    continue;
                }
                partial_forward(local_graph, i, k, i, k, k, k);
                partial_forward(local_graph, k, i, k, k, k, i);
            }

            #pragma omp for collapse(2) schedule(static,1)
            for (int i = 0; i < block_size; i += 1) {
                for (int j = 0; j < block_size; j += 1) {
                    if (i == k || j == k) {
                        continue;
                    }
                    partial_forward(local_graph, i, j, i, k, k, j);
                }
            }
        }

        // Map the targeted output
        for (int i = 0; i < graph_size; i += 1) {
            for (int j = 0; j < graph_size; j += 1) {
                output->at(i).at(j) = local_graph->at(i).at(ext - graph_size - residual + j);
            }
        }
    }

    return output;
}

void FloydWarshallPar::partial_forward(vector<vector<int> > *graph, int x0, int y0,
                                       int x1, int y1, int x2, int y2) {

    x0 *= parallel_factor;
    x1 *= parallel_factor;
    x2 *= parallel_factor;
    y0 *= parallel_factor;
    y1 *= parallel_factor;
    y2 *= parallel_factor;

    for (int k = 0; k < parallel_factor; k += 1) {
        for (int i = 0; i < parallel_factor; i += 1) {
            for (int j = 0; j < parallel_factor; j += 1) {
                if (graph->at(x0 + i).at(y0 + j) > graph->at(x1 + i).at(y1 + k) + graph->at(x2 + k).at(y2 + j)) {
                    graph->at(x0 + i).at(y0 + j) = graph->at(x1 + i).at(y1 + k) + graph->at(x2 + k).at(y2 + j);
                }
            }
        }
    }
}