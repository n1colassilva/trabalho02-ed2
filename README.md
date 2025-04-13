


# **Compactação e Descompactação usando Huffman**

Este projeto implementa um algoritmo de compressão de arquivos baseado na técnica de **Códigos de Huffman**. O código foi desenvolvido em C e permite a compactação de arquivos, porém a descompactação ainda não foi implementada.

## 🛠 **Compilação**

Para compilar o programa, basta rodar o seguinte comando:

```bash
gcc HuffmanCompleto.c -o huffman
```

Isso gerará um executável chamado `huffman`.

## 🚀 **Execução**

Para rodar o programa, execute o comando:

```bash
./huffman
```

O programa irá compactar o arquivo de entrada, comparará os tamanhos antes e depois da compactação, e exibirá as informações no terminal. A funcionalidade de descompactação ainda não está implementada.

## ⚠️ **Limitações e Testes**

- **Descompactação**: A função de descompactação ainda precisa ser implementada. Neste momento, o programa é capaz apenas de compactar arquivos.
  
- **Arquivos suportados**: Por enquanto, o programa foi testado com arquivos de texto (`.txt`), como o exemplo "Teste.txt" incluído no repositório. No entanto, o algoritmo é projetado para funcionar com qualquer tipo de arquivo, incluindo imagens, PDFs, e outros formatos binários. Para uma análise mais completa, o código pode ser ajustado para trabalhar com arquivos como `.pdf`.

## 🔧 **Como funciona**

1. O programa lê o arquivo de entrada e gera uma árvore de Huffman.
2. Em seguida, compacta o arquivo utilizando essa árvore, gerando um arquivo de saída com a versão compactada.
3. Os tamanhos do arquivo original e do arquivo compactado são comparados e exibidos no terminal.

## 🔄 **Próximos passos**

- Implementação da função de descompactação.
- Adaptação do código para suportar uma gama mais ampla de tipos de arquivos.
- Testes adicionais com outros formatos (PDFs),.


