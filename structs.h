#include "openssl/sha.h"

struct BlocoNaoMinerado
{
    unsigned int numero;                              // 4
    unsigned int nonce;                               // 4
    unsigned char data[184];                          // nao alterar. Deve ser inicializado com zeros.
    unsigned char hashAnterior[SHA256_DIGEST_LENGTH]; // 32
};
typedef struct BlocoNaoMinerado BlocoNaoMinerado;

struct BlocoMinerado
{
    BlocoNaoMinerado bloco;                   // neste campo você pode atribuir a variável blocoAMinerar diretamente
    unsigned char hash[SHA256_DIGEST_LENGTH]; // Neste campo você põe o hash válido da variável blocoAMinerar
};
typedef struct BlocoMinerado BlocoMinerado;