#include <stdio.h>
#include <string.h>

#include "huffman.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf(
        "Uso: huffman [OPÇÃO] [ARQUIVO]\n"
        "Usa o algoritmo de huffman em um arquivo determinado.\n"
        "\n"
        "Opções:\n"
        "  ./huffman -c    Comprime um Arquivo\n"
        "  ./huffman -d    Descomprime ARQUIVO caso seja válido\n\n");
    return 1;
  }

  char *flag = argv[1];
  char *arquivo = argv[2];

  if (strcmp(flag, "-c") == 0) {
    printf("Comprimindo arquivo: %s\n", arquivo);

    // Criar nome de saída com ".huff"
    char nome_saida[512];
    int written = snprintf(nome_saida, sizeof(nome_saida), "%s.huff", arquivo);
    if (written < 0 || written >= (int)sizeof(nome_saida)) {
      printf("Erro: nome de arquivo de saída muito longo.\n");
      return 1;
    }

    // Perguntar ao usuário se deseja comprimir com confirmação
    if (comprimir_com_confirmação(arquivo) != 0) {
      printf("Erro ao comprimir o arquivo.\n");
      return 1;
    }

    printf("Arquivo comprimido com sucesso: %s\n", arquivo);
  } else if (strcmp(flag, "-d") == 0) {
    printf("Descomprimindo arquivo: %s\n", arquivo);
    // Descomprimir arquivo
    FILE *entrada = fopen(arquivo, "rb");
    if (!entrada) {
      printf("Erro ao abrir o arquivo de entrada.\n");
      return 1;
    }

    // Gerar nome de saída
    char nome_saida[256];
    snprintf(nome_saida, sizeof(nome_saida), "%s.dec", arquivo);

    FILE *saida = fopen(nome_saida, "wb");
    if (!saida) {
      printf("Erro ao criar o arquivo de saída.\n");
      fclose(entrada);
      return 1;
    }

    if (descompactar_arquivo(entrada, saida)) {
      printf("Erro ao descomprimir o arquivo.\n");
      fclose(entrada);
      fclose(saida);
      return 1;
    }

    fclose(entrada);
    fclose(saida);
    printf("Arquivo descomprimido com sucesso: %s\n", nome_saida);
    return 0;

  } else {
    printf("Opção desconhecida %s\n", flag);
    return 1;
  }

  return 0;
}
