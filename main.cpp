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

int main(int argc, char *argv[]) {

    bool omp_flag = false;
    bool test_flag = false;
    bool print_flag = false;
    bool optimized_flag = false;
    int num_thread = 1;
    int num_layers = 0;
    int graph_size = 0;
    bool floyd_warshall_flag = false;

    int seed = 2024;
    double connect_rate = 0.8;

    Solver* solver;
    vector< vector<int> >* graph_matrix;
    vector< vector<int> >* output;

    int c;

    while ((c = getopt(argc, argv, "ftpom:l:g:")) != -1) {

        switch (c) {

            case 'f':
                floyd_warshall_flag = true;
                break;

            case 't':
                test_flag = true;
                break;

            case 'p':
                print_flag = true;
                break;

            case 'o':
                optimized_flag = true;
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

            case 'l':

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

    if (num_layers != 0) {
        graph_matrix = add_graph_layers(graph_matrix, num_layers);
    }

    if (floyd_warshall_flag) {
        if (omp_flag) {
            solver = new FloydWarshallPar(num_thread, graph_size);
        } else {
            solver = new FloydWarshallSeq(graph_size, num_layers);
        }
    } else {
        if (omp_flag) {
            solver = new FloydWarshallPar(num_thread, graph_size);
        } else {
            solver = new FloydWarshallSeq(graph_size, num_layers);
        }
    }

    auto tic = chrono::high_resolution_clock::now();

    if (optimized_flag) {
        output = solver->forward_optimized(graph_matrix);
    } else {
        output = solver->forward(graph_matrix);
    }

    auto toc = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> exec_time = toc - tic;
    cout << "Program execution time: " << exec_time.count() << "ms" << endl;

    if (test_flag) {
        testCorrectness(output, graph_matrix, graph_size, num_layers);
    }

    if (print_flag) {
        print_matrix(output);
    }
}