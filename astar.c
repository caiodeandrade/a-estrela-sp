#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

// Estrutura para representar uma aresta do grafo
typedef struct aresta {
    int outroExtr;      // Vértice que esta aresta conecta
    float peso;         // Peso da aresta (distância entre vértices)
    struct aresta *prox; // Ponteiro para a próxima aresta na lista
} Aresta;

// Estrutura para representar um vértice do grafo
typedef struct vertice {
    int id;             // Identificador numérico do vértice
    char nome[100];     // Nome do vértice
    float gCost;        // Custo do caminho desde o início até este vértice
    float hCost;        // Heurística estimada para chegar ao destino
    float fCost;        // Soma de gCost e hCost
    int visitado;       // Flag para indicar se o vértice foi visitado
    Aresta *prim;       // Ponteiro para a lista de arestas incidentes
    int anterior;       // Para rastrear o caminho percorrido
} Vert;

// Funções para manipulação do grafo
void criaGrafo(Vert **G, int ordem);
void destroiGrafo(Vert **G, int ordem);
int incluiNovaAresta(Vert G[], int ordem, int v1, int v2, float peso);

// Funções para o Algoritmo A*
float calculaHeuristica(int v1, int v2);
void aStar(Vert G[], int ordem, int inicio, int destino, int exibirDetalhes);

// Criação de um grafo com vértices sem arestas
void criaGrafo(Vert **G, int ordem) {
    *G = (Vert*) malloc(sizeof(Vert) * ordem); // Alocação de memória para os vértices
    for (int i = 0; i < ordem; i++) {
        (*G)[i].id = i;           // Atribuindo o identificador do vértice
        (*G)[i].prim = NULL;        // Inicializando a lista de arestas como vazia
        (*G)[i].gCost = INT_MAX;    // Inicializando gCost como infinito
        (*G)[i].hCost = 0;          // Inicializando hCost como zero
        (*G)[i].fCost = INT_MAX;    // Inicializando fCost como infinito
        (*G)[i].visitado = 0;       // Marcando o vértice como não visitado
        (*G)[i].anterior = -1;      // Sem predecessor no início
        strcpy((*G)[i].nome, "");   // Inicializa o nome como vazio
    }
}

// Deasaloca a memória de um grafo
void destroiGrafo(Vert **G, int ordem) {
    for (int i = 0; i < ordem; i++) {
        Aresta *a = (*G)[i].prim;
        while (a != NULL) {
            Aresta *prox = a->prox; // Armazena o próximo para liberar memória
            free(a);                // Libera a aresta atual
            a = prox;               // Avança para a próxima aresta
        }
    }
    free(*G); // Libera a memória do grafo
}

// Adiciona uma aresta bidirecional com peso
int incluiNovaAresta(Vert G[], int ordem, int v1, int v2, float peso) {
    if (v1 < 0 || v1 >= ordem || v2 < 0 || v2 >= ordem) return 0; // Verifica se os vértices são válidos
    // Cria a aresta de v1 para v2
    Aresta *A1 = (Aresta *) malloc(sizeof(Aresta));
    A1->outroExtr = v2;      // Define o vértice de destino
    A1->peso = peso;         // Define o peso da aresta
    A1->prox = G[v1].prim;   // Adiciona à lista de arestas do vértice v1
    G[v1].prim = A1;         // Atualiza a lista de arestas de v1

    // Cria a aresta de v2 para v1 (grafo não orientado)
    Aresta *A2 = (Aresta *) malloc(sizeof(Aresta));
    A2->outroExtr = v1;      // Define o vértice de destino
    A2->peso = peso;         // Define o peso da aresta
    A2->prox = G[v2].prim;   // Adiciona à lista de arestas do vértice v2
    G[v2].prim = A2;         // Atualiza a lista de arestas de v2
    
    return 1; // Indica que a aresta foi adicionada com sucesso
}

// Calcula a heurística entre dois vértices (pode ser ajustada para o problema específico)
float calculaHeuristica(int v1, int v2) {
    return fabs(v1 - v2); // Exemplo simples de heurística (substituir por uma fórmula real se disponível)
}

// Implementação do algoritmo A* com exibição de detalhes
void aStar(Vert G[], int ordem, int inicio, int destino, int exibirDetalhes) {
    // Inicializa o vértice inicial com custo 0 e calcula a heurística até o destino
    G[inicio].gCost = 0;
    G[inicio].hCost = calculaHeuristica(inicio, destino);
    G[inicio].fCost = G[inicio].gCost + G[inicio].hCost;
    
    int atual = inicio; // Define o ponto de partida como o vértice atual

    // Laço principal que continua até encontrar o destino
    while (atual != destino) {
        G[atual].visitado = 1; // Marca o vértice atual como visitado

        // Se o usuário optou por exibir detalhes, imprime informações sobre o vértice atual
        if (exibirDetalhes) {
            printf("\nExpandindo %s:\n", G[atual].nome);
            printf("  gCost: %.2f, hCost: %.2f, fCost: %.2f\n", G[atual].gCost, G[atual].hCost, G[atual].fCost);
        }
        
        // Itera sobre todos os vizinhos do vértice atual
        Aresta *a = G[atual].prim;
        while (a != NULL) {
            int vizinho = a->outroExtr;

            // Verifica se o vizinho já foi visitado; se não, calcula novos custos
            if (!G[vizinho].visitado) {
                float novoGCost = G[atual].gCost + a->peso; // Calcula o custo total para o vizinho

                // Atualiza os custos do vizinho se um caminho mais curto foi encontrado
                if (novoGCost < G[vizinho].gCost) {
                    G[vizinho].gCost = novoGCost;
                    G[vizinho].hCost = calculaHeuristica(vizinho, destino);
                    G[vizinho].fCost = G[vizinho].gCost + G[vizinho].hCost;
                    G[vizinho].anterior = atual; // Armazena o vértice atual como o predecessor do vizinho

                    // Se detalhes estão habilitados, exibe as atualizações do vizinho
                    if (exibirDetalhes) {
                        printf("  Atualizando %s: gCost=%.2f, hCost=%.2f, fCost=%.2f\n", G[vizinho].nome, G[vizinho].gCost, G[vizinho].hCost, G[vizinho].fCost);
                    }
                }
            }
            a = a->prox; // Passa para a próxima aresta (vizinho)
        }
        
        // Seleciona o próximo vértice a ser expandido: aquele com o menor fCost
        float menorF = INT_MAX;
        atual = -1;
        for (int i = 0; i < ordem; i++) {
            if (!G[i].visitado && G[i].fCost < menorF) {
                menorF = G[i].fCost;
                atual = i;
            }
        }
        
        // Se nenhum vértice foi encontrado, então não há caminho para o destino
        if (atual == -1) {
            printf("Caminho não encontrado\n");
            return;
        }
    }
    
    // Imprime o caminho encontrado ao final do algoritmo
    printf("\nCaminho encontrado:\n");
    int caminho[ordem];
    int passos = 0;
    float distTotal = G[destino].gCost;

    // Armazena o caminho percorrido para exibição
    for (int v = destino; v != -1; v = G[v].anterior) {
        caminho[passos++] = v;
    }
    
    // Imprime o caminho em ordem correta, do início ao destino
    for (int i = passos - 1; i > 0; i--) {
        printf("%s -> ", G[caminho[i]].nome);
    }
    printf("%s\n", G[caminho[0]].nome);
    printf("Distância total: %.2f metros\n", distTotal);
}

// Função principal para exibir a lista de locais e permitir entrada do usuário
int main() {
    Vert *G;
    int ordemG = 35; // Define a quantidade de vértices

    criaGrafo(&G, ordemG); // Cria o grafo com 35 vértices

    // Nomes dos vértices
    char* vertexNames[35] = {
        "Esquina Paulista - Rocha Azevedo",    // V1
        "Esquina Santos - Rocha Azevedo",      // V2
        "Esquina Jau - Rocha Azevedo",         // V3
        "Esquina Itu - Rocha Azevedo",         // V4
        "Esquina Franca - Rocha Azevedo",      // V5
        "Esquina Lisboa - Rocha Azevedo",      // V6
        "Esquina Paulista - Manuel",           // V7
        "Esquina Santos - Manuel",             // V8
        "Esquina Jau - Manuel",                // V9
        "Esquina Itu - Manuel",                // V10
        "Esquina Franca - Manuel",             // V11
        "Esquina Lisboa - Manuel",             // V12
        "Esquina Paulista - Augusta",          // V13
        "Esquina Santos - Augusta",            // V14
        "Esquina Jau - Augusta",               // V15
        "Esquina Itu - Augusta",               // V16
        "Esquina Franca - Augusta",            // V17
        "Esquina Tiete - Augusta",             // V18
        "Esquina Paulista - Lobo",             // V19
        "Esquina Santos - Lobo",               // V20
        "Esquina Jau - Lobo",                  // V21
        "Esquina Itu - Lobo",                  // V22
        "Esquina Franca - Lobo",               // V23
        "Esquina Paulista - Cintra",           // V24
        "Esquina Santos - Cintra",             // V25
        "Esquina Jau - Cintra",                // V26
        "Esquina Itu - Cintra",                // V27
        "Esquina Franca - Cintra",             // V28
        "Esquina Santos - Consolacao",         // V29
        "Esquina Jau - Consolacao",            // V30
        "Esquina Itu - Consolacao",            // V31
        "Esquina Franca - Consolacao",         // V32
        "Esquina Jau - Reboucas",              // V33
        "Esquina Itu - Reboucas",              // V34
        "Esquina Franca - Reboucas"            // V35
    };

    // Atribui os nomes aos vértices
    for (int i = 0; i < ordemG; i++) {
        strcpy(G[i].nome, vertexNames[i]);
    }

    // Inclui as arestas conforme as distâncias fornecidas
    // Arestas horizontais (paralelas à Avenida Paulista)
    incluiNovaAresta(G, ordemG, 0, 1, 130.0);    // V1-V2
    incluiNovaAresta(G, ordemG, 1, 2, 130.0);    // V2-V3
    incluiNovaAresta(G, ordemG, 2, 3, 140.0);    // V3-V4
    incluiNovaAresta(G, ordemG, 3, 4, 120.0);    // V4-V5
    incluiNovaAresta(G, ordemG, 4, 5, 110.0);    // V5-V6

    incluiNovaAresta(G, ordemG, 6, 7, 130.0);    // V7-V8
    incluiNovaAresta(G, ordemG, 7, 8, 130.0);    // V8-V9
    incluiNovaAresta(G, ordemG, 8, 9, 130.0);    // V9-V10
    incluiNovaAresta(G, ordemG, 9,10, 140.0);    // V10-V11
    incluiNovaAresta(G, ordemG,10,11,160.0);     // V11-V12

    incluiNovaAresta(G, ordemG,12,13,130.0);     // V13-V14
    incluiNovaAresta(G, ordemG,13,14,130.0);     // V14-V15
    incluiNovaAresta(G, ordemG,14,15,140.0);     // V15-V16
    incluiNovaAresta(G, ordemG,15,16,140.0);     // V16-V17
    incluiNovaAresta(G, ordemG,16,17,160.0);     // V17-V18

    incluiNovaAresta(G, ordemG,18,19,120.0);     // V19-V20
    incluiNovaAresta(G, ordemG,19,20,130.0);     // V20-V21
    incluiNovaAresta(G, ordemG,20,21,140.0);     // V21-V22
    incluiNovaAresta(G, ordemG,21,22,140.0);     // V22-V23

    incluiNovaAresta(G, ordemG,23,24,130.0);     // V24-V25
    incluiNovaAresta(G, ordemG,24,25,130.0);     // V25-V26
    incluiNovaAresta(G, ordemG,24,26,140.0);     // V25-V27
    incluiNovaAresta(G, ordemG,26,27,140.0);     // V27-V28

    incluiNovaAresta(G, ordemG,28,29,130.0);     // V29-V30
    incluiNovaAresta(G, ordemG,29,30,140.0);     // V30-V31
    incluiNovaAresta(G, ordemG,30,31,140.0);     // V31-V32

    incluiNovaAresta(G, ordemG,32,33,150.0);     // V33-V34
    incluiNovaAresta(G, ordemG,33,34,150.0);     // V34-V35

    // Arestas verticais (perpendiculares à Avenida Paulista)
    incluiNovaAresta(G, ordemG, 0, 6, 170.0);    // V1-V7
    incluiNovaAresta(G, ordemG, 6,12, 140.0);    // V7-V13
    incluiNovaAresta(G, ordemG,12,18, 130.0);    // V13-V19
    incluiNovaAresta(G, ordemG,18,23, 140.0);    // V19-V24

    incluiNovaAresta(G, ordemG, 1, 7, 170.0);    // V2-V8
    incluiNovaAresta(G, ordemG, 7,13, 140.0);    // V8-V14
    incluiNovaAresta(G, ordemG,13,19, 120.0);    // V14-V20
    incluiNovaAresta(G, ordemG,19,24, 130.0);    // V20-V25
    incluiNovaAresta(G, ordemG,24,28, 140.0);    // V25-V29

    incluiNovaAresta(G, ordemG, 2, 8, 170.0);    // V3-V9
    incluiNovaAresta(G, ordemG, 8,14, 140.0);    // V9-V15
    incluiNovaAresta(G, ordemG,14,20, 120.0);    // V15-V21
    incluiNovaAresta(G, ordemG,20,25, 130.0);    // V21-V26
    incluiNovaAresta(G, ordemG,25,29, 140.0);    // V26-V30
    incluiNovaAresta(G, ordemG,29,32,  92.0);    // V30-V33

    incluiNovaAresta(G, ordemG, 3, 9, 170.0);    // V4-V10
    incluiNovaAresta(G, ordemG, 9,15, 140.0);    // V10-V16
    incluiNovaAresta(G, ordemG,15,21, 120.0);    // V16-V22
    incluiNovaAresta(G, ordemG,21,26, 130.0);    // V22-V27
    incluiNovaAresta(G, ordemG,26,30, 140.0);    // V27-V31
    incluiNovaAresta(G, ordemG,30,33, 120.0);    // V31-V34

    incluiNovaAresta(G, ordemG, 4,10, 170.0);    // V5-V11
    incluiNovaAresta(G, ordemG,10,16, 140.0);    // V11-V17
    incluiNovaAresta(G, ordemG,16,22, 120.0);    // V17-V23
    incluiNovaAresta(G, ordemG,22,27, 130.0);    // V23-V28
    incluiNovaAresta(G, ordemG,27,31, 140.0);    // V28-V32
    incluiNovaAresta(G, ordemG,31,34, 140.0);    // V32-V35

    incluiNovaAresta(G, ordemG, 5,11, 170.0);    // V6-V12

    // Exibe os vértices disponíveis para escolha
    printf("Locais disponíveis:\n");
    for (int i = 0; i < ordemG; i++) {
        printf("%d: %s\n", i, G[i].nome);
    }
    printf("\n");

    int inicio, destino;
    // Solicita ao usuário o vértice de origem
    printf("Escolha o local de origem (0 a %d): ", ordemG - 1);
    scanf("%d", &inicio);
    // Solicita ao usuário o vértice de destino
    printf("Escolha o local de destino (0 a %d): ", ordemG - 1);
    scanf("%d", &destino);

    int exibirDetalhes;
    // Pergunta se o usuário deseja ver os detalhes da execução do A*
    printf("Deseja exibir os detalhes da árvore gerada pelo A*? (1 para sim, 0 para não): ");
    scanf("%d", &exibirDetalhes);

    // Executa o algoritmo A* com os parâmetros fornecidos pelo usuário
    printf("\nExecutando A* de %s para %s:\n", G[inicio].nome, G[destino].nome);
    aStar(G, ordemG, inicio, destino, exibirDetalhes);
    
    // Libera a memória alocada para o grafo
    destroiGrafo(&G, ordemG);
    return 0;
}
