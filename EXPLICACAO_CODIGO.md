# Explicacao do Codigo - Coloracao Defeituosa de Grafos

## 1. Visao Geral do Projeto

Este projeto resolve o problema de **Coloracao Defeituosa de Grafos**, que eh uma variacao da coloracao tradicional.

**Coloracao Tradicional**: Nenhum vertice adjacente pode ter a mesma cor.

**Coloracao Defeituosa (d-defeituosa)**: Um vertice pode ter ATE `d` vizinhos com a mesma cor dele.

**Objetivo**: Minimizar o numero de cores usadas, respeitando que cada vertice tenha no maximo `d` deficiencias (vizinhos com a mesma cor).

---

## 2. Estrutura de Arquivos

```
src/
├── Grafo.h              -> Interface da classe Grafo
├── Grafo.cpp            -> Implementacao do TAD Grafo
├── ColoracaoDefeituosa.h -> Interface dos algoritmos
├── ColoracaoDefeituosa.cpp -> Implementacao dos 3 algoritmos
└── main.cpp             -> Interface CLI e execucao
```

---

## 3. Classe Grafo (Grafo.h e Grafo.cpp)

### Por que existe?
Precisamos de uma estrutura para representar o grafo e suas operacoes basicas.

### Estrutura de Dados Escolhida: Lista de Adjacencias
```cpp
vector<vector<int>> adj;  // adj[v] = lista de vizinhos do vertice v
```

**Por que lista de adjacencias e nao matriz?**
- Grafos esparsos (poucas arestas): Lista usa menos memoria
- Operacoes mais eficientes para percorrer vizinhos

### Funcoes Principais:

#### `Grafo(int n)`
**O que faz**: Construtor que cria um grafo com `n` vertices.
**Por que**: Inicializa a estrutura de dados (vector de tamanho n).

#### `void adicionarAresta(int u, int v)`
**O que faz**: Adiciona uma aresta entre vertices `u` e `v`.
**Por que**: Grafo nao-direcionado, entao adiciona u->v E v->u.
**Detalhe importante**: Evita arestas duplicadas com verificacao.

#### `int getGrau(int v)`
**O que faz**: Retorna quantos vizinhos o vertice `v` tem.
**Por que**: Usado no algoritmo Guloso para ordenar vertices por grau.

#### `bool existeAresta(int u, int v)`
**O que faz**: Verifica se existe aresta entre `u` e `v`.
**Por que**: Usado na validacao da solucao para contar deficiencias.

#### `static Grafo lerArquivo(const string& nomeArquivo)`
**O que faz**: Le um arquivo de texto e cria o grafo.
**Por que**: Formato padrao de entrada do trabalho.
**Formato esperado**:
```
n m      <- n vertices, m arestas
u1 v1    <- primeira aresta
u2 v2    <- segunda aresta
...
```

#### `void exportarGraphEditor(const string& arquivo)`
**O que faz**: Exporta o grafo para formato do GraphEditor.
**Por que**: Permite visualizar o grafo online (http://csacademy.com/app/grapheditor/).

---

## 4. Estrutura Solucao (ColoracaoDefeituosa.h)

### Por que existe?
Precisamos guardar o resultado de um algoritmo de coloracao.

### Campos:
```cpp
struct Solucao {
    vector<int> cores;           // cores[v] = cor do vertice v
    int numCores;                // Quantas cores foram usadas
    vector<int> deficiencias;    // deficiencias[v] = num de vizinhos com mesma cor
    double tempo;                // Tempo de execucao em segundos
    double alphaMelhor;          // Melhor alpha (so para Reativo)
    double mediaIteracoes;       // Media de iteracoes por alpha (Reativo)
};
```

**Por que cada campo?**
- `cores`: A solucao propriamente dita
- `numCores`: Metrica principal (queremos minimizar)
- `deficiencias`: Para validar se eh d-defeituosa
- `tempo`: Medir eficiencia do algoritmo
- `alphaMelhor` e `mediaIteracoes`: Estatisticas do algoritmo Reativo

---

## 5. Algoritmos de Coloracao

### 5.1 Algoritmo Guloso (`algoritmoGuloso`)

**O que faz**: Colore vertices em ordem decrescente de grau, escolhendo a menor cor valida.

**Como funciona (passo a passo)**:
1. Ordena vertices por grau (maior grau primeiro)
2. Para cada vertice v:
   - Conta quantos vizinhos ja tem de cada cor
   - Escolhe a menor cor que mantenha deficiencia <= d
   - Se nenhuma cor serve, cria uma cor nova

**Por que ordenar por grau?**
- Vertices com mais vizinhos sao mais "dificeis"
- Colorir os dificeis primeiro da mais opcoes para os faceis

**Complexidade**: O(n²) - rapido e deterministico

**Codigo simplificado**:
```cpp
// Ordena por grau decrescente
sort(ordem.begin(), ordem.end(), [&](int a, int b) {
    return grafo.getGrau(a) > grafo.getGrau(b);
});

for (int v : ordem) {
    // Conta vizinhos de cada cor
    for (int viz : grafo.adj[v]) {
        if (cores[viz] != -1)
            contCores[cores[viz]]++;
    }
    
    // Escolhe menor cor valida
    for (int cor = 0; cor < numCores; cor++) {
        if (contCores[cor] <= d) {
            cores[v] = cor;
            break;
        }
    }
    
    // Se nenhuma cor serve, cria nova
    if (cores[v] == -1) {
        cores[v] = numCores++;
    }
}
```

---

### 5.2 Algoritmo GRASP (`algoritmoRandomizado`)

**O que faz**: Versao randomizada que explora multiplas solucoes.

**GRASP = Greedy Randomized Adaptive Search Procedure**

**Como funciona (passo a passo)**:
1. Repete `numIteracoes` vezes:
   - **Fase Construtiva**: Constroi uma solucao aleatoria gulosa
   - Guarda a melhor solucao encontrada

**Fase Construtiva (o diferencial)**:
- Cria uma **Lista Restrita de Candidatos (RCL)**
- Em vez de sempre escolher o melhor vertice, escolhe aleatoriamente da RCL

**O que eh a RCL?**
```cpp
// Calcula grau minimo e maximo dos vertices nao coloridos
double limiar = grauMin + alpha * (grauMax - grauMin);

// RCL = vertices com grau >= limiar
if (grau >= limiar)
    rcl.push_back(v);
```

**Parametro alpha (0.0 a 1.0)**:
- `alpha = 0.0`: RCL so tem vertices de grau maximo (mais guloso)
- `alpha = 1.0`: RCL tem todos os vertices (mais aleatorio)
- `alpha = 0.3`: Balanco entre guloso e aleatorio

**Por que usar GRASP?**
- Algoritmo Guloso pode ficar preso em solucao local
- Aleatoriedade ajuda a explorar outras solucoes
- Multiplas iteracoes aumentam chance de achar o otimo

**Complexidade**: O(iteracoes × n²) - mais lento, mas solucoes melhores

---

### 5.3 Algoritmo Reativo (`algoritmoReativo`)

**O que faz**: GRASP inteligente que **aprende** qual alpha funciona melhor.

**Problema do GRASP normal**: Como escolher o melhor alpha?

**Solucao do Reativo**: Testa varios alphas e aprende qual eh melhor!

**Como funciona (passo a passo)**:
1. Define 10 valores de alpha (0.1, 0.2, ..., 1.0)
2. Cada alpha comeca com probabilidade igual (10%)
3. A cada iteracao:
   - Sorteia um alpha baseado nas probabilidades
   - Executa GRASP com esse alpha
   - Se achar solucao melhor: aumenta probabilidade desse alpha
   - Atualiza probabilidades a cada `atualizacao` iteracoes

**Como atualiza probabilidades?**
```cpp
// Calcula valor medio de cada alpha
double qi = somaValores[i] / numUsos[i];

// Alpha que acha melhores solucoes fica com maior probabilidade
probabilidades[i] = qi / somaQs;
```

**Mecanismo de Aprendizado**:
- Alpha que acha solucoes com menos cores -> mais usado
- Alpha que acha solucoes ruins -> menos usado
- Sistema se adapta ao grafo especifico

**Por que Reativo eh melhor?**
- Nao precisa adivinhar o melhor alpha
- Aprende automaticamente durante a execucao
- Cada grafo tem caracteristicas diferentes

**Complexidade**: O(iteracoes × n²) - similar ao GRASP, mas aprende

---

## 6. Funcoes de Utilidade

### `validarSolucao`
**O que faz**: Verifica se a solucao respeita d-defeituoso.
**Como**: Para cada vertice, conta vizinhos com mesma cor.
**Retorna**: true se TODOS vertices tem <= d deficiencias.

### `salvarCSV`
**O que faz**: Exporta resultados para arquivo CSV.
**Por que**: Facilita analise dos resultados no Excel/Python.
**Colunas exportadas**:
- Timestamp, algoritmo, parametros (d, iteracoes, alpha)
- Metricas (numCores, deficiencias, tempo)
- Validacao, seed, arquivo

### `calcularDeficiencias`
**O que faz**: Conta deficiencias de cada vertice.
**Como**: Para cada vertice, percorre vizinhos e conta cores iguais.
**Por que**: Necessario para validacao e exportacao.

---

## 7. Main.cpp - Interface CLI

### Por que dois modos?

#### Modo Interativo (sem argumentos)
**Quando usar**: Testes rapidos, explorar o programa.
**Como funciona**: Menu com opcoes numeradas.

#### Modo Linha de Comando (com argumentos)
**Quando usar**: Executar varios testes, scripts automatizados.
**Formato**:
```
./coloracao <arquivo> <algoritmo> <d> [iteracoes] [alpha] [seed]
```

### Geracao de Seed
```cpp
unsigned int seed = time(0);
srand(seed);
cout << "Seed: " << seed << endl;
```

**Por que?**
- Reprodutibilidade: mesma seed = mesmos resultados
- Seed opcional: pode passar como parametro
- Impressao: usuario sabe qual seed foi usada

---

## 8. Fluxo de Execucao Completo

### Exemplo: `./coloracao grafo.txt grasp 2 100 0.3`

1. **main.cpp**: Le argumentos da linha de comando
2. **Grafo::lerArquivo**: Le e constroi o grafo
3. **main.cpp**: Gera seed e inicializa gerador aleatorio
4. **algoritmoRandomizado**: Executa GRASP
   - 100 iteracoes
   - alpha = 0.3
   - Para cada iteracao:
     - Constroi RCL baseada em graus
     - Escolhe vertice aleatorio da RCL
     - Colore com menor cor valida
5. **validarSolucao**: Verifica se solucao eh valida
6. **salvarCSV**: Exporta resultado para resultados.csv
7. **main.cpp**: Imprime resumo na tela

---

## 9. Diferencas Entre os Algoritmos

| Aspecto | Guloso | GRASP | Reativo |
|---------|--------|-------|---------|
| Deterministico | SIM | NAO | NAO |
| Velocidade | Rapido | Medio | Medio |
| Qualidade | Boa | Melhor | Melhor+ |
| Parametros | Nenhum | alpha | alpha auto |
| Uso | Baseline | Qualidade | Melhor opcao |

---

## 10. Decisoes de Implementacao

### Por que C++?
- Velocidade (grafos grandes)
- STL (vector, map, set) facilita implementacao
- Padrao da disciplina

### Por que vector<int> para cores?
- Acesso O(1) por indice
- Tamanho dinamico (numero de vertices varia)

### Por que map<int, int> para contar cores?
- Numero de cores eh dinamico
- map[cor] retorna 0 se cor nao existe (conveniente)

### Por que chrono para medir tempo?
- Precisao de microsegundos
- Padrao C++11 (multiplataforma)

---

## 11. Pontos de Atencao para Apresentacao

### Validacao eh Crucial
```cpp
// SEMPRE valida antes de exportar
if (validarSolucao(grafo, solucao, d)) {
    cout << "[OK] Solucao valida!" << endl;
}
```

### Seed Permite Reproducao
- Mesmo seed = mesmos resultados aleatorios
- Importante para depuracao e comparacao

### CSV Permite Analise Estatistica
- Rodar 30 vezes cada algoritmo
- Calcular media, desvio padrao
- Comparar algoritmos cientificamente

### GraphEditor Permite Visualizacao
- Pequenos grafos: visualizar coloracao
- Entender por que certos algoritmos funcionam melhor

---

## 12. Complexidade Computacional

### Algoritmo Guloso
- Ordenacao: O(n log n)
- Coloracao: O(n × m) onde m = numero de arestas
- **Total: O(n × m)**

### GRASP e Reativo
- Por iteracao: O(n²)
- Total: **O(iteracoes × n²)**
- Mais lento, mas solucoes melhores

---

## 13. Exemplo Pratico de Execucao

### Grafo de Exemplo:
```
6 8
0 1
0 2
1 2
1 3
2 3
3 4
3 5
4 5
```

### Resultado Guloso (d=2):
```
Vertices ordenados por grau: [3, 1, 2, 4, 5, 0]
Vertice 3: cor 0 (primeiro vertice)
Vertice 1: cor 0 (pode ter 2 vizinhos com cor 0)
Vertice 2: cor 0 (ja tem 2 vizinhos cor 0, ok)
Vertice 4: cor 0 
Vertice 5: cor 0
Vertice 0: cor 1 (nao pode mais ser cor 0)

Resultado: 2 cores
```

---

## 14. Dicas para a Apresentacao

1. **Comece pelo problema**: O que eh coloracao defeituosa?
2. **Mostre o grafo de exemplo**: Use GraphEditor para visualizar
3. **Explique a estrutura**: TAD Grafo -> Algoritmos -> Main
4. **Demonstre cada algoritmo**: Comece pelo Guloso (mais simples)
5. **Compare resultados**: Mostre CSV com comparacao
6. **Destaque o Reativo**: Mecanismo de aprendizado eh interessante
7. **Mostre o codigo rodando**: Execute com diferentes parametros

---

## 15. Comandos Uteis

### Compilar
```bash
g++ -std=c++11 -O3 -o coloracao.exe main.cpp Grafo.cpp ColoracaoDefeituosa.cpp
```

### Executar modo interativo
```bash
./coloracao
```

### Executar GRASP com parametros
```bash
./coloracao grafo_exemplo.txt grasp 2 100 0.3
```

### Executar Reativo
```bash
./coloracao grafo_exemplo.txt reativo 2 100
```

### Visualizar resultados
```bash
# Abrir resultados.csv no Excel
# Ou usar Python/R para graficos
```

---

## Resumo Final

Este projeto implementa 3 algoritmos para coloracao defeituosa:
1. **Guloso**: Rapido, deterministico, baseline
2. **GRASP**: Randomizado, explora multiplas solucoes
3. **Reativo**: Aprende automaticamente o melhor parametro

A estrutura modular (Grafo separado dos algoritmos) facilita manutencao e testes.
Os resultados sao exportados em CSV para analise estatistica.
A validacao garante que todas as solucoes respeitam a restricao d-defeituosa.
