#ifndef OMPTESTCPP_FLOYDWARSHALLSEQ_H
#define OMPTESTCPP_FLOYDWARSHALLSEQ_H

#include "Solver.h"

class FloydWarshallSeq : public Solver{
public:
    FloydWarshallSeq(int graph_size, int num_layers);

    vector< vector<int> >* forward(vector< vector<int> >* graph);
    vector< vector<int> >* forward_optimized(vector< vector<int> >* graph);

private:
    int graph_size;
    int num_layers;
    int parallel_factor;

    void partial_forward(vector< vector<int> >* graph, int x0, int y0,
                         int x1, int y1, int x2, int y2);
};


#endif //OMPTESTCPP_FLOYDWARSHALLSEQ_H
