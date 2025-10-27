/**
 * Desafio: Mochila de Loot (Nível 2)
 *
 * Objetivo: Comparar o desempenho de um inventário (mochila)
 * implementado com Vetor (Lista Sequencial) vs. Lista Encadeada.
 *
 * Destaques:
 * - Implementação de Inserir, Remover, Listar e Buscar em ambas as estruturas.
 * - Ordenação (Selection Sort) e Busca Binária (apenas no vetor).
 * - Contador de comparações para analisar a eficiência das buscas.
 */

// --- Bibliotecas Necessárias ---
#include <stdio.h>  // Para entrada e saída (printf, scanf)
#include <stdlib.h> // Para alocação dinâmica (malloc, free)
#include <string.h> // Para manipulação de strings (strcmp, strcpy)

// --- Constantes e Contadores Globais ---
#define CAPACIDADE_VETOR 10 // Limite da mochila de vetor
int comparacoesBusca = 0;   // Contador global para operações de busca

// --- Definição das Estruturas ---

/**
 * Struct Item: Representa o objeto coletado.
 * É usada em ambas as implementações.
 */
struct Item {
    char nome[30];
    char tipo[20];
    int quantidade;
};

/**
 * Struct No: Bloco de construção da Lista Encadeada.
 * Contém o 'Item' (dados) e um ponteiro para o próximo 'No'.
 */
struct No {
    struct Item dados;
    struct No* proximo;
};

// --- Globais da Mochila (VETOR) ---
struct Item mochilaVetor[CAPACIDADE_VETOR];
int totalItensVetor = 0;
int vetorOrdenado = 0; // Flag: 0 = Não, 1 = Sim

// --- Globais da Mochila (LISTA ENCADEADA) ---
// 'cabecaLista' é o ponteiro inicial. Se for NULL, a lista está vazia.
struct No* cabecaLista = NULL;

// --- Protótipos das Funções ---

// Funções Utilitárias
void limparBufferEntrada();
struct Item lerDadosItem(); // Função auxiliar para ler um item do usuário
void imprimirItem(struct Item item); // Função auxiliar para imprimir um item

// Funções de Menu
void menuPrincipal();
void menuVetor();
void menuLista();

// Funções da Mochila (Vetor)
void inserirItemVetor();
void removerItemVetor();
void listarItensVetor();
void ordenarVetor(); // Usaremos Selection Sort
void buscarSequencialVetor();
void buscarBinariaVetor();

// Funções da Mochila (Lista Encadeada)
void inserirItemLista();
void removerItemLista();
void listarItensLista();
void buscarSequencialLista();
void liberarLista(); // Limpar memória ao sair

/**
 * Função Principal (main)
 *
 * Ponto de entrada do programa. Inicia o menu principal.
 */
int main() {
    menuPrincipal();
    liberarLista(); // Garante que a memória da lista seja liberada ao sair
    return 0;
}

// --- Implementação das Funções de Menu e Utilitárias ---

/**
 * Controla o menu de alto nível, permitindo ao usuário
 * escolher qual implementação da mochila testar.
 */
void menuPrincipal() {
    int opcao;
    do {
        printf("\n--- COMPARADOR DE ESTRUTURAS (MOCHILA) ---\n");
        printf("Qual mochila voce quer usar?\n");
        printf("1. Mochila com Vetor (Lista Sequencial)\n");
        printf("2. Mochila com Lista Encadeada (Dinamica)\n");
        printf("0. Sair do Jogo\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                menuVetor();
                break;
            case 2:
                menuLista();
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
}

/**
 * Gerencia as operações específicas da mochila baseada em VETOR.
 */
void menuVetor() {
    int opcaoVetor;
    do {
        printf("\n--- Mochila (VETOR) [%d/%d] | Ordenado: %s ---\n",
               totalItensVetor, CAPACIDADE_VETOR, vetorOrdenado ? "Sim" : "Nao");
        printf("1. Inserir Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Ordenar Itens (por nome)\n");
        printf("5. Buscar (Sequencial)\n");
        printf("6. Buscar (Binaria)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha: ");
        scanf("%d", &opcaoVetor);
        limparBufferEntrada();

        switch (opcaoVetor) {
            case 1: inserirItemVetor(); break;
            case 2: removerItemVetor(); break;
            case 3: listarItensVetor(); break;
            case 4: ordenarVetor(); break;
            case 5: buscarSequencialVetor(); break;
            case 6: buscarBinariaVetor(); break;
            case 0: printf("Voltando...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcaoVetor != 0);
}

/**
 * Gerencia as operações específicas da mochila baseada em LISTA ENCADEADA.
 */
void menuLista() {
    int opcaoLista;
    do {
        printf("\n--- Mochila (LISTA ENCADEADA) ---\n");
        printf("1. Inserir Item (insere no inicio)\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar (Sequencial)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha: ");
        scanf("%d", &opcaoLista);
        limparBufferEntrada();

        switch (opcaoLista) {
            case 1: inserirItemLista(); break;
            case 2: removerItemLista(); break;
            case 3: listarItensLista(); break;
            case 4: buscarSequencialLista(); break;
            case 0: printf("Voltando...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcaoLista != 0);
}

/**
 * Limpa caracteres extras do buffer de entrada (stdin).
 * Essencial após leituras com scanf para evitar problemas
 * com leituras de string subsequentes.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Pede ao usuário os dados de um novo item e retorna a struct preenchida.
 * Reutilizada por ambas as implementações.
 */
struct Item lerDadosItem() {
    struct Item novoItem;
    printf("\nNome do item: ");
    scanf(" %[^\n]", novoItem.nome);
    printf("Tipo (arma, municao, cura): ");
    scanf(" %[^\n]", novoItem.tipo);
    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    limparBufferEntrada(); // Limpa o '\n' do scanf de inteiro
    return novoItem;
}

/**
 * Imprime os dados de um único item de forma formatada.
 */
void imprimirItem(struct Item item) {
    printf("| %-20s | %-15s | %d\n", item.nome, item.tipo, item.quantidade);
}

// --- Implementação das Funções da Mochila (VETOR) ---

/**
 * (VETOR) Insere um item na mochila.
 *
 * Custo: O(1) - Tempo constante, pois apenas adiciona ao final.
 *
 * NOTA: Ao inserir, a ordenação é quebrada.
 */
void inserirItemVetor() {
    if (totalItensVetor >= CAPACIDADE_VETOR) {
        printf("Mochila (VETOR) esta cheia! Nao foi possivel adicionar.\n");
        return;
    }

    mochilaVetor[totalItensVetor] = lerDadosItem();
    totalItensVetor++;
    vetorOrdenado = 0; // Inserção quebra a ordenação!
    printf("Item adicionado ao VETOR com sucesso.\n");
}

/**
 * (VETOR) Remove um item pelo nome.
 *
 * Custo: O(N) - O(N) para buscar + O(N) para deslocar os elementos.
 * Custo total é O(N).
 */
void removerItemVetor() {
    if (totalItensVetor == 0) {
        printf("Mochila (VETOR) esta vazia.\n");
        return;
    }

    char nomeRemover[30];
    printf("Nome do item a remover: ");
    scanf(" %[^\n]", nomeRemover);
    limparBufferEntrada();

    int indiceEncontrado = -1;
    // 1. Buscar o item
    for (int i = 0; i < totalItensVetor; i++) {
        if (strcmp(mochilaVetor[i].nome, nomeRemover) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("Item '%s' nao encontrado no VETOR.\n", nomeRemover);
    } else {
        // 2. Deslocar (shift) elementos para cobrir o "buraco"
        // Esta é a operação custosa de remoção em vetores.
        for (int i = indiceEncontrado; i < totalItensVetor - 1; i++) {
            mochilaVetor[i] = mochilaVetor[i + 1];
        }
        totalItensVetor--;
        printf("Item '%s' removido do VETOR.\n", nomeRemover);
        // Nota: A remoção de um vetor ordenado *não* quebra a ordenação.
        // Não é necessário alterar a flag 'vetorOrdenado'.
    }
}

/**
 * (VETOR) Lista todos os itens.
 *
 * Custo: O(N) - Precisa percorrer todos os N elementos.
 */
void listarItensVetor() {
    if (totalItensVetor == 0) {
        printf("\nMochila (VETOR) esta vazia.\n");
        return;
    }
    printf("\n--- Itens no VETOR (%d/%d) ---\n", totalItensVetor, CAPACIDADE_VETOR);
    printf("--------------------------------------------------\n");
    printf("| %-20s | %-15s | %s\n", "Nome", "Tipo", "Qtd.");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < totalItensVetor; i++) {
        imprimirItem(mochilaVetor[i]);
    }
    printf("--------------------------------------------------\n");
}

/**
 * (VETOR) Ordena o vetor de itens por nome usando Selection Sort.
 *
 * Custo: O(N^2) - Custo padrão de algoritmos simples de ordenação.
 * Necessário para a Busca Binária.
 */
void ordenarVetor() {
    if (totalItensVetor < 2) {
        printf("Nao ha itens suficientes para ordenar.\n");
        vetorOrdenado = 1;
        return;
    }

    // Algoritmo Selection Sort
    for (int i = 0; i < totalItensVetor - 1; i++) {
        int indiceMenor = i;
        // Encontra o menor elemento restante
        for (int j = i + 1; j < totalItensVetor; j++) {
            if (strcmp(mochilaVetor[j].nome, mochilaVetor[indiceMenor].nome) < 0) {
                indiceMenor = j;
            }
        }
        // Troca (swap)
        if (i != indiceMenor) {
            struct Item temp = mochilaVetor[i];
            mochilaVetor[i] = mochilaVetor[indiceMenor];
            mochilaVetor[indiceMenor] = temp;
        }
    }

    vetorOrdenado = 1; // Marca o vetor como ordenado
    printf("VETOR ordenado por nome com sucesso!\n");
    listarItensVetor(); // Mostra o resultado
}

/**
 * (VETOR) Busca Sequencial por nome.
 *
 * Custo: O(N) - Percorre o vetor elemento por elemento.
 */
void buscarSequencialVetor() {
    if (totalItensVetor == 0) {
        printf("Mochila (VETOR) esta vazia.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do item a buscar (Seq.): ");
    scanf(" %[^\n]", nomeBusca);
    limparBufferEntrada();

    comparacoesBusca = 0; // Reseta o contador
    int encontrado = 0;

    for (int i = 0; i < totalItensVetor; i++) {
        comparacoesBusca++; // Conta cada comparação de string
        if (strcmp(mochilaVetor[i].nome, nomeBusca) == 0) {
            printf("\n--- Item Encontrado (Sequencial) ---\n");
            imprimirItem(mochilaVetor[i]);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Item '%s' nao encontrado.\n", nomeBusca);
    }
    printf("Total de comparacoes (Sequencial Vetor): %d\n", comparacoesBusca);
}

/**
 * (VETOR) Busca Binária por nome.
 *
 * Custo: O(log N) - Muito mais rápido que O(N) para grandes volumes.
 * Exige que o vetor esteja ORDENADO.
 */
void buscarBinariaVetor() {
    if (vetorOrdenado == 0) {
        printf("ERRO: A busca binaria so funciona se o vetor estiver ordenado.\n");
        printf("Use a Opcao 4 (Ordenar Itens) primeiro.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do item a buscar (Binaria): ");
    scanf(" %[^\n]", nomeBusca);
    limparBufferEntrada();

    comparacoesBusca = 0; // Reseta o contador
    int encontrado = 0;

    int inicio = 0;
    int fim = totalItensVetor - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        comparacoesBusca++; // Conta apenas a comparação principal

        int cmp = strcmp(nomeBusca, mochilaVetor[meio].nome);

        if (cmp == 0) {
            // Encontrou
            printf("\n--- Item Encontrado (Binaria) ---\n");
            imprimirItem(mochilaVetor[meio]);
            encontrado = 1;
            break;
        } else if (cmp < 0) {
            // O item buscado vem antes do 'meio'
            fim = meio - 1;
        } else {
            // O item buscado vem depois do 'meio'
            inicio = meio + 1;
        }
    }

    if (!encontrado) {
        printf("Item '%s' nao encontrado.\n", nomeBusca);
    }
    printf("Total de comparacoes (Binaria Vetor): %d\n", comparacoesBusca);
}

// --- Implementação das Funções da Mochila (LISTA ENCADEADA) ---

/**
 * (LISTA) Insere um item na mochila (inserção no início).
 *
 * Custo: O(1) - Tempo constante. Apenas manipula ponteiros no início.
 * Muito eficiente para inserção.
 */
void inserirItemLista() {
    // 1. Aloca memória para o novo nó
    struct No* novoNo = (struct No*) malloc(sizeof(struct No));
    if (novoNo == NULL) {
        printf("Erro de alocacao de memoria! (LISTA)\n");
        return;
    }

    // 2. Preenche os dados do nó
    novoNo->dados = lerDadosItem();

    // 3. "Engata" o novo nó no início da lista
    novoNo->proximo = cabecaLista;
    cabecaLista = novoNo; // O novo nó é agora a cabeça

    printf("Item adicionado a LISTA com sucesso.\n");
}

/**
 * (LISTA) Remove um item pelo nome.
 *
 * Custo: O(N) - O(N) para buscar. A remoção em si (passo 2) é O(1).
 * Custo total é O(N).
 */
void removerItemLista() {
    if (cabecaLista == NULL) {
        printf("Mochila (LISTA) esta vazia.\n");
        return;
    }

    char nomeRemover[30];
    printf("Nome do item a remover: ");
    scanf(" %[^\n]", nomeRemover);
    limparBufferEntrada();

    struct No* atual = cabecaLista;
    struct No* anterior = NULL;

    // 1. Percorrer a lista para encontrar o item
    while (atual != NULL && strcmp(atual->dados.nome, nomeRemover) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    // 2. Se 'atual' é NULL, o item não foi encontrado
    if (atual == NULL) {
        printf("Item '%s' nao encontrado na LISTA.\n", nomeRemover);
        return;
    }

    // 3. Se 'atual' não é NULL, o item foi encontrado.
    // A "religação" dos ponteiros é uma operação O(1).
    if (anterior == NULL) {
        // O item a ser removido é a cabeça da lista
        cabecaLista = atual->proximo;
    } else {
        // O item está no meio ou fim da lista
        anterior->proximo = atual->proximo;
    }

    // 4. Libera a memória do nó removido
    free(atual);
    printf("Item '%s' removido da LISTA.\n", nomeRemover);
}

/**
 * (LISTA) Lista todos os itens.
 *
 * Custo: O(N) - Precisa percorrer todos os N elementos.
 */
void listarItensLista() {
    if (cabecaLista == NULL) {
        printf("\nMochila (LISTA) esta vazia.\n");
        return;
    }

    printf("\n--- Itens na LISTA ---\n");
    printf("--------------------------------------------------\n");
    printf("| %-20s | %-15s | %s\n", "Nome", "Tipo", "Qtd.");
    printf("--------------------------------------------------\n");
    
    struct No* temp = cabecaLista;
    while (temp != NULL) {
        imprimirItem(temp->dados);
        temp = temp->proximo; // Avança para o próximo nó
    }
    printf("--------------------------------------------------\n");
}

/**
 * (LISTA) Busca Sequencial por nome.
 *
 * Custo: O(N) - Percorre a lista elemento por elemento.
 * É a única forma de busca viável em uma lista encadeada simples.
 */
void buscarSequencialLista() {
    if (cabecaLista == NULL) {
        printf("Mochila (LISTA) esta vazia.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do item a buscar (Seq.): ");
    scanf(" %[^\n]", nomeBusca);
    limparBufferEntrada();

    comparacoesBusca = 0; // Reseta o contador
    int encontrado = 0;
    struct No* temp = cabecaLista;

    while (temp != NULL) {
        comparacoesBusca++; // Conta cada comparação de string
        if (strcmp(temp->dados.nome, nomeBusca) == 0) {
            printf("\n--- Item Encontrado (Sequencial) ---\n");
            imprimirItem(temp->dados);
            encontrado = 1;
            break;
        }
        temp = temp->proximo;
    }

    if (!encontrado) {
        printf("Item '%s' nao encontrado.\n", nomeBusca);
    }
    printf("Total de comparacoes (Sequencial Lista): %d\n", comparacoesBusca);
}

/**
 * (LISTA) Libera toda a memória alocada para a lista encadeada.
 * Essencial para evitar vazamentos de memória (memory leaks).
 */
void liberarLista() {
    struct No* atual = cabecaLista;
    struct No* proximo;
    
    while (atual != NULL) {
        proximo = atual->proximo; // Guarda a referência para o próximo
        free(atual);              // Libera o nó atual
        atual = proximo;          // Avança para o próximo
    }
    cabecaLista = NULL; // Marca a lista como vazia
}