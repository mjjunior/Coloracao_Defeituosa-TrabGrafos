# Script para executar o programa
# Uso: .\executar.ps1 [teste]
# Exemplos:
#   .\executar.ps1           # Modo interativo
#   .\executar.ps1 teste     # Teste rapido
#   .\executar.ps1 guloso    # Algoritmo guloso
#   .\executar.ps1 grasp     # Algoritmo GRASP
#   .\executar.ps1 reativo   # Algoritmo Reativo

# Adiciona MSYS2 ao PATH temporariamente
$env:Path += ";C:\msys64\mingw64\bin"

$opcao = $args[0]

if ($opcao -eq "teste") {
    Write-Host "`n=== TESTE RAPIDO ===" -ForegroundColor Green
    .\teste.exe
}
elseif ($opcao -eq "guloso") {
    Write-Host "`n=== ALGORITMO GULOSO ===" -ForegroundColor Green
    .\coloracao.exe ..\grafo_exemplo.txt 1 guloso
}
elseif ($opcao -eq "grasp") {
    Write-Host "`n=== ALGORITMO GRASP ===" -ForegroundColor Green
    Write-Host "Parametros: alpha=0.5, 50 iteracoes`n" -ForegroundColor Cyan
    .\coloracao.exe ..\grafo_exemplo.txt 1 grasp 0.5 50
}
elseif ($opcao -eq "reativo") {
    Write-Host "`n=== ALGORITMO REATIVO ===" -ForegroundColor Green
    Write-Host "Parametros: 50 iteracoes, blocos de 10`n" -ForegroundColor Cyan
    .\coloracao.exe ..\grafo_exemplo.txt 1 reativo 50 10
}
else {
    Write-Host "`n=== MODO INTERATIVO ===" -ForegroundColor Green
    Write-Host "Use Ctrl+C para sair`n" -ForegroundColor Yellow
    .\coloracao.exe
}
