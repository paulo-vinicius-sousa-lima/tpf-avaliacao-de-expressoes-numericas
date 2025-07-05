# 📐 Avaliador de Expressões Numéricas

Este trabalho prático tem por objetivo desenvolver um programa em linguagem C capaz de **avaliar expressões matemáticas escritas**, com suporte a operações básicas e funções matemáticas especiais. O sistema também deverá ser capaz de **converter e avaliar expressões entre as formas infixada e pós-fixada**, utilizando pilhas como estrutura fundamental.

---

## 🎯 Funcionalidades obrigatórias

O código-fonte deve ser capaz de:

✅ A) Traduzir a expressão da notação **infixada para pós-fixada**
✅ B) Traduzir a expressão da notação **pós-fixada para infixada**
✅ C) Suportar operações binárias com: `+`, `-`, `*`, `/`, `%`, `^`
✅ D) Suportar operações unárias com:

* `raiz` → raiz quadrada
* `sen` → seno
* `cos` → cosseno
* `tg` → tangente
* `log` → logaritmo de base 10

✅ E) As funções `sen`, `cos` e `tg` devem receber **arcos em graus**
✅ F) Operações do item D aplicam-se a apenas **um operando**
✅ G) Avaliar expressões para obter valores conforme a tabela de testes abaixo

---

## 🧪 Casos de Teste

| Teste | Notação Pós-fixa    | Notação Infixa          | Valor Aproximado |
| ----- | ------------------- | ----------------------- | ---------------- |
| 1     | `3 4 + 5 *`         | `(3 + 4) * 5`           | 35               |
| 2     | `7 2 * 4 +`         | `7 * 2 + 4`             | 18               |
| 3     | `8 5 2 4 + * +`     | `8 + (5 * (2 + 4))`     | 38               |
| 4     | `6 2 / 3 + 4 *`     | `(6 / 2 + 3) * 4`       | 24               |
| 5     | `9 5 2 8 * 4 + * +` | `9 + (5 * (2 + 8 * 4))` | -                |
| 6     | `2 3 + log 5 /`     | `log(2 + 3) / 5`        | ≈ 0.14           |
| 7     | `10 log 3 ^ 2 +`    | `(log10)^3 + 2`         | -                |
| 8     | `45 60 + 30 cos *`  | `(45 + 60) * cos(30)`   | ≈ 90.93          |
| 9     | `0.5 45 sen 2 ^ +`  | `sen(45)^2 + 0.5`       | -                |

> ℹ️ Outros testes poderão (e **deverão**) ser realizados.

---

## ✅ O que será avaliado

* ✅ Correção da solução proposta
* ✅ Código-fonte disponibilizado e **compartilhado no GitHub**
* ✅ Documentação conforme o template do enunciado
* ✅ Modularização do código-fonte
* ✅ Uso da **linguagem C padrão**
* ✅ Código organizado em três arquivos:

  * `expressao.c`
  * `expressao.h`
  * `main.c`
* ✅ Identificação de inconsistências nos dados de entrada
* ✅ Implementação das condições para execução correta das operações

---

## ⚠️ Atenção

O código-fonte deve estar estruturado em **três arquivos**, a serem compilados com:

```bash
gcc expressao.c main.c -o expressao.exe
```

---

## 🗂️ Arquivo expressao.h

O arquivo `expressao.h` **não deve ser modificado** e deve ter o seguinte conteúdo:

```c
#ifndef EXPRESSAO_H
#define EXPRESSAO_H

typedef struct {
    char posFixa[512];   // Expressão em notação pós-fixa (ex: "3 12 4 + *")
    char inFixa[512];    // Expressão em notação infixa (ex: "3 * (12 + 4)")
    float Valor;         // Valor numérico da expressão
} Expressao;

char *getFormaInFixa(char *Str);          // Retorna a forma infixa de Str (pós-fixa)
char *getFormaPosFixa(char *Str);         // Retorna a forma pós-fixa de Str (infixa)
float getValorPosFixa(char *StrPosFixa);  // Calcula o valor de Str na forma pós-fixa
float getValorInFixa(char *StrInFixa);    // Calcula o valor de Str na forma infixa

#endif
```

