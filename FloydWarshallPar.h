#ifndef OMPTESTCPP_FLOYDWARSHALLPAR_H
#define OMPTESTCPP_FLOYDWARSHALLPAR_H

#include "Solver.h"

class FloydWarshallPar : public Solver {
public:
    FloydWarshallPar(int num_thread, int graph_size);

    vector< vector<int> >* forward(vector< vector<int> >* graph);
    vector< vector<int> >* forward_optimized(vector< vector<int> >* graph);

private:
    int num_thread;
    int graph_size;
    int parallel_factor;

    void partial_forward(vector< vector<int> >* graph, int x0, int y0,
                         int x1, int y1, int x2, int y2);
};


#endif //OMPTESTCPP_FLOYDWARSHALLPAR_H
