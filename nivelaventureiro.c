#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 100


typedef struct Sala {
    char nome[MAX_STR];
    char pista[MAX_STR];  
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;


typedef struct PistaNode {
    char pista[MAX_STR];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;


Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}


PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}


void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;

    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}


void explorarSalasComPistas(Sala *atual, PistaNode **pistas) {
    char opcao;

    while (atual != NULL) {

        printf("\nVocê está em: %s\n", atual->nome);

        
        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            *pistas = inserirPista(*pistas, atual->pista);
        }

        printf("\nEscolha seu próximo movimento:\n");
        if (atual->esquerda != NULL)
            printf("  (e) Ir para a esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf("  (d) Ir para a direita (%s)\n", atual->direita->nome);
        printf("  (s) Sair e encerrar exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda)
                atual = atual->esquerda;
            else
                printf("Não há sala à esquerda!\n");
        }
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita)
                atual = atual->direita;
            else
                printf("Não há sala à direita!\n");
        }
        else if (opcao == 's' || opcao == 'S') {
            printf("\nEncerrando exploração...\n");
            return;
        }
        else {
            printf("Opção inválida!\n");
        }
    }
}


int main() {

    
    Sala *hall         = criarSala("Hall de Entrada",     "Pegada de sapato");
    Sala *salaEstar    = criarSala("Sala de Estar",       "Copo quebrado");
    Sala *cozinha      = criarSala("Cozinha",             "Pano manchado de sangue");
    Sala *biblioteca   = criarSala("Biblioteca",          "Livro fora do lugar");
    Sala *jardim       = criarSala("Jardim",              "Pegada de bota na terra");
    Sala *quarto       = criarSala("Quarto",              "");
    Sala *porao        = criarSala("Porão",               "Cofre aberto");
    
    
    hall->esquerda = salaEstar;
    hall->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = quarto;

    quarto->direita = porao;

    cozinha->direita = jardim;


    PistaNode *arvorePistas = NULL;

    printf("\n=== Detective Quest – Exploração e Coleta de Pistas ===\n");
    explorarSalasComPistas(hall, &arvorePistas);

    printf("\n===== Pistas coletadas =====\n");
    if (arvorePistas == NULL)
        printf("Nenhuma pista foi coletada.\n");
    else
        exibirPistas(arvorePistas);

    return 0;
}
