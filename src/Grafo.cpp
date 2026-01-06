#include "Grafo.h"

// Construtor
Grafo::Grafo(int n) {
    numVertices = n;
    adj.resize(n);
}

// Adiciona aresta u - v
void Grafo::adicionarAresta(int u, int v) {
    if (u < 0 || v < 0 || u >= numVertices || v >= numVertices) {
        cout << "Aresta invalida: " << u << " - " << v << endl;
        return;
    }

    adj[u].push_back(v);
    adj[v].push_back(u); 
}

// Retorna lista de adjacência
const vector<int>& Grafo::getAdjacentes(int v) const {
    return adj[v];
}

// Imprime o grafo
void Grafo::imprimir() const {
    for (int i = 0; i < numVertices; i++) {
        cout << i << ": ";
        for (int v : adj[i]) {
            cout << v << " ";
        }
        cout << endl;
    }
}
