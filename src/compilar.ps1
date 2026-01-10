# Script de compilação para Windows
# Execute: .\compilar.ps1

Write-Host "=== Compilando Coloracao Defeituosa ===" -ForegroundColor Green

# Adiciona MSYS2 ao PATH temporariamente
$env:Path += ";C:\msys64\mingw64\bin"

$compiler = "g++"
$flags = "-std=c++11 -O3 -Wall"
$output = "coloracao.exe"
$sources = "main.cpp Grafo.cpp ColoracaoDefeituosa.cpp"

# Verifica se g++ está disponível
if (!(Get-Command $compiler -ErrorAction SilentlyContinue)) {
    Write-Host "ERRO: Compilador g++ nao encontrado!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Instale o MinGW-w64 ou MSYS2:" -ForegroundColor Yellow
    Write-Host "  - MinGW-w64: https://www.mingw-w64.org/downloads/"
    Write-Host "  - MSYS2: https://www.msys2.org/"
    Write-Host ""
    Write-Host "Ou use o Visual Studio Build Tools com MSVC" -ForegroundColor Yellow
    exit 1
}

# Compila
Write-Host "Compilando..." -ForegroundColor Cyan
$cmd = "$compiler $flags -o $output $sources"
Write-Host $cmd -ForegroundColor Gray

Invoke-Expression $cmd

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "=== Compilacao bem-sucedida! ===" -ForegroundColor Green
    Write-Host "Executavel: $output"
    Write-Host ""
    Write-Host "Para executar:" -ForegroundColor Cyan
    Write-Host "  .\executar.ps1          # Modo interativo" -ForegroundColor White
    Write-Host "  .\executar.ps1 guloso   # Algoritmo guloso" -ForegroundColor White
    Write-Host "  .\executar.ps1 grasp    # Algoritmo GRASP" -ForegroundColor White
    Write-Host "  .\executar.ps1 reativo  # Algoritmo Reativo" -ForegroundColor White
} else {
    Write-Host ""
    Write-Host "=== Falha na compilacao ===" -ForegroundColor Red
    exit 1
}
