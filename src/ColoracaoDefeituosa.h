#ifndef COLORACAO_DEFEITUOSA_H
#define COLORACAO_DEFEITUOSA_H

#include "Grafo.h"
#include <vector>
#include <set>
#include <string>

using namespace std;

struct Solucao {
    vector<int> cores;           // cores[v] = cor do vértice v
    int numCores;                 // número de cores utilizadas
    int numDeficiencias;          // número total de deficiências (arestas mal coloridas)
    double tempoExecucao;         // tempo de execução em segundos
    int alphaMelhor;              // alpha que gerou melhor solução (para reativo)
    double mediaIteracoes;        // média dos valores em todas as iterações
    
    Solucao() : numCores(0), numDeficiencias(0), tempoExecucao(0.0), alphaMelhor(-1), mediaIteracoes(0.0) {}
};

class ColoracaoDefeituosa {
private:
    const Grafo& grafo;
    int d;  // parâmetro d - número máximo de deficiências permitidas por vértice
    
    // Funções auxiliares
    int calcularDeficiencias(const vector<int>& cores);
    int contarDeficienciasVertice(int v, const vector<int>& cores);
    bool verificarRestricaoD(int v, int cor, const vector<int>& cores);
    
public:
    ColoracaoDefeituosa(const Grafo& g, int deficiencia);
    
    // Algoritmo Guloso
    Solucao algoritmoGuloso();
    
    // Algoritmo Randomizado (GRASP)
    Solucao algoritmoRandomizado(int numIteracoes, double alpha);
    
    // Algoritmo Reativo (GRASP Reativo)
    Solucao algoritmoReativo(int numIteracoes, int tamBloco);
    
    // Função para imprimir solução
    void imprimirSolucao(const Solucao& sol);
    
    // Exportar solução para GraphEditor
    void exportarSolucaoGraphEditor(const Solucao& sol, const string& nomeArquivo);
    
    // Salvar resultado em CSV
    void salvarCSV(const Solucao& sol, const string& nomeInstancia, const string& algoritmo, 
                   double alpha, int numIteracoes, int tamBloco, unsigned int semente);
    
    // Verificar se a solução é válida
    bool validarSolucao(const Solucao& sol);
};

#endif
