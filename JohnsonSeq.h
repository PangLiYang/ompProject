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
};


#endif //OMPPROJECT_JOHNSONSEQ_H
