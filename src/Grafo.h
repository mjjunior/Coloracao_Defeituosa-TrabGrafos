#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Grafo {
private:
    int numVertices;
    int numArestas;
    vector<vector<int>> adj; // lista de adjacência

public:
    
    Grafo(int n);

    // Adiciona uma aresta entre u e v (grafo não direcionado)
    void adicionarAresta(int u, int v);

    // Retorna os adjacentes de um vértice
    const vector<int>& getAdjacentes(int v) const;

    // Retorna o número de vértices
    int getNumVertices() const;

    // Retorna o número de arestas
    int getNumArestas() const;

    // Retorna o grau de um vértice
    int getGrau(int v) const;

    // Verifica se existe aresta entre u e v
    bool existeAresta(int u, int v) const;

    // Leitura de arquivo no formato padrão
    static Grafo* lerArquivo(const string& nomeArquivo);

    // Imprime o grafo
    void imprimir() const;
    
    // Exporta para formato GraphEditor (csacademy.com)
    void exportarGraphEditor(const string& nomeArquivo) const;
};

#endif
