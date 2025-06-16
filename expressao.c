#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "expressao.h"

#define MAX 512

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    int topo;
    float dados[MAX];
} PilhaNumeros;

typedef struct {
    int topo;
    char dados[MAX][MAX];
} PilhaStrings;

// Funções de pilha para números
void pushNum(PilhaNumeros *p, float val) { 
    p->dados[++p->topo] = val; 
}

float popNum(PilhaNumeros *p) {
    if (p->topo < 0) {
        printf("Erro: pilha de números vazia!\n");
        exit(1);
    }
    return p->dados[p->topo--];
}

// Funções de pilha para strings
void criarPilhaString(PilhaStrings *p) {
    p->topo = -1;
}

void pushStr(PilhaStrings *p, const char *str) {
    if (p->topo >= MAX - 1) {
        printf("Erro: pilha de strings cheia!\n");
        exit(1);
    }
    strcpy(p->dados[++p->topo], str);
}

char *popStr(PilhaStrings *p) {
    if (p->topo < 0) {
        printf("Erro: pilha de strings vazia!\n");
        exit(1);
    }
    return p->dados[p->topo--];
}

int prioridade(char *op) {
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 1;
    else if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0)
        return 2;
    else if (strcmp(op, "^") == 0)
        return 4;
    else if (strcmp(op, "sen") == 0 || strcmp(op, "cos") == 0 || strcmp(op, "tg") == 0 || strcmp(op, "log") == 0 || strcmp(op, "raiz") == 0)
        return 5; // maior prioridade para funções
    return 0;
}

int eh_operador(char *token) {
    return (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 || strcmp(token, "*") == 0 || strcmp(token, "/") == 0 || strcmp(token, "%") == 0 || strcmp(token, "^") == 0);
}

int eh_funcao(char *token) {
    return (strcmp(token, "sen") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 || strcmp(token, "log") == 0 || strcmp(token, "raiz") == 0);
}

int associatividadeDireita(char *op) {
    return (strcmp(op, "^") == 0 || eh_funcao(op));
}

float getValorPosFixa(char *StrPosFixa) {
    PilhaNumeros p;
    p.topo = -1;
    char copia[MAX];
    strcpy(copia, StrPosFixa);
    char *token = strtok(copia, " ");

    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            pushNum(&p, atof(token));
        } else if (eh_operador(token)) {
            if (p.topo < 1) {
                printf("Erro: operandos insuficientes para operador %s\n", token);
                exit(1);
            }
            float b = popNum(&p);
            float a = popNum(&p);
            if (strcmp(token, "+") == 0)
                pushNum(&p, a + b);
            else if (strcmp(token, "-") == 0)
                pushNum(&p, a - b);
            else if (strcmp(token, "*") == 0)
                pushNum(&p, a * b);
            else if (strcmp(token, "/") == 0) {
                if (b == 0) {
                    printf("Erro: divisão por zero\n");
                    exit(1);
                }
                pushNum(&p, a / b);
            } else if (strcmp(token, "%") == 0) {
                int ia = (int)a;
                int ib = (int)b;
                if ((float)ia != a || (float)ib != b) {
                    printf("Erro: operador %% requer operandos inteiros\n");
                    exit(1);
                }
                if (ib == 0) {
                    printf("Erro: divisão por zero no operador %%\n");
                    exit(1);
                }
                pushNum(&p, ia % ib);
            } else if (strcmp(token, "^") == 0)
                pushNum(&p, powf(a, b));
        } else if (eh_funcao(token)) {
            if (p.topo < 0) {
                printf("Erro: operando insuficiente para função %s\n", token);
                exit(1);
            }
            float a = popNum(&p);
            if (strcmp(token, "sen") == 0)
                pushNum(&p, sinf(a * (float)M_PI / 180.0f)); // converte graus para radianos
            else if (strcmp(token, "cos") == 0)
                pushNum(&p, cosf(a * (float)M_PI / 180.0f));
            else if (strcmp(token, "tg") == 0)
                pushNum(&p, tanf(a * (float)M_PI / 180.0f));
            else if (strcmp(token, "log") == 0) {
                if (a <= 0) {
                    printf("Erro: logaritmo de valor não positivo\n");
                    exit(1);
                }
                pushNum(&p, log10f(a));
            } else if (strcmp(token, "raiz") == 0) {
                if (a < 0) {
                    printf("Erro: raiz quadrada de valor negativo\n");
                    exit(1);
                }
                pushNum(&p, sqrtf(a));
            }
        } else {
            printf("Erro: token inválido '%s'\n", token);
            exit(1);
        }
        token = strtok(NULL, " ");
    }

    if (p.topo != 0) {
        printf("Erro: expressão posfixa inválida\n");
        exit(1);
    }

    return popNum(&p);
}

char *getFormaPosFixa(char *StrInFixa) {
    static char saida[MAX];
    char *pilha[MAX];   // pilha com strings pois operadores podem ser funções
    int topo = -1;
    int j = 0;

    int len = (int)strlen(StrInFixa);
    char token[MAX];
    int pos = 0;

    saida[0] = '\0';

    // Tokenizar manualmente (separar por espaços e gerenciar funções e números)
    while (pos < len) {
        int k = 0;
        // Pula espaços
        while (pos < len && isspace(StrInFixa[pos])) pos++;

        if (pos >= len) break;

        // Se letra: ler palavra (função)
        if (isalpha(StrInFixa[pos])) {
            while (pos < len && isalpha(StrInFixa[pos]) && k < MAX - 1)
                token[k++] = StrInFixa[pos++];
            token[k] = '\0';

            // Empilha a função
            pilha[++topo] = strdup(token);
        } else if (isdigit(StrInFixa[pos]) || StrInFixa[pos] == '.' || (StrInFixa[pos] == '-' && (pos == 0 || isspace(StrInFixa[pos-1])))) {
            // Número sério (podendo ter ponto e sinal)
            if (StrInFixa[pos] == '-') {
                token[k++] = StrInFixa[pos++];
            }
            while (pos < len && (isdigit(StrInFixa[pos]) || StrInFixa[pos] == '.') && k < MAX - 1) {
                token[k++] = StrInFixa[pos++];
            }
            token[k] = '\0';

            // Adiciona número na saída direto
            strcat(saida, token);
            strcat(saida, " ");
        } else if (StrInFixa[pos] == '(') {
            // Empilha "("
            pilha[++topo] = strdup("(");
            pos++;
        } else if (StrInFixa[pos] == ')') {
            // Desempilha até "("
            while (topo >= 0 && strcmp(pilha[topo], "(") != 0) {
                strcat(saida, pilha[topo]);
                strcat(saida, " ");
                free(pilha[topo]);
                topo--;
            }
            if (topo < 0) {
                printf("Erro: parênteses desbalanceados\n");
                exit(1);
            }
            // Remove "("
            free(pilha[topo]);
            topo--;

            // Se antes de "(" tinha uma função, desempilha e coloca na saída
            if (topo >= 0 && eh_funcao(pilha[topo])) {
                strcat(saida, pilha[topo]);
                strcat(saida, " ");
                free(pilha[topo]);
                topo--;
            }
            pos++;
        } else {
            // É operador único como + - * / % ^
            char opstr[2] = {StrInFixa[pos], '\0'};

            while (topo >= 0 && eh_operador(pilha[topo]) && 
                (prioridade((char*)pilha[topo]) > prioridade(opstr) || 
                (prioridade((char*)pilha[topo]) == prioridade(opstr) && !associatividadeDireita(opstr)))) {
                strcat(saida, pilha[topo]);
                strcat(saida, " ");
                free(pilha[topo]);
                topo--;
            }

            pilha[++topo] = strdup(opstr);
            pos++;
        }
    }

    // Desempilha tudo que sobrou
    while (topo >= 0) {
        if (strcmp(pilha[topo], "(") == 0) {
            printf("Erro: parênteses desbalanceados\n");
            exit(1);
        }
        strcat(saida, pilha[topo]);
        strcat(saida, " ");
        free(pilha[topo]);
        topo--;
    }

    return saida;
}

float getValorInFixa(char *StrInFixa) {
    char pos[MAX];
    strcpy(pos, getFormaPosFixa(StrInFixa));
    return getValorPosFixa(pos);
}

char *getFormaInFixa(char *Str) {
    static char resultado[MAX];
    char copia[MAX];
    strcpy(copia, Str);
    resultado[0] = '\0';

    PilhaStrings p;
    criarPilhaString(&p);

    char *token = strtok(copia, " ");
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1])) || (token[0] == '.' && isdigit(token[1]))) {
            pushStr(&p, token);
        } else if (eh_operador(token) || eh_funcao(token)) {
            // Se função: desempilha um operando, aplica função
            if (eh_funcao(token)) {
                char a[MAX], expr[MAX];
                char *a_str = popStr(&p);
                strcpy(a, a_str);

                sprintf(expr, "%s(%s)", token, a);
                pushStr(&p, expr);
            } else {
                // operador binário
                if (p.topo < 1) {
                    printf("Erro: operandos insuficientes para operador %s\n", token);
                    exit(1);
                }
                char b[MAX], a[MAX], expr[MAX];
                char *b_str = popStr(&p);
                char *a_str = popStr(&p);
                strcpy(b, b_str);
                strcpy(a, a_str);

                sprintf(expr, "(%s %s %s)", a, token, b);
                pushStr(&p, expr);
            }
        } else {
            printf("Erro: token inválido na expressão posfixa: '%s'\n", token);
            exit(1);
        }
        token = strtok(NULL, " ");
    }

    if (p.topo != 0) {
        printf("Erro: expressão posfixa inválida para conversão em infixa\n");
        exit(1);
    }
    strcpy(resultado, popStr(&p));
    return resultado;
}
