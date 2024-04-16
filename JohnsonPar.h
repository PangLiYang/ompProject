#ifndef OMPPROJECT_JOHNSONPAR_H
#define OMPPROJECT_JOHNSONPAR_H

#include <queue>
#include "Solver.h"

class JohnsonPar : public Solver {
public:
    JohnsonPar(int num_thread, int graph_size);

    vector< vector<int> >* forward(vector< vector<int> >* graph);
    vector< vector<int> >* forward_optimized(vector< vector<int> >* graph);

private:
    int graph_size;
    int num_thread;

    graph_t* init_adjacency_list(vector< vector<int> >* graph);

    vector<int>* bellman_ford(int n, graph_t* adj_graph, int src);
};


#endif //OMPPROJECT_JOHNSONPAR_H
