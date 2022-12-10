#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mtwister.h"
#include "structs.h"
#include "openssl/sha.h"

BlocoNaoMinerado createBlock(unsigned int numeroBloco, unsigned char qtdTransacoes, unsigned char *hashAnterior, MTRand randNumber)
{
    BlocoNaoMinerado bloco;
    bloco.numero = numeroBloco;
    bloco.nonce = 0;
    memset(bloco.data, 0, 184);
    for (int i = 0; i < qtdTransacoes; i++)
    {
        unsigned char endOrigem = (unsigned char)genRandLong(&randNumber) % 256; // gera aleatorio de 0 a 255
        unsigned char endDst = (unsigned char)genRandLong(&randNumber) % 256;    // gera aleatorio de 0 a 256
        unsigned char valor = (unsigned char)genRandLong(&randNumber) % 50;      // gera aleatorio de 0 a 50
        bloco.data[i] = endOrigem;
        bloco.data[i + 1] = endDst;
        bloco.data[i + 2] = valor;
    }
    // copia o hash anterior para o bloco
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        bloco.hashAnterior[i] = hashAnterior[i];
    }
    return bloco;
}

BlocoMinerado mineBlock(BlocoNaoMinerado block, unsigned char hashAnterior)
{
    unsigned char hash[SHA256_DIGEST_LENGTH]; // vetor que armazenará o resultado do hash. Tamanho definido pela libssl
    SHA256((unsigned char *)&block, sizeof(BlocoNaoMinerado), hash);
    // enquanto o hash não começar com 3 zeros, incrementa o nonce
    printf("Mineirando bloco %d...\n", block.numero);
    while (hash[0] != 0 || hash[1] != 0 || hash[2] != 0)
    {
        block.nonce++;
        SHA256((unsigned char *)&block, sizeof(BlocoNaoMinerado), hash);
    }
    BlocoMinerado blocoMinerado;
    blocoMinerado.bloco = block;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        blocoMinerado.hash[i] = hash[i];
    }

    return blocoMinerado;
}

void printUnminedBlock(BlocoNaoMinerado bloco)
{
    printf("Bloco %d\n", bloco.numero);
    printf("Nonce: %d\n", bloco.nonce);
    printf("Hash anterior: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        printf("%02x", bloco.hashAnterior[i]);
    }
    printf("\n");
    printf("Transacoes: ");
    for (int i = 0; i < 61; i++)
    {
        if (bloco.data[i] == 0)
        {
            break;
        }
        printf("Origem: %d, ", bloco.data[i]);
        printf("Destino: %d, ", bloco.data[i + 1]);
        printf("Valor: %d, ", bloco.data[i + 2]);
        printf("\n");
    }
    printf("\n");
}

void printMinedBlock(BlocoMinerado bloco)
{
    printf("Bloco %d\n", bloco.bloco.numero);
    printf("Hash:");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        printf("%02x", bloco.hash[i]);
    }
    printf("\n");
    printf("Nonce: %d\n", bloco.bloco.nonce);
    printf("Hash anterior: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        printf("%02x", bloco.bloco.hashAnterior[i]);
    }
}

int main(int argc, char *argv[])
{
    // Initialize the random number generator
    MTRand randNumber = seedRand(1234567); // objeto gerador com semente 1234567
    int numeroBloco = 1;
    // create a previous hash for the first block
    unsigned char hashAnterior[SHA256_DIGEST_LENGTH];
    memset(hashAnterior, 0, SHA256_DIGEST_LENGTH);
    while (1)
    {
        printf("Hash anterior: ");
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            printf("%02x", hashAnterior[i]);
        }
        printf("\n");
        unsigned char qtdTransacoes = (unsigned char)(1 + (genRandLong(&randNumber) % 61));
        BlocoNaoMinerado blocoAMinerar = createBlock(numeroBloco, qtdTransacoes, hashAnterior, randNumber);
        // printUnminedBlock(blocoAMinerar);
        clock_t begin = clock();
        BlocoMinerado blocoMinerado = mineBlock(blocoAMinerar, &hashAnterior);
        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("Tempo gasto: %.5f s\n\n\n\n", time_spent);
        printMinedBlock(blocoMinerado);
        // copia o hash do bloco minerado para o hash anterior
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            hashAnterior[i] = blocoMinerado.hash[i];
        }
        printf("\n\n\n\n\n");
        numeroBloco++;
    }
    return 0;
}