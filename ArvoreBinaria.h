typedef struct NO* ArvBin;

// Árvores binárias básicas
ArvBin* cria_ArvBin();
void libera_ArvBin(ArvBin *raiz);
int insere_ArvBin(ArvBin* raiz, int valor);
int remove_ArvBin(ArvBin *raiz, int valor);
int estaVazia_ArvBin(ArvBin *raiz);
int altura_ArvBin(ArvBin *raiz);
int totalNO_ArvBin(ArvBin *raiz);
int consulta_ArvBin(ArvBin *raiz, int valor);
void preOrdem_ArvBin(ArvBin *raiz);
void emOrdem_ArvBin(ArvBin *raiz);
void posOrdem_ArvBin(ArvBin *raiz);

// Funções relacionadas à compressão de Huffman
int compactarArquivo(const char* caminhoEntrada, const char* caminhoSaida);
int descompactarArquivo(const char* caminhoCompactado, const char* caminhoSaida);
void compararTamanhosArquivo(const char* caminhoOriginal, const char* caminhoCompactado);