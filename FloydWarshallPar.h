#ifndef OMPTESTCPP_FLOYDWARSHALLPAR_H
#define OMPTESTCPP_FLOYDWARSHALLPAR_H

#include "Solver.h"

class FloydWarshallPar : public Solver {
public:
    FloydWarshallPar(int num_thread);

    vector< vector<int> >* forward(vector< vector<int> >* graph, int graph_size);
private:
    int num_thread;
};


#endif //OMPTESTCPP_FLOYDWARSHALLPAR_H
