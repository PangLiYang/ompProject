#ifndef OMPTESTCPP_FLOYDWARSHALLPAR_H
#define OMPTESTCPP_FLOYDWARSHALLPAR_H

#include "Solver.h"

class FloydWarshallPar : public Solver {
public:
    FloydWarshallPar(int num_thread, int graph_size, int num_layers);

    vector< vector<int> >* forward(vector< vector<int> >* graph);
private:
    int num_thread;
    int graph_size;
    int num_layers;
};


#endif //OMPTESTCPP_FLOYDWARSHALLPAR_H
