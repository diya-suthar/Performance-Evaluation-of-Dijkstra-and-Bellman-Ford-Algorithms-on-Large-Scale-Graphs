#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <fstream>
#include <random>
#include <chrono>

using namespace std;
using namespace chrono;

struct Edge {
    int u, v, w;
};

class Graph {
public:
    int V;
    vector<vector<pair<int,int>>> adj;
    vector<Edge> edges;

    Graph(int vertices) {
        V = vertices;
        adj.resize(V);
    }

    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        edges.push_back({u, v, w});
    }

    // Dijkstra
    void dijkstra(int source) {
        vector<int> dist(V, INT_MAX);

        priority_queue<
            pair<int,int>,
            vector<pair<int,int>>,
            greater<pair<int,int>>
        > pq;

        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            int u = pq.top().second;
            int d = pq.top().first;
            pq.pop();

            if (d > dist[u]) continue;

            for (auto edge : adj[u]) {
                int v = edge.first;
                int w = edge.second;

                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    // Bellman-Ford
    void bellmanFord(int source) {
        vector<int> dist(V, INT_MAX);
        dist[source] = 0;

        for (int i = 1; i < V; i++) {
            for (auto e : edges) {
                if (dist[e.u] != INT_MAX &&
                    dist[e.v] > dist[e.u] + e.w) {
                    dist[e.v] = dist[e.u] + e.w;
                }
                if (dist[e.v] != INT_MAX &&
                    dist[e.u] > dist[e.v] + e.w) {
                    dist[e.u] = dist[e.v] + e.w;
                }
            }
        }
    }
};

// Random graph generator
Graph generateGraph(int V, int E) {
    Graph g(V);

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> vertex(0, V - 1);
    uniform_int_distribution<> weight(1, 100);

    for (int i = 0; i < E; i++) {
        int u = vertex(gen);
        int v = vertex(gen);
        int w = weight(gen);

        if (u != v) {
            g.addEdge(u, v, w);
        }
    }
    return g;
}

int main() {
    

    ofstream file("results.csv");
    file << "Nodes,Edges,Dijkstra_ms,BellmanFord_ms\n";
vector<int> sizes;
for (int v = 100; v <= 5000; v += 200) {
    sizes.push_back(v);
}

    for (int V : sizes) {
        int E = V * 4;

        cout << "Running V = " << V << endl;

        Graph g = generateGraph(V, E);

        // Dijkstra timing
        auto start1 = high_resolution_clock::now();
        g.dijkstra(0);
        auto end1 = high_resolution_clock::now();

        double dijkstra_time =
            duration_cast<milliseconds>(end1 - start1).count();

        // Bellman-Ford timing
        auto start2 = high_resolution_clock::now();
        g.bellmanFord(0);
        auto end2 = high_resolution_clock::now();

        double bellman_time =
            duration_cast<milliseconds>(end2 - start2).count();

        file << V << "," << E << ","
             << dijkstra_time << ","
             << bellman_time << "\n";
    }

    file.close();
    cout << "results.csv created\n";
}