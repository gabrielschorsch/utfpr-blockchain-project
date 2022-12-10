#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mtwister.h"
#include "structs.h"

BlocoNaoMinerado *createBlock(unsigned int numeroBloco, unsigned char qtdTransacoes, MTRand randNumber)
{
    BlocoNaoMinerado *bloco = (BlocoNaoMinerado *)malloc(sizeof(BlocoNaoMinerado));
    bloco->numero = numeroBloco;
    bloco->nonce = 0;
    memset(bloco->data, 0, 184);
    for (int i = 0; i < qtdTransacoes; i++)
    {
        unsigned char endOrigem = (unsigned char)genRandLong(&randNumber) % 256; // gera aleatorio de 0 a 255
        unsigned char endDst = (unsigned char)genRandLong(&randNumber) % 256;    // gera aleatorio de 0 a 256
        unsigned char valor = (unsigned char)genRandLong(&randNumber) % 50;      // gera aleatorio de 0 a 50
        bloco->data[i] = endOrigem;
        bloco->data[i + 1] = endDst;
        bloco->data[i + 2] = valor;
    }
    return bloco;
}


int main(int argc, char *argv[])
{
    // Initialize the random number generator
    MTRand randNumber = seedRand(1234567); // objeto gerador com semente 1234567
    int numeroBloco = 1;
    unsigned char qtdTransacoes = (unsigned char)(1 + (genRandLong(&randNumber) % 61));
    printf("Quantidade de transacoes: %d\n", qtdTransacoes);
    BlocoNaoMinerado *blocoAMinerar = createBlock(numeroBloco, qtdTransacoes, randNumber);
    printf("Bloco: 1\n");
    for (int i = 0; i < 184 / 3; i++)
    {
        printf("Origem: %d, Destino: %d, Valor: %d\n", blocoAMinerar->data[i], blocoAMinerar->data[i + 1], blocoAMinerar->data[i + 2]);
    }

    return 0;
}