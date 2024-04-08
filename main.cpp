#include <iostream>
#include <chrono>
#include <unistd.h>
#include <omp.h>

#include <vector>

#include "GeneralUtils.h"
#include "Solver.h"
#include "FloydWarshallSeq.h"
#include "FloydWarshallPar.h"

using namespace std;

void testCorrectness(vector< vector<int> >* output, vector< vector<int> >* graph_matrix, int graph_size, int num_layers);

int main(int argc, char *argv[]) {

    bool omp_flag = false;
    int num_thread = 1;
    int num_layers = 1;
    int graph_size = 0;
    bool floyd_warshall_flag = false;

    int seed = 2024;
    double connect_rate = 0.8;

    Solver* solver;
    vector< vector<int> >* graph_matrix;
    vector< vector<int> >* output;

    int c;

    while ((c = getopt(argc, argv, "fm:s:g:")) != -1) {

        switch (c) {

            case 'f':
                floyd_warshall_flag = true;

                break;

            case 'm':
                omp_flag = true;

                if (optarg != nullptr) {
                    num_thread = stoi(optarg);
                    omp_set_num_threads(num_thread);
                } else {
                    cout << "Assign the number of threads!" << endl;
                    return 42;
                }

                break;

            case 's':

                if (optarg != nullptr) {
                    num_layers = stoi(optarg);
                } else {
                    cout << "Assign the number of separate points!" << endl;
                    return 42;
                }

                break;

            case 'g':

                if (optarg != nullptr) {
                    graph_size = stoi(optarg);
                } else {
                    cout << "Assign the graph size!" << endl;
                    return 42;
                }

                break;

            default:
                return 42;
        }
    }

    graph_matrix = init_graph_matrix(graph_size, connect_rate, seed);

    if (floyd_warshall_flag) {
        if (omp_flag) {
            solver = new FloydWarshallPar(num_thread, graph_size, num_layers);
        } else {
            solver = new FloydWarshallSeq(graph_size, num_layers);
        }
    } else {
        if (omp_flag) {
            solver = new FloydWarshallPar(num_thread, graph_size, num_layers);
        } else {
            solver = new FloydWarshallSeq(graph_size, num_layers);
        }
    }

    auto tic = chrono::high_resolution_clock::now();

    output = solver->forward(graph_matrix);

    auto toc = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> exec_time = toc - tic;
    cout << "Program execution time: " << exec_time.count() << "ms" << endl;

    testCorrectness(output, graph_matrix, graph_size, num_layers);
}

void testCorrectness(vector< vector<int> >* output, vector< vector<int> >* graph_matrix, int graph_size, int num_layers) {
    Solver* solver = new FloydWarshallSeq(graph_size, num_layers);
    int n = graph_matrix->size();

    auto fit = solver->forward(graph_matrix);

    for (int i = 0; i < n; i += 1) {
        for (int j = 0; j < n; j += 1) {
            if (fit->at(i).at(j) != output->at(i).at(j)) {
                cout << "Not Correct!" << endl;
                return;
            }
        }
    }

    cout << "Correct!" << endl;
}