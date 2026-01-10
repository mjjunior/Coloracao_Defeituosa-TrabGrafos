#include "ColoracaoDefeituosa.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>

using namespace std;

// Construtor
ColoracaoDefeituosa::ColoracaoDefeituosa(const Grafo& g, int deficiencia) 
    : grafo(g), d(deficiencia) {}

// Calcula o número total de deficiências na solução
int ColoracaoDefeituosa::calcularDeficiencias(const vector<int>& cores) {
    int totalDeficiencias = 0;
    int n = grafo.getNumVertices();
    
    for (int v = 0; v < n; v++) {
        const vector<int>& vizinhos = grafo.getAdjacentes(v);
        for (int u : vizinhos) {
            if (u > v && cores[v] == cores[u]) {
                totalDeficiencias++;
            }
        }
    }
    
    return totalDeficiencias;
}

// Conta deficiências de um vértice específico
int ColoracaoDefeituosa::contarDeficienciasVertice(int v, const vector<int>& cores) {
    int deficiencias = 0;
    const vector<int>& vizinhos = grafo.getAdjacentes(v);
    
    for (int u : vizinhos) {
        if (cores[v] == cores[u] && cores[u] != -1) {
            deficiencias++;
        }
    }
    
    return deficiencias;
}

// Verifica se atribuir uma cor a um vértice respeita a restrição d
bool ColoracaoDefeituosa::verificarRestricaoD(int v, int cor, const vector<int>& cores) {
    int deficienciasV = 0;
    const vector<int>& vizinhos = grafo.getAdjacentes(v);
    
    // Conta quantos vizinhos de v já têm a cor 'cor'
    for (int u : vizinhos) {
        if (cores[u] == cor) {
            deficienciasV++;
            
            // Verifica também se u não ultrapassa d deficiências
            int deficienciasU = 0;
            for (int w : grafo.getAdjacentes(u)) {
                if (cores[w] == cor) {
                    deficienciasU++;
                }
            }
            if (deficienciasU >= d) {
                return false;
            }
        }
    }
    
    return deficienciasV <= d;
}

// Algoritmo Guloso
Solucao ColoracaoDefeituosa::algoritmoGuloso() {
    auto inicio = chrono::high_resolution_clock::now();
    
    Solucao sol;
    int n = grafo.getNumVertices();
    sol.cores.assign(n, -1);
    
    // Cria lista de vértices ordenada por grau decrescente
    vector<pair<int, int>> vertices; // (grau, vértice)
    for (int v = 0; v < n; v++) {
        vertices.push_back({grafo.getGrau(v), v});
    }
    sort(vertices.begin(), vertices.end(), greater<pair<int, int>>());
    
    int corAtual = 0;
    
    // Colore cada vértice
    for (auto& par : vertices) {
        int v = par.second;
        
        // Tenta usar a menor cor possível
        for (int cor = 0; cor <= corAtual; cor++) {
            if (verificarRestricaoD(v, cor, sol.cores)) {
                sol.cores[v] = cor;
                break;
            }
        }
        
        // Se não conseguiu colorir com nenhuma cor existente, usa nova cor
        if (sol.cores[v] == -1) {
            sol.cores[v] = ++corAtual;
        }
    }
    
    sol.numCores = corAtual + 1;
    sol.numDeficiencias = calcularDeficiencias(sol.cores);
    
    auto fim = chrono::high_resolution_clock::now();
    sol.tempoExecucao = chrono::duration<double>(fim - inicio).count();
    
    return sol;
}

// Algoritmo Randomizado (GRASP)
Solucao ColoracaoDefeituosa::algoritmoRandomizado(int numIteracoes, double alpha) {
    auto inicio = chrono::high_resolution_clock::now();
    
    Solucao melhorSol;
    melhorSol.numCores = INT_MAX;
    
    double somaQualidades = 0.0;
    int n = grafo.getNumVertices();
    
    for (int iter = 0; iter < numIteracoes; iter++) {
        Solucao sol;
        sol.cores.assign(n, -1);
        
        vector<int> verticesRestantes;
        for (int i = 0; i < n; i++) {
            verticesRestantes.push_back(i);
        }
        
        int corAtual = 0;
        
        // Enquanto houver vértices não coloridos
        while (!verticesRestantes.empty()) {
            // Calcula grau de cada vértice não colorido
            vector<pair<int, int>> candidatos; // (grau, vértice)
            for (int v : verticesRestantes) {
                candidatos.push_back({grafo.getGrau(v), v});
            }
            
            // Ordena por grau decrescente
            sort(candidatos.begin(), candidatos.end(), greater<pair<int, int>>());
            
            // Cria Lista Restrita de Candidatos (RCL)
            int grauMax = candidatos[0].first;
            int grauMin = candidatos.back().first;
            double limiar = grauMin + alpha * (grauMax - grauMin);
            
            vector<int> rcl;
            for (auto& par : candidatos) {
                if (par.first >= limiar) {
                    rcl.push_back(par.second);
                }
            }
            
            // Escolhe vértice aleatório da RCL
            int idx = rand() % rcl.size();
            int v = rcl[idx];
            
            // Remove vértice da lista de restantes
            verticesRestantes.erase(
                remove(verticesRestantes.begin(), verticesRestantes.end(), v),
                verticesRestantes.end()
            );
            
            // Tenta colorir com cor existente
            bool colorido = false;
            for (int cor = 0; cor <= corAtual; cor++) {
                if (verificarRestricaoD(v, cor, sol.cores)) {
                    sol.cores[v] = cor;
                    colorido = true;
                    break;
                }
            }
            
            // Se não conseguiu, usa nova cor
            if (!colorido) {
                sol.cores[v] = ++corAtual;
            }
        }
        
        sol.numCores = corAtual + 1;
        sol.numDeficiencias = calcularDeficiencias(sol.cores);
        somaQualidades += sol.numCores;
        
        // Atualiza melhor solução
        if (sol.numCores < melhorSol.numCores || 
            (sol.numCores == melhorSol.numCores && sol.numDeficiencias < melhorSol.numDeficiencias)) {
            melhorSol = sol;
        }
    }
    
    melhorSol.mediaIteracoes = somaQualidades / numIteracoes;
    
    auto fim = chrono::high_resolution_clock::now();
    melhorSol.tempoExecucao = chrono::duration<double>(fim - inicio).count();
    
    return melhorSol;
}

// Algoritmo Reativo (GRASP Reativo)
Solucao ColoracaoDefeituosa::algoritmoReativo(int numIteracoes, int tamBloco) {
    auto inicio = chrono::high_resolution_clock::now();
    
    // Conjunto de valores alpha
    vector<double> alphas = {0.0, 0.25, 0.5, 0.75, 1.0};
    int numAlphas = alphas.size();
    
    // Inicializa probabilidades uniformes
    vector<double> probabilidades(numAlphas, 1.0 / numAlphas);
    
    // Inicializa contadores e qualidades
    vector<int> vezesUsado(numAlphas, 0);
    vector<double> somaQualidades(numAlphas, 0.0);
    vector<double> qualidadeMedia(numAlphas, 0.0);
    
    Solucao melhorSol;
    melhorSol.numCores = INT_MAX;
    
    double somaTodasIteracoes = 0.0;
    
    for (int iter = 0; iter < numIteracoes; iter++) {
        // Seleciona alpha baseado nas probabilidades
        double r = (double)rand() / RAND_MAX;
        double acumulado = 0.0;
        int idxAlpha = 0;
        
        for (int i = 0; i < numAlphas; i++) {
            acumulado += probabilidades[i];
            if (r <= acumulado) {
                idxAlpha = i;
                break;
            }
        }
        
        double alpha = alphas[idxAlpha];
        
        // Executa uma iteração do GRASP com o alpha selecionado
        Solucao sol = algoritmoRandomizado(1, alpha);
        
        vezesUsado[idxAlpha]++;
        somaQualidades[idxAlpha] += sol.numCores;
        somaTodasIteracoes += sol.numCores;
        
        // Atualiza melhor solução
        if (sol.numCores < melhorSol.numCores || 
            (sol.numCores == melhorSol.numCores && sol.numDeficiencias < melhorSol.numDeficiencias)) {
            melhorSol = sol;
            melhorSol.alphaMelhor = (int)(alpha * 100); // Salva alpha como inteiro (0-100)
        }
        
        // A cada tamBloco iterações, atualiza probabilidades
        if ((iter + 1) % tamBloco == 0) {
            // Calcula qualidade média de cada alpha
            for (int i = 0; i < numAlphas; i++) {
                if (vezesUsado[i] > 0) {
                    qualidadeMedia[i] = somaQualidades[i] / vezesUsado[i];
                } else {
                    qualidadeMedia[i] = INT_MAX;
                }
            }
            
            // Atualiza probabilidades (quanto menor a qualidade média, maior a probabilidade)
            // Usa o inverso da qualidade média
            double somaInversos = 0.0;
            for (int i = 0; i < numAlphas; i++) {
                if (qualidadeMedia[i] > 0) {
                    somaInversos += 1.0 / qualidadeMedia[i];
                }
            }
            
            if (somaInversos > 0) {
                for (int i = 0; i < numAlphas; i++) {
                    if (qualidadeMedia[i] > 0) {
                        probabilidades[i] = (1.0 / qualidadeMedia[i]) / somaInversos;
                    } else {
                        probabilidades[i] = 0.0;
                    }
                }
            }
        }
    }
    
    melhorSol.mediaIteracoes = somaTodasIteracoes / numIteracoes;
    
    auto fim = chrono::high_resolution_clock::now();
    melhorSol.tempoExecucao = chrono::duration<double>(fim - inicio).count();
    
    // Imprime estatísticas dos alphas
    cout << "\n=== Estatísticas dos Alphas ===" << endl;
    for (int i = 0; i < numAlphas; i++) {
        cout << "Alpha " << fixed << setprecision(2) << alphas[i] 
             << ": usado " << vezesUsado[i] << " vezes, ";
        if (vezesUsado[i] > 0) {
            cout << "qualidade média = " << setprecision(2) 
                 << (somaQualidades[i] / vezesUsado[i]) << endl;
        } else {
            cout << "não foi usado" << endl;
        }
    }
    
    return melhorSol;
}

// Imprime solução
void ColoracaoDefeituosa::imprimirSolucao(const Solucao& sol) {
    cout << "\n=== SOLUÇÃO ===" << endl;
    cout << "Número de cores: " << sol.numCores << endl;
    cout << "Número de deficiências: " << sol.numDeficiencias << endl;
    cout << "Tempo de execução: " << fixed << setprecision(4) << sol.tempoExecucao << " segundos" << endl;
    
    if (sol.alphaMelhor >= 0) {
        cout << "Alpha da melhor solução: " << (sol.alphaMelhor / 100.0) << endl;
    }
    
    if (sol.mediaIteracoes > 0) {
        cout << "Média de cores (todas iterações): " << fixed << setprecision(2) 
             << sol.mediaIteracoes << endl;
    }
    
    cout << "\nColoração dos vértices:" << endl;
    for (size_t i = 0; i < sol.cores.size(); i++) {
        cout << "Vértice " << i << ": cor " << sol.cores[i] << endl;
    }
    
    if (validarSolucao(sol)) {
        cout << "\n✓ Solução válida!" << endl;
    } else {
        cout << "\n✗ Solução inválida!" << endl;
    }
}

// Exporta solução para formato GraphEditor
void ColoracaoDefeituosa::exportarSolucaoGraphEditor(const Solucao& sol, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cerr << "Erro ao criar arquivo: " << nomeArquivo << endl;
        return;
    }
    
    int n = grafo.getNumVertices();
    set<pair<int, int>> arestasProcessadas;
    
    // Escreve arestas
    for (int u = 0; u < n; u++) {
        for (int v : grafo.getAdjacentes(u)) {
            if (arestasProcessadas.find({min(u, v), max(u, v)}) == arestasProcessadas.end()) {
                arquivo << u << " " << v << endl;
                arestasProcessadas.insert({min(u, v), max(u, v)});
            }
        }
    }
    
    arquivo.close();
    cout << "Solução exportada para " << nomeArquivo << " (formato GraphEditor)" << endl;
}

// Salva resultado em arquivo CSV
void ColoracaoDefeituosa::salvarCSV(const Solucao& sol, const string& nomeInstancia, 
                                     const string& algoritmo, double alpha, int numIteracoes, 
                                     int tamBloco, unsigned int semente) {
    ofstream arquivo;
    
    // Verifica se arquivo já existe
    ifstream teste("resultados.csv");
    bool arquivoExiste = teste.good();
    teste.close();
    
    arquivo.open("resultados.csv", ios::app);
    
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir arquivo CSV" << endl;
        return;
    }
    
    // Escreve cabeçalho se arquivo é novo
    if (!arquivoExiste) {
        arquivo << "Data/Hora,Instancia,Algoritmo,Parametro_d,Alpha,NumIteracoes,TamanhoBloco,"
                << "Semente,Tempo(s),NumCores,NumDeficiencias,AlphaMelhor,MediaIteracoes" << endl;
    }
    
    // Pega data/hora atual
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    arquivo << fixed << setprecision(4);
    arquivo << (1900 + ltm->tm_year) << "-" 
            << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-"
            << setfill('0') << setw(2) << ltm->tm_mday << " "
            << setfill('0') << setw(2) << ltm->tm_hour << ":"
            << setfill('0') << setw(2) << ltm->tm_min << ":"
            << setfill('0') << setw(2) << ltm->tm_sec << ",";
    arquivo << nomeInstancia << ",";
    arquivo << algoritmo << ",";
    arquivo << d << ",";
    arquivo << alpha << ",";
    arquivo << numIteracoes << ",";
    arquivo << tamBloco << ",";
    arquivo << semente << ",";
    arquivo << sol.tempoExecucao << ",";
    arquivo << sol.numCores << ",";
    arquivo << sol.numDeficiencias << ",";
    arquivo << (sol.alphaMelhor / 100.0) << ",";
    arquivo << sol.mediaIteracoes << endl;
    
    arquivo.close();
    cout << "Resultado salvo em resultados.csv" << endl;
}

// Valida solução
bool ColoracaoDefeituosa::validarSolucao(const Solucao& sol) {
    int n = grafo.getNumVertices();
    
    // Verifica se todos os vértices foram coloridos
    for (int v = 0; v < n; v++) {
        if (sol.cores[v] == -1) {
            return false;
        }
    }
    
    // Verifica se cada vértice respeita a restrição d
    for (int v = 0; v < n; v++) {
        int deficiencias = 0;
        const vector<int>& vizinhos = grafo.getAdjacentes(v);
        
        for (int u : vizinhos) {
            if (sol.cores[v] == sol.cores[u]) {
                deficiencias++;
            }
        }
        
        if (deficiencias > d) {
            return false;
        }
    }
    
    return true;
}
