


# **Compactação e Descompactação usando Huffman**

Este projeto implementa um algoritmo de compressão de arquivos baseado na técnica de **Códigos de Huffman**. O código foi desenvolvido em C e permite a compactação de arquivos, porém a descompactação ainda não foi implementada.

## 🛠 **Compilação**

Para compilar o programa, basta rodar o seguinte comando:

```bash
gcc huffman.c main.c -o huffman.exe ```

Isso gerará um executável chamado `huffman`.

## 🚀 **Execução**

Para rodar o programa, execute o comando:

```bash
./huffman [OPÇÃO] [ARQUIVO]
```


Uso: huffman \[OPÇÃO] \[ARQUIVO]

Utiliza o algoritmo de Huffman em um arquivo especificado.

Opções:
  ./huffman -c    Comprime um arquivo
  ./huffman -d    Descomprime o arquivo, se válido
  ./huffman -v    Visualiza um arquivo comprimido



## ⚠️ **Limitações e Testes**

- O programa assume que é um arquivo que usa o conjunto ASCII

## 🔧 **Como funciona**

1. O programa lê o arquivo de entrada e gera uma árvore de Huffman.
2. Em seguida, compacta o arquivo utilizando essa árvore, gerando um arquivo de saída com a versão compactada.
3. Os tamanhos do arquivo original e do arquivo compactado são comparados e exibidos no terminal.

## 🔄 **Próximos passos**

- [] Adicionar visualização de arquivo
- [] Adicionar texto mostrando a diferença de tamanho

