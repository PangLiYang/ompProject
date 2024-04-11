#ifndef OMPPROJECT_JOHNSONPAR_H
#define OMPPROJECT_JOHNSONPAR_H

#include <unordered_map>
#include <list>
#include <queue>
#include "Solver.h"

typedef struct graph {
    int V;
    unordered_map<int, list<pair<int, int> > > adjList;
} graph_t;

class JohnsonPar : public Solver {
public:
    JohnsonPar(int num_thread, int graph_size);

    vector< vector<int> >* forward(vector< vector<int> >* graph);
    vector< vector<int> >* forward_optimized(vector< vector<int> >* graph);

private:
    int graph_size;
    int num_thread;

    graph_t* init_adjacency_list(vector< vector<int> >* graph);
};


#endif //OMPPROJECT_JOHNSONPAR_H
