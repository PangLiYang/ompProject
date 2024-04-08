#ifndef OMPTESTCPP_SOLVER_H
#define OMPTESTCPP_SOLVER_H

#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

class Solver {
public:
    virtual vector< vector<int> >* forward(vector< vector<int> >* graph) = 0;

};

#endif //OMPTESTCPP_SOLVER_H
