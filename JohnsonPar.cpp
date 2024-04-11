#include "JohnsonPar.h"

JohnsonPar::JohnsonPar(int num_thread, int graph_size) {
    this->num_thread = num_thread;
    this->graph_size = graph_size;
}

vector<vector<int> > *JohnsonPar::forward(vector<vector<int> > *graph) {

    int V = graph->size();

    auto* adj_graph = new graph_t;
    adj_graph->V = V;

    auto *temp = new vector<vector<int> >(V, vector<int>(V));
    auto *output = new vector<vector<int> >(graph_size, vector<int>(graph_size));

    #pragma omp parallel
    {
//        #pragma omp for
        for (int i = 0; i < V; i += 1) {
            for (int j = 0; j < V; j += 1) {
                if (i != j && graph->at(i).at(j) != INT_MAX / 2) {
//                    #pragma omp critical
                    adj_graph->adjList[i].push_back(make_pair(j, graph->at(i).at(j)));
                }
            }
        }

        #pragma omp for
        for (int u = 0; u < V; u += 1) {

            vector<int> *curr;
            priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > *pq;

            #pragma omp critical
            {
                curr = new vector<int>(V, INT_MAX / 2);
                pq = new priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > >();
            }

            curr->at(u) = 0;
            pq->push(make_pair(0, u));

            while (!pq->empty()) {
                int root = pq->top().second;
                pq->pop();

                for (auto &node: adj_graph->adjList[root]) {
                    int next = node.first;
                    int weight = node.second;

                    if (curr->at(next) > curr->at(root) + weight) {
                        curr->at(next) = curr->at(root) + weight;
                        pq->push(make_pair(curr->at(next), next));
                    }
                }
            }

            for (int v = 0; v < V; v += 1) {
                temp->at(u).at(v) = curr->at(v);
            }
        }

        #pragma omp for collapse(2)
        for (int i = 0; i < graph_size; i += 1) {
            for (int j = 0; j < graph_size; j += 1) {
                output->at(i).at(j) = temp->at(i).at(V - graph_size + j);
            }
        }
    }

    return output;
}

vector<vector<int> > *JohnsonPar::forward_optimized(vector<vector<int> > *graph) {
    return nullptr;
}
