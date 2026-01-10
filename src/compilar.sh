#!/bin/bash
# Script de compilação para Linux

echo "=== Compilando Coloracao Defeituosa ==="

compiler="g++"
flags="-std=c++11 -O3 -Wall"
output="coloracao"
sources="main.cpp Grafo.cpp ColoracaoDefeituosa.cpp"

# Verifica se g++ está disponível
if ! command -v $compiler &> /dev/null; then
    echo "ERRO: Compilador g++ não encontrado!"
    echo ""
    echo "Instale com:"
    echo "  Ubuntu/Debian: sudo apt-get install g++"
    echo "  Fedora: sudo dnf install gcc-c++"
    echo "  Arch: sudo pacman -S gcc"
    exit 1
fi

# Compila
echo "Compilando..."
echo "$compiler $flags -o $output $sources"

$compiler $flags -o $output $sources

if [ $? -eq 0 ]; then
    echo ""
    echo "=== Compilação bem-sucedida! ==="
    echo "Executável: $output"
    chmod +x $output
else
    echo ""
    echo "=== Falha na compilação ==="
    exit 1
fi
