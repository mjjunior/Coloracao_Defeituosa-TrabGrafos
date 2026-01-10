# Coloração Defeituosa

Implementação de algoritmos para o problema de Coloração Defeituosa de Grafos.

## Descrição

O problema permite que vértices adjacentes recebam a mesma cor, desde que cada vértice não exceda `d` deficiências (vizinhos com mesma cor). O objetivo é minimizar o número de cores utilizadas.

## Algoritmos Implementados

1. **Guloso**: Coloração por ordem de grau decrescente
2. **GRASP**: Guloso Randomizado Adaptativo com parâmetro α
3. **GRASP Reativo**: Aprendizado automático de probabilidades de α

## Compilação

### Linux
```bash
cd src
make
# ou
./compilar.sh
```

### Windows
```powershell
cd src
.\compilar.ps1
```

## Execução

### Linha de Comando
```bash
# Algoritmo Guloso
./coloracao <instancia> <d> guloso

# GRASP
./coloracao <instancia> <d> grasp <alpha> <num_iteracoes> [semente]

# Reativo
./coloracao <instancia> <d> reativo <num_iteracoes> <tamanho_bloco> [semente]
```

### Exemplos
```bash
./coloracao grafo_exemplo.txt 1 guloso
./coloracao grafo_exemplo.txt 1 grasp 0.5 100
./coloracao grafo_exemplo.txt 1 reativo 100 10
```

### Modo Interativo
```bash
./coloracao
```

## Formato de Instância

```
<numero_vertices>
<numero_arestas>
<u> <v>
<u> <v>
...
```

## Saída

- **Console**: Solução detalhada com número de cores e deficiências
- **resultados.csv**: Histórico de execuções com métricas
- **Validação**: Verifica automaticamente se solução respeita restrição d

## Estrutura do Projeto

```
├── src/
│   ├── Grafo.h/cpp              # TAD Grafo
│   ├── ColoracaoDefeituosa.h/cpp # Algoritmos
│   ├── main.cpp                  # Programa principal
│   ├── Makefile                  # Compilação Linux
│   ├── compilar.ps1              # Script Windows
│   └── compilar.sh               # Script Linux
├── grafo_exemplo.txt             # Instância exemplo
└── INSTRUCOES.txt                # Instruções detalhadas
```

## Autores

Trabalho de Grafos - DCC059

## Instruções Detalhadas

Veja [INSTRUCOES.txt](INSTRUCOES.txt) para instruções completas de compilação e execução.
