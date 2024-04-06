#ifndef OMPTESTCPP_FLOYDWARSHALLSEQ_H
#define OMPTESTCPP_FLOYDWARSHALLSEQ_H

#include "Solver.h"

class FloydWarshallSeq : public Solver{
public:
    FloydWarshallSeq();

    vector< vector<int> >* forward(vector< vector<int> >* graph, int graph_size);
};


#endif //OMPTESTCPP_FLOYDWARSHALLSEQ_H
