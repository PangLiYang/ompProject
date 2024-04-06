#ifndef OMPTESTCPP_FLOYDWARSHALLPAR_H
#define OMPTESTCPP_FLOYDWARSHALLPAR_H

#include "Solver.h"

class FloydWarshallPar : public Solver {
public:
    FloydWarshallPar();

    vector< vector<int> >* forward(vector< vector<int> >* graph, int graph_size);
};


#endif //OMPTESTCPP_FLOYDWARSHALLPAR_H
