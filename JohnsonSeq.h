#ifndef OMPPROJECT_JOHNSONSEQ_H
#define OMPPROJECT_JOHNSONSEQ_H

#include <unordered_map>
#include <list>
#include <queue>
#include "Solver.h"

typedef struct graph {
    int V;
    unordered_map<int, list<pair<int, int> > > adjList;
} graph_t;

class JohnsonSeq : public Solver{
public:
    JohnsonSeq(int graph_size);

    vector< vector<int> >* forward(vector< vector<int> >* graph);
    vector< vector<int> >* forward_optimized(vector< vector<int> >* graph);

private:
    int graph_size;

    graph_t* init_adjacency_list(vector< vector<int> >* graph);
};


#endif //OMPPROJECT_JOHNSONSEQ_H
