#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Grafo.h"
#include "ColoracaoDefeituosa.h"

using namespace std;

void imprimirUso() {
    cout << "Uso: ./programa <instancia> <d> <algoritmo> [parametros]" << endl;
    cout << "\nParametros:" << endl;
    cout << "  <instancia> : caminho para arquivo de instancia do grafo" << endl;
    cout << "  <d>         : parametro d (numero maximo de deficiencias por vertice)" << endl;
    cout << "  <algoritmo> : guloso | grasp | reativo" << endl;
    cout << "\nParametros adicionais:" << endl;
    cout << "  Para GRASP:" << endl;
    cout << "    <alpha> <numIteracoes> [semente]" << endl;
    cout << "  Para Reativo:" << endl;
    cout << "    <numIteracoes> <tamanhoBloco> [semente]" << endl;
    cout << "\nExemplos:" << endl;
    cout << "  ./programa grafo.txt 1 guloso" << endl;
    cout << "  ./programa grafo.txt 1 grasp 0.5 100" << endl;
    cout << "  ./programa grafo.txt 1 grasp 0.5 100 12345" << endl;
    cout << "  ./programa grafo.txt 1 reativo 100 10" << endl;
    cout << "  ./programa grafo.txt 1 reativo 100 10 67890" << endl;
}

int main(int argc, char* argv[]) {
    // Modo interativo se não houver argumentos
    if (argc == 1) {
        cout << "=== Coloracao Defeituosa - Modo Interativo ===" << endl;
        
        // Cria um grafo de exemplo
        cout << "\nCriando grafo de exemplo..." << endl;
        Grafo g(6);
        g.adicionarAresta(0, 1);
        g.adicionarAresta(0, 2);
        g.adicionarAresta(1, 2);
        g.adicionarAresta(1, 3);
        g.adicionarAresta(2, 3);
        g.adicionarAresta(3, 4);
        g.adicionarAresta(3, 5);
        g.adicionarAresta(4, 5);
        
        cout << "Grafo:" << endl;
        g.imprimir();
        
        int d = 1;
        cout << "\nParametro d (deficiencias maximas por vertice): " << d << endl;
        
        // Inicializa semente
        unsigned int semente = time(0);
        srand(semente);
        cout << "Semente de randomizacao: " << semente << endl;
        
        ColoracaoDefeituosa coloracao(g, d);
        
        int opcao;
        do {
            cout << "\n=== MENU ===" << endl;
            cout << "1 - Algoritmo Guloso" << endl;
            cout << "2 - Algoritmo Randomizado (GRASP)" << endl;
            cout << "3 - Algoritmo Reativo (GRASP Reativo)" << endl;
            cout << "4 - Exportar grafo para GraphEditor" << endl;
            cout << "0 - Sair" << endl;
            cout << "Escolha uma opcao: ";
            cin >> opcao;
            
            switch(opcao) {
                case 1: {
                    cout << "\n--- ALGORITMO GULOSO ---" << endl;
                    Solucao sol = coloracao.algoritmoGuloso();
                    coloracao.imprimirSolucao(sol);
                    coloracao.salvarCSV(sol, "exemplo", "Guloso", -1, 0, 0, semente);
                    break;
                }
                
                case 2: {
                    cout << "\n--- ALGORITMO RANDOMIZADO (GRASP) ---" << endl;
                    double alpha;
                    int numIter;
                    cout << "Digite o valor de alpha (0.0 a 1.0): ";
                    cin >> alpha;
                    cout << "Digite o numero de iteracoes: ";
                    cin >> numIter;
                    
                    Solucao sol = coloracao.algoritmoRandomizado(numIter, alpha);
                    coloracao.imprimirSolucao(sol);
                    coloracao.salvarCSV(sol, "exemplo", "GRASP", alpha, numIter, 0, semente);
                    break;
                }
                
                case 3: {
                    cout << "\n--- ALGORITMO REATIVO (GRASP Reativo) ---" << endl;
                    int numIter, tamBloco;
                    cout << "Digite o numero de iteracoes: ";
                    cin >> numIter;
                    cout << "Digite o tamanho do bloco: ";
                    cin >> tamBloco;
                    
                    Solucao sol = coloracao.algoritmoReativo(numIter, tamBloco);
                    coloracao.imprimirSolucao(sol);
                    coloracao.salvarCSV(sol, "exemplo", "Reativo", -1, numIter, tamBloco, semente);
                    break;
                }
                
                case 4: {
                    g.exportarGraphEditor("grafo_output.txt");
                    cout << "Grafo exportado para grafo_output.txt" << endl;
                    cout << "Cole o conteudo em: http://csacademy.com/app/grapheditor/" << endl;
                    break;
                }
                
                case 0:
                    cout << "Saindo..." << endl;
                    break;
                    
                default:
                    cout << "Opcao invalida!" << endl;
            }
        } while(opcao != 0);
        
        return 0;
    }
    
    // Modo linha de comando
    if (argc < 4) {
        imprimirUso();
        return 1;
    }
    
    string arquivoInstancia = argv[1];
    int d = atoi(argv[2]);
    string algoritmo = argv[3];
    
    // Lê grafo do arquivo
    cout << "Lendo instancia: " << arquivoInstancia << endl;
    Grafo* g = Grafo::lerArquivo(arquivoInstancia);
    
    if (g == nullptr) {
        cerr << "Erro ao carregar instancia!" << endl;
        return 1;
    }
    
    cout << "Grafo carregado: " << g->getNumVertices() << " vertices, " 
         << g->getNumArestas() << " arestas" << endl;
    cout << "Parametro d: " << d << endl;
    
    ColoracaoDefeituosa coloracao(*g, d);
    
    // Inicializa semente de randomizacao
    unsigned int semente;
    
    if (algoritmo == "guloso") {
        semente = time(0);
        srand(semente);
        cout << "Semente: " << semente << endl;
        
        cout << "\n--- EXECUTANDO ALGORITMO GULOSO ---" << endl;
        Solucao sol = coloracao.algoritmoGuloso();
        coloracao.imprimirSolucao(sol);
        coloracao.salvarCSV(sol, arquivoInstancia, "Guloso", -1, 0, 0, semente);
        
    } else if (algoritmo == "grasp") {
        if (argc < 6) {
            cout << "Parametros insuficientes para GRASP!" << endl;
            imprimirUso();
            delete g;
            return 1;
        }
        
        double alpha = atof(argv[4]);
        int numIteracoes = atoi(argv[5]);
        
        // Semente opcional
        if (argc >= 7) {
            semente = atoi(argv[6]);
        } else {
            semente = time(0);
        }
        srand(semente);
        
        cout << "Semente: " << semente << endl;
        cout << "Alpha: " << alpha << endl;
        cout << "Numero de iteracoes: " << numIteracoes << endl;
        
        cout << "\n--- EXECUTANDO ALGORITMO GRASP ---" << endl;
        Solucao sol = coloracao.algoritmoRandomizado(numIteracoes, alpha);
        coloracao.imprimirSolucao(sol);
        coloracao.salvarCSV(sol, arquivoInstancia, "GRASP", alpha, numIteracoes, 0, semente);
        
    } else if (algoritmo == "reativo") {
        if (argc < 6) {
            cout << "Parametros insuficientes para Reativo!" << endl;
            imprimirUso();
            delete g;
            return 1;
        }
        
        int numIteracoes = atoi(argv[4]);
        int tamBloco = atoi(argv[5]);
        
        // Semente opcional
        if (argc >= 7) {
            semente = atoi(argv[6]);
        } else {
            semente = time(0);
        }
        srand(semente);
        
        cout << "Semente: " << semente << endl;
        cout << "Numero de iteracoes: " << numIteracoes << endl;
        cout << "Tamanho do bloco: " << tamBloco << endl;
        
        cout << "\n--- EXECUTANDO ALGORITMO REATIVO ---" << endl;
        Solucao sol = coloracao.algoritmoReativo(numIteracoes, tamBloco);
        coloracao.imprimirSolucao(sol);
        coloracao.salvarCSV(sol, arquivoInstancia, "Reativo", -1, numIteracoes, tamBloco, semente);
        
    } else {
        cout << "Algoritmo invalido: " << algoritmo << endl;
        imprimirUso();
        delete g;
        return 1;
    }
    
    delete g;
    return 0;
}
