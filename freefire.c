/**
 * Desafio: Torre de Resgate (Nível 3)
 *
 * Objetivo: Implementar e analisar o desempenho de diferentes algoritmos
 * de ordenação (Bubble, Insertion, Selection) aplicados a um
 * inventário de componentes de uma torre de fuga.
 *
 * Funcionalidades:
 * - Cadastro de componentes (nome, tipo, prioridade).
 * - Ordenação por 3 critérios usando 3 algoritmos diferentes.
 * - Medição de tempo (clock()) e número de comparações para cada sort.
 * - Busca Binária otimizada (apenas após ordenar por nome).
 * - Menu interativo para gerenciar a "montagem final".
 */

// --- Bibliotecas Necessárias ---
#include <stdio.h>    // Para entrada e saída (printf, fgets, scanf)
#include <stdlib.h>   // Para alocação dinâmica (embora não usada, é padrão)
#include <string.h>   // Para manipulação de strings (strcmp, strcpy, strcspn)
#include <time.h>     // Para medir o tempo de execução (clock)

// --- Constantes e Variáveis Globais ---
#define MAX_COMPONENTES 20 // Capacidade máxima de componentes

/**
 * Estrutura de dados (struct) para um Componente da torre.
 */
struct Componente {
    char nome[30];
    char tipo[20];
    int prioridade; // Prioridade de montagem (1-10)
};

// Array global (vetor) que armazena os componentes
struct Componente torre[MAX_COMPONENTES];
int totalComponentes = 0; // Controla o número atual de componentes

/**
 * Flag de estado da ordenação.
 * 0 = Desordenado (ou recém-cadastrado)
 * 1 = Ordenado por NOME
 * 2 = Ordenado por TIPO
 * 3 = Ordenado por PRIORIDADE
 * (Crucial para a regra da Busca Binária)
 */
int estadoOrdenacao = 0;

// Contadores globais para medição de desempenho
long long int comparacoesSort = 0;
long long int comparacoesBusca = 0;

// --- Protótipos das Funções ---

// Funções Utilitárias
void limparBufferEntrada();
void limparFgets(char *str); // Remove o '\n' do fgets
void mostrarComponentes();

// Funções do Menu Principal
void menu();
void cadastrarComponente();

// Funções de Ordenação (Internas)
// Estas são as implementações puras dos algoritmos
void algoritmoBubbleSortNome();
void algoritmoInsertionSortTipo();
void algoritmoSelectionSortPrioridade();

// Funções de Busca
void executarBuscaBinaria();
int buscaBinariaPorNome(char nomeBusca[]);

// Função de Análise de Desempenho
void medirTempoEExecutar(void (*algoritmo)(), const char* nomeAlgoritmo);


/**
 * Função Principal (main)
 *
 * Ponto de entrada do programa. Apenas inicia o menu principal.
 */
int main() {
    menu(); // Chama o loop principal do menu
    return 0;
}

// --- Implementação das Funções de Menu e Utilitárias ---

/**
 * Controla o menu interativo e o fluxo do programa.
 */
void menu() {
    int opcao;
    do {
        printf("\n--- TORRE DE RESGATE (Safe Zone) ---\n");
        printf("Componentes na base: %d/%d | Estado: ", totalComponentes, MAX_COMPONENTES);
        switch (estadoOrdenacao) {
            case 1: printf("Ordenado por Nome\n"); break;
            case 2: printf("Ordenado por Tipo\n"); break;
            case 3: printf("Ordenado por Prioridade\n"); break;
            default: printf("Desordenado\n");
        }
        printf("----------------------------------------\n");
        printf("1. Cadastrar Componente\n");
        printf("2. Estrategia: Ordenar por NOME (Bubble Sort)\n");
        printf("3. Estrategia: Ordenar por TIPO (Insertion Sort)\n");
        printf("4. Estrategia: Ordenar por PRIORIDADE (Selection Sort)\n");
        printf("5. Localizar Componente-Chave (Busca Binaria por Nome)\n");
        printf("6. Inspecionar Montagem (Listar Componentes)\n");
        printf("0. Sair (Desistir da Fuga)\n");
        printf("Escolha sua acao: ");
        
        scanf("%d", &opcao);
        limparBufferEntrada(); // Essencial após o scanf

        switch (opcao) {
            case 1:
                cadastrarComponente();
                break;
            case 2:
                // Mede o tempo e executa o Bubble Sort por Nome
                medirTempoEExecutar(algoritmoBubbleSortNome, "Bubble Sort (Nome)");
                break;
            case 3:
                // Mede o tempo e executa o Insertion Sort por Tipo
                medirTempoEExecutar(algoritmoInsertionSortTipo, "Insertion Sort (Tipo)");
                break;
            case 4:
                // Mede o tempo e executa o Selection Sort por Prioridade
                medirTempoEExecutar(algoritmoSelectionSortPrioridade, "Selection Sort (Prioridade)");
                break;
            case 5:
                executarBuscaBinaria();
                break;
            case 6:
                mostrarComponentes();
                break;
            case 0:
                printf("Fuga abortada... A zona o alcancou.\n");
                break;
            default:
                printf("Comando invalido! Tente novamente.\n");
        }
    } while (opcao != 0);
}

/**
 * Limpa o buffer de entrada (stdin) para evitar
 * leituras indesejadas, especialmente após scanf.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Remove o caractere de nova linha ('\n') que o fgets()
 * captura no final da string.
 */
void limparFgets(char *str) {
    str[strcspn(str, "\n")] = 0;
}

/**
 * Exibe todos os componentes cadastrados de forma formatada.
 * Esta é a "Inspeção da Montagem Final".
 */
void mostrarComponentes() {
    if (totalComponentes == 0) {
        printf("\nNenhum componente cadastrado na base.\n");
        return;
    }

    printf("\n--- ESTADO ATUAL DA MONTAGEM (%d itens) ---\n", totalComponentes);
    printf("--------------------------------------------------------------\n");
    printf("| %-25s | %-15s | %s\n", "Nome do Componente", "Tipo", "Prioridade");
    printf("--------------------------------------------------------------\n");
    for (int i = 0; i < totalComponentes; i++) {
        printf("| %-25s | %-15s | %d\n",
               torre[i].nome,
               torre[i].tipo,
               torre[i].prioridade);
    }
    printf("--------------------------------------------------------------\n");
}

/**
 * Adiciona um novo componente à torre.
 */
void cadastrarComponente() {
    if (totalComponentes >= MAX_COMPONENTES) {
        printf("\nBase de montagem cheia! Impossivel adicionar.\n");
        return;
    }

    printf("\n--- Cadastro de Novo Componente ---\n");
    struct Componente novo;

    printf("Nome (ex: Chip Central): ");
    fgets(novo.nome, 30, stdin);
    limparFgets(novo.nome); // Limpa o \n

    printf("Tipo (ex: Controle, Suporte, Propulsao): ");
    fgets(novo.tipo, 20, stdin);
    limparFgets(novo.tipo); // Limpa o \n

    printf("Prioridade (1-10): ");
    scanf("%d", &novo.prioridade);
    limparBufferEntrada(); // Limpa o \n do scanf

    torre[totalComponentes] = novo; // Adiciona ao vetor
    totalComponentes++;
    estadoOrdenacao = 0; // Novo item = desordem!

    printf("Componente '%s' adicionado com sucesso!\n", novo.nome);
}

// --- Implementação das Funções de Ordenação ---

/**
 * Função wrapper (invólucro) para medir o tempo e
 * as comparações de qualquer algoritmo de ordenação.
 * Usa um ponteiro de função para executar o algoritmo.
 */
void medirTempoEExecutar(void (*algoritmoSort)(), const char* nomeAlgoritmo) {
    if (totalComponentes < 2) {
        printf("\nComponentes insuficientes para ordenar.\n");
        return;
    }

    comparacoesSort = 0; // Reseta o contador global de comparações
    clock_t inicio, fim;

    printf("\nIniciando estrategia: %s...\n", nomeAlgoritmo);
    
    // Inicia o cronômetro
    inicio = clock();
    
    // Executa o algoritmo passado como parâmetro
    algoritmoSort();
    
    // Para o cronômetro
    fim = clock();
    
    // Calcula o tempo em segundos
    double tempoGasto = (double)(fim - inicio) / CLOCKS_PER_SEC;

    printf("\n--- Relatorio de Desempenho (%s) ---\n", nomeAlgoritmo);
    printf("Tempo de execucao: %f segundos\n", tempoGasto);
    printf("Numero de comparacoes: %lld\n", comparacoesSort);
    printf("-------------------------------------------------\n");

    // O próprio algoritmo já chama o mostrarComponentes()
}

/**
 * Algoritmo 1: Bubble Sort
 * Ordena o vetor 'torre' pelo campo 'nome' (alfabética).
 * Custo: O(N^2).
 */
void algoritmoBubbleSortNome() {
    struct Componente temp;
    for (int i = 0; i < totalComponentes - 1; i++) {
        for (int j = 0; j < totalComponentes - i - 1; j++) {
            
            // Incrementa o contador a cada comparação de string
            comparacoesSort++;
            if (strcmp(torre[j].nome, torre[j + 1].nome) > 0) {
                // Troca (swap)
                temp = torre[j];
                torre[j] = torre[j + 1];
                torre[j + 1] = temp;
            }
        }
    }
    estadoOrdenacao = 1; // Define o estado global
    mostrarComponentes(); // Mostra o resultado
}

/**
 * Algoritmo 2: Insertion Sort
 * Ordena o vetor 'torre' pelo campo 'tipo' (alfabética).
 * Custo: O(N^2) no pior caso, mas eficiente para dados quase ordenados.
 */
void algoritmoInsertionSortTipo() {
    struct Componente chave;
    int i, j;
    for (i = 1; i < totalComponentes; i++) {
        chave = torre[i];
        j = i - 1;

        // Move os elementos que são maiores que a 'chave'
        // Incrementa o contador a cada comparação de string no loop
        while (j >= 0 && (comparacoesSort++, strcmp(torre[j].tipo, chave.tipo) > 0)) {
            torre[j + 1] = torre[j];
            j = j - 1;
        }
        torre[j + 1] = chave;
    }
    estadoOrdenacao = 2; // Define o estado global
    mostrarComponentes(); // Mostra o resultado
}

/**
 * Algoritmo 3: Selection Sort
 * Ordena o vetor 'torre' pelo campo 'prioridade' (numérico).
 * Custo: O(N^2). Faz menos trocas que o Bubble Sort.
 */
void algoritmoSelectionSortPrioridade() {
    int i, j, indiceMenor;
    struct Componente temp;
    
    for (i = 0; i < totalComponentes - 1; i++) {
        indiceMenor = i;
        // Encontra o item de menor prioridade no restante do vetor
        for (j = i + 1; j < totalComponentes; j++) {
            
            // Incrementa o contador a cada comparação de inteiros
            comparacoesSort++;
            if (torre[j].prioridade < torre[indiceMenor].prioridade) {
                indiceMenor = j;
            }
        }
        // Troca (swap) o menor encontrado com o elemento atual
        if (indiceMenor != i) {
            temp = torre[i];
            torre[i] = torre[indiceMenor];
            torre[indiceMenor] = temp;
        }
    }
    estadoOrdenacao = 3; // Define o estado global
    mostrarComponentes(); // Mostra o resultado
}


// --- Implementação das Funções de Busca ---

/**
 * Função wrapper (invólucro) que gerencia a Busca Binária.
 * Pede o input ao usuário e verifica se a ordenação está correta.
 */
void executarBuscaBinaria() {
    // REQUISITO: A busca binária SÓ PODE ser feita se
    // o vetor estiver ordenado por NOME.
    if (estadoOrdenacao != 1) {
        printf("\nALERTA: Busca Binaria falhou!\n");
        printf("A estrategia 'Ordenar por NOME' (Opcao 2) deve ser executada primeiro.\n");
        return;
    }

    if (totalComponentes == 0) {
        printf("\nBase de montagem vazia. Nada para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do Componente-Chave para ativar a torre: ");
    fgets(nomeBusca, 30, stdin);
    limparFgets(nomeBusca);

    comparacoesBusca = 0; // Reseta o contador da busca
    int indiceEncontrado = buscaBinariaPorNome(nomeBusca);

    if (indiceEncontrado != -1) {
        printf("\n--- COMPONENTE-CHAVE LOCALIZADO! ---\n");
        printf("Nome: %s\n", torre[indiceEncontrado].nome);
        printf("Tipo: %s\n", torre[indiceEncontrado].tipo);
        printf("Prioridade: %d\n", torre[indiceEncontrado].prioridade);
        printf("\nTorre pronta para ativacao!\n");
    } else {
        printf("\nFALHA: Componente-Chave '%s' nao encontrado na montagem.\n", nomeBusca);
        printf("Verifique se cadastrou o item correto.\n");
    }
    
    printf("Comparacoes realizadas nesta busca: %lld\n", comparacoesBusca);
}

/**
 * Algoritmo 4: Busca Binária
 * Localiza um componente pelo 'nome' em um vetor ORDENADO.
 * Custo: O(log N).
 * Retorna o índice do item se encontrado, ou -1 se não.
 */
int buscaBinariaPorNome(char nomeBusca[]) {
    int inicio = 0;
    int fim = totalComponentes - 1;

    while (inicio <= fim) {
        // (inicio + fim) / 2 pode causar overflow se os números forem muito grandes
        // Esta é uma forma mais segura:
        int meio = inicio + (fim - inicio) / 2;

        // Incrementa o contador de comparações da busca
        comparacoesBusca++;
        int cmp = strcmp(nomeBusca, torre[meio].nome);

        if (cmp == 0) {
            // Encontrou!
            return meio;
        } else if (cmp > 0) {
            // O nome buscado está "à direita" (é maior)
            inicio = meio + 1;
        } else {
            // O nome buscado está "à esquerda" (é menor)
            fim = meio - 1;
        }
    }

    // Se o loop terminar, o item não foi encontrado
    return -1;
}