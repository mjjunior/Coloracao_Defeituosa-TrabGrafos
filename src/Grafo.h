#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <iostream>

using namespace std;

class Grafo {
private:
    int numVertices;
    vector<vector<int>> adj; // lista de adjacência

public:
    
    Grafo(int n);

    // Adiciona uma aresta entre u e v (grafo não direcionado)
    void adicionarAresta(int u, int v);

    // Retorna os adjacentes de um vértice
    const vector<int>& getAdjacentes(int v) const;

    void imprimir() const;
};

#endif
