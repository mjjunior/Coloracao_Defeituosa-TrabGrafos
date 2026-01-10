#include "Grafo.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

// Construtor
Grafo::Grafo(int n) {
    numVertices = n;
    numArestas = 0;
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
    numArestas++;
}

// Retorna lista de adjacência
const vector<int>& Grafo::getAdjacentes(int v) const {
    return adj[v];
}

// Retorna o número de vértices
int Grafo::getNumVertices() const {
    return numVertices;
}

// Retorna o número de arestas
int Grafo::getNumArestas() const {
    return numArestas;
}

// Retorna o grau de um vértice
int Grafo::getGrau(int v) const {
    if (v < 0 || v >= numVertices) {
        return -1;
    }
    return adj[v].size();
}

// Verifica se existe aresta entre u e v
bool Grafo::existeAresta(int u, int v) const {
    if (u < 0 || v < 0 || u >= numVertices || v >= numVertices) {
        return false;
    }
    
    for (int vizinho : adj[u]) {
        if (vizinho == v) {
            return true;
        }
    }
    return false;
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

// Leitura de arquivo no formato padrão
// Formato esperado:
// Linha 1: número de vértices
// Linha 2: número de arestas
// Linhas seguintes: pares de vértices (u v) representando arestas
Grafo* Grafo::lerArquivo(const string& nomeArquivo) {
    ifstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir arquivo: " << nomeArquivo << endl;
        return nullptr;
    }
    
    int n, m;
    arquivo >> n >> m;
    
    Grafo* g = new Grafo(n);
    
    for (int i = 0; i < m; i++) {
        int u, v;
        arquivo >> u >> v;
        g->adicionarAresta(u, v);
    }
    
    arquivo.close();
    return g;
}

// Exporta para formato GraphEditor (csacademy.com/app/grapheditor/)
void Grafo::exportarGraphEditor(const string& nomeArquivo) const {
    ofstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cerr << "Erro ao criar arquivo: " << nomeArquivo << endl;
        return;
    }
    
    // Evita duplicatas (grafo não direcionado)
    set<pair<int, int>> arestasProcessadas;
    
    for (int u = 0; u < numVertices; u++) {
        for (int v : adj[u]) {
            if (arestasProcessadas.find({min(u, v), max(u, v)}) == arestasProcessadas.end()) {
                arquivo << u << " " << v << endl;
                arestasProcessadas.insert({min(u, v), max(u, v)});
            }
        }
    }
    
    arquivo.close();
    cout << "Grafo exportado para " << nomeArquivo << endl;
}
