#include <stdio.h>
#include <string.h>
#include "expressao.h"

#define MAX 512

int main()
{
    Expressao e;
    char entrada[MAX];
    char tipo[10];

    printf("Você quer digitar uma expressão [infixa] ou [posfixa]?\n> ");
    fgets(tipo, sizeof(tipo), stdin);
    tipo[strcspn(tipo, "\n")] = 0;

    if (strcmp(tipo, "infixa") == 0)
    {
        printf("Digite a expressão infixa (ex: 3 * (12 + 4)):\n> ");
        fgets(entrada, MAX, stdin);
        entrada[strcspn(entrada, "\n")] = 0;

        strcpy(e.inFixa, entrada);
        strcpy(e.posFixa, getFormaPosFixa(e.inFixa));
        e.Valor = getValorInFixa(e.inFixa);

        printf("\n📌 Resultado:\n");
        printf("Infixa: %s\n", e.inFixa);
        printf("Posfixa: %s\n", e.posFixa);
        printf("Valor: %.2f\n", e.Valor);
    }
    else if (strcmp(tipo, "posfixa") == 0)
    {
        printf("Digite a expressão posfixa (ex: 3 12 4 + *, sen, cos, tg, log, raiz):\n> ");
        fgets(entrada, MAX, stdin);
        entrada[strcspn(entrada, "\n")] = 0;

        strcpy(e.posFixa, entrada);

        char copiaPosFixa[MAX];
        strcpy(copiaPosFixa, e.posFixa); // para não alterar

        strcpy(e.inFixa, getFormaInFixa(copiaPosFixa));
        e.Valor = getValorPosFixa(e.posFixa);

        printf("\n📌 Resultado:\n");
        printf("Posfixa: %s\n", e.posFixa);
        printf("Infixa: %s\n", e.inFixa);
        printf("Valor: %.2f\n", e.Valor);
    }
    else
    {
        printf("Tipo inválido. Use 'infixa' ou 'posfixa'.\n");
        return 1;
    }

    system("pause");

    return 0;
}
