#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;


Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;

    return nova;
}


void explorarSalas(Sala *atual) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais caminhos. Fim da exploração!\n");
            return;
        }

        printf("Escolha um caminho:\n");
        if (atual->esquerda != NULL) printf("  (e) Ir para a esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita != NULL) printf("  (d) Ir para a direita (%s)\n", atual->direita->nome);
        printf("  (s) Sair da mansão\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda != NULL)
                atual = atual->esquerda;
            else
                printf("Não há sala à esquerda!\n");
        }
        else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita != NULL)
                atual = atual->direita;
            else
                printf("Não há sala à direita!\n");
        }
        else if (opcao == 's' || opcao == 'S') {
            printf("Saindo da mansão...\n");
            return;
        }
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}


int main() {

    
    Sala *hall       = criarSala("Hall de Entrada");
    Sala *salaEstar  = criarSala("Sala de Estar");
    Sala *cozinha    = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim     = criarSala("Jardim");

   

    hall->esquerda = salaEstar;
    hall->direita  = cozinha;

    salaEstar->esquerda = biblioteca;
    cozinha->direita = jardim;

    
    printf("\n=== Detective Quest — Exploração da Mansão ===\n");
    explorarSalas(hall);

    
    free(biblioteca);
    free(salaEstar);
    free(jardim);
    free(cozinha);
    free(hall);

    return 0;
}
