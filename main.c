#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mtwister.c"
#include "structs.h"
#include "openssl/sha.h"

BlocoNaoMinerado createBlock(unsigned int numeroBloco, unsigned char *hashAnterior)
{
    BlocoNaoMinerado bloco;
    bloco.numero = numeroBloco;
    bloco.nonce = 0;
    // copia o hash anterior para o bloco
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        bloco.hashAnterior[i] = hashAnterior[i];
    }
    return bloco;
}

BlocoMinerado mineBlock(BlocoNaoMinerado block, unsigned char *hashAnterior)
{
    unsigned char hash[SHA256_DIGEST_LENGTH]; // vetor que armazenará o resultado do hash. Tamanho definido pela libssl
    SHA256((unsigned char *)&block, sizeof(BlocoNaoMinerado), hash);
    // enquanto o hash não começar com 3 zeros, incrementa o nonce
    printf("Minerando bloco %d...\n", block.numero);
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
    printf("Transacoes: \n");
    for (int i = 0; i < 181; i += 3)
    {
        printf("%d ", bloco.data[i]);
        printf("%d ", bloco.data[i + 1]);
        printf("%d ", bloco.data[i + 2]);
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

void imprimeMenu(int *opcao)
{
    printf("1 - Continuar mineração\n");
    printf("2 - Listar informações do bloco\n");
    printf("3 - Quantidade de bitcoins\n");
    printf("4 - Endereco com mais bitcoins\n");
    printf("5 - Enderecos com mais bitcoins\n");
    printf("6 - Sair\n");
    scanf("%d", opcao);
}

void continueAndClear()
{
    printf("\nPressione enter para continuar...");
    getchar();
    getchar();
    system("clear");
}

void saveBlocksToFile(BlocoMinerado *blocoEspera, BlocoMinerado blocoMinerado)
{
    FILE *fp;
    fp = fopen("blockchain.bin", "ab");
    fwrite(blocoEspera, sizeof(BlocoMinerado), 1, fp);
    fwrite(&blocoMinerado, sizeof(BlocoMinerado), 1, fp);
    fclose(fp);

    FILE *fp2;
    fp2 = fopen("blockchain.txt", "a");
    fprintf(fp2, "Bloco %d\n", blocoEspera->bloco.numero);
    fprintf(fp2, "Hash:");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        fprintf(fp2, "%02x", blocoEspera->hash[i]);
    }
    fprintf(fp2, "\n");
    fprintf(fp2, "Nonce: %d\n", blocoEspera->bloco.nonce);
    fprintf(fp2, "Hash anterior: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        fprintf(fp2, "%02x", blocoEspera->bloco.hashAnterior[i]);
    }
    fprintf(fp2, "\n");
    fprintf(fp2, "Transacoes: ");
    for (int i = 0; i < 184; i += 3)
    {
        if (blocoEspera->bloco.data[i] == 0)
        {
            break;
        }
        fprintf(fp2, "Origem: %d, ", blocoEspera->bloco.data[i]);
        fprintf(fp2, "Destino: %d, ", blocoEspera->bloco.data[i + 1]);
        fprintf(fp2, "Valor: %d, ", blocoEspera->bloco.data[i + 2]);
        fprintf(fp2, "\n");
    }

    fprintf(fp2, "\n\n");

    fprintf(fp2, "Bloco %d\n", blocoMinerado.bloco.numero);
    fprintf(fp2, "Hash:");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        fprintf(fp2, "%02x", blocoMinerado.hash[i]);
    }
    fprintf(fp2, "\n");
    fprintf(fp2, "Nonce: %d\n", blocoMinerado.bloco.nonce);
    fprintf(fp2, "Hash anterior: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        fprintf(fp2, "%02x", blocoMinerado.bloco.hashAnterior[i]);
    }
    fprintf(fp2, "\n");
    fprintf(fp2, "Transacoes: ");
    for (int i = 0; i < 184; i += 3)
    {
        if (blocoMinerado.bloco.data[i] == 0)
        {
            break;
        }
        fprintf(fp2, "Origem: %d, ", blocoMinerado.bloco.data[i]);
        fprintf(fp2, "Destino: %d, ", blocoMinerado.bloco.data[i + 1]);
        fprintf(fp2, "Valor: %d, ", blocoMinerado.bloco.data[i + 2]);
        fprintf(fp2, "\n");
    }

    fprintf(fp2, "\n\n");
    fclose(fp2);
}

void saveBalances(unsigned int *balances)
{
    FILE *fp;
    fp = fopen("balances.bin", "wb");

    fwrite(balances, sizeof(unsigned int), 256, fp);
    fclose(fp);

    // save on txt file for debug
    FILE *fp2;
    fp2 = fopen("balances.txt", "w");
    for (int i = 0; i < 256; i++)
    {
        fprintf(fp2, "Endereco %d: %d\n", i, balances[i]);
    }
    fclose(fp2);
}

void mine(BlocoNaoMinerado blocoAMinerar, BlocoMinerado *blocoEspera, int *cantSave, unsigned int *balances, unsigned char *hashAnterior)
{
    printf("Hash anterior: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        printf("%02x", hashAnterior[i]);
    }
    printf("\n");

    clock_t begin = clock();
    BlocoMinerado blocoMinerado = mineBlock(blocoAMinerar, blocoAMinerar.hashAnterior);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tempo gasto: %.5f s\n\n\n\n", time_spent);
    // printMinedBlock(blocoMinerado);
    // copia o hash do bloco minerado para o hash anterior
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        hashAnterior[i] = blocoMinerado.hash[i];
    }
    if (*cantSave)
    {
        *blocoEspera = blocoMinerado;
        *cantSave = 0;
    }
    else
    {
        // append both blocks to file
        saveBlocksToFile(blocoEspera, blocoMinerado);
        saveBalances(balances);
        *cantSave = 1;
    }
    printf("\n\n");
}

void getBlockById()
{
    int numeroBloco;
    printf("\nDigite o numero do bloco: ");
    scanf("%d", &numeroBloco);
    FILE *fp;
    fp = fopen("blockchain.bin", "rb");
    BlocoMinerado bloco;
    while (fread(&bloco, sizeof(BlocoMinerado), 1, fp))
    {
        if (bloco.bloco.numero == numeroBloco)
        {
            printUnminedBlock(bloco.bloco);
            return;
        }
    }
    printf("Bloco nao encontrado\n");
}

int *getDescendingBalances()
{
    // fetch balances from file
    FILE *fp;
    fp = fopen("balances.bin", "rb");
    int *balances = malloc(256 * sizeof(unsigned int));
    fread(balances, sizeof(unsigned int), 256, fp);
    fclose(fp);
    // sort balances descending
    for (int i = 0; i < 256; i++)
    {
        for (int j = i + 1; j < 256; j++)
        {
            if (balances[i] < balances[j])
            {
                unsigned int temp = balances[i];
                balances[i] = balances[j];
                balances[j] = temp;
            }
        }
    }
    return balances;
}

void getBalanceByAccountAddress()
{
    printf("Digite o endereco da conta: ");
    unsigned char endereco;
    scanf("%c", &endereco);
    FILE *fp;
    fp = fopen("balances.bin", "rb");
    unsigned int balance;
    fseek(fp, endereco * sizeof(unsigned int), SEEK_SET);
    fread(&balance, sizeof(unsigned int), 1, fp);
    if (balance)
    {
        printf("O endereco %d possui %d bitcoins\n", endereco, balance);
        return;
    }
    else
    {
        printf("O endereco %d nao possui bitcoins\n", endereco);
        return;
    }
}

void getHighestBalance()
{
    // fetch balances from file
    unsigned int *balances = getDescendingBalances();
    printf("O maior saldo e: %d bitcoins\n", balances[0]);
}

void showHighestBalances()
{
    // fetch balances from file
    int *balances = getDescendingBalances();
    printf("Os 30 maiores saldos sao:\n");
    for (int i = 0; i < 30; i++)
    {
        printf("%d bitcoins\n", balances[i]);
    }
}

int main(int argc, char *argv[])
{
    unsigned int balances[256];
    MTRand randNumber = seedRand(1234567); // objeto gerador com semente 1234567

    unsigned char hashAnterior[SHA256_DIGEST_LENGTH];
    FILE *file;
    file = fopen("blockchain.bin", "rb");

    // save all balances from file to array
    FILE *fp;
    fp = fopen("balances.bin", "rb");
    if (fp)
    {
        fread(balances, sizeof(unsigned int), 256, fp);
        fclose(fp);
    }
    else
    {
        memset(balances, 0, 256 * sizeof(unsigned int));
    }

    // get last block number
    BlocoMinerado lastBlock;
    int lastBlockNumber;
    if (file)
    {
        while (fread(&lastBlock, sizeof(BlocoMinerado), 1, file))
        {
            lastBlockNumber = lastBlock.bloco.numero;
            for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
            {
                hashAnterior[i] = lastBlock.hash[i];
            }
        }
        fclose(file);
    }
    else
    {
        lastBlockNumber = 0;
        memset(hashAnterior, 0, SHA256_DIGEST_LENGTH);
    }
    int numeroBloco = lastBlockNumber + 1;
    BlocoMinerado blocoEspera;
    int cantSave = 1;
    // create a previous hash for the first block
    int opcao = 0;
    imprimeMenu(&opcao);
    while (opcao != 6)
    {
        switch (opcao)
        {
        case 1:
            // ask how many blocks to mine
            int blocksToMine;
            printf("Quantos blocos deseja minerar? ");
            scanf("%d", &blocksToMine);
            if (blocksToMine % 2 != 0)
            {
                printf("O numero de blocos deve ser par, serao minerados %d blocos.\n", ++blocksToMine);
            }
            for (int i = 0; i < blocksToMine; i++)
            {
                unsigned char qtdTransacoes = (unsigned char)(1 + (genRandLong(&randNumber) % 61));
                // set all values of transacoes to zero

                BlocoNaoMinerado blocoNaoMinerado = createBlock(numeroBloco, hashAnterior);
                memset(blocoNaoMinerado.data, 0, sizeof(blocoNaoMinerado.data));
                printf("Bloco /////////////////////////////////////////////////////////////////////////////////////////////%d\n", qtdTransacoes);
                for (int i = 0; i < (qtdTransacoes * 3); i += 3)
                {
                    unsigned char endOrigem = (unsigned char)genRandLong(&randNumber) % 256;  // gera aleatorio de 0 a 255
                    unsigned char endDst = (unsigned char)genRandLong(&randNumber) % 256;     // gera aleatorio de 0 a 255
                    unsigned char valor = (unsigned char)(1 + genRandLong(&randNumber) % 50); // gera aleatorio de 0 a 50
                    *(balances + endOrigem) -= valor;
                    *(balances + endDst) += valor;
                    blocoNaoMinerado.data[i] = endOrigem;
                    blocoNaoMinerado.data[i + 1] = endDst;
                    blocoNaoMinerado.data[i + 2] = valor;
                }
                mine(blocoNaoMinerado, &blocoEspera, &cantSave, balances, hashAnterior);
                numeroBloco++;
            }
            break;
        case 2:
            getBlockById();
            break;
        case 3:
            getBalanceByAccountAddress();
            break;
        case 4:
            getHighestBalance();
            break;
        case 5:
            showHighestBalances();
            break;
        }
        continueAndClear();
        opcao = 0;
        imprimeMenu(&opcao);
    }
    return 0;
}
