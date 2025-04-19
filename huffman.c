#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman.h"

int heapify(NoHeap** heap, int n, int i) {
  int menor = i;
  int esq = 2 * i + 1;
  int dir = 2 * i + 2;

  // verifica se o filho esquerdo existe e é menor que o nó raiz
  if (esq < n && heap[esq]->frequencia < heap[menor]->frequencia) menor = esq;

  // Veririca se o filho direito existe e é menor que o nó raiz
  if (dir < n && heap[dir]->frequencia < heap[menor]->frequencia) menor = dir;

  // Se o menor não for nó raiz, trocar e heapificar recursivamente
  if (menor != i) {
    NoHeap* temp = heap[i];
    heap[i] = heap[menor];
    heap[menor] = temp;

    // Heapificar recursivamente a subárvore afetada
    heapify(heap, n, menor);
  }

  return 0;
}

// Função para inserir um novo nó no min-heap
int inserir_heap(NoHeap* heap[], int* tamanho, NoHeap* novo_no) {
  heap[*tamanho] = novo_no;
  (*tamanho)++;

  // "subir" o nó para manter a propriedade do min-heap
  int i = *tamanho - 1;
  while (i > 0 && heap[(i - 1) / 2]->frequencia > heap[i]->frequencia) {
    NoHeap* temp = heap[i];
    heap[i] = heap[(i - 1) / 2];
    heap[(i - 1) / 2] = temp;
    i = (i - 1) / 2;
  }

  return 0;
}

// Função para extrair o nó com a menor frequência do heap
NoHeap* extrair_minimo(NoHeap* heap[], int* tamanho) {
  if (*tamanho == 0) return NULL;

  // O elemento mínimo está na raíz
  NoHeap* no_minimo = heap[0];

  // Mover o último nó para a raiz e reduzir o tamanho do heap
  heap[0] = heap[*tamanho - 1];
  (*tamanho)--;

  // Heapificar a raiz para manter a pripriedade do min-heap
  heapify(heap, *tamanho, 0);

  return no_minimo;
}

// Função para construir o min-eap a partir de um array de nós
int construir_heap(NoHeap* heap[], int tamanho) {
  // Começar a heapificação a partir do último nó não-folha
  for (int i = tamanho / 2 - 1; i >= 0; i--) heapify(heap, tamanho, i);
  return 0;
}

// Liberando memória do heap
int destruir_heap(NoHeap* heap[], int tamanho) {
  for (int i = 0; i < tamanho; i++) {
    free(heap[i]);
  }
  return 0;
}

NoHeap* construir_arvore_huffman(int frequencias[MAX_CARACTERES]) {
  NoHeap* heap[MAX_CARACTERES];
  int tamanho_heap = 0;

  // Criar o heap inicial com nós para caracteres com frequência maior
  for (int i = 0; i < MAX_CARACTERES; i++) {
    if (frequencias[i] > 0) {
      NoHeap* novo_no = (NoHeap*)malloc(sizeof(NoHeap));
      novo_no->caractere = (unsigned char)i;
      novo_no->frequencia = frequencias[i];
      novo_no->esq = novo_no->dir = NULL;
      inserir_heap(heap, &tamanho_heap, novo_no);
    }
  }

  // Construir a árvore de Huffman combinando dois nós com menor frequência
  while (tamanho_heap > 1) {
    NoHeap* esq = extrair_minimo(heap, &tamanho_heap);
    NoHeap* dir = extrair_minimo(heap, &tamanho_heap);

    // Criar um novo nó interno com a soma das frequências
    NoHeap* interno = (NoHeap*)malloc(sizeof(NoHeap));
    interno->caractere = '\0';  // Nó interno não possui caractere, /0 é usado
    interno->frequencia = esq->frequencia + dir->frequencia;
    interno->esq = esq;
    interno->dir = dir;

    // Inserir o novo nó de volta no heap
    inserir_heap(heap, &tamanho_heap, interno);
  }

  // O nó restante é a raiz da árvore de Huffman
  return extrair_minimo(heap, &tamanho_heap);
}

int liberar_arvore(NoHeap* raiz) {
  if (raiz == NULL) return 0;

  liberar_arvore(raiz->esq);
  liberar_arvore(raiz->dir);

  free(raiz);

  return 0;
}

int gerar_codigos(NoHeap* raiz, char* caminho, int profundidade,
                  char* codigos[MAX_CARACTERES]) {
  if (raiz == NULL) return 1;

  // Verifica se é folha
  if (raiz->esq == NULL && raiz->dir == NULL) {
    caminho[profundidade] = '\0';                // termina string
    codigos[raiz->caractere] = strdup(caminho);  // Salva cópia do caminho

    if (codigos[raiz->caractere] == NULL)
      return 1;  // deus ajude se isso acontecer

    return 0;
  }

  // se gerar_codigos() retorna algo além de 0 algo deu errado
  // Visita subárvore esquerda
  caminho[profundidade] = '0';
  if (gerar_codigos(raiz->esq, caminho, profundidade + 1, codigos)) return 1;

  // Visita subárvore direita
  caminho[profundidade] = '1';
  if (gerar_codigos(raiz->dir, caminho, profundidade + 1, codigos)) return 1;

  return 0;
}

int serializar_arvore(NoHeap* raiz, FILE* arquivo) {
  if (raiz == NULL) return 0;

  // Se for folha escrevemos '0' e o caractere
  if (raiz->esq == NULL && raiz->dir == NULL) {
    // fputc retorna EOF em caso de falha
    if (fputc('0', arquivo) == EOF) return 1;
    if (fputc(raiz->caractere, arquivo) == EOF) return 1;
    return 0;
  }

  // Se não for folha escrevemos 1
  if (fputc('1', arquivo) == EOF) return 1;

  // Faz o mesmo com as subárvores esquerda e direita
  if (serializar_arvore(raiz->esq, arquivo) != 0) return 1;
  if (serializar_arvore(raiz->dir, arquivo) != 0) return 1;
  return 0;
}

int escrever_codigo_huffman(FILE* entrada, FILE* saida, char* codigos[256]) {
  if (entrada == NULL || saida == NULL || codigos == NULL) return 1;

  rewind(entrada);   // Garante que estamos no início do arquivo
  if (errno != 0) {  // clang demandou que verificassemos esse errno
    perror("Erro ao rebobinar arquivo");
    return 1;
  }

  unsigned char buffer = 0;
  int bit_count = 0;

  int ch;
  while ((ch = fgetc(entrada)) != EOF) {
    const char* codigo = codigos[(unsigned char)ch];
    if (codigo == NULL) continue;

    for (int i = 0; codigo[i] != '\0'; i++) {
      buffer <<= 1;  // Desloca os bits existentes para a esquerda
      if (codigo[i] == '1') buffer |= 1;  // Adiciona bit 1 se necessário
      bit_count++;

      if (bit_count == 8) {
        if (fputc(buffer, saida) == EOF) return 1;  // Escreve byte completo
        buffer = 0;
        bit_count = 0;
      }
    }
  }

  // Escreve o último byte, se houver bits pendentes
  if (bit_count > 0) {
    buffer <<= (8 - bit_count);  // Preenche com zeros à direita
    if (fputc(buffer, saida) == EOF) return 1;
  }

  return 0;
}

int ler_frequencias(char* nome_arquivo, int freq[256]) {
  FILE* arquivo = fopen(nome_arquivo, "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir arquivo: %s\n", nome_arquivo);
    return 1;
  }

  int ch;
  // Inicializa todas as frequências com 0
  for (int i = 0; i < 256; i++) {
    freq[i] = 0;
  }

  // Lê cada caractere do arquivo e incrementa a frequência
  while ((ch = fgetc(arquivo)) != EOF) {
    freq[ch]++;
  }

  fclose(arquivo);  // Fecha o arquivo depois de processar
  return 0;         // Sucesso
}

int compactar_arquivo(char* nome_entrada, char* nome_saida) {
  // Verificar se o arquivo de entrada pode ser aberto
  FILE* entrada = fopen(nome_entrada, "r");
  if (entrada == NULL) {
    printf("Erro ao abrir arquivo de entrada: %s\n", nome_entrada);
    return 1;
  }

  // Verificar se o arquivo de saída pode ser aberto
  FILE* saida = fopen(nome_saida, "wb");  // "wb" para escrever em binário
  if (saida == NULL) {
    printf("Erro ao abrir arquivo de saída: %s\n", nome_saida);
    fclose(entrada);  // Fechar o arquivo de entrada antes de sair
    return 1;
  }

  // Ler as frequências dos caracteres do arquivo de entrada
  int frequencias[256] = {0};  // Inicializa todas as frequências com 0
  if (ler_frequencias(nome_entrada, frequencias)) {
    printf("Erro ao ler as frequências do arquivo.\n");
    fclose(entrada);
    fclose(saida);
    return 1;
  }

  // Construir a árvore de Huffman a partir das frequências
  NoHeap* raiz = construir_arvore_huffman(frequencias);
  if (raiz == NULL) {
    printf("Erro ao construir a árvore de Huffman.\n");
    fclose(entrada);
    fclose(saida);
    return 1;
  }

  // Serializar a árvore de Huffman no arquivo de saída
  if (serializar_arvore(raiz, saida)) {
    printf("Erro ao serializar a árvore de Huffman.\n");
    fclose(entrada);
    fclose(saida);
    return 1;
  }

  // Gerar os códigos de Huffman para cada caractere
  // Inicializa todos os códigos com NULL
  char* codigos[MAX_CARACTERES] = {NULL};
  // Inicaliza o caminho
  char caminho[MAX_CARACTERES];
  if (gerar_codigos(raiz, caminho, 0, codigos)) {
    printf("Erro ao gerar os códigos de Huffman.\n");
    fclose(entrada);
    fclose(saida);
    return 1;
  }

  // Codificar o arquivo de entrada e escrever no arquivo de saída
  if (escrever_codigo_huffman(entrada, saida, codigos)) {
    printf("Erro ao codificar o arquivo com Huffman.\n");
    fclose(entrada);
    fclose(saida);
    // Liberando a memória dos códigos
    for (int i = 0; i < MAX_CARACTERES; i++)
      if (codigos[i] != NULL) free(codigos[i]);
    return 1;
  }

  // Fechar os arquivos após terminar o processo
  fclose(entrada);
  fclose(saida);

  // Liverar a memória dos códigos
  for (int i = 0; i < MAX_CARACTERES; i++)
    if (codigos[i] != NULL) free(codigos[i]);

  // Liberar memória da árvore e dos códigos
  liberar_arvore(raiz);

  return 0;  // Retorna 0 em caso de sucesso
}

NoHeap* desserializar_arvore(FILE* arquivo) {
  int c = fgetc(arquivo);
  if (c == EOF) {
    printf("Erro: Falha ao ler o arquivo (EOF alcançado inesperadamente).\n");
    return NULL;
  }

  // Nó folha: '0' seguido de um caractere
  if (c == '0') {
    int caractere = fgetc(arquivo);
    if (caractere == EOF) {
      printf("Erro: Falha ao ler caractere após marcador de folha.\n");
      return NULL;
    }

    NoHeap* folha = malloc(sizeof(NoHeap));
    if (!folha) {
      printf("Erro: Falha ao alocar memória para nó folha.\n");
      return NULL;
    }

    folha->caractere = (unsigned char)caractere;
    folha->frequencia = 0;
    folha->esq = folha->dir = NULL;
    return folha;
  }

  // Nó interno: '1', seguido de duas subárvores
  if (c == '1') {
    NoHeap* no = malloc(sizeof(NoHeap));
    if (no == NULL) {
      printf("Erro: Falha ao alocar memória para nó interno.\n");
      return NULL;
    }

    no->esq = desserializar_arvore(arquivo);
    if (no->esq == NULL) {
      printf("Erro: Falha ao desserializar subárvore esquerda.\n");
      free(no);  // Liberando memória alocada para o nó
      return NULL;
    }

    no->dir = desserializar_arvore(arquivo);
    if (no->dir == NULL) {
      printf("Erro: Falha ao desserializar subárvore direita.\n");
      free(no->esq);  // Liberando memória da subárvore esquerda
      free(no);       // Liberando memória alocada para o nó
      return NULL;
    }

    no->frequencia = 0;
    no->caractere = 0;
    return no;
  }

  // Caso inválido (caractere inesperado)
  printf("Erro: Caractere inesperado durante a desserialização.\n");
  return NULL;
}

// Lê um bit por vez de um arquivo binário
int ler_bit(FILE* arquivo, int* byte_atual, int* bits_restantes) {
  if (*bits_restantes == 0) {
    *byte_atual = fgetc(arquivo);
    if (*byte_atual == EOF) return EOF;
    *bits_restantes = 8;
  }

  int bit = (*byte_atual >> 7) & 1;  // Pega o bit mais significativo
  *byte_atual <<= 1;
  (*bits_restantes)--;

  return bit;
}

int descompactar_arquivo(FILE* arquivo_entrada, FILE* arquivo_saida) {
  // Desserializar a árvore de Huffman
  NoHeap* raiz = desserializar_arvore(arquivo_entrada);
  if (raiz == NULL) {
    printf("Erro ao desserializar a árvore de Huffman.\n");
    return 1;
  }

  NoHeap* no_atual = raiz;
  int byte_atual = 0;
  int bits_restantes = 0;
  int bit;

  while ((bit = ler_bit(arquivo_entrada, &byte_atual, &bits_restantes)) !=
         EOF) {
    if (bit == 0) {
      if (!no_atual || !no_atual->esq) {
        printf("Erro: nó esquerdo inválido.\n");
        liberar_arvore(raiz);
        return 1;
      }
      no_atual = no_atual->esq;
    } else if (bit == 1) {
      if (!no_atual || !no_atual->dir) {
        printf("Erro: nó direito inválido.\n");
        liberar_arvore(raiz);
        return 1;
      }
      no_atual = no_atual->dir;
    }

    // Chegamos numa folha
    if (no_atual && no_atual->esq == NULL && no_atual->dir == NULL) {
      if (fputc(no_atual->caractere, arquivo_saida) == EOF) {
        printf("Erro ao escrever no arquivo de saída.\n");
        liberar_arvore(raiz);
        return 1;
      }
      no_atual = raiz;  // Volta pro início pra decodificar o próximo caractere
    }
  }

  // Fica ativando toda hora e até agora não vi nenhum erro ocorrer portanto
  // este erro nunca ocorrerá mhm sim

  // Se terminar com um nó incompleto, é erro
  // if (no_atual && (no_atual->esq != NULL || no_atual->dir != NULL)) {
  // printf("Erro: fim inesperado do arquivo, caminho incompleto.\n");
  // liberar_arvore(raiz);
  // return 1;
  //}

  liberar_arvore(raiz);
  return 0;
}

int comprimir_com_confirmação(char* nome_arquivo) {
  // Nome do arquivo temporário (com extensão .temp)
  char nome_temp[256];
  snprintf(nome_temp, sizeof(nome_temp), "%s.temp", nome_arquivo);

  // Comprimir o arquivo para o arquivo temporário
  if (compactar_arquivo(nome_arquivo, nome_temp) != 0) {
    printf("Erro ao comprimir o arquivo.\n");
    return 1;
  }

  // Obter o tamanho do arquivo original
  FILE* entrada = fopen(nome_arquivo, "rb");
  if (!entrada) {
    printf("Erro ao abrir o arquivo original.\n");
    return 1;
  }
  fseek(entrada, 0, SEEK_END);
  long tamanho_original = ftell(entrada);
  fclose(entrada);

  // Obter o tamanho do arquivo comprimido (temporário)
  FILE* temp = fopen(nome_temp, "rb");
  if (!temp) {
    printf("Erro ao abrir o arquivo temporário.\n");
    return 1;
  }
  fseek(temp, 0, SEEK_END);
  long tamanho_comprimido = ftell(temp);
  fclose(temp);

  // Exibir informações de tamanho dos arquivos
  printf("Tamanho do arquivo original:   %ld bytes\n", tamanho_original);
  printf("Tamanho após compressão:    %ld bytes\n", tamanho_comprimido);
  printf("Diferença:                    %ld bytes\n",
         tamanho_comprimido - tamanho_original);

  // Perguntar ao usuário se deseja prosseguir com a compressão
  printf("Deseja salvar o arquivo comprimido '%s.huff'? [s/n]: ",
         nome_arquivo);
  char resposta = (char)getchar();
  while (getchar() != '\n');  // limpa o buffer

  // Se a resposta for 's' ou 'S', renomeia o arquivo temporário para o nome
  // final
  if (resposta == 's' || resposta == 'S') {
    // Renomear o arquivo temporário para o arquivo comprimido final
    if (rename(nome_temp, strcat(nome_arquivo, ".huff")) != 0) {
      printf("Erro ao renomear o arquivo comprimido.\n");
      return 1;
    }
  } else {
    // Se o usuário cancelar, exclui o arquivo temporário
    remove(nome_temp);
    printf("Operação de compressão cancelada.\n");
  }

  return 0;
}

int descomprimir_com_confirmacao(const char* nome_arquivo) {
  // Abre o arquivo original comprimido
  FILE* entrada = fopen(nome_arquivo, "rb");
  if (!entrada) {
    printf("Erro ao abrir o arquivo comprimido.\n");
    return 1;
  }

  // Cria nome temporário para saída
  char nome_temp[512];
  snprintf(nome_temp, sizeof(nome_temp), "%s.dec.temp", nome_arquivo);

  FILE* temp_saida = fopen(nome_temp, "wb");
  if (!temp_saida) {
    printf("Erro ao criar arquivo temporário de saída.\n");
    fclose(entrada);
    return 1;
  }

  // Executa a descompactação para o arquivo temporário
  if (descompactar_arquivo(entrada, temp_saida) != 0) {
    // A função interna já trata mensagens de erro
    fclose(entrada);
    fclose(temp_saida);
    remove(nome_temp);
    return 1;
  }

  fclose(entrada);
  fclose(temp_saida);

  // Obtém tamanhos dos arquivos
  FILE* f_orig = fopen(nome_arquivo, "rb");
  FILE* f_temp = fopen(nome_temp, "rb");
  if (!f_orig || !f_temp) {
    printf("Erro ao reabrir arquivos para verificação de tamanho.\n");
    if (f_orig) fclose(f_orig);
    if (f_temp) fclose(f_temp);
    remove(nome_temp);
    return 1;
  }

  fseek(f_orig, 0, SEEK_END);
  long tamanho_original = ftell(f_orig);
  fseek(f_temp, 0, SEEK_END);
  long tamanho_descomprimido = ftell(f_temp);

  fclose(f_orig);
  fclose(f_temp);

  // Mostra diferenças e pede confirmação
  printf("Tamanho do arquivo comprimido: %ld bytes\n", tamanho_original);
  printf("Tamanho após descompressão:    %ld bytes\n", tamanho_descomprimido);
  printf("Diferença:                    +%ld bytes\n",
         tamanho_descomprimido - tamanho_original);
  printf("Deseja salvar o arquivo '%s.dec'? [s/n]: ",
         nome_arquivo);

  char resposta = (char)getchar();
  while (getchar() != '\n');  // limpa o buffer

  if (resposta == 's' || resposta == 'S') {
    char nome_final[512];
    snprintf(nome_final, sizeof(nome_final), "%s.dec", nome_arquivo);
    if (rename(nome_temp, nome_final) != 0) {
      printf("Erro ao renomear arquivo descomprimido.\n");
      remove(nome_temp);
      return 1;
    }
    printf("Arquivo descomprimido salvo como: %s\n", nome_final);
    return 0;
  } else {
    printf("Descompressão cancelada. Arquivo temporário removido.\n");
    remove(nome_temp);
    return 0;
  }
}
