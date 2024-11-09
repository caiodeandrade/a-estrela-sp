# Shortest Path Calculation with A* Algorithm
Este projeto implementa um sistema de cálculo de rotas para encontrar o caminho mais curto em uma área modelada como grafo. A estrutura do grafo representa as ruas de um bairro, com distâncias reais entre interseções, e utiliza o algoritmo A* para a busca eficiente do caminho entre um ponto de origem e um de destino.

## Funcionalidades
- Modelagem de um bairro como grafo, onde os vértices representam pontos de interseção e as arestas representam as ruas.
- Utilização do algoritmo A* para encontrar o caminho mais curto entre dois pontos no grafo.
- Apresentação de uma lista de pontos (vértices) disponíveis para seleção como origem e destino.
- Exibição do caminho calculado, com detalhes como sequência de pontos visitados e distância total percorrida.
- Opção para exibir os detalhes da árvore gerada pelo algoritmo A* durante a execução.

## Estrutura do Projeto
O projeto está estruturado em um arquivo principal, astar.c, que contém as implementações:

1 - Criação do Grafo: Inicializa os vértices e arestas com base na área escolhida e nas distâncias fornecidas.
2 - Algoritmo A*: Implementação completa para busca de menor caminho com custos g (custo do caminho) e h (heurística estimada).
3 - Interação com Usuário: Permite que o usuário selecione pontos de origem e destino e visualize o caminho encontrado.

## Como Executar
### Compile o programa:
```bash
gcc astar.c -o astar
```
### Execute o programa:

```bash
./astar
```
Escolha os pontos de origem e destino conforme a lista exibida e opte por exibir (ou não) os detalhes da execução do algoritmo A*.

## Estrutura de Dados
O grafo é representado por uma lista de adjacências, onde cada vértice possui um identificador, nome, custos de caminho e heurística, e uma lista de arestas incidentes. A modelagem reflete a área selecionada e respeita as distâncias reais entre as interseções.

## Exemplos de Uso
O programa apresenta uma lista com todos os pontos do grafo. Você pode selecionar um ponto de origem e um destino, e o sistema calculará a rota mais curta, exibindo o caminho e a distância total.
