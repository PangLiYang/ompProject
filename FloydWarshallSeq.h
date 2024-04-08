#ifndef OMPTESTCPP_FLOYDWARSHALLSEQ_H
#define OMPTESTCPP_FLOYDWARSHALLSEQ_H

#include "Solver.h"

class FloydWarshallSeq : public Solver{
public:
    FloydWarshallSeq(int graph_size, int num_layers);

    vector< vector<int> >* forward(vector< vector<int> >* graph);
private:
    int graph_size;
    int num_layers;
};


#endif //OMPTESTCPP_FLOYDWARSHALLSEQ_H
