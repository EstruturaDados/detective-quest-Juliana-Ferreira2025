#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR 100
#define HASH_SIZE 101 




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


typedef struct HashNode {
    char chave[MAX_STR];      
    char suspeito[MAX_STR];   
    struct HashNode *proximo;
} HashNode;


typedef struct HashTable {
    HashNode *buckets[HASH_SIZE];
} HashTable;


unsigned long hash_str(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash % HASH_SIZE;
}


char *to_lower_copy(const char *s) {
    char *t = malloc(strlen(s) + 1);
    if (!t) { perror("malloc"); exit(1); }
    for (size_t i = 0; s[i]; ++i) t[i] = (char)tolower((unsigned char)s[i]);
    t[strlen(s)] = '\0';
    return t;
}


Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) { perror("malloc"); exit(1); }
    strncpy(nova->nome, nome, MAX_STR-1); nova->nome[MAX_STR-1] = '\0';
    strncpy(nova->pista, pista, MAX_STR-1); nova->pista[MAX_STR-1] = '\0';
    nova->esquerda = nova->direita = NULL;
    return nova;
}


PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *no = (PistaNode*) malloc(sizeof(PistaNode));
        if (!no) { perror("malloc"); exit(1); }
        strncpy(no->pista, pista, MAX_STR-1);
        no->pista[MAX_STR-1] = '\0';
        no->esquerda = no->direita = NULL;
        return no;
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (cmp > 0) raiz->direita = inserirPista(raiz->direita, pista);
    
    return raiz;
}


int existePista(PistaNode *raiz, const char *pista) {
    if (!raiz) return 0;
    int cmp = strcmp(pista, raiz->pista);
    if (cmp == 0) return 1;
    if (cmp < 0) return existePista(raiz->esquerda, pista);
    return existePista(raiz->direita, pista);
}


void exibirPistas(PistaNode *raiz) {
    if (!raiz) return;
    exibirPistas(raiz->esquerda);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}


void inserirNaHash(HashTable *ht, const char *pista, const char *suspeito) {
    unsigned long h = hash_str(pista);
    HashNode *cur = ht->buckets[h];
    while (cur) {
        if (strcmp(cur->chave, pista) == 0) {
           
            strncpy(cur->suspeito, suspeito, MAX_STR-1);
            cur->suspeito[MAX_STR-1] = '\0';
            return;
        }
        cur = cur->proximo;
    }
    
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    if (!novo) { perror("malloc"); exit(1); }
    strncpy(novo->chave, pista, MAX_STR-1); novo->chave[MAX_STR-1] = '\0';
    strncpy(novo->suspeito, suspeito, MAX_STR-1); novo->suspeito[MAX_STR-1] = '\0';
    novo->proximo = ht->buckets[h];
    ht->buckets[h] = novo;
}


const char* encontrarSuspeito(HashTable *ht, const char *pista) {
    unsigned long h = hash_str(pista);
    HashNode *cur = ht->buckets[h];
    while (cur) {
        if (strcmp(cur->chave, pista) == 0) return cur->suspeito;
        cur = cur->proximo;
    }
    return NULL;
}



void explorarSalas(Sala *atual, PistaNode **arvorePistas, HashTable *ht) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê entrou em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: \"%s\"\n", atual->pista);
            if (!existePista(*arvorePistas, atual->pista)) {
                *arvorePistas = inserirPista(*arvorePistas, atual->pista);
                
                const char *s = encontrarSuspeito(ht, atual->pista);
                if (s) printf(" -> Esta pista está associada ao suspeito: %s\n", s);
                else printf(" -> Nenhum suspeito associado a essa pista.\n");
            } else {
                printf("Você já coletou essa pista anteriormente.\n");
            }
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        
        printf("\nEscolha seu próximo movimento:\n");
        if (atual->esquerda) printf("  (e) Esquerda -> %s\n", atual->esquerda->nome);
        if (atual->direita)  printf("  (d) Direita  -> %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e' || opcao == 'E') {
            if (atual->esquerda) atual = atual->esquerda;
            else printf("Não há sala à esquerda!\n");
        } else if (opcao == 'd' || opcao == 'D') {
            if (atual->direita) atual = atual->direita;
            else printf("Não há sala à direita!\n");
        } else if (opcao == 's' || opcao == 'S') {
            printf("\nVocê escolheu encerrar a exploração.\n");
            return;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}


void contarPorSuspeito(PistaNode *raiz, HashTable *ht, const char *acusado, int *contador) {
    if (!raiz) return;
    contarPorSuspeito(raiz->esquerda, ht, acusado, contador);

    const char *s = encontrarSuspeito(ht, raiz->pista);
    if (s) {
        
        char *sl = to_lower_copy(s);
        char *al = to_lower_copy(acusado);
        if (strcmp(sl, al) == 0) (*contador)++;
        free(sl); free(al);
    }

    contarPorSuspeito(raiz->direita, ht, acusado, contador);
}

void verificarSuspeitoFinal(PistaNode *arvorePistas, HashTable *ht, const char *acusado) {
    int count = 0;
    contarPorSuspeito(arvorePistas, ht, acusado, &count);

    printf("\n===== JULGAMENTO FINAL =====\n");
    printf("Suspeito acusado: %s\n", acusado);
    printf("Número de pistas que apontam para %s: %d\n", acusado, count);

    if (count >= 2) {
        printf("Resultado: Há evidências suficientes. %s é considerado o culpado!\n", acusado);
    } else {
        printf("Resultado: Evidências insuficientes para condenação de %s.\n", acusado);
    }
}


void liberarPistas(PistaNode *raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

void liberarHash(HashTable *ht) {
    for (int i = 0; i < HASH_SIZE; ++i) {
        HashNode *cur = ht->buckets[i];
        while (cur) {
            HashNode *tmp = cur;
            cur = cur->proximo;
            free(tmp);
        }
    }
}

int main() {
    
    Sala *hall       = criarSala("Hall de Entrada", "Pegada de sapato");
    Sala *estar      = criarSala("Sala de Estar",   "Copo com resquício de veneno");
    Sala *cozinha    = criarSala("Cozinha",         "Pano manchado de sangue");
    Sala *biblioteca = criarSala("Biblioteca",      "Página arrancada do diário");
    Sala *jardim     = criarSala("Jardim",          "Pegada de bota na terra");
    Sala *quarto     = criarSala("Quarto",          "Lenço com perfume floral");
    Sala *escritorio = criarSala("Escritório",      "Carta ameaçadora");

    
    hall->esquerda = estar;
    hall->direita = cozinha;
    estar->esquerda = biblioteca;
    estar->direita = quarto;
    biblioteca->direita = escritorio;
    cozinha->direita = jardim;

    
    HashTable ht;
    for (int i = 0; i < HASH_SIZE; ++i) ht.buckets[i] = NULL;

    
    inserirNaHash(&ht, "Pegada de sapato", "Sr. Almeida");
    inserirNaHash(&ht, "Copo com resquício de veneno", "Sra. Beatriz");
    inserirNaHash(&ht, "Pano manchado de sangue", "Sr. Carlos");
    inserirNaHash(&ht, "Página arrancada do diário", "Sra. Beatriz");
    inserirNaHash(&ht, "Pegada de bota na terra", "Sr. Almeida");
    inserirNaHash(&ht, "Lenço com perfume floral", "Sra. Diana");
    inserirNaHash(&ht, "Carta ameaçadora", "Sr. Carlos");

    
    PistaNode *arvorePistas = NULL;

    printf("=== Detective Quest: Investigue a mansão ===\n");
    explorarSalas(hall, &arvorePistas, &ht);

    
    printf("\n===== Pistas coletadas =====\n");
    if (!arvorePistas) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(arvorePistas);
    }

    
    char acusado[MAX_STR];
    printf("\nQuem você acusa? (Digite o nome completo do suspeito):\n> ");
    
    getchar(); 
    if (!fgets(acusado, MAX_STR, stdin)) acusado[0] = '\0';
    
    size_t ln = strlen(acusado);
    if (ln > 0 && acusado[ln-1] == '\n') acusado[ln-1] = '\0';
    if (strlen(acusado) == 0) {
        printf("Nenhum suspeito informado. Encerrando.\n");
    } else {
        verificarSuspeitoFinal(arvorePistas, &ht, acusado);
    }

    d
    liberarPistas(arvorePistas);
    liberarHash(&ht);
    free(hall); free(estar); free(cozinha); free(biblioteca);
    free(jardim); free(quarto); free(escritorio);

    return 0;
}