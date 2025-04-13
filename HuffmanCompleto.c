#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArvoreBinaria.h"

#define TAM_CHAR 256

// --- Estrutura da Árvore de Huffman ---
typedef struct HuffmanNode {
    unsigned char caractere;
    int frequencia;
    struct HuffmanNode *esq, *dir;
} HuffmanNode;

// Cria novo nó
HuffmanNode* novoHuffmanNode(unsigned char caractere, int frequencia) {
    HuffmanNode* node = (HuffmanNode*)malloc(sizeof(HuffmanNode));
    node->caractere = caractere;
    node->frequencia = frequencia;
    node->esq = NULL;
    node->dir = NULL;
    return node;
}

// Compara nós (para qsort)
int compararHuffmanNodes(const void* a, const void* b) {
    HuffmanNode* n1 = *(HuffmanNode**)a;
    HuffmanNode* n2 = *(HuffmanNode**)b;
    return n1->frequencia - n2->frequencia;
}

// Constrói a árvore de Huffman com base nas frequências
HuffmanNode* construirArvoreHuffman(int freq[]) {
    int i, tamanho = 0;
    HuffmanNode* lista[TAM_CHAR];

    for (i = 0; i < TAM_CHAR; i++) {
        if (freq[i] > 0) {
            lista[tamanho++] = novoHuffmanNode((unsigned char)i, freq[i]);
        }
    }

    while (tamanho > 1) {
        qsort(lista, tamanho, sizeof(HuffmanNode*), compararHuffmanNodes);

        HuffmanNode *esq = lista[0];
        HuffmanNode *dir = lista[1];

        HuffmanNode *novo = novoHuffmanNode('*', esq->frequencia + dir->frequencia);
        novo->esq = esq;
        novo->dir = dir;

        lista[0] = novo;
        for (i = 1; i < tamanho - 1; i++)
            lista[i] = lista[i + 1];

        tamanho--;
    }

    return lista[0];
}

// Gera códigos binários recursivamente
void gerarCodigos(HuffmanNode* raiz, char* codigo, int profundidade, char* codigos[TAM_CHAR]) {
    if (!raiz)
        return;

    if (!raiz->esq && !raiz->dir) {
        codigo[profundidade] = '\0';
        codigos[raiz->caractere] = strdup(codigo);
        return;
    }

    codigo[profundidade] = '0';
    gerarCodigos(raiz->esq, codigo, profundidade + 1, codigos);

    codigo[profundidade] = '1';
    gerarCodigos(raiz->dir, codigo, profundidade + 1, codigos);
}

// --- Compactar arquivo de texto ---
int compactarArquivo(const char* caminhoEntrada, const char* caminhoSaida) {
    FILE *in = fopen(caminhoEntrada, "rb");
    if (!in) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 0;
    }

    int freq[TAM_CHAR] = {0};
    int c;

    while ((c = fgetc(in)) != EOF)
        freq[c]++;

    rewind(in);

    HuffmanNode* raiz = construirArvoreHuffman(freq);

    char* codigos[TAM_CHAR] = {0};
    char codigoTemp[256];
    gerarCodigos(raiz, codigoTemp, 0, codigos);

    FILE *out = fopen(caminhoSaida, "w");
    if (!out) {
        printf("Erro ao criar o arquivo de saida.\n");
        return 0;
    }

    while ((c = fgetc(in)) != EOF)
        fputs(codigos[c], out);  // Grava como texto "0101..."

    fclose(in);
    fclose(out);

    printf("Arquivo compactado com sucesso.\n");
    return 1;
}

// --- Descompactar (simplificada, será aprimorada) ---
int descompactarArquivo(const char* caminhoCompactado, const char* caminhoSaida) {
    printf("Descompactação ainda não implementada.\n");
    return 0;
}

// --- Comparar tamanhos dos arquivos ---
void compararTamanhosArquivo(const char* caminhoOriginal, const char* caminhoCompactado) {
    FILE *f1 = fopen(caminhoOriginal, "rb");
    FILE *f2 = fopen(caminhoCompactado, "rb");

    if (!f1 || !f2) {
        printf("Erro ao abrir os arquivos para comparacao.\n");
        return;
    }

    fseek(f1, 0, SEEK_END);
    fseek(f2, 0, SEEK_END);
    long tamanhoOriginal = ftell(f1);
    long tamanhoCompactado = ftell(f2);

    fclose(f1);
    fclose(f2);

    printf("Tamanho original: %ld bytes\n", tamanhoOriginal);
    printf("Tamanho compactado: %ld bytes\n", tamanhoCompactado);
    printf("Diferença: %ld bytes\n", tamanhoOriginal - tamanhoCompactado);
}
