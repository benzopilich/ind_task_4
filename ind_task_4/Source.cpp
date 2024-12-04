#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <set>
#include <algorithm>
using namespace std;

vector<vector<int>> graph, reverseGraph;
vector<bool> visited;
vector<int> componentId, terminals;
stack<int> finishOrder;
int componentCount = 0;

void dfs1(int v) {
    visited[v] = true;
    for (int u : graph[v]) {
        if (!visited[u])
            dfs1(u);
    }
    finishOrder.push(v);
}

void dfs2(int v) {
    visited[v] = true;
    componentId[v] = componentCount;
    for (int u : reverseGraph[v]) {
        if (!visited[u])
            dfs2(u);
    }
}

int main() {
    ifstream input("input.txt");
    ofstream output("output.txt");

    int n;
    input >> n;

    terminals.resize(n);
    graph.resize(n);
    reverseGraph.resize(n);

    for (int i = 0; i < n; ++i) {
        input >> terminals[i];
    }

    int a, b;
    while (input >> a >> b && (a != 0 || b != 0)) {
        graph[a - 1].push_back(b - 1);
        reverseGraph[b - 1].push_back(a - 1);
    }

    visited.assign(n, false);
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            dfs1(i);
        }
    }

    visited.assign(n, false);
    componentId.resize(n);
    while (!finishOrder.empty()) {
        int v = finishOrder.top();
        finishOrder.pop();

        if (!visited[v]) {
            dfs2(v);
            ++componentCount;
        }
    }

    vector<int> minTerminal(componentCount, INT_MAX);
    vector<int> minNode(componentCount, -1);
    vector<bool> hasIncomingEdge(componentCount, false);

    for (int v = 0; v < n; ++v) {
        int comp = componentId[v];
        if (terminals[v] < minTerminal[comp]) {
            minTerminal[comp] = terminals[v];
            minNode[comp] = v;
        }
        for (int u : graph[v]) {
            if (componentId[u] != comp) {
                hasIncomingEdge[componentId[u]] = true;
            }
        }
    }

    set<int> result;
    for (int i = 0; i < componentCount; ++i) {
        if (!hasIncomingEdge[i]) {
            result.insert(minNode[i] + 1);
        }
    }

    output << result.size() << endl;
    for (int node : result) {
        output << node << " ";
    }
    output << endl;

    return 0;
}
