/**
 * Desafio: Mochila de Loot (Nível 1)
 *
 * Este programa simula um sistema de inventário (mochila) para um jogo
 * de sobrevivência, utilizando structs e um vetor (lista sequencial).
 *
 * Funcionalidades:
 * - Cadastrar, remover, listar e buscar itens.
 * - Capacidade máxima de 10 itens.
 *
 * Requisitos:
 * - Struct Item: {nome, tipo, quantidade}
 * - Funções: inserirItem(), removerItem(), listarItens(), buscarItem()
 */

// Bibliotecas necessárias
#include <stdio.h>  // Para operações de entrada e saída (printf, scanf)
#include <string.h> // Para manipulação de strings (strcmp, strcpy)
#include <stdlib.h> // Para funções utilitárias (embora não estritamente usada aqui, é boa prática)

// Constante para definir o tamanho máximo da mochila
#define CAPACIDADE_MAXIMA 10

/**
 * Definição da estrutura de dados (struct) para um Item.
 * Esta struct agrupa as três informações essenciais de cada
 * objeto que o jogador pode coletar.
 */
struct Item {
    char nome[30];      // Nome do item (ex: "Kit Medico")
    char tipo[20];      // Categoria (ex: "Cura", "Arma", "Municao")
    int quantidade;     // Quantidade do item
};

// --- Variáveis Globais ---
// O vetor 'mochila' é a nossa lista sequencial que armazena os itens.
struct Item mochila[CAPACIDADE_MAXIMA];
// 'totalItens' controla quantos itens *atualmente* estão na mochila.
int totalItens = 0;

// --- Protótipos das Funções ---
// Declarar as funções antes do main() é uma boa prática em C.
void inserirItem();
void removerItem();
void listarItens();
void buscarItem();
void limparBufferEntrada(); // Função utilitária

/**
 * Função Principal (main)
 *
 * Controla o fluxo de execução do programa, exibindo o menu
 * principal e chamando as funções apropriadas com base
 * na escolha do usuário.
 */
int main() {
    int opcao;

    // Loop principal do menu. Continua executando até o usuário escolher 0.
    do {
        // Exibe o menu de opções
        printf("\n--- MOCHILA DE LOOT (%d/%d) ---\n", totalItens, CAPACIDADE_MAXIMA);
        printf("1. Cadastrar Item\n");
        printf("2. Remover Item\n");
        printf("3. Listar Itens\n");
        printf("4. Buscar Item\n");
        printf("0. Sair\n");
        printf("Escolha sua acao: ");
        
        // Lê a opção do usuário
        scanf("%d", &opcao);
        
        // Limpa o buffer de entrada para evitar problemas com leituras
        // de string (fgets/scanf %s) nas próximas funções.
        limparBufferEntrada();

        // Estrutura switch para decidir qual função chamar
        switch (opcao) {
            case 1:
                inserirItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                buscarItem();
                break;
            case 0:
                printf("Fechando mochila... Ate logo!\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }

        // Requisito: Listar os itens após cada operação de cadastro ou remoção
        if (opcao == 1 || opcao == 2) {
            printf("\n--- Situacao Atual da Mochila ---\n");
            listarItens();
        }

    } while (opcao != 0); // Condição de saída do loop

    return 0; // Indica que o programa terminou com sucesso
}

/**
 * Função: limparBufferEntrada
 *
 * Limpa caracteres extras do buffer de entrada (stdin),
 * principalmente o caractere '\n' (Enter) deixado pelo scanf,
 * que pode interferir na leitura de strings.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Função: inserirItem
 *
 * Permite ao jogador cadastrar um novo item na mochila.
 * Verifica se a mochila já está cheia. Se houver espaço,
 * solicita nome, tipo e quantidade e adiciona o item
 * na próxima posição livre do vetor.
 */
void inserirItem() {
    // 1. Verifica se a mochila está cheia
    if (totalItens >= CAPACIDADE_MAXIMA) {
        printf("\nMochila cheia! Impossivel adicionar '%s'.\n", "novo item");
        return; // Retorna ao menu principal
    }

    // 2. Se há espaço, solicita os dados do novo item
    printf("\n--- Cadastrar Novo Item ---\n");
    
    printf("Nome do item: ");
    // Usa " %[^\n]" para ler strings com espaços.
    // O espaço inicial ignora quaisquer quebras de linha pendentes.
    scanf(" %[^\n]", mochila[totalItens].nome);

    printf("Tipo (arma, municao, cura, ferramenta): ");
    scanf(" %[^\n]", mochila[totalItens].tipo);

    printf("Quantidade: ");
    scanf("%d", &mochila[totalItens].quantidade);
    limparBufferEntrada(); // Limpa o \n deixado pelo scanf de inteiro

    // 3. Incrementa o contador total de itens
    totalItens++;

    printf("\nItem '%s' cadastrado com sucesso!\n", mochila[totalItens - 1].nome);
}

/**
 * Função: removerItem
 *
 * Permite ao jogador remover um item informando seu nome.
 * A função primeiro busca o item. Se o encontra, "remove"
 * o item deslocando todos os itens subsequentes uma posição
 * para a esquerda no vetor.
 */
void removerItem() {
    // 1. Verifica se a mochila está vazia
    if (totalItens == 0) {
        printf("\nA mochila ja esta vazia. Nao ha o que remover.\n");
        return;
    }

    char nomeRemover[30];
    printf("\nDigite o nome do item que deseja remover: ");
    scanf(" %[^\n]", nomeRemover);
    limparBufferEntrada(); // Limpa o \n

    int indiceEncontrado = -1; // -1 significa não encontrado

    // 2. Busca sequencial para encontrar o índice do item
    for (int i = 0; i < totalItens; i++) {
        // strcmp retorna 0 se as strings forem idênticas
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            indiceEncontrado = i;
            break; // Para o loop assim que encontrar o item
        }
    }

    // 3. Processa a remoção
    if (indiceEncontrado == -1) {
        // Se o índice não mudou, o item não foi encontrado
        printf("\nItem '%s' nao encontrado na mochila.\n", nomeRemover);
    } else {
        // 4. Deslocamento (shift) dos elementos
        // Pega todos os itens *após* o item removido e os
        // move uma posição para trás.
        for (int i = indiceEncontrado; i < totalItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }

        // 5. Decrementa o número total de itens
        totalItens--;

        printf("\nItem '%s' removido com sucesso!\n", nomeRemover);
    }
}

/**
 * Função: listarItens
 *
 * Exibe todos os itens atualmente registrados na mochila,
 * formatados em uma tabela para melhor legibilidade.
 * Se a mochila estiver vazia, exibe uma mensagem.
 */
void listarItens() {
    // 1. Verifica se a mochila está vazia
    if (totalItens == 0) {
        printf("\nA mochila esta vazia.\n");
        return;
    }

    // 2. Cabeçalho da tabela
    printf("\n--- Itens na Mochila (%d/%d) ---\n", totalItens, CAPACIDADE_MAXIMA);
    printf("--------------------------------------------------\n");
    // Formata a saída: %-Xs alinha o texto à esquerda em X colunas
    printf("| %-20s | %-15s | %s\n", "Nome", "Tipo", "Qtd.");
    printf("--------------------------------------------------\n");

    // 3. Loop para percorrer o vetor e imprimir cada item
    for (int i = 0; i < totalItens; i++) {
        printf("| %-20s | %-15s | %d\n",
               mochila[i].nome,
               mochila[i].tipo,
               mochila[i].quantidade);
    }
    printf("--------------------------------------------------\n");
}

/**
 * Função: buscarItem
 *
 * Implementa uma busca sequencial. O jogador digita um nome,
 * e a função percorre o vetor 'mochila' do início ao fim,
 * comparando o nome de cada item com o nome buscado.
 * Se encontrar, exibe os detalhes do item.
 */
void buscarItem() {
    // 1. Verifica se a mochila está vazia
    if (totalItens == 0) {
        printf("\nA mochila esta vazia. Nao ha o que buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\nDigite o nome do item que deseja buscar: ");
    scanf(" %[^\n]", nomeBusca);
    limparBufferEntrada(); // Limpa o \n

    int encontrado = 0; // Flag para indicar se o item foi achado

    // 2. Loop de busca sequencial
    for (int i = 0; i < totalItens; i++) {
        // Compara o nome buscado com o nome do item atual no loop
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            // Se encontrou (strcmp == 0)
            printf("\n--- Item Encontrado! ---\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            encontrado = 1; // Marca como encontrado
            break; // Interrompe o loop, pois a busca terminou
        }
    }

    // 3. Se o loop terminar e a flag 'encontrado' for 0
    if (encontrado == 0) {
        printf("\nItem '%s' nao foi encontrado na mochila.\n", nomeBusca);
    }
}