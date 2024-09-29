#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <getopt.h>
using namespace std;

class Graph {
private:
    int V;  // Number of vertices
    vector<list<int>> adjList;  // Adjacency list

public:
    // Constructor
    Graph(int V) {
        this->V = V;
        adjList.resize(V);
    }

    // Add edge (undirected)
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    // Remove edge between u and v
    void removeEdge(int u, int v) {
        adjList[u].remove(v);
        adjList[v].remove(u);
    }

    // Function to check if all vertices with non-zero degree are connected
    bool isConnected() {
        vector<bool> visited(V, false);
        int nonZeroDegreeVertex = -1;

        // Find a vertex with non-zero degree
        for (int i = 0; i < V; ++i) {
            if (!adjList[i].empty()) {
                nonZeroDegreeVertex = i;
                break;
            }
        }

        // If there are no edges in the graph, return true
        if (nonZeroDegreeVertex == -1)
            return true;

        // Start DFS from a vertex with non-zero degree
        dfs(nonZeroDegreeVertex, visited);

        // Check if all vertices with non-zero degree are visited
        for (int i = 0; i < V; ++i) {
            if (!visited[i] && !adjList[i].empty())
                return false;
        }
        return true;
    }

    // Depth-first search
    void dfs(int v, vector<bool>& visited) {
        visited[v] = true;
        for (auto adjVertex : adjList[v]) {
            if (!visited[adjVertex]) {
                dfs(adjVertex, visited);
            }
        }
    }

    // Check if all vertices have even degree
    bool hasEvenDegree() {
        for (int i = 0; i < V; ++i) {
            if (adjList[i].size() % 2 != 0)
                return false;
        }
        return true;
    }

    // Find and print the Eulerian Circuit using Hierholzer’s algorithm
    void findEulerianCircuit() {
        if (!isConnected()) {
            cout << "Graph is not connected. No Eulerian Circuit exists." << endl;
            return;
        }

        if (!hasEvenDegree()) {
            cout << "Graph has vertices with odd degree. No Eulerian Circuit exists." << endl;
            return;
        }

        // Stack to store the current path and final circuit
        stack<int> currentPath;
        vector<int> circuit;

        // Start from any vertex with edges
        int startVertex = 0;
        for (int i = 0; i < V; ++i) {
            if (!adjList[i].empty()) {
                startVertex = i;
                break;
            }
        }

        // Push the start vertex onto the current path
        currentPath.push(startVertex);
        int currentVertex = startVertex;

        // Hierholzer’s Algorithm
        while (!currentPath.empty()) {
            if (!adjList[currentVertex].empty()) {
                // If current vertex has neighbors, push it onto the stack
                currentPath.push(currentVertex);

                // Find and remove the next vertex
                int nextVertex = adjList[currentVertex].front();
                removeEdge(currentVertex, nextVertex);

                // Move to next vertex
                currentVertex = nextVertex;
            } else {
                // If current vertex has no neighbors, it's part of the final circuit
                circuit.push_back(currentVertex);

                // Backtrack to the previous vertex
                currentVertex = currentPath.top();
                currentPath.pop();
            }
        }

        // Print the Eulerian Circuit
        cout << "Eulerian Circuit: ";
        for (int i = 0; i < circuit.size(); ++i) {
            cout << circuit[i];
            if (i != circuit.size() - 1)
                cout << " -> ";
        }
        cout << endl;
    }

    // Generate a graph ensuring all vertices have even degree
    void generateRandomEvenDegreeGraph(int numEdges, unsigned seed) {
        srand(seed);

        int u, v;
        int edgeCount = 0;

        // Step 1: Start by creating a simple cycle to ensure even degrees
        for (int i = 0; i < V; ++i) {
            int next = (i + 1) % V; // Connect in a cycle
            addEdge(i, next);
            edgeCount++;
        }

        // Step 2: Add random edges ensuring degrees remain even
        while (edgeCount < numEdges) {
            u = rand() % V;
            v = rand() % V;

            // Avoid self-loops and duplicate edges
            if (u != v && !edgeExists(u, v)) {
                addEdge(u, v);
                edgeCount++;
            }
        }
    }

    // Check if edge exists between u and v
    bool edgeExists(int u, int v) {
        for (int neighbor : adjList[u]) {
            if (neighbor == v)
                return true;
        }
        return false;
    }
};

// Command-line options processing using getopt
void processArgs(int argc, char *argv[], int &vertices, int &edges, unsigned &seed) {
    int opt;
    while ((opt = getopt(argc, argv, "v:e:s:")) != -1) {
        switch (opt) {
        case 'v':
            vertices = atoi(optarg);
            break;
        case 'e':
            edges = atoi(optarg);
            break;
        case 's':
            seed = atoi(optarg);
            break;
        default:
            cerr << "Usage: " << argv[0] << " -v <vertices> -e <edges> -s <seed>" << endl;
            exit(EXIT_FAILURE);
        }
    }
    // Check that valid inputs were provided
    if (vertices <= 0 || edges < 0 || seed <= 0) {
        cerr << "Error: Invalid parameters. Ensure vertices > 0, edges >= 0, seed > 0." << endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    int vertices = 0, edges = 0;
    unsigned seed = 0;

    // Process command-line arguments
    processArgs(argc, argv, vertices, edges, seed);

    // Create a graph with the specified number of vertices
    Graph g(vertices);

    // Generate random graph with the given number of edges and random seed
    g.generateRandomEvenDegreeGraph(edges, seed);

    // Find and print the Eulerian Circuit (if exists)
    g.findEulerianCircuit();

    return 0;
}
