#include "JohnsonPar.h"

JohnsonPar::JohnsonPar(int num_thread, int graph_size) {
    this->num_thread = num_thread;
    this->graph_size = graph_size;
}

vector<vector<int> > *JohnsonPar::forward(vector<vector<int> > *graph) {

    auto* adj_graph = init_adjacency_list(graph);
    int V = adj_graph->V;

    for (int i = 0; i < V; i += 1) {
        adj_graph->adjList[V].push_back(make_pair(i, 0));
    }

    auto* h = bellman_ford(V + 1, adj_graph, V);

    auto* temp = new vector< vector<int> >(V, vector<int>(V));
    auto* output = new vector< vector<int> >(graph_size, vector<int>(graph_size));

    #pragma omp parallel
    {
        #pragma omp for schedule(dynamic)
        for (int u = 0; u < V; u += 1) {
            vector<int> curr(V, INT_MAX / 2);
            priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

            curr.at(u) = 0;
            pq.push(make_pair(0, u));

            while (!pq.empty()) {
                int root = pq.top().second;
                pq.pop();

                auto it = adj_graph->adjList.find(root);
                if (it != adj_graph->adjList.end()) {
                    for (auto &node: adj_graph->adjList[root]) {
                        int next = node.first;
                        int weight = node.second + h->at(root) - h->at(next);

                        if (curr.at(next) > curr.at(root) + weight) {
                            curr.at(next) = curr.at(root) + weight;
                            pq.push(make_pair(curr.at(next), next));
                        }
                    }
                }
            }

            for (int v = 0; v < V; v += 1) {
                temp->at(u).at(v) = curr.at(v) + h->at(v) - h->at(u);
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

    auto* adj_graph = init_adjacency_list(graph);
    int V = adj_graph->V;

    for (int i = 0; i < V; i += 1) {
        adj_graph->adjList[V].push_back(make_pair(i, 0));
    }

    auto* h = bellman_ford(V + 1, adj_graph, V);

    auto* output = new vector< vector<int> >(graph_size, vector<int>(graph_size));

    #pragma omp parallel for schedule(dynamic)
    for (int u = 0; u < graph_size; u += 1) {

        vector<int> curr(V, INT_MAX / 2);
        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

        curr.at(u) = 0;
        pq.push(make_pair(0, u));

        while (!pq.empty()) {
            int root = pq.top().second;
            pq.pop();

            auto it = adj_graph->adjList.find(root);
            if (it != adj_graph->adjList.end()) {
                for (auto &node: adj_graph->adjList[root]) {
                    int next = node.first;
                    int weight = node.second + h->at(root) - h->at(next);

                    if (curr.at(next) > curr.at(root) + weight) {
                        curr.at(next) = curr.at(root) + weight;
                        pq.push(make_pair(curr.at(next), next));
                    }
                }
            }
        }

        for (int v = 0; v < graph_size; v += 1) {
            output->at(u).at(v) = curr.at(V - graph_size + v) + h->at(V - graph_size + v) - h->at(u);
        }
    }


    return output;
}

graph_t* JohnsonPar::init_adjacency_list(vector<vector<int> > *graph) {

    int n = graph->size();

    auto* adj_graph = new graph_t;
    adj_graph->V = n;

    for (int i = 0; i < n; i += 1) {
        for (int j = 0; j < n; j += 1) {
            if (i != j && graph->at(i).at(j) != INT_MAX / 2) {
                adj_graph->adjList[i].push_back(make_pair(j, graph->at(i).at(j) ));
            }
        }
    }

    return adj_graph;
}

vector<int>* JohnsonPar::bellman_ford(int n, graph_t* adj_graph, int src) {

    auto* h = new vector<int>(n, INT_MAX / 2);

    h->at(src) = 0;

    for (int k = 1; k < n; k += 1) {
        #pragma omp parallel for
        for (int i = 0; i < n; i += 1) {
            auto it = adj_graph->adjList.find(i);
            if (it != adj_graph->adjList.end()) {
                for (auto &node: adj_graph->adjList[i]) {
                    int v = node.first;
                    int curr_dist = node.second + h->at(i);
                    if (h->at(i) != INT_MAX / 2 && h->at(v) > curr_dist) {
                        h->at(v) = curr_dist;
                    }
                }
            }
        }
    }

    return h;
}