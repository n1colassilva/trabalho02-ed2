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
    // Usa a função com confirmação e exibição de tamanhos
    if (descomprimir_com_confirmacao(arquivo) != 0) {
      printf("Erro ao descomprimir o arquivo.\n");
      return 1;
    }

    return 0;
  } else {
    printf("Opção desconhecida %s\n", flag);
    return 1;
  }

  return 0;
}
