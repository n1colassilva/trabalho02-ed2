
# Compactação e Descompactação usando Huffman

Este projeto implementa um algoritmo de compressão de arquivos baseado na
técnica de **[Códigos de Huffman](https://pt.wikipedia.org/wiki/Codifica%C3%A7%C3%A3o_de_Huffman)**. O código foi desenvolvido em C e permite a
compactação de arquivos.

##  Compilação

Para compilar o programa, basta rodar o seguinte comando:

```bash

gcc huffman.c main.c -o huffman.exe 
```

Isso gerará um executável chamado `huffman.exe`.

## Execução

Para executar o programa, execute o comando:

```bash

./huffman
```


> Uso: huffman \[OPÇÃO] \[ARQUIVO]
> 
> Utiliza o algoritmo de Huffman em um arquivo especificado.
> 
> Opções: 
>   ./huffman -c    Comprime um arquivo
>   ./huffman -d    Descomprime o arquivo, se válido

Por exemplo:

```bash
./huffman -c textos/epico_de_gilgamesh.txt
./huffman -d textos/epico_de_gilgamesh.txt.huff

```


## Limitações e Testes

- O programa assume que o arquivo manipulado usa o conjunto ASCII
- Devido aos acentos possuirá problemas de UI em alguns terminais/sistemas
operacionais (windows)

## Como funciona

### Compressão

1. O programa lê o arquivo de entrada e gera um heap mínimo.
2. Usando o heap monta uma árvore de Huffman
3. É montado um arquivo temporário já comprimido, o usuário é mostrado a comparação dos tamanhos
4. O usuario pode concordar e ter o arquivo salvo de forma permanente ou negar e
   ter o arquivo temporário excluído

### Descompressão
1. O programa inicialmente reconstrói a árvore de huffman armazenada no início
   do arquivo
2. Usando esta árvore analiza os bits e reconstrói o texto original
3. Um arquivo temporário armazena o texto, se o usuário concordar o arquivo se
   torna permanente, se negar ele é excluído
