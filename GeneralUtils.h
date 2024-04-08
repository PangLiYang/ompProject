#ifndef OMPTESTCPP_GENERALUTILS_H
#define OMPTESTCPP_GENERALUTILS_H

#include <iostream>
#include <cstring>
#include <random>
#include <vector>
#include <climits>

#include "Solver.h"
#include "FloydWarshallSeq.h"
using namespace std;

vector<vector<int> >* init_graph_matrix(int graph_size, double p, unsigned long seed);

vector< vector<int> >* add_graph_layers(vector< vector<int> >* graph, int num_layers);

void testCorrectness(vector< vector<int> >* output, vector< vector<int> >* graph_matrix,
                     int graph_size, int num_layers);


#endif //OMPTESTCPP_GENERALUTILS_H
