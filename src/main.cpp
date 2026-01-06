#include <iostream>
#include "Grafo.h"

using namespace std;

int main() {
    cout << "Teste do Grafo" << endl;

    Grafo g(5);

    g.adicionarAresta(0, 1);
    g.adicionarAresta(0, 2);
    g.adicionarAresta(1, 3);
    g.adicionarAresta(3, 4);

    g.imprimir();

    //cout << "\nPressione ENTER para sair...";
   // cin.get();

    return 0;
}
