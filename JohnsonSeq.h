#ifndef OMPPROJECT_JOHNSONSEQ_H
#define OMPPROJECT_JOHNSONSEQ_H

#include <queue>
#include "Solver.h"

class JohnsonSeq : public Solver {
public:
    JohnsonSeq(int graph_size);

    vector< vector<int> >* forward(vector< vector<int> >* graph);
    vector< vector<int> >* forward_optimized(vector< vector<int> >* graph);

private:
    int graph_size;

    graph_t* init_adjacency_list(vector< vector<int> >* graph);

    vector<int>* bellman_ford(int n, graph_t* adj_graph, int src);
};


#endif //OMPPROJECT_JOHNSONSEQ_H
