#include <stdio.h>
#include <stdlib.h>
#include "ArvoreBinaria.h"
#include "HuffmanCompleto.c"

// Declarações das funções (caso estejam em outro arquivo .c ou você junte tudo)
int compactarArquivo(const char* caminhoEntrada, const char* caminhoSaida);
int descompactarArquivo(const char* caminhoCompactado, const char* caminhoSaida);
void compararTamanhosArquivo(const char* caminhoOriginal, const char* caminhoCompactado);

int main() {
    const char* arquivoOriginal = "Teste.txt";
    const char* arquivoCompactado = "compactado.huff";
    const char* arquivoDescompactado = "descompactado.txt";

    // Compacta o arquivo original
    compactarArquivo(arquivoOriginal, arquivoCompactado);

    // Compara os tamanhos
    compararTamanhosArquivo(arquivoOriginal, arquivoCompactado);

    // Tenta descompactar (ainda não implementado)
    descompactarArquivo(arquivoCompactado, arquivoDescompactado);

    return 0;
}
