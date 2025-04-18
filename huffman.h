#include <stdio.h>
#ifndef HUFFMAN_H
#define HUFFMAN_H

#define MAX_CARACTERES 256

/*
 * Representa um nó da heap e da árvore de Huffman.
 */
typedef struct NoHeap {
    unsigned char caractere;  ///< Caractere representado (válido apenas em nós folha)
    int frequencia;           ///< Frequência do caractere ou soma das frequências dos filhos
    struct NoHeap *esq;       ///< Ponteiro para o filho à esquerda (NULL se for folha)
    struct NoHeap *dir;       ///< Ponteiro para o filho à direita (NULL se for folha)
} NoHeap;


/*
 * Garante a propriedade de min-heap a partir do índice i
 *
 * @param heap vetor de ponteiros para nós
 * @param n tamanho atual do heap
 * @param i índice a ser ajustado
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int heapify(NoHeap* heap[], int n, int i);

/*
 * Insere um novo nó no heap mantendo a ordenação por frequência
 *
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int inserir_heap(NoHeap* heap[], int* tamanho, NoHeap* novo_no);

/*
 * Remove e retorna o nó com menor frequência do heap
 */
NoHeap* extrair_minimo(NoHeap* heap[], int* tamanho);

/*
 * Constrói um min-heap a partir de um vetor de nós
 *
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int construir_heap(NoHeap* heap[], int tamanho);

/*
 * Libera todos os nós do heap
 *
 * @return 0 em caso de sucesso, 1 em caso de erro
 */
int destruir_heap(NoHeap* heap[], int tamanho);

/*
 * Constrói a árvore de Huffman a partir de um array de frequências
 *
 * @param frequências vetor com frequência de cada caractere (0-255)
 * @return ponteiro para o nó raiz da árvore de Huffman
 */
NoHeap* construir_arvore_huffman(int frequencias[MAX_CARACTERES]);

/**
 * Libera a memória da árvore de Huffman
 *
 * @param raiz Nó raiz da árvore
 * @return 0 se sucesso, 1 se erro
 */
int liberar_arvore(NoHeap* raiz);

/**
 * Percorre a árvore de Huffman e gera os códigos binários para cada caractere.
 *
 * O código de cada caractere é armazenado em `codigos[c]` como uma string de '0' e '1'.
 *
 * @param raiz raiz da árvore de Huffman
 * @param caminho buffer temporário usado para montar os códigos
 * @param profundidade posição atual no buffer
 * @param codigos array onde serão salvos os códigos gerados
 * @return 0 em caso de sucesso, 1 se ocorrer falha na alocação de memória
 */
int gerar_codigos(NoHeap* raiz, char* caminho, int profundidade, char* codigos[MAX_CARACTERES]);


int serializar_arvore(NoHeap* raiz, FILE* arquivo);

/**
 * Codifica o conteúdo de um arquivo usando os códigos de Huffman e escreve os bits resultantes em outro arquivo.
 *
 * @param entrada ponteiro para o arquivo a ser lido (aberto para leitura)
 * @param saida ponteiro para o arquivo de saída (aberto para escrita, árvore já serializada)
 * @param codigos vetor de 256 ponteiros para strings contendo os códigos de Huffman para cada caractere (índice == valor do caractere)
 * @return 0 em caso de sucesso, 1 em caso de erro (ex: falha ao escrever no arquivo)
 */
int escrever_codigo_huffman(FILE* entrada, FILE* saida, char* codigos[256]);

// Finalmente

/**
 * Lê o arquivo de entrada e calcula a frequência de cada caractere.
 *
 * @param nome_arquivo caminho do arquivo de entrada
 * @param freq vetor de 256 inteiros a ser preenchido com as frequências
 * @return 0 em caso de sucesso, 1 em caso de erro (ex: falha ao abrir o arquivo)
 */
int ler_frequencias(char* nome_arquivo, int freq[256]);

/**
 *
 */
int compactar_arquivo(char* nome_entrada, char* nome_saida);

/*
 * Desserializa uma árvore de Huffman de um arquivo.
 *
 * Lê o arquivo e reconstrói a árvore de Huffman a partir da sequência de bits
 * representada no arquivo. O arquivo deve estar no formato previamente
 * serializado, com '0' indicando um nó folha, seguido do caractere,
 * e '1' indicando um nó interno, seguido das subárvores esquerda e direita.
 *
 * @param arquivo Ponteiro para o arquivo a ser lido.
 * @return Ponteiro para o nó raiz da árvore de Huffman ou NULL em caso de erro.
 */
NoHeap* desserializar_arvore(FILE* arquivo);

// Lê um bit por vez de um arquivo binário
int ler_bit(FILE *arquivo, int *byte_atual, int *bits_restantes);

/**
 * @brief Descomprime um arquivo codificado com Huffman.
 *
 * Lê a árvore de Huffman serializada e os bits codificados do arquivo de entrada,
 * reconstruindo os dados originais no arquivo de saída.
 *
 * @param arquivo_entrada Ponteiro para o arquivo binário comprimido (modo "rb").
 * @param arquivo_saida Ponteiro para o arquivo onde os dados descomprimidos serão escritos (modo "wb").
 * @return int Retorna 0 em caso de sucesso, ou 1 em caso de erro.
 */
int descompactar_arquivo(FILE* arquivo_entrada, FILE* arquivo_saida);

/**
 * @brief Comprime um arquivo e solicita confirmação ao usuário antes de realizar a operação final.
 *
 * Esta função comprime um arquivo para um arquivo temporário, calcula os tamanhos do arquivo original
 * e do arquivo comprimido, e exibe essas informações ao usuário. Em seguida, pergunta ao usuário se deseja
 * continuar com a compressão e renomear o arquivo temporário para o nome final com a extensão `.huff`.
 *
 * Se o usuário concordar, o arquivo temporário é renomeado para o arquivo comprimido final. Caso contrário,
 * o arquivo temporário é removido e a operação é cancelada.
 *
 * @param nome_arquivo Nome do arquivo de entrada a ser comprimido.
 *
 * @return 0 em caso de sucesso, ou 1 se ocorrer algum erro durante a compressão ou a interação com o usuário.
 */
int comprimir_com_confirmação(char* nome_arquivo);

/**
 * @brief Descomprime um arquivo após confirmação do usuário.
 *
 * Essa função descomprime o arquivo especificado em um arquivo temporário,
 * calcula o tamanho antes e depois, e solicita confirmação ao usuário antes
 * de salvar a versão final descomprimida.
 *
 * @param nome_arquivo Caminho para o arquivo comprimido.
 * @return int 0 em sucesso, 1 em caso de erro.
 */
int descomprimir_com_confirmacao(const char* nome_arquivo);

#endif // HUFFMAN_H

